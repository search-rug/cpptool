#include "visitor/primary_visitor.hpp"

namespace ct {
    PrimaryVisitor::PrimaryVisitor(RuntimeContext &&context) : context(std::move(context)) {
    }

    bool PrimaryVisitor::VisitTagDecl(clang::TagDecl *D) {
        if (declaredInMain(D))
            context.out().exportTagDecl(D->getDeclContext(), D);

        return true;
    }

    bool PrimaryVisitor::VisitVarDecl(clang::VarDecl *D) {
        if (declaredInMain(D))
            context.out().exportVarDecl(D);

        return true;
    }

    bool PrimaryVisitor::VisitTranslationUnitDecl(clang::TranslationUnitDecl *D) {
        exportIncludes();
        return true;
    }

    void PrimaryVisitor::exportIncludes() {
        auto &&sm = context.getCompilationInstance().getSourceManager();
        auto rootId = sm.getMainFileID();

        for (auto it = sm.fileinfo_begin(), end = sm.fileinfo_end(); it != end; ++it) {
            auto &&file = it->getFirst();
            auto &&fileId = sm.translateFile(file);
            auto &&includedBy = sm.getFileID(sm.getIncludeLoc(fileId));

            if (includedBy == rootId) {
                context.out().exportInclude(file);
            }
        }
    }

    bool PrimaryVisitor::declaredInMain(const clang::Decl *D) const {
        auto &&sm = context.getCompilationInstance().getSourceManager();
        return sm.getMainFileID() == sm.getFileID(D->getLocation());
    }
}