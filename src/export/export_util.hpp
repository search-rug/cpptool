#pragma once

#ifndef CPPTOOL_EXPORT_UTIL_HPP
#define CPPTOOL_EXPORT_UTIL_HPP

#include <string>
#include <sstream>

#include <clang/AST/AST.h>

namespace ct {
    namespace Export {
        std::string getFullIdentifier(clang::Decl const *decl);

        std::string getContextIdentifier(clang::DeclContext const *context);

        std::string getParameterRepr(clang::FunctionDecl::param_const_range params);

        std::string getFunctionName(clang::FunctionDecl const *func);

        struct Internal {
            static void unpackIdentifier(std::ostringstream &out, clang::Decl const *decl);

            static void unpackParameterRepr(std::ostringstream &out, clang::FunctionDecl::param_const_range params);
        };
    }
}

#endif //CPPTOOL_EXPORT_UTIL_HPP
