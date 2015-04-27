#pragma once

#ifndef CPPTOOL_PRIMARY_VISITOR_HPP
#define CPPTOOL_PRIMARY_VISITOR_HPP

#include <clang/AST/RecursiveASTVisitor.h>

#include "core/runtime_context.hpp"

namespace ct {
    class PrimaryVisitor : public clang::RecursiveASTVisitor<PrimaryVisitor> {
        RuntimeContext context;
    public:
        PrimaryVisitor(RuntimeContext context);

        bool VisitTagDecl(clang::TagDecl *D);
    };
}

#endif //CPPTOOL_PRIMARY_VISITOR_HPP
