#pragma once

#ifndef CPPTOOL_PRIMARY_VISITOR_HPP
#define CPPTOOL_PRIMARY_VISITOR_HPP

#include <clang/AST/RecursiveASTVisitor.h>

#include "core/runtime_context.hpp"

namespace ct {
    class PrimaryVisitor : public clang::RecursiveASTVisitor<PrimaryVisitor> {
        RuntimeContext context;
    public:
        PrimaryVisitor(RuntimeContext &&context);

        bool VisitTagDecl(clang::TagDecl *D);

        bool VisitVarDecl(clang::VarDecl *D);

        bool VisitTranslationUnitDecl(clang::TranslationUnitDecl *D);
    private:
        void exportIncludes();

        inline bool declaredInMain(const clang::Decl *D) const;
    };
}

#endif //CPPTOOL_PRIMARY_VISITOR_HPP
