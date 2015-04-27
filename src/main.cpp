#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <iostream>

#include "core/clang_interface.hpp"
#include "export/dump_export.hpp"

static llvm::cl::OptionCategory toolCat("CppTool options");

std::unique_ptr<ct::CTExport> createExportObject(llvm::StringRef fileName) {
    std::cout << "Creating export object for: " << fileName.str() << std::endl;
    return std::unique_ptr<ct::CTExport>(new ct::DumpExport(std::cout));
}

int main(int argc, const char *argv[]) {
    //clang::tooling::CommonOptionsParser options(argc, argv, toolCat);
    //clang::tooling::ClangTool tool(options.getCompilations(), options.getSourcePathList());

	auto action = ct::buildActionFactory(&createExportObject).get()->create();
	clang::tooling::runToolOnCode(action, argv[1]);

    //return tool.run(ct::buildActionFactory(&createExportObject).get());
}