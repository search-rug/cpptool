#pragma once

#ifndef CPPTOOL_EXPORT_UTIL_HPP
#define CPPTOOL_EXPORT_UTIL_HPP

#include <string>

#include <clang/AST/AST.h>

namespace ct {
    namespace Export {
        std::string getFullIdentifier(clang::Decl const *decl);
        std::string getContextIdentifier(clang::DeclContext const *context);
    }
}

#endif //CPPTOOL_EXPORT_UTIL_HPP
