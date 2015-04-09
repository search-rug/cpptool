#pragma once

#include <ostream>

#include "clang/Frontend/FrontendAction.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"

namespace ct {
    class DemoAction : public clang::ASTFrontendAction {
    public:
        virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                                                                      clang::CompilerInstance &compiler,
                                                                      llvm::StringRef inFileˆ
                                                                      ) override;
    };
    
    class DemoVisitor : public clang::RecursiveASTVisitor<DemoVisitor> {
        clang::CompilerInstance &instance;
    public:
        explicit DemoVisitor(clang::CompilerInstance &instance);
        
        void printIncludes(std::ostream &out);
        
        void printMacros(std::ostream &out);
        
    private:
        void printMacro(const clang::IdentifierInfo &II, const clang::MacroInfo &MI,
                        clang::Preprocessor &PP, std::ostream &OS);
    };
    
    class DemoConsumer : public clang::ASTConsumer {
        DemoVisitor visitor;
        
    public:
        explicit DemoConsumer(clang::CompilerInstance &instance);
        
        virtual void HandleTranslationUnit(clang::ASTContext &context) override;
    };
}