#ifndef CPPTOOL_EXPORT_API_HPP
#define CPPTOOL_EXPORT_API_HPP

#include <clang/AST/AST.h>

namespace ct {
    class CTExport {
    public:
        virtual ~CTExport() = 0;

        //Names use PascalCase to avoid using keywords

        //#include
		virtual void Include(clang::FileEntry const &origin, clang::FileEntry const &target) = 0;

        //class/struct/union
        virtual void Record(clang::RecordDecl const *record) = 0;

        //enum
        virtual void Enum(clang::EnumDecl const *enumDef) = 0;

        //function
        virtual void Function(clang::FunctionDecl const *function) = 0;

        //function parameters
        virtual void ParameterVariable(clang::ParmVarDecl const *param) = 0;

        //variable of a class
        virtual void MemberVariable(clang::FieldDecl const *field) = 0;

        //variable in an unbound scope (namespace or translationunit)
        virtual void GlobalVariable(clang::VarDecl const *var) = 0;

        //variable in local scope
        virtual void LocalVariable(clang::VarDecl const *var) = 0;

        //type remapping
        virtual void TypeDef(clang::TypedefNameDecl const *typeDefinition) = 0;

		//friend relations
		virtual void Friend(clang::FriendDecl const *friends) = 0;
    };
}

#endif //CPPTOOL_EXPORT_API_HPP
