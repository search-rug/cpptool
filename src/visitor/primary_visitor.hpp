#pragma once

#ifndef CPPTOOL_PRIMARY_VISITOR_HPP
#define CPPTOOL_PRIMARY_VISITOR_HPP

#include <clang/AST/RecursiveASTVisitor.h>
#include <set>
#include <functional>

#include "core/runtime_context.hpp"
#include "core/ref_util.template.hpp"

namespace ct {
	template<typename T>
	using RefSet = std::set<std::reference_wrapper<T>>;

    class PrimaryVisitor : public clang::RecursiveASTVisitor<PrimaryVisitor> {

        RuntimeContext context;
        clang::FileID primaryFileId;
		RefSet<llvm::sys::fs::UniqueID const> const &inputFiles;
    public:
		PrimaryVisitor(RuntimeContext &&context, RefSet<llvm::sys::fs::UniqueID const> const &inputFiles);

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

        inline bool shouldTraverse(const clang::Decl *D) const;

        inline CTExport &out() const;
    };
}

#endif //CPPTOOL_PRIMARY_VISITOR_HPP
