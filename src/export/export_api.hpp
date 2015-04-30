#ifndef CPPTOOL_EXPORT_API_HPP
#define CPPTOOL_EXPORT_API_HPP

#include <clang/AST/AST.h>

namespace ct {
    class CTExport {
    public:
        virtual ~CTExport() = 0;

        //Names use PascalCase to avoid using keywords

        //#include
        virtual void Include(clang::FileEntry const *include) const = 0;

        //class/struct/union
        virtual void Record(clang::RecordDecl const *record) const = 0;

        //enum
        virtual void Enum(clang::EnumDecl const *enumDef) const = 0;

        //function
        virtual void Function(clang::FunctionDecl const *function) const = 0;

        //function parameters
        virtual void ParameterVariable(clang::ParmVarDecl const *param) const = 0;

        //variable of a class
        virtual void MemberVariable(clang::FieldDecl const *field) const = 0;

        //variable in an unbound scope (namespace or translationunit)
        virtual void GlobalVariable(clang::VarDecl const *var) const = 0;

        //variable in local scope
        virtual void LocalVariable(clang::VarDecl const *var) const = 0;

        //type remapping
        virtual void TypeDef(clang::TypedefNameDecl const *typeDefinition) const = 0;
    };
}

#endif //CPPTOOL_EXPORT_API_HPP
