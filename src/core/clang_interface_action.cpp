#include "core/clang_interface.hpp"

namespace ct {

    CppToolAction::CppToolAction(ExportFactory const &&exportFactory)
            : exportFactory(std::forward<ExportFactory const>(exportFactory)) {
    }

    std::unique_ptr<clang::ASTConsumer> CppToolAction::CreateASTConsumer(clang::CompilerInstance &compiler,
                                                                         llvm::StringRef inFile) {
        return std::unique_ptr<clang::ASTConsumer>(new CppToolConsumer(exportFactory(inFile), compiler));
    }

    class CppToolActionFactory : public clang::tooling::FrontendActionFactory {
        ExportFactory const exportFactory;
    public:
        CppToolActionFactory(ExportFactory const factory) : exportFactory(factory) {
        }

        virtual clang::FrontendAction *create() {
            return new CppToolAction(std::forward<ExportFactory const>(exportFactory));
        }
    };

    std::unique_ptr<clang::tooling::FrontendActionFactory> buildActionFactory(ExportFactory const factory) {
        return std::unique_ptr<clang::tooling::FrontendActionFactory>(new CppToolActionFactory(factory));
    }
}