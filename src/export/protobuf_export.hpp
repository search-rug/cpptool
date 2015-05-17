#pragma once

#ifndef CPPTOOL_PROTOBUF_EXPORT_HPP
#define CPPTOOL_PROTOBUF_EXPORT_HPP

#include <memory>

#include <clang/Frontend/CompilerInstance.h>

#include "export/export_api.hpp"
#include "export/protobuf_output.hpp"
#include "export/type_mapper.hpp"

namespace ct {
    class ProtoBufExport : public CTExport {
    public:
		ProtoBufExport(std::FILE *file, llvm::StringRef const targetName, clang::CompilerInstance const &ci);

        virtual void Include(clang::FileEntry const *include);

        virtual void Record(clang::RecordDecl const *record);

        virtual void Enum(clang::EnumDecl const *enumDef);

        virtual void Function(clang::FunctionDecl const *function);

        virtual void ParameterVariable(clang::ParmVarDecl const *param);

        virtual void MemberVariable(clang::FieldDecl const *field);

        virtual void GlobalVariable(clang::VarDecl const *var);

        virtual void LocalVariable(clang::VarDecl const *var);

        virtual void TypeDef(clang::TypedefNameDecl const *typeDefinition);
	private:
		struct file_deleter {
			void operator()(std::FILE *file) {
				std::fclose(file); //TODO: track delete
			}
		};

		std::unique_ptr<std::FILE, file_deleter> outputFile;
		ProtobufOutput out;
		TypeMapper mapper;

		template<typename Callable>
		inline void exportData(Callable c) {
			ct::proto::Envelope env;
			c(env);
			out.writeMessage<ct::proto::Envelope>(env);
		}
    };
}

#endif //CPPTOOL_PROTOBUF_EXPORT_HPP
