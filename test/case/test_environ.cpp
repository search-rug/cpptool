#include <string>

using namespace std;

string i;

namespace a { namespace b { namespace c {
    typedef std::string d;
} } }

class forward_declared;

namespace test {
    using namespace a::b::c;
    
    d test;
    d **test2;
    forward_declared *fwd;
    std::string string;
    std::string volatile const test3;
    //int i;
}

class friend_test {
    friend std::string;
    friend a::b::c::d;
};

template<typename T>
class template_test {
    using Forward = T;
};

template<>
class template_test<int>; //Test template declaration