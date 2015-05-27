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

		virtual void Include(clang::FileEntry const &origin, clang::FileEntry const &target);

        virtual void Record(clang::RecordDecl const *record);

        virtual void Enum(clang::EnumDecl const *enumDef);

        virtual void Function(clang::FunctionDecl const *function);

        virtual void ParameterVariable(clang::ParmVarDecl const *param);

        virtual void MemberVariable(clang::FieldDecl const *field);

        virtual void GlobalVariable(clang::VarDecl const *var);

        virtual void LocalVariable(clang::VarDecl const *var);

        virtual void TypeDef(clang::TypedefNameDecl const *typeDefinition);
    private:
        inline llvm::raw_ostream &OS() const;
    };
}

#endif //CPPTOOL_DUMP_EXPORT_HPP
