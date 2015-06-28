#pragma once

#ifndef CPPTOOL_EXPORT_UTIL_HPP
#define CPPTOOL_EXPORT_UTIL_HPP

#include <string>
#include <sstream>

#include <clang/AST/AST.h>

/**
 * The functions contained in this header file are a set of utility functions to deterministically
 * name various aspects of the Clang AST
 */
namespace ct {
    namespace Export {
        /**
         * Get fully qualified name of the given declaration
         */
        std::string getFullIdentifier(clang::Decl const *decl);

        /**
         * Utility method that calls getFullIdentifier(Decl) on the given context
         */
        std::string getContextIdentifier(clang::DeclContext const *context);

        /**
         * Extract a string which represents all the types of the parameters, wrapped by [ and ]
         */
        std::string getParameterRepr(clang::FunctionDecl::param_const_range params);

        /**
         * Safely get the name of the function, as well as its parameters as a single string.
         * Also handles special cases such as class constructors/destructors.
         */
        std::string getFunctionName(clang::FunctionDecl const &func);

        /**
         * Delegates to getFunctionName(FunctionDecl &)
         */
        std::string getFunctionName(clang::FunctionDecl const *func);
    }
}

#endif //CPPTOOL_EXPORT_UTIL_HPP
