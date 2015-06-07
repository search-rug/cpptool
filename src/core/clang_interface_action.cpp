#include "core/clang_interface.hpp"

namespace ct {

    CppToolAction::CppToolAction(ExportFactory const &&exportFactory, IdSet const &inputSet)
            : exportFactory(std::forward<ExportFactory const>(exportFactory)),
              inputSet(inputSet) {
    }

    std::unique_ptr<clang::ASTConsumer> CppToolAction::CreateASTConsumer(clang::CompilerInstance &compiler,
                                                                         llvm::StringRef inFile) {
        return std::unique_ptr<clang::ASTConsumer>(
                new CppToolConsumer(exportFactory(inFile, compiler), compiler, inputSet));
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

    std::unique_ptr<clang::tooling::FrontendActionFactory> buildActionFactory(ExportFactory const factory,
                                                                              IdSet const &&inputIds) {
        return std::unique_ptr<clang::tooling::FrontendActionFactory>(
                new CppToolActionFactory(factory, std::forward<IdSet const>(inputIds)));
    }

    IdSet const createInputIdSet(std::vector<std::string> inputFiles, clang::FileManager &fm) {
        IdSet ids;

        //Remove non-file entries from the input.
        //We can safely modify since its a value-copy of the original vector.
        inputFiles.erase(std::remove_if(std::begin(inputFiles), std::end(inputFiles), [&](std::string &file) {
            return fm.getFile(file) == nullptr;
        }), std::end(inputFiles));

        std::transform(std::begin(inputFiles), std::end(inputFiles), std::inserter(ids, std::end(ids)),
                       [&](std::string &file) {
                           //getFile can return nullptr
                           auto &&fileEntry = fm.getFile(file);
                           if (!fileEntry) { // sanity check, since a nullptr can easily result in corrupt data
                               llvm::outs() << "Unable to locate file: " << file << "\n";
                               llvm::outs().flush();
                               llvm_unreachable(
                                       "File has suddenly disappeared, failure to find one of the input files.");
                           }
                           return std::cref(fileEntry->getUniqueID());
                       });
        return ids;
    }
}