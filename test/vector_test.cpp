#include "minunit.h"
#include "vector.hpp"
#include <allocator.h>
#include <vector>
#include <iostream>




using namespace pdstl;

MU_TEST(test_vector_check) {
    //ctor
    {
    vector<int> i(3,5);
    mu_assert_int_eq(5,i[1]);
    mu_assert_int_eq(5,i.at(2));

    //Iterator ctor
    std::vector<int> p(3,5);
    vector<int> q(p.cbegin(),p.cend());
    mu_assert_int_eq(5,q[0]);
    mu_assert_int_eq(5,q.at(1));
    mu_assert_int_eq(5,q.at(2));

    //initializer_list ctor
    vector<int> vec3{1,2,3,4,5};
    for(int i = 0;i<5;++i)
        mu_assert_int_eq((i+1),vec3.at(i));

    //copy ctor
    vector<int> vec4(vec3);
    for(int i = 0;i<5;++i)
        mu_assert_int_eq((i+1),vec4.at(i));

    //move ctor
    vector<int> vec5(vector<int>{1,2,3,4,5});
    for(int i = 0;i<5;++i)
        mu_assert_int_eq((i+1),vec5.at(i));
    }

    //allocator support
    {
    vector<int,std::allocator<int>> p(3,5);
    mu_assert_int_eq(5,p[1]);
    mu_assert_int_eq(5,p.at(2));
    }
    //capacity
    {
    vector<char> vec1(10,'a');
    mu_check(vec1.at(1) = 'a');
    mu_assert_int_eq(10,vec1.size());
    mu_assert_int_eq(20,vec1.capacity());
    mu_assert_int_eq(0,vec1.empty());
    }
    //complex T
    {
    vector<vector<int>> vec1(3,{1,2,3});
    mu_assert_int_eq(3,vec1.size());
    mu_assert_int_eq(6,vec1.capacity());
    mu_assert_int_eq(0,vec1.empty());
    mu_assert_int_eq(1,vec1.at(1).at(0));
    mu_assert_int_eq(3,vec1.at(1).at(2));
    }
    //resize
    {
        vector<int> vec1{100};
        vec1.resize(5);
        mu_assert_int_eq(100,vec1.at(0));
        mu_assert_int_eq(5,vec1.size());
        for(int i=1;i<5;++i)
            mu_assert_int_eq(0,vec1.at(i));
        vec1.resize(3);
        mu_assert_int_eq(3,vec1.size());
        for(int i=1;i<2;++i)
            mu_assert_int_eq(0,vec1.at(i));

        vector<int> vec2 = {2,3,3};
        vec2.resize(5,100);
        mu_assert_int_eq(2,vec2.at(0));
        mu_assert_int_eq(3,vec2.at(1));
        mu_assert_int_eq(3,vec2.at(2));
        mu_assert_int_eq(100,vec2.at(3));
        mu_assert_int_eq(100,vec2.at(4));

        vector<std::string> vec3{"a"};
        vec3.resize(15,"a");
        for(int i=0;i<15;++i)
            mu_check(vec3.at(i) == "a");
        vec3.resize(4);
        for(int i=0;i<4;++i)
            mu_check(vec3.at(i) == "a");

        mu_assert_int_eq(4,vec3.size());
        mu_assert_int_eq(15,vec3.capacity());
    }
    //reserve
    {
        vector<int> vec1{1,2,3};
        vec1.reserve(100);
        mu_assert_int_eq(3,vec1.size());
        mu_assert_int_eq(100,vec1.capacity());


    }

}
MU_TEST_SUITE(test_vector_suite) {
    MU_RUN_TEST(test_vector_check);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_vector_suite);
    MU_REPORT();
    return minunit_status;
}
