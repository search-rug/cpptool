#include "export/export_util.hpp"

#include <llvm/Support/Casting.h>

namespace ct {
    namespace Export {

        std::string getFullIdentifier(clang::Decl *decl) {
            if (clang::FunctionDecl *func = clang::dyn_cast<clang::FunctionDecl>(decl)) {
                //TODO: does not take arguments/return into account.
                //Perhaps consider overloads to be dependencies of the same root function?
                return "::" + func->getQualifiedNameAsString();
            } else if (clang::NamedDecl *named = clang::dyn_cast<clang::NamedDecl>(decl)) {
                return "::" + named->getQualifiedNameAsString();
            } else if (decl == decl->getTranslationUnitDecl()) {
                return "::";
            } else {
                return getFullIdentifier(clang::Decl::castFromDeclContext(decl->getDeclContext()));
            }
        }

        std::string getContextIdentifier(clang::DeclContext const *context) {
            return getFullIdentifier(clang::Decl::castFromDeclContext(context));
        }
    }
}