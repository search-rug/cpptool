#include "export/type_mapper.hpp"

#include "wrapper.pb.h"
#include "export/export_util.hpp"

#include <clang/AST/AST.h>

namespace ct {
	TypeMapper::TypeMapper(ProtobufOutput &typeOutput, clang::CompilerInstance const &clang) : typeOutput(typeOutput), clang(clang) {}

	void TypeMapper::ResolveType(ct::proto::Type &target, clang::QualType type) {
		clang::QualType selectedType = type.getLocalUnqualifiedType();

		//Unpack and build type modifiers (* and &)
		for (;;) {
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
			}
			//TODO: probably need to handle array types here too.

			auto attemptedDesugar = selectedType.getSingleStepDesugaredType(clang.getASTContext()).getLocalUnqualifiedType();
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
		name.set_name(decl.getName());
		name.set_context(Export::getContextIdentifier(decl.getDeclContext()));
		//TODO: resolve anonymous contexts.
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

	void TypeMapper::DetermineTypeDeclaration(ct::proto::TypeDefinition &def, clang::QualType type) {
		if (clang::TypedefType const *tdef = clang::dyn_cast<clang::TypedefType>(type.getTypePtr())) {
			ResolveName(*def.mutable_specifier(), *tdef->getDecl());
			ResolveLocation(*def.mutable_location(), tdef->getDecl()->getSourceRange());
		} else if (clang::TagType const *tag = clang::dyn_cast<clang::TagType>(type.getTypePtr())) {
			ResolveName(*def.mutable_specifier(), *tag->getDecl());
			ResolveLocation(*def.mutable_location(), tag->getDecl()->getSourceRange());
		} else {
			def.mutable_specifier()->set_name(type.getAsString());
		}
	}
}