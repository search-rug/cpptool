#include "visitor/primary_visitor.hpp"

namespace ct {
    PrimaryVisitor::PrimaryVisitor(RuntimeContext &&context, RefSet<llvm::sys::fs::UniqueID const> const &inputFiles)
            : context(std::move(context)),
              primaryFileId(context.getCompilationInstance().getSourceManager().getMainFileID()),
              inputFiles(inputFiles) {
    }

    void PrimaryVisitor::exportIncludes() {
        auto &&sm = context.getCompilationInstance().getSourceManager();

        for (auto it = sm.fileinfo_begin(), end = sm.fileinfo_end(); it != end; ++it) {
            auto &&file = it->getFirst();
            auto &&fileId = sm.translateFile(file);
            auto &&includedBy = sm.getFileID(sm.getIncludeLoc(fileId));

            if (includedBy.isInvalid()) continue; //The file is not included by anything

            out().Include(sm.getFileEntryForID(includedBy), file);
        }
    }

    bool PrimaryVisitor::shouldTraverse(const clang::Decl *D) const {
        auto &&sm = context.getCompilationInstance().getSourceManager();
        auto &&id = sm.getFileID(D->getLocation());
        auto &&entry = sm.getFileEntryForID(id);
        return this->primaryFileId == id //Primary File
               ||
               (entry != nullptr //Some files only exists in memory, do not attempt to figure out if its an input file!
                && inputFiles.find(entry->getUniqueID()) != inputFiles.cend() //Any file in input set
               );
    }

    bool PrimaryVisitor::VisitFieldDecl(clang::FieldDecl *D) {
        out().MemberVariable(D);
        return true;
    }

    bool PrimaryVisitor::VisitVarDecl(clang::VarDecl *D) {
        if (clang::ParmVarDecl *param = clang::dyn_cast<clang::ParmVarDecl>(D)) {
            //Undefined functions still have their parameters traversed.
            clang::FunctionDecl *func = clang::dyn_cast<clang::FunctionDecl>(param->getDeclContext());
            if (!func || !func->isDefined()) return true;

            out().ParameterVariable(param);
        } else if (D->isLocalVarDecl()) {
            out().LocalVariable(D);
        } else if (D->isDefinedOutsideFunctionOrMethod()) {
            out().GlobalVariable(D);
        } else {
            // Are there other types of variable
            llvm_unreachable("Unexpected variable type");
        }

        return true;
    }

    bool PrimaryVisitor::VisitTranslationUnitDecl(clang::TranslationUnitDecl *D) {
        exportIncludes();
        return true;
    }

    bool PrimaryVisitor::VisitFunctionDecl(clang::FunctionDecl *D) {
        if (!D->isDefined()) return true;

        out().Function(D);
        return true;
    }

    bool PrimaryVisitor::VisitEnumDecl(clang::EnumDecl *D) {
        if (!D->isCompleteDefinition()) return true;

        out().Enum(D);
        return true;
    }

    bool PrimaryVisitor::VisitRecordDecl(clang::RecordDecl *D) {
        if (!D->isCompleteDefinition()) return true;

        out().Record(D);
        return true;
    }

    bool PrimaryVisitor::VisitTypedefNameDecl(clang::TypedefNameDecl *D) {
        out().TypeDef(D);
        return true;
    }

    bool PrimaryVisitor::VisitFriendDecl(clang::FriendDecl *D) {
        out().Friend(D);
        return true;
    }

    bool PrimaryVisitor::VisitClassTemplateDecl(clang::ClassTemplateDecl *D) {
        exportTemplateParameters<clang::TypeDecl>(D->getTemplateParameters(), D->getTemplatedDecl());
        out().Template(D);
        return true;
    }

    bool PrimaryVisitor::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl *D) {
        exportTemplateParameters<clang::NamedDecl>(D->getTemplateParameters(), D->getTemplatedDecl());
        out().Template(D);
        return true;
    }

    bool PrimaryVisitor::VisitTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl *D) {
        if (D->getTemplatedDecl()->getTypeForDecl() == nullptr) return true; //TODO: why does this happen?
        exportTemplateParameters<clang::TypeDecl>(D->getTemplateParameters(), D->getTemplatedDecl());
        out().Template(D);
        return true;
    }

    bool PrimaryVisitor::VisitVarTemplateDecl(clang::VarTemplateDecl *D) {
        exportTemplateParameters<clang::NamedDecl>(D->getTemplateParameters(), D->getTemplatedDecl());
        out().Template(D);
        return true;
    }

    CTExport &PrimaryVisitor::out() const {
        return context.out();
    }

    template<typename T>
    void PrimaryVisitor::exportTemplateParameters(clang::TemplateParameterList *parameters, T const *tmpl) {
        for (auto it = parameters->begin(), end = parameters->end(); it != end; it++) {
            auto named = *it;
            // template parameters can be non-types too
            if (clang::TemplateTypeParmDecl const *type_param = clang::dyn_cast<clang::TemplateTypeParmDecl>(named)) {
                out().TemplateParam(type_param, tmpl);
            }
        }
    }

    bool PrimaryVisitor::TraverseTranslationUnitDecl(clang::TranslationUnitDecl *D) {
        if (!clang::RecursiveASTVisitor<PrimaryVisitor>::WalkUpFromTranslationUnitDecl(D)) return false;

        //Original traversal code from: clang::RecursiveASTVisitor<PrimaryVisitor>::TraverseDeclContextHelper

        //By overriding the traversal of the translation unit we can filter out all
        //declarations in included files at the source.
        //This also avoids unnecessary traversal of the AST.

		//Used to keep track of the file that is being read
		auto &&sm = context.getCompilationInstance().getSourceManager();
		clang::FileID currentFile;

        for (auto *Child : D->decls()) {
            if (!clang::isa<clang::BlockDecl>(Child) &&
                !clang::isa<clang::CapturedDecl>(Child) &&
                shouldTraverse(Child)) {

				//Check if the file being read has changed, emit InputChanged if it has.
				auto fileId = sm.getFileID(Child->getLocation());
				if (currentFile != fileId) {
					currentFile = fileId;
					auto entry = sm.getFileEntryForID(fileId);
					if (entry == nullptr) ::abort(); //Sanity check
					out().InputChanged(entry);
				}

                if (!TraverseDecl(Child)) {
                    return false;
                }
            }
        }

        return true;
    }
}