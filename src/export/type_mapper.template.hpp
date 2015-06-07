#ifndef CPPTOOL_EXPORT_TYPEMAPPER_TEMPLATE_HPP
#define CPPTOOL_EXPORT_TYPEMAPPER_TEMPLATE_HPP

#include "base.pb.h"
#include "export/type_mapper.hpp"

namespace ct {
    namespace TypeMapperTemplate {
        template<typename T>
        auto mapType(ct::TypeMapper &mapper, ct::proto::TypeDefinition &out,
                     typename std::remove_reference<T>::type const &type, int)
        -> decltype(type.getDecl(), void()) { //SFINAE causes this method to be invalid if the method getDecl() does not exist.
            mapper.ResolveName(*out.mutable_specifier(), *type.getDecl());
            mapper.ResolveLocation(*out.mutable_location(), type.getDecl()->getSourceRange());
        }

        template<typename T>
        void mapType(ct::TypeMapper &mapper, ct::proto::TypeDefinition &out,
                     typename std::remove_reference<T>::type const &type, long) {
            out.mutable_specifier()->set_name(type.getLocallyUnqualifiedSingleStepDesugaredType().getAsString());
        }


        template<typename T>
        void mapType(ct::TypeMapper &mapper, ct::proto::TypeDefinition &out,
                     typename std::remove_reference<T>::type const &type) {
            mapType<T>(mapper, out, type, 0);
        }
    }
}

#endif //CPPTOOL_EXPORT_TYPEMAPPER_TEMPLATE_HPP