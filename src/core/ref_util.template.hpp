#ifndef CPPTOOL_CORE_REFUTIL_HPP
#define CPPTOOL_CORE_REFUTIL_HPP

#include <functional>

/*
	C++ STD doesn't define a std::less overload for reference wrappers, so we define it here.
*/
template<typename T>
struct std::less<std::reference_wrapper<T> > {
    std::less<T> compare = std::less<T>();

    bool operator()(const std::reference_wrapper<T> &ref1, const std::reference_wrapper<T> &ref2) const {
        return compare(ref1, ref2);
    }
};

#endif //CPPTOOL_CORE_REFUTIL_HPP