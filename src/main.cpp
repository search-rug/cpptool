#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Basic/Version.h>

#include <iostream>

#include "core/clang_interface.hpp"
#include "export/protobuf_export.hpp"

using namespace llvm;

static cl::OptionCategory toolCat("CppTool options");
static cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);

static cl::opt<std::string> OutputDirectory("out",
                                            cl::desc("Output directory for the binary data files"),
                                            cl::value_desc("directory"),
                                            cl::init("data_out"),
                                            cl::cat(toolCat)
);

inline char os_filesep() {
#ifdef _WIN32
	return '\\';
#else
    return '/';
#endif //_WIN32
}

llvm::SmallString<1024> getAbsoluteOutputPath() {
    llvm::SmallString<1024> tmp;
    tmp.assign(OutputDirectory);
    llvm::sys::fs::make_absolute(tmp);

    //Ensure it ends with a file separator
    if (!(tmp.endswith("/") && tmp.endswith("\\"))) {
        tmp.append(1, os_filesep());
    }

    return tmp;
}

llvm::SmallString<1024> &cachedAbsolutePath() {
    // Cached as a static variable since it cannot change once we reach this point
    static llvm::SmallString<1024> outputDirectory = getAbsoluteOutputPath();
    return outputDirectory;
}

std::FILE *generateOutputFile(llvm::StringRef const fileName) {
    using namespace llvm::sys;

    //Get last part of the path
    auto targetFileName = fileName.rsplit(os_filesep()).second.str();

    //Add last part to the output directory and add a unique identifier token
    Twine const nonUniqueFile = Twine(cachedAbsolutePath()).concat(targetFileName).concat("-%%-%%-%%-%%.pb");

    //Ensure the name is unique.
    llvm::SmallString<1024> tmpFile;
    auto err = fs::createUniqueFile(nonUniqueFile, tmpFile);

    if (err) {
        llvm::errs() << "Error creating binary output fileName" << err.message() << "\n";
        llvm_unreachable("Check the permissions on the output directory");
    }

    std::FILE *ptr = std::fopen(tmpFile.c_str(), "w");
    if (!ptr) {
        llvm::outs() << "fopen failed: " << std::strerror(errno) << "\n";
        llvm_unreachable("Check the permissions on the output directory");
    } else {
        return ptr;
    }
}

std::unique_ptr<ct::CTExport> createBinaryExport(llvm::StringRef const fileName, clang::CompilerInstance const &ci) {
    llvm::outs() << "EXPORT: " << fileName.str() << "\n";
    return std::unique_ptr<ct::CTExport>(new ct::ProtoBufExport(generateOutputFile(fileName), fileName, ci));
}

int main(int argc, const char *argv[]) {
    llvm::outs() << clang::getClangToolFullVersion("CppTool - clang") << "\n";

    clang::tooling::CommonOptionsParser options(argc, argv, toolCat);
    clang::tooling::ClangTool tool(options.getCompilations(), options.getSourcePathList());

    {
        //Ensure binary export directory is a directory and exists
        using namespace llvm::sys;
        Twine const directory(cachedAbsolutePath());

        if (directory.isTriviallyEmpty()) {
            llvm::errs() << "Output directory cannot be unspecified.\n";
        } else if (fs::exists(directory) && !fs::is_directory(directory)) {
            //Ensure its a directory
            llvm::errs() << "Given output directory \"" << OutputDirectory << "\" is not a directory.\n";
            exit(EXIT_FAILURE);
        } else {
            auto err = fs::create_directory(directory);
            if (err) {
                llvm::errs() << "Error whilst creating output directory: " << err.message() << "\n";
                exit(err.value());
            }
        }

        llvm::SmallString<1024> tmpFile;
        directory.toVector(tmpFile);
        fs::make_absolute(tmpFile);
        llvm::outs() << "Using output directory: \"" << tmpFile << "\"\n";
    }

    llvm::outs().flush();

    return tool.run(ct::buildActionFactory(
            &createBinaryExport,
            ct::createInputIdSet(options.getSourcePathList(), tool.getFiles())
    ).get());
}