#include "visitor/primary_visitor.hpp"

namespace ct {
	PrimaryVisitor::PrimaryVisitor(RuntimeContext context) : context(std::move(context)) {
    }

    bool PrimaryVisitor::VisitTagDecl(clang::TagDecl *D) {
        context.export().exportTagDecl(D->getDeclContext(), D);
        return true;
    }
}