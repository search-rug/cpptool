#include "core/clang_interface.hpp"

namespace ct {

    CppToolConsumer::CppToolConsumer(std::unique_ptr<CTExport> param, clang::CompilerInstance &instance, RefSet<llvm::sys::fs::UniqueID const> const &inputSet)
		: visitor(RuntimeContext(std::move(param), instance), inputSet) {
    }

    void CppToolConsumer::HandleTranslationUnit(clang::ASTContext &context) {
        visitor.TraverseTranslationUnitDecl(context.getTranslationUnitDecl());
    }
}