#include "export/dump_export.hpp"

#include <llvm/Support/raw_os_ostream.h>

#include "export/export_util.hpp"

namespace ct {
    DumpExport::DumpExport(std::ostream &out) : out(std::unique_ptr<llvm::raw_ostream>(new llvm::raw_os_ostream(out))) {
    }

    void DumpExport::exportTagDecl(const clang::DeclContext *context, const clang::TagDecl *tag) const {
        (*out) << "TagDecl{"
                "context={"
                "name=" << Export::getContextIdentifier(context) << ","
                "type=" << context->getDeclKindName() <<
        "},decl={"
                "name=" << tag->getDeclName() << ","
                "type=" << clang::Decl::castToDeclContext(tag)->getDeclKindName() <<
        "}"
                "}\n";
    }

    void DumpExport::exportVarDecl(const clang::VarDecl *decl) const {
        (*out) << "VarDecl{where={name=" << Export::getContextIdentifier(decl->getDeclContext()) << "},what={type="
        << decl->getType().getAsString() << ",name=" << decl->getNameAsString() << ",kind="
        << decl->getDeclKindName() << "}}\n";
    }

    void DumpExport::exportInclude(clang::FileEntry const *include) const {
        (*out) << "Include{path=\"" << include->getName() << "\"}\n";
    }
}