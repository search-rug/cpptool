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

        CTExport &out() const;

        clang::ASTContext const &context() const;

        clang::CompilerInstance const &getCompilationInstance() const;
    };
}

#endif //CPPTOOL_RUNTIME_CONTEXT_HPP
