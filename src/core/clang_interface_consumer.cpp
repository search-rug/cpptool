#include "core/clang_interface.hpp"

namespace ct {

    CppToolConsumer::CppToolConsumer(std::unique_ptr<CTExport> param, clang::CompilerInstance &instance)
		: visitor(RuntimeContext(std::move(param), instance)) {
    }

    void CppToolConsumer::HandleTranslationUnit(clang::ASTContext &context) {
        visitor.TraverseTranslationUnitDecl(context.getTranslationUnitDecl());
    }
}