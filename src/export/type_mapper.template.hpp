#ifndef CPPTOOL_EXPORT_TYPEMAPPER_TEMPLATE_HPP
#define CPPTOOL_EXPORT_TYPEMAPPER_TEMPLATE_HPP

#include "base.pb.h"
#include "export/type_mapper.hpp"

namespace ct {
    namespace TypeMapperTemplate {
        /**
         * This method relies on two tricks to turn a Declaration into a stored TypeDefinition
         * 1. It uses SFINAE (Substitution Failure is not an Error) to create a method if T.getDecl() exists using
         * decltype.
         * 2. The main difference between the method that uses T.getDecl and the fallback method is that it uses a
         * int and a long respectively.
         * What does this it that when it is called with the parameter '0', it will use the function with the int
         * parameter before the one with the long parameter due to type matching rules, preferring the T.getDecl version
         * to the fallback method when resolving which method to call.
         */

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