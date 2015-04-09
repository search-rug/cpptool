#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "demo.h"

#define USELESS_DEFINE 2
#define USELESS_DEFINE 3

static llvm::cl::OptionCategory toolCat("CppTool options");

int main(int argc, const char *argv[]) {
    clang::tooling::CommonOptionsParser options(argc, argv, toolCat);
    clang::tooling::ClangTool tool(options.getCompilations(), options.getSourcePathList());
    return tool.run(clang::tooling::newFrontendActionFactory<ct::DemoAction>().get());
}