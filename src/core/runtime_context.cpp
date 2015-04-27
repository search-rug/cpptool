#include "core/runtime_context.hpp"

namespace ct {
    RuntimeContext::RuntimeContext(
            std::unique_ptr<CTExport> exportInterface,
            clang::CompilerInstance const &compilationInstance
    ) : exportInterface(std::move(exportInterface)), compilationInstance(compilationInstance) {
    }

    CTExport const &RuntimeContext::export() const {
        return *exportInterface;
    }

    clang::ASTContext const &RuntimeContext::context() const {
        return compilationInstance.getASTContext();
    }

    clang::CompilerInstance const &RuntimeContext::getCompilationInstance() const {
        return compilationInstance;
    }

    RuntimeContext::RuntimeContext(RuntimeContext &&orig)
            : exportInterface(std::move(orig.exportInterface)),
              compilationInstance(orig.compilationInstance) {

    }
}