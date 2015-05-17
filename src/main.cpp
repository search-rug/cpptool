#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <iostream>

#include "core/clang_interface.hpp"
#include "export/dump_export.hpp"
#include "export/protobuf_export.hpp"

static llvm::cl::OptionCategory toolCat("CppTool options");
static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);

std::unique_ptr<ct::CTExport> createExportObject(llvm::StringRef const fileName, clang::CompilerInstance const &ci) {
    std::cout << "Creating export object for: " << fileName.str() << std::endl;
#ifdef CPPTOOL_DUMP_EXPORT
    return std::unique_ptr<ct::CTExport>(new ct::DumpExport(std::cout));
#else
	return std::unique_ptr<ct::CTExport>(new ct::ProtoBufExport(std::tmpfile(), fileName, ci));
#endif //CPPTOOL_DUMP_EXPORT
}

int main(int argc, const char *argv[]) {
    clang::tooling::CommonOptionsParser options(argc, argv, toolCat);
    clang::tooling::ClangTool tool(options.getCompilations(), options.getSourcePathList());
    return tool.run(ct::buildActionFactory(&createExportObject).get());
}