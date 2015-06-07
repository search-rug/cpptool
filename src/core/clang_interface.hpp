#pragma once

#ifndef CPPTOOL_CLANG_INTERFACE_HPP
#define CPPTOOL_CLANG_INTERFACE_HPP

#include <clang/Frontend/FrontendAction.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Tooling/Tooling.h>

#include <functional>
#include <vector>

#include "core/runtime_context.hpp"
#include "visitor/primary_visitor.hpp"
#include "core/ref_util.template.hpp"

namespace ct {
    using ExportFactory = std::function<std::unique_ptr<ct::CTExport>(llvm::StringRef const,
                                                                      clang::CompilerInstance const &)>;
    using IdSet = RefSet<llvm::sys::fs::UniqueID const>;

    class CppToolConsumer : public clang::ASTConsumer {
        PrimaryVisitor visitor;
    public:
        explicit CppToolConsumer(
                std::unique_ptr<CTExport> param,
                clang::CompilerInstance &instance,
                RefSet<llvm::sys::fs::UniqueID const> const &inputSet
        );

        virtual void HandleTranslationUnit(clang::ASTContext &context) override;
    };

    class CppToolAction : public clang::ASTFrontendAction {
        ExportFactory const exportFactory;
        IdSet const &inputSet;
    public:
        CppToolAction(ExportFactory const &&exportFactory, IdSet const &inputSet);

        virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                clang::CompilerInstance &compiler,
                llvm::StringRef inFile
        ) override;
    };

    std::unique_ptr<clang::tooling::FrontendActionFactory> buildActionFactory(ExportFactory const factory,
                                                                              IdSet const &&inputIds);

    IdSet const createInputIdSet(std::vector<std::string> inputFiles, clang::FileManager &fm);
}

#endif //CPPTOOL_CLANG_INTERFACE_HPP
