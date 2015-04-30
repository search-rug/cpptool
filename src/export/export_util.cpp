#include "export/export_util.hpp"

namespace ct {
    namespace Export {

        std::string getFullIdentifier(clang::Decl *decl) {
            std::ostringstream out;
            out << '"';
            Internal::unpackIdentifier(out, decl);
            out << '"';
            return out.str();
        }

        std::string getContextIdentifier(clang::DeclContext const *context) {
            return getFullIdentifier(clang::Decl::castFromDeclContext(context));
        }

        void Internal::unpackIdentifier(std::ostringstream &out, clang::Decl const *decl) {
            using clang::Decl;

            if (decl->getKind() == Decl::Kind::TranslationUnit) return;

            //Output parent context
            Internal::unpackIdentifier(out, clang::Decl::castFromDeclContext(decl->getDeclContext()));

            out << "::";

            if (clang::FunctionDecl const *func = clang::dyn_cast<clang::FunctionDecl>(decl)) {
                out << getFunctionName(func) << '(';
                unpackParameterRepr(out, func->params());
                out << ')';
            } else if (clang::NamedDecl const *named = clang::dyn_cast<clang::NamedDecl>(decl)) {
                out << named->getName().str();
            } else {
                out << "{unknown}";
            }
        }

        void Internal::unpackParameterRepr(std::ostringstream &out, clang::FunctionDecl::param_const_range params) {
            bool firstDone = false;
            for (auto param : params) {
                if (firstDone) {
                    out << ',';
                } else {
                    firstDone = true;
                }

                out << param->getType().getAsString();
            }
        }

        std::string getParameterRepr(clang::FunctionDecl::param_const_range params) {
            std::ostringstream out;
            out << '[';
            Internal::unpackParameterRepr(out, params);
            out << ']';
            return out.str();
        }

        std::string getFunctionName(clang::FunctionDecl const *func) {
            auto name = func->getDeclName();
            if (name.isIdentifier()) {
                return name.getAsIdentifierInfo()->getName().str();
            } else {
                return name.getAsString();
            }
        }
    }
}