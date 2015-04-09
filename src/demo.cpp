#include "demo.h"

#include <iostream>
#include "clang/Lex/Preprocessor.h"

namespace ct {
    std::unique_ptr<clang::ASTConsumer> DemoAction::CreateASTConsumer(
                                                                      clang::CompilerInstance &compiler,
                                                                      llvm::StringRef inFile
                                                                      ) {
        return std::unique_ptr<clang::ASTConsumer>(new DemoConsumer(compiler));
    }
    
    DemoConsumer::DemoConsumer(clang::CompilerInstance &instance) : visitor(instance) {
    }
    
    void DemoConsumer::HandleTranslationUnit(clang::ASTContext &context) {
        visitor.printIncludes(std::cout);
        std::cout << std::endl << std::endl;
        visitor.printMacros(std::cout);
    }
    
    DemoVisitor::DemoVisitor(clang::CompilerInstance &instance) : instance(instance) {
    }
    
    void DemoVisitor::printMacros(std::ostream &out) {
        out << "--- Macros ---" << std::endl;
        
        auto &&pp = instance.getPreprocessor();
        const auto mainId = pp.getSourceManager().getMainFileID();
        
        for (auto it = pp.macro_begin(), end = pp.macro_end(); it != end; it++) {
            auto macro = it->getSecond()->getDefinition();
            
            //Go through the macro and its previous definitions
            do {
                auto id = pp.getSourceManager().getFileID(macro.getLocation());
                
                if (id == mainId) { //Avoid printing all of the defines in every single include
                    printMacro(*it->getFirst(), *macro.getMacroInfo(), pp, out);
                    out << std::endl;
                }
            } while ((macro = macro.getPreviousDefinition()));
        }
    }
    
    //http://clang.llvm.org/doxygen/PrintPreprocessedOutput_8cpp_source.html#l00674
    void DemoVisitor::printMacro(const clang::IdentifierInfo &II, const clang::MacroInfo &MI, clang::Preprocessor &PP, std::ostream &OS) {
        OS << "#define " << II.getNameStart();
        
        if (MI.isFunctionLike()) {
            OS << '(';
            if (!MI.arg_empty()) {
                auto AI = MI.arg_begin(), E = MI.arg_end();
                for (; AI + 1 != E; ++AI) {
                    OS << (*AI)->getNameStart();
                    OS << ',';
                }
                
                if (strncmp((*AI)->getNameStart(), "__VA_ARGS__", 11)) {
                    OS << "...";
                } else {
                    OS << (*AI)->getNameStart();
                }
            }
            
            OS << ')';
        }
        
        if (MI.tokens_empty() || !MI.tokens_begin()->hasLeadingSpace()) {
            OS << ' ';
        }
        
        clang::SmallString<128> buffer;
        for (auto it = MI.tokens_begin(), end=MI.tokens_end(); it != end; ++it) {
            if (it->hasLeadingSpace()) {
                OS << ' ';
            }
            
            OS << PP.getSpelling(*it, buffer).str();
        }
    }
    
    void DemoVisitor::printIncludes(std::ostream &out) {
        out << "--- Includes ---" << std::endl;
        
        auto &&sm = instance.getSourceManager();
        auto rootId = sm.getMainFileID();
        
        for (auto it = sm.fileinfo_begin(), end = sm.fileinfo_end(); it != end; ++it) {
            auto &&file = it->getFirst();
            auto &&fileId = sm.translateFile(file);
            auto &&includedBy = sm.getFileID(sm.getIncludeLoc(fileId));
            
            if (includedBy == rootId) {
                out << "#include \"" << file->getName() << '"' << std::endl;
            }
        }
    }
}