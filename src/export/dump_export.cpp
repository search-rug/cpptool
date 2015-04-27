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
}