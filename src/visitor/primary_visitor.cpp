#include "visitor/primary_visitor.hpp"

namespace ct {
    PrimaryVisitor::PrimaryVisitor(RuntimeContext &&context)
            : context(std::move(context)),
              primaryFileId(context.getCompilationInstance().getSourceManager().getMainFileID()) {
    }

    void PrimaryVisitor::exportIncludes() {
        auto &&sm = context.getCompilationInstance().getSourceManager();
        auto rootId = primaryFileId;

        for (auto it = sm.fileinfo_begin(), end = sm.fileinfo_end(); it != end; ++it) {
            auto &&file = it->getFirst();
            auto &&fileId = sm.translateFile(file);
            auto &&includedBy = sm.getFileID(sm.getIncludeLoc(fileId));

			if (includedBy.isInvalid()) continue; //The file is not included by anything

			out().Include(sm.getFileEntryForID(includedBy), file);
        }
    }

    bool PrimaryVisitor::declaredInMain(const clang::Decl *D) const {
        auto &&sm = context.getCompilationInstance().getSourceManager();
        return this->primaryFileId == sm.getFileID(D->getLocation());
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

    CTExport &PrimaryVisitor::out() const {
        return context.out();
    }

    bool PrimaryVisitor::TraverseTranslationUnitDecl(clang::TranslationUnitDecl *D) {
        if (!clang::RecursiveASTVisitor<PrimaryVisitor>::WalkUpFromTranslationUnitDecl(D)) return false;

        //Original traversal code from: clang::RecursiveASTVisitor<PrimaryVisitor>::TraverseDeclContextHelper

        //By overriding the traversal of the translation unit we can filter out all
        //declarations in included files at the source.
        //This also avoids unnecessary traversal of the AST.

        for (auto *Child : D->decls()) {
            if (!clang::isa<clang::BlockDecl>(Child) &&
                !clang::isa<clang::CapturedDecl>(Child) &&
                declaredInMain(Child)) {

                if (!TraverseDecl(Child)) {
                    return false;
                }
            }
        }

        return true;
    }
}