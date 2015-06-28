#ifndef CPPTOOL_EXPORT_API_HPP
#define CPPTOOL_EXPORT_API_HPP

#include <clang/AST/AST.h>

namespace ct {
    class CTExport {
    public:
        virtual ~CTExport() = 0;

        //Names use PascalCase to avoid using keywords

        //HANDLES: #include
        virtual void Include(clang::FileEntry const *origin, clang::FileEntry const *target) = 0;

        //HANDLES: class/struct/union
        virtual void Record(clang::RecordDecl const *record) = 0;

        //HANDLES: enum
        virtual void Enum(clang::EnumDecl const *enumDef) = 0;

        //HANDLES: function
        virtual void Function(clang::FunctionDecl const *function) = 0;

        //HANDLES: function parameters
        virtual void ParameterVariable(clang::ParmVarDecl const *param) = 0;

        //HANDLES: variable of a class
        virtual void MemberVariable(clang::FieldDecl const *field) = 0;

        //HANDLES: variable in an unbound scope (namespace or translationunit)
        virtual void GlobalVariable(clang::VarDecl const *var) = 0;

        //HANDLES: variable in local scope
        virtual void LocalVariable(clang::VarDecl const *var) = 0;

        //HANDLES: type remapping
        virtual void TypeDef(clang::TypedefNameDecl const *typeDefinition) = 0;

        //HANDLES: friend relation
        virtual void Friend(clang::FriendDecl const *friends) = 0;

        //HANDLES: template parameter
        //template parameters can be either attached to a type (record or alias)
        virtual void TemplateParam(clang::TemplateTypeParmDecl const *param, clang::TypeDecl const *owner) = 0;

        //or attached to a named decl (function/variable)
        virtual void TemplateParam(clang::TemplateTypeParmDecl const *param, clang::NamedDecl const *owner) = 0;

        //HANDLES: template
        //All types of template are handled seperately, no cleaner way of doing this
        //template<typename T> class X;
        virtual void Template(clang::ClassTemplateDecl const *Template) = 0;

        //template<typename T> T func(int a);
        virtual void Template(clang::FunctionTemplateDecl const *Template) = 0;

        //template<typename T> T n = T(10);
        virtual void Template(clang::VarTemplateDecl const *Template) = 0;

        //template<typename T> using Test = T;
        virtual void Template(clang::TypeAliasTemplateDecl const *Template) = 0;

        //change in input file
        virtual void InputChanged(clang::FileEntry const *file) = 0;
    };
}

#endif //CPPTOOL_EXPORT_API_HPP
