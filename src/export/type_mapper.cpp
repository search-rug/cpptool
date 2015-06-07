#include "export/type_mapper.hpp"

#include "wrapper.pb.h"
#include "export/export_util.hpp"

namespace ct {
    TypeMapper::TypeMapper(ProtobufOutput &typeOutput, clang::CompilerInstance const &clang) : typeOutput(typeOutput),
                                                                                               clang(clang) { }

    void TypeMapper::ResolveType(ct::proto::Type &target, clang::QualType type) {
        clang::QualType selectedType = type.getLocalUnqualifiedType();

        //Unpack and build type modifiers (* and &)
        for (; ;) {
            auto typePtr = selectedType.getTypePtr();

            if (clang::isa<clang::TypedefType>(typePtr)) {
                break; //We've reached a typedef
            } else if (clang::PointerType const *ptr = clang::dyn_cast<clang::PointerType>(typePtr)) {
                //Pointer type
                target.add_modifiers(ct::proto::Type_Modifier_POINTER);
                selectedType = ptr->getPointeeType();
                continue;
            } else if (clang::ReferenceType const *ref = clang::dyn_cast<clang::ReferenceType>(typePtr)) {
                //Reference type
                target.add_modifiers(ct::proto::Type_Modifier_REFERENCE);
                selectedType = ref->getPointeeType();
                continue;
            } else if (clang::ArrayType const *array = clang::dyn_cast<clang::ArrayType>(typePtr)) {
                //Array type (considered a pointer)
                target.add_modifiers(ct::proto::Type_Modifier_POINTER);
                selectedType = array->getElementType();
                continue;
			} else if (clang::TemplateTypeParmType const *tmpl = clang::dyn_cast<clang::TemplateTypeParmType>(typePtr)) {
				//Template parameter. Desugaring causes unexpected nullptr dereference
				break;
			}

            auto attemptedDesugar = selectedType.getSingleStepDesugaredType(clang.getASTContext())
				.getLocalUnqualifiedType();
            if (attemptedDesugar == selectedType) {
                //No sugar left to remove, we've reached a concrete type.
                break;
            } else {
                selectedType = attemptedDesugar;
            }
        }

        target.set_type_id(GetTypeId(selectedType));
    }

    void TypeMapper::ResolveLocation(ct::proto::SourceRange &range, clang::SourceRange clangRange) {
        auto &&sm = clang.getSourceManager();
        range.set_file(sm.getFilename(clangRange.getBegin()));
        {
            auto start = range.mutable_start();
            start->set_column(sm.getSpellingColumnNumber(clangRange.getBegin()));
            start->set_line(sm.getSpellingLineNumber(clangRange.getBegin()));
        }
        {
            auto end = range.mutable_end();
            end->set_column(sm.getSpellingColumnNumber(clangRange.getEnd()));
            end->set_line(sm.getSpellingLineNumber(clangRange.getEnd()));
        }
    }

    void TypeMapper::ResolveName(ct::proto::ScopedName &name, clang::NamedDecl const &decl) {
        if (clang::isa<clang::FunctionDecl>(decl)) {
            name.set_name(Export::getFunctionName(static_cast<clang::FunctionDecl const &>(decl)));
        } else {
            name.set_name(decl.getName());
        }
		ResolveContext(name, decl.getDeclContext());
	}

	void TypeMapper::ResolveContext(ct::proto::ScopedName &name, clang::DeclContext const *context) {
		clang::DeclContext const * const tuDecl = context->getParentASTContext().getTranslationUnitDecl();
		clang::DeclContext const * current = context->getPrimaryContext();
		for (;;) {
			if (current == tuDecl) {//Top of the tree is the translation unit declaration
				break;
			} else if (clang::CXXRecordDecl const *rec = clang::dyn_cast<clang::CXXRecordDecl>(current)) {
				if (rec->isLambda()) {
					//Resolve the lambda's context!
					GetContext(name, rec);
					return;
				}
			}
			current = current->getParent()->getPrimaryContext();
		}

		//Base case, just unpack the context
		name.set_context(Export::getContextIdentifier(context));
	}

    TypeMapper::PtrInt TypeMapper::GetTypeId(clang::QualType type) {
        auto id = reinterpret_cast<TypeMapper::PtrInt>(type.getTypePtr());
        if (seenTypes.emplace(id).second) {
            ct::proto::Envelope env;
            auto output = env.mutable_type();

            output->set_type_id(id);
            output->set_strongly_defined(!type->isIncompleteType());
            DetermineTypeDeclaration(*output, type); //Set name and location

            typeOutput.writeMessage<ct::proto::Envelope>(env);
        }
        return id;
	}

	void TypeMapper::GetContext(ct::proto::ScopedName &name, clang::CXXRecordDecl const *lambdaContext) {
		auto id = reinterpret_cast<TypeMapper::PtrInt>(lambdaContext);
		auto result = emittedContexts.emplace(id, "");
		if (result.second) { //This context is new, we need to emit the data and record its parent context
			ct::proto::Envelope env;
			auto output = env.mutable_iso_context();

			ResolveContext(*output->mutable_parent(), lambdaContext->getParent());
			ResolveType(*output->mutable_own_type(), clang::QualType(lambdaContext->getTypeForDecl(), 0));

			auto virtualContext = output->parent().context();
			output->set_context_id(id);
			output->mutable_parent()->set_name(""); //Parent name is required, but not relevant in this case

			//Record to map for future usage, emit info
			(*result.first).second = virtualContext;
			typeOutput.writeMessage<ct::proto::Envelope>(env);

			//Finally, set the data on the parent
			name.set_isolated_context_id(id);
			name.set_context(virtualContext);
		} else {
			name.set_context((*result.first).second);
			name.set_isolated_context_id(id);
		}
	}

    void TypeMapper::DetermineTypeDeclaration(ct::proto::TypeDefinition &def, clang::QualType type) {
		using clang::Type;
		auto typePtr = type.getTypePtr();
		switch (typePtr->getTypeClass()) {
#define ABSTRACT_TYPE(CLASS, BASE)
#define TYPE(CLASS, BASE)													\
		case Type::CLASS:													\
			TypeMapperTemplate::mapType<clang::CLASS##Type>(		\
				*this,														\
				def,														\
				static_cast<clang::CLASS##Type const &>(*typePtr)			\
			);																\
			break;
#include "clang/AST/TypeNodes.def"
		}
	}

	void TypeMapper::ResolveTemplateArgs(clang::TemplateArgumentList const &list, std::unordered_set<PtrInt> &out) {
		for (auto arg : list.asArray()) {
			if (arg.getKind() == clang::TemplateArgument::ArgKind::Type) {
				out.emplace(GetTypeId(arg.getAsType()));
			}
		}
	}
}