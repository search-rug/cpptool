#pragma once

#ifndef CPPTOOL_RUNTIME_CONTEXT_HPP
#define CPPTOOL_RUNTIME_CONTEXT_HPP

#include <clang/Frontend/CompilerInstance.h>
#include "export/export_api.hpp"

namespace ct {
    class RuntimeContext {
        std::unique_ptr<CTExport> exportInterface;
        clang::CompilerInstance const &compilationInstance;
    public:
        RuntimeContext(
                std::unique_ptr<CTExport> exportInterface,
                clang::CompilerInstance const &compilationInstance
        );

        RuntimeContext(RuntimeContext &&orig);

        /**
         * Exporting interface used for processing the data
         */
        CTExport &out() const;

        /**
         * The ASTContext provides access to variables specific to the current compilation run, including lookup
         * methods and ways of retrieving language built-in types.
         */
        clang::ASTContext const &context() const;

        /**
         * The compilation context contains all the Clang and LLVM subsystems used in the parsing and building
         * process.
         */
        clang::CompilerInstance const &getCompilationInstance() const;
    };
}

#endif //CPPTOOL_RUNTIME_CONTEXT_HPP
