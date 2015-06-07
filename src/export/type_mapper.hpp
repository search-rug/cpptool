#pragma once

#ifndef CPPTOOL_TYPE_MAPPER_HPP
#define CPPTOOL_TYPE_MAPPER_HPP

#include <unordered_set>
#include <unordered_map>
#include <clang/AST/Type.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/AST/DeclTemplate.h>
#include <cstdint>
#include <type_traits>

#include "base.pb.h"
#include "export/protobuf_output.hpp"

namespace ct {
    class TypeMapper {
    public:
        using PtrInt = ::google::protobuf::uint64;

        TypeMapper(ProtobufOutput &typeOutput, clang::CompilerInstance const &clang);

        void ResolveType(ct::proto::Type &target, clang::QualType type);

        void ResolveLocation(ct::proto::SourceRange &range, clang::SourceRange clangRange);

        void ResolveName(ct::proto::ScopedName &name, clang::NamedDecl const &decl);

        void ResolveContext(ct::proto::ScopedName &name, clang::DeclContext const *context);

        void ResolveTemplateArgs(clang::TemplateArgumentList const &list, std::unordered_set<PtrInt> &out);

    private:
        static_assert(
                sizeof(PtrInt) >= sizeof(clang::Type *),
                "Container value must be large enough to contain any pointer."
        );

        ProtobufOutput &typeOutput;
        clang::CompilerInstance const &clang;
        std::unordered_set<PtrInt> seenTypes;
        std::unordered_map<PtrInt, std::string> emittedContexts;

        PtrInt GetTypeId(clang::QualType type);

        void GetContext(ct::proto::ScopedName &name, clang::CXXRecordDecl const *lambdaContext);

        void DetermineTypeDeclaration(ct::proto::TypeDefinition &def, clang::QualType type);
    };
}

#include "export/type_mapper.template.hpp"

#endif //CPPTOOL_TYPE_MAPPER_HPP
