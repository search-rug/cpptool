#pragma once

#ifndef CPPTOOL_CLANG_INTERFACE_HPP
#define CPPTOOL_CLANG_INTERFACE_HPP

#include <clang/Frontend/FrontendAction.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Tooling/Tooling.h>

#include <functional>

#include "core/runtime_context.hpp"
#include "visitor/primary_visitor.hpp"

namespace ct {
    using ExportFactory = std::function<std::unique_ptr<ct::CTExport>(llvm::StringRef)>;

    class CppToolConsumer : public clang::ASTConsumer {
        PrimaryVisitor visitor;
    public:
        explicit CppToolConsumer(std::unique_ptr<CTExport> param, clang::CompilerInstance &instance);

        virtual void HandleTranslationUnit(clang::ASTContext &context) override;
    };

    class CppToolAction : public clang::ASTFrontendAction {
        ExportFactory const exportFactory;
    public:
        CppToolAction(ExportFactory const &&exportFactory);

        virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                clang::CompilerInstance &compiler,
                llvm::StringRef inFile
        ) override;
    };

    std::unique_ptr<clang::tooling::FrontendActionFactory> buildActionFactory(ExportFactory const factory);
}

#endif //CPPTOOL_CLANG_INTERFACE_HPP
