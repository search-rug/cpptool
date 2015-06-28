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

        /**
         * Determine the modifiers of a type and resolve a AST-specific type id of the base type.
         * Base types are: Records, Typedefs or Built-ins
         * References, Pointers and Arrays are stored as modifiers of the underlying base type.
         * If this is the first time the given type has been encountered, it will emit a TypeDefintion for that type.
         */
        void ResolveType(ct::proto::Type &target, clang::QualType type);

        /**
         * Uses the Clang SourceManager to map the given source location to its location in the original files.
         */
        void ResolveLocation(ct::proto::SourceRange &range, clang::SourceRange clangRange);

        /**
         * Resolves the name as well as the calling ResolveContext to resolve the context in which the name
         * was defined.
         */
        void ResolveName(ct::proto::ScopedName &name, clang::NamedDecl const &decl);

        /**
         * Unwraps the declaration context tree, if it encounters a lambda (lambda) it will call GetContext to resolve
         * the anonymous contexts between the given declaration and the root (global namespace)
         */
        void ResolveContext(ct::proto::ScopedName &name, clang::DeclContext const *context);

        /**
         * Performs a partial lookup of the associated type id of all the "type" template arguments.
         * Template arguments that are not types are ignored, while the associated type id of the types is inserted
         * into the given output set.
         */
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

        /**
         * Maps the given type to an uint64 that unique identifies the type for this AST tree.
         * The first time a type is encountered it will also be emitted as a TypeDefinition to the protobuffer stream.
         */
        PtrInt GetTypeId(clang::QualType type);

        /**
         * Performs the (potentially recursive) lookup of lambda contexts and where they belong, emits
         * IsolatedContextDefinitions when a new isolated context (lambda) is encountered.
         */
        void GetContext(ct::proto::ScopedName &name, clang::CXXRecordDecl const *lambdaContext);

        /**
         * Utility method to map the given type to the most accurate TypeDefinition possible.
         */
        void DetermineTypeDeclaration(ct::proto::TypeDefinition &def, clang::QualType type);
    };
}

#include "export/type_mapper.template.hpp"

#endif //CPPTOOL_TYPE_MAPPER_HPP
