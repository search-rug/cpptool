#include "visitor/primary_visitor.hpp"

namespace ct {
    PrimaryVisitor::PrimaryVisitor(RuntimeContext &&context) : context(std::move(context)) {
    }

    bool PrimaryVisitor::VisitTagDecl(clang::TagDecl *D) {
        context.out().exportTagDecl(D->getDeclContext(), D);
        return true;
    }

    bool PrimaryVisitor::VisitVarDecl(clang::VarDecl *D) {
        context.out().exportVarDecl(D);
        return true;
    }
}