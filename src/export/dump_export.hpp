#pragma once

#ifndef CPPTOOL_DUMP_EXPORT_HPP
#define CPPTOOL_DUMP_EXPORT_HPP

#include "export/export_api.hpp"
#include <llvm/Support/raw_ostream.h>
#include <ostream>

namespace ct {
    class DumpExport : public CTExport {
        std::unique_ptr<llvm::raw_ostream> out;
    public:
        DumpExport(std::ostream &out);

        virtual void exportTagDecl(const clang::DeclContext *context, const clang::TagDecl *tag) const override;

        virtual void exportVarDecl(const clang::VarDecl *decl) const override;
    };
}

#endif //CPPTOOL_DUMP_EXPORT_HPP
