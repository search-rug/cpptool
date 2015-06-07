#include <string>

using namespace std;

string i;

namespace a { namespace b { namespace c {
    typedef std::string d;
} } }

class forward_declared;

auto global_lambda = [](int i) {
    i = i + 1;
};

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

void lambda_nesting() {
    auto p = [&](int i) {
        auto nested_lamda = [&](int j) {
            int z = i + j;
        };
    };
}

template<typename T>
class template_test {
    using Forward = T;
    
    template<int N>
    void test() const;
};

template<>
class template_test<int>; //Test template declaration

namespace template_specialization_test {
    template<typename T, int N>
    class test {
    };
    
    template<>
    class test<bool, 2> {
    };
    
    template<>
    class test<int, 2> {
    };
    
    template<>
    class test<int, 42> {
    };
    
    template<typename P>
    P test2();
    
    template<>
    int test2<int>();
    
    template<typename Z>
    using Ptr = Z*;
    
    using Ptr2 = Ptr<bool>;
    
    template<typename T>
    T n = T(10);
}