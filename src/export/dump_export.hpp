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

        virtual void Include(clang::FileEntry const *include) const;

        virtual void Record(clang::RecordDecl const *record) const;

        virtual void Enum(clang::EnumDecl const *enumDef) const;

        virtual void Function(clang::FunctionDecl const *function) const;

        virtual void ParameterVariable(clang::ParmVarDecl const *param) const;

        virtual void MemberVariable(clang::FieldDecl const *field) const;

        virtual void GlobalVariable(clang::VarDecl const *var) const;

        virtual void LocalVariable(clang::VarDecl const *var) const;

        virtual void TypeDef(clang::TypedefNameDecl const *typeDefinition) const;
    private:
        inline llvm::raw_ostream &OS() const;
    };
}

#endif //CPPTOOL_DUMP_EXPORT_HPP
