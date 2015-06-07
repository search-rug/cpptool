#include "export/protobuf_export.hpp"

#include "wrapper.pb.h"

#include <clang/AST/DeclFriend.h>
#include <algorithm>

namespace ct {
	ProtoBufExport::ProtoBufExport(std::FILE *file, llvm::StringRef const targetName, clang::CompilerInstance const &ci)
		: outputFile(file), out(outputFile.get()), mapper(out, ci) {
		ct::proto::Prelude prelude;
		prelude.set_targetfile(targetName.str());
		out.writeMessage<ct::proto::Prelude>(prelude);
	}

	void ProtoBufExport::Include(clang::FileEntry const *origin, clang::FileEntry const *target) {
		exportData([&](ct::proto::Envelope &env) {
			auto incl = env.mutable_include();
			incl->set_origin(origin->getName());
			incl->set_target(target->getName());
		});
	}

	void ProtoBufExport::Record(clang::RecordDecl const *record) {
		exportData([&](ct::proto::Envelope &env) {
			ct::proto::RecordDef *rec;
			if (clang::CXXRecordDecl const *cxxRecord = clang::dyn_cast<clang::CXXRecordDecl>(record)) {
				auto cRecord = env.mutable_c_record();
				rec = cRecord->mutable_base();

				std::for_each(cxxRecord->bases_begin(), cxxRecord->bases_end(), [&](clang::CXXBaseSpecifier const &base) {
					mapper.ResolveType(*cRecord->add_parents(), base.getType());
				});
			} else {
				rec = env.mutable_record();
			}

			mapper.ResolveType(*rec->mutable_internal_type(), clang::QualType(record->getTypeForDecl(), 0));
			mapper.ResolveName(*rec->mutable_name(), *record);
			if (record->hasBody()) {
				mapper.ResolveLocation(*rec->mutable_definition(), record->getBody()->getSourceRange());
			}
		});
	}

	void ProtoBufExport::Enum(clang::EnumDecl const *enumDef) {
		exportData([&](ct::proto::Envelope &env) {
			auto e = env.mutable_enum_def();

			mapper.ResolveName(*e->mutable_name(), *enumDef);
			if (enumDef->hasBody()) {
				mapper.ResolveLocation(*e->mutable_definition(), enumDef->getBody()->getSourceRange());
			}
		});
	}

	void ProtoBufExport::Function(clang::FunctionDecl const *function) {
		exportData([&](ct::proto::Envelope &env) {
			ct::proto::FunctionDef *func;
			if (clang::CXXMethodDecl const *method = clang::dyn_cast<clang::CXXMethodDecl>(function)) {
				auto c_func = env.mutable_c_func();
				func = c_func->mutable_base();

				mapper.ResolveType(*c_func->mutable_parent(), clang::QualType(method->getParent()->getTypeForDecl(), 0));
				c_func->set_static_(method->isStatic());
				c_func->set_virtual_(method->isVirtual());
			} else {
				func = env.mutable_func();
			}

			mapper.ResolveName(*func->mutable_name(), *function);
			mapper.ResolveType(*func->mutable_return_type(), function->getReturnType());
			if (function->hasBody()) {
				mapper.ResolveLocation(*func->mutable_body(), function->getBody()->getSourceRange());
			}
		});
	}

	void ProtoBufExport::ParameterVariable(clang::ParmVarDecl const *param) {
		exportData([&](ct::proto::Envelope &env) {
			auto var = env.mutable_var();
			mapper.ResolveName(*var->mutable_name(), *param);
			mapper.ResolveType(*var->mutable_own_type(), param->getType());
			var->set_kind(ct::proto::Var_VarKind_PARAMETER);
		});
	}

	void ProtoBufExport::MemberVariable(clang::FieldDecl const *field) {
		exportData([&](ct::proto::Envelope &env) {
			auto c_var = env.mutable_c_var();
			mapper.ResolveType(*c_var->mutable_parent(), clang::QualType(field->getParent()->getTypeForDecl(), 0));

			auto var = c_var->mutable_base();
			mapper.ResolveName(*var->mutable_name(), *field);
			mapper.ResolveType(*var->mutable_own_type(), field->getType());
			var->set_kind(ct::proto::Var_VarKind_LOCAL);
		});
	}

	void ProtoBufExport::GlobalVariable(clang::VarDecl const *varDecl) {
		exportData([&](ct::proto::Envelope &env) {
			auto var = env.mutable_var();
			mapper.ResolveName(*var->mutable_name(), *varDecl);
			mapper.ResolveType(*var->mutable_own_type(), varDecl->getType());
			var->set_kind(ct::proto::Var_VarKind_GLOBAL);
		});
	}

	void ProtoBufExport::LocalVariable(clang::VarDecl const *varDecl) {
		exportData([&](ct::proto::Envelope &env) {
			auto var = env.mutable_var();
			mapper.ResolveName(*var->mutable_name(), *varDecl);
			mapper.ResolveType(*var->mutable_own_type(), varDecl->getType());
			var->set_kind(ct::proto::Var_VarKind_LOCAL);
		});
	}

	void ProtoBufExport::TypeDef(clang::TypedefNameDecl const *typeDefinition) {
		exportData([&](ct::proto::Envelope &env) {
			auto tdef = env.mutable_tdef();
			mapper.ResolveName(*tdef->mutable_name(), *typeDefinition);
			mapper.ResolveType(*tdef->mutable_mappedtype(), typeDefinition->getUnderlyingType());
		});
	}


	void ProtoBufExport::Friend(clang::FriendDecl const *friends) {
		exportData([&](ct::proto::Envelope &env) {
			auto fdef = env.mutable_friend_();

			if (clang::TagDecl const *tag = clang::dyn_cast<clang::TagDecl>(friends->getDeclContext())) {
				mapper.ResolveType(*fdef->mutable_origin(), clang::QualType(tag->getTypeForDecl(), 0));
			} else {
				llvm_unreachable("Unable to discover type for friend relation");
			}

			if (friends->getFriendType()) {
				mapper.ResolveType(*fdef->mutable_type_friend(), friends->getFriendType()->getType());
			} else {
				mapper.ResolveName(*fdef->mutable_friend_(), *friends->getFriendDecl());
			}
		});
	}

	void ProtoBufExport::TemplateParam(clang::TemplateTypeParmDecl const *param, clang::TypeDecl const *owner) {
		exportData([&](ct::proto::Envelope &env) {
			auto parm = env.mutable_tmpl_param();
			mapper.ResolveType(*parm->mutable_own_type(), clang::QualType(param->getTypeForDecl(), 0));
			mapper.ResolveType(*parm->mutable_owner_type(), clang::QualType(owner->getTypeForDecl(), 0));
		});
	}

	void ProtoBufExport::TemplateParam(clang::TemplateTypeParmDecl const *param, clang::NamedDecl const *owner) {
		exportData([&](ct::proto::Envelope &env) {
			auto parm = env.mutable_tmpl_param();
			mapper.ResolveType(*parm->mutable_own_type(), clang::QualType(param->getTypeForDecl(), 0));
			mapper.ResolveName(*parm->mutable_owner_name(), *owner);
		});
	}

	void ProtoBufExport::Template(clang::ClassTemplateDecl const *Template) {
		std::unordered_set<TypeMapper::PtrInt> specialized_types;
		gatherSpecializationTypes<clang::ClassTemplateDecl>(specialized_types, *Template);

		exportData([&](ct::proto::Envelope &env) {
			auto tmpl = env.mutable_tmpl();
			mapper.ResolveType(*tmpl->mutable_attached_type(), clang::QualType(Template->getTemplatedDecl()->getTypeForDecl(), 0));
			for (auto id : specialized_types) {
				tmpl->add_specializations()->set_type_id(id);
			}
		});
	}

	void ProtoBufExport::Template(clang::FunctionTemplateDecl const *Template) {
		std::unordered_set<TypeMapper::PtrInt> specialized_types;
		gatherSpecializationTypes<clang::FunctionTemplateDecl>(specialized_types, *Template);

		exportData([&](ct::proto::Envelope &env) {
			auto tmpl = env.mutable_tmpl();
			mapper.ResolveName(*tmpl->mutable_attached_name(), *Template->getTemplatedDecl());
			for (auto id : specialized_types) {
				tmpl->add_specializations()->set_type_id(id);
			}
		});
	}

	void ProtoBufExport::Template(clang::VarTemplateDecl const *Template) {
		std::unordered_set<TypeMapper::PtrInt> specialized_types;
		gatherSpecializationTypes<clang::VarTemplateDecl>(specialized_types, *Template);

		exportData([&](ct::proto::Envelope &env) {
			auto tmpl = env.mutable_tmpl();
			mapper.ResolveName(*tmpl->mutable_attached_name(), *Template->getTemplatedDecl());
			for (auto id : specialized_types) {
				tmpl->add_specializations()->set_type_id(id);
			}
		});
	}

	void ProtoBufExport::Template(clang::TypeAliasTemplateDecl const *Template) {
		Template->dumpColor();

		//Does not have access to specialization information?
	}

}