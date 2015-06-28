#ifndef CPPTOOL_CORE_REFUTIL_HPP
#define CPPTOOL_CORE_REFUTIL_HPP

#include <functional>

/**
 * C++ STD doesn't define a std::less overload for reference wrappers, so we define it here.
 */
namespace std {
    template<typename T>
    struct less<std::reference_wrapper<T>> {
        less<T> compare = less<T>();

        bool operator()(const std::reference_wrapper<T> &ref1, const std::reference_wrapper<T> &ref2) const {
            return compare(ref1, ref2);
        }
    };
} //std

#endif //CPPTOOL_CORE_REFUTIL_HPP