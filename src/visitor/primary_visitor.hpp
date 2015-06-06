#pragma once

#ifndef CPPTOOL_PRIMARY_VISITOR_HPP
#define CPPTOOL_PRIMARY_VISITOR_HPP

#include <clang/AST/RecursiveASTVisitor.h>

#include "core/runtime_context.hpp"

namespace ct {
    class PrimaryVisitor : public clang::RecursiveASTVisitor<PrimaryVisitor> {
        RuntimeContext context;
        clang::FileID primaryFileId;
    public:
        PrimaryVisitor(RuntimeContext &&context);

        bool VisitFieldDecl(clang::FieldDecl *D);

        bool VisitVarDecl(clang::VarDecl *D);

        bool VisitTranslationUnitDecl(clang::TranslationUnitDecl *D);

        bool VisitFunctionDecl(clang::FunctionDecl *D);

        bool VisitEnumDecl(clang::EnumDecl *D);

        bool VisitRecordDecl(clang::RecordDecl *D);

		bool VisitTypedefNameDecl(clang::TypedefNameDecl *D);

		bool VisitFriendDecl(clang::FriendDecl *D);

        bool TraverseTranslationUnitDecl(clang::TranslationUnitDecl *D);

    private:
        void exportIncludes();

        inline bool declaredInMain(const clang::Decl *D) const;

        inline CTExport &out() const;
    };
}

#endif //CPPTOOL_PRIMARY_VISITOR_HPP
