#pragma once

#ifndef CPPTOOL_TYPE_MAPPER_HPP
#define CPPTOOL_TYPE_MAPPER_HPP

#include <unordered_set>
#include <clang/AST/Type.h>
#include <clang/Frontend/CompilerInstance.h>
#include <cstdint>
#include <type_traits>

#include "base.pb.h"
#include "export/protobuf_output.hpp"

namespace ct {
    class TypeMapper {
		using PtrInt = ::google::protobuf::uint64;
		static_assert(
			sizeof(PtrInt) >= sizeof(clang::Type *), 
			"Container value must be large enough to contain any pointer."
		);

		ProtobufOutput &typeOutput;
		clang::CompilerInstance const &clang;
		std::unordered_set<PtrInt> seenTypes;
	public:
		TypeMapper(ProtobufOutput &typeOutput, clang::CompilerInstance const &clang);

		void ResolveType(ct::proto::Type &target, clang::QualType type);

		void ResolveLocation(ct::proto::SourceRange &range, clang::SourceRange &clangRange);

		void ResolveName(ct::proto::ScopedName &name, clang::NamedDecl const &decl);
	private:
		PtrInt GetTypeId(clang::QualType type);

		void DetermineTypeDeclaration(ct::proto::TypeDefinition &def, clang::QualType type);
    };
}

#endif //CPPTOOL_TYPE_MAPPER_HPP
