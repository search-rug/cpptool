#ifndef CPPTOOL_EXPORT_API_HPP
#define CPPTOOL_EXPORT_API_HPP

#include <clang/AST/AST.h>

namespace ct {
    class CTExport {
    public:
        virtual ~CTExport() = 0;

        virtual void exportTagDecl(const clang::DeclContext *context, const clang::TagDecl *tag) const = 0;

        virtual void exportVarDecl(const clang::VarDecl *decl) const = 0;

        virtual void exportInclude(const clang::FileEntry *include) const = 0;
    };
}

#endif //CPPTOOL_EXPORT_API_HPP
