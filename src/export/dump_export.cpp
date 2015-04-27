#include "export/dump_export.hpp"

#include <llvm/Support/raw_os_ostream.h>

namespace ct {
    DumpExport::DumpExport(std::ostream &out) : out(std::unique_ptr<llvm::raw_ostream>(new llvm::raw_os_ostream(out))) {
    }

	DumpExport::~DumpExport() {
		*out << "Destructor called";
	}

    void DumpExport::exportTagDecl(const clang::DeclContext *context, const clang::TagDecl *tag) const {
        context->dumpLookups(*out);
        tag->dump(*out);
    }
}