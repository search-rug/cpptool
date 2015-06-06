#include "core/clang_interface.hpp"

namespace ct {

	CppToolAction::CppToolAction(ExportFactory const &&exportFactory, IdSet const &inputSet)
            : exportFactory(std::forward<ExportFactory const>(exportFactory)),
			inputSet(inputSet) {
    }

    std::unique_ptr<clang::ASTConsumer> CppToolAction::CreateASTConsumer(clang::CompilerInstance &compiler,
                                                                         llvm::StringRef inFile) {
        return std::unique_ptr<clang::ASTConsumer>(new CppToolConsumer(exportFactory(inFile, compiler), compiler, inputSet));
    }

    class CppToolActionFactory : public clang::tooling::FrontendActionFactory {
        ExportFactory const exportFactory;
		IdSet const inputIds; //Ids will be stored here for the lifetime of this tool
    public:
		CppToolActionFactory(ExportFactory const factory, IdSet const &&inputIds) 
			: exportFactory(factory), 
			inputIds(std::forward<IdSet const>(inputIds)) {
        }

        virtual clang::FrontendAction *create() {
            return new CppToolAction(std::forward<ExportFactory const>(exportFactory), inputIds);
        }
    };

    std::unique_ptr<clang::tooling::FrontendActionFactory> buildActionFactory(ExportFactory const factory, IdSet const &&inputIds) {
        return std::unique_ptr<clang::tooling::FrontendActionFactory>(new CppToolActionFactory(factory, std::forward<IdSet const>(inputIds)));
    }

	IdSet const createInputIdSet(std::vector<std::string> inputFiles, clang::FileManager &fm) {
		IdSet ids;
		std::transform(std::begin(inputFiles), std::end(inputFiles), std::inserter(ids, std::end(ids)), [&](std::string &file) {
			//getFile can return nullptr, but we can presume the input files exist...
			return std::cref(fm.getFile(file)->getUniqueID());
		});
		return ids;
	}
}