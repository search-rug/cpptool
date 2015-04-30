#include "export/dump_export.hpp"

#include <llvm/Support/raw_os_ostream.h>

#include "export/export_util.hpp"

namespace ct {
    DumpExport::DumpExport(std::ostream &out) : out(std::unique_ptr<llvm::raw_ostream>(new llvm::raw_os_ostream(out))) {
    }

    void DumpExport::Include(clang::FileEntry const *include) const {
        OS() << "Include{path=\"" << include->getName() << "\"}\n";
    }

    void DumpExport::Record(clang::RecordDecl const *record) const {
        using clang::TagDecl;
        std::string type;
        switch (record->getTagKind()) {
            case TagDecl::TagKind::TTK_Class:
                type = "class";
                break;
            case TagDecl::TagKind::TTK_Struct:
                type = "struct";
                break;
            case TagDecl::TagKind::TTK_Union:
                type = "union";
                break;
            default:
                llvm_unreachable("Unhandled TagKind");
        }

        OS() << "Record{where=" << Export::getContextIdentifier(record->getDeclContext())
        << ",name=" << record->getName()
        << ",type=" << type << "}\n";
    }

    void DumpExport::Enum(clang::EnumDecl const *enumDef) const {
        OS() << "Enum{where=" << Export::getContextIdentifier(enumDef->getDeclContext())
        << ",name=" << enumDef->getName() << "}\n";
    }

    void DumpExport::Function(clang::FunctionDecl const *function) const {
        bool isClassMember = clang::isa<clang::CXXMethodDecl>(function);
        OS() << "Function{where=" << Export::getContextIdentifier(function->getDeclContext())
        << ",name=" << function->getName()
        << ",params=" << Export::getParameterRepr(function->params())
        << ",class_function=" << (isClassMember ? "true" : "false") << "}\n";
    }

    void DumpExport::MemberVariable(clang::FieldDecl const *field) const {
        OS() << "MemberField{where=" << Export::getContextIdentifier(field->getDeclContext())
        << ",name=" << field->getName()
        << ",type=" << field->getType().getAsString() << "}\n";
    }

    void DumpExport::GlobalVariable(clang::VarDecl const *var) const {
        OS() << "GlobalVar{where=" << Export::getContextIdentifier(var->getDeclContext())
        << ",name=" << var->getName()
        << ",type=" << var->getType().getAsString() << "}\n";
    }

    void DumpExport::LocalVariable(clang::VarDecl const *var) const {
        OS() << "LocalVar{where=" << Export::getContextIdentifier(var->getDeclContext())
        << ",name=" << var->getName()
        << ",type=" << var->getType().getAsString() << "}\n";
    }

    void DumpExport::ParameterVariable(clang::ParmVarDecl const *param) const {
        OS() << "Parameter{where=" << Export::getContextIdentifier(param->getDeclContext())
        << ",name=" << param->getName()
        << ",type=" << param->getType().getAsString() << "}\n";
    }

    void DumpExport::TypeDef(clang::TypedefNameDecl const *typeDefinition) const {
        OS() << "Typedef{where=" << Export::getContextIdentifier(typeDefinition->getDeclContext())
        << ",from=" << typeDefinition->getUnderlyingType().getAsString()
        << ",to=" << typeDefinition->getName() << "}\n";
    }

    llvm::raw_ostream &DumpExport::OS() const {
        return *out;
    }
}