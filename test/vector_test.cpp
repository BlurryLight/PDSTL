#include "minunit.h"
#include "vector.hpp"
#include <allocator.h>
#include <iostream>
#include <vector>

using namespace pdstl;

MU_TEST(test_vector_check)
{
    //ctor
    {
        vector<int> i(3, 5);
        mu_assert_int_eq(5, i[1]);
        mu_assert_int_eq(5, i.at(2));

        //Iterator ctor
        std::vector<int> p(3, 5);
        vector<int> q(p.cbegin(), p.cend());
        mu_assert_int_eq(5, q[0]);
        mu_assert_int_eq(5, q.at(1));
        mu_assert_int_eq(5, q.at(2));

        //initializer_list ctor
        vector<int> vec3{1, 2, 3, 4, 5};
        for (int i = 0; i < 5; ++i)
            mu_assert_int_eq((i + 1), vec3.at(i));

        //copy ctor
        vector<int> vec4(vec3);
        for (int i = 0; i < 5; ++i)
            mu_assert_int_eq((i + 1), vec4.at(i));

        //move ctor
        vector<int> vec5(vector<int>{1, 2, 3, 4, 5});
        for (int i = 0; i < 5; ++i)
            mu_assert_int_eq((i + 1), vec5.at(i));
    }

    //allocator support
    {
        vector<int, std::allocator<int>> p(3, 5);
        mu_assert_int_eq(5, p[1]);
        mu_assert_int_eq(5, p.at(2));
    }
    //capacity
    {
        vector<char> vec1(10, 'a');
        mu_check(vec1.at(1) = 'a');
        mu_assert_int_eq(10, vec1.size());
        mu_assert_int_eq(20, vec1.capacity());
        mu_assert_int_eq(0, vec1.empty());
    }
    //complex T
    {
        vector<vector<int>> vec1(3, {1, 2, 3});
        mu_assert_int_eq(3, vec1.size());
        mu_assert_int_eq(6, vec1.capacity());
        mu_assert_int_eq(0, vec1.empty());
        mu_assert_int_eq(1, vec1.at(1).at(0));
        mu_assert_int_eq(3, vec1.at(1).at(2));
    }
    //resize
    {
        vector<int> vec1{100};
        vec1.resize(5);
        mu_assert_int_eq(100, vec1.at(0));
        mu_assert_int_eq(5, vec1.size());
        for (int i = 1; i < 5; ++i)
            mu_assert_int_eq(0, vec1.at(i));
        vec1.resize(3);
        mu_assert_int_eq(3, vec1.size());
        for (int i = 1; i < 2; ++i)
            mu_assert_int_eq(0, vec1.at(i));

        vector<int> vec2 = {2, 3, 3};
        vec2.resize(5, 100);
        mu_assert_int_eq(2, vec2.at(0));
        mu_assert_int_eq(3, vec2.at(1));
        mu_assert_int_eq(3, vec2.at(2));
        mu_assert_int_eq(100, vec2.at(3));
        mu_assert_int_eq(100, vec2.at(4));

        vector<std::string> vec3{"a"};
        vec3.resize(15, "a");
        for (int i = 0; i < 15; ++i)
            mu_check(vec3.at(i) == "a");
        vec3.resize(4);
        for (int i = 0; i < 4; ++i)
            mu_check(vec3.at(i) == "a");

        mu_assert_int_eq(4, vec3.size());
        mu_assert_int_eq(15, vec3.capacity());
    }
    //reserve
    {
        vector<int> vec1{1, 2, 3};
        vec1.reserve(100);
        mu_assert_int_eq(3, vec1.size());
        mu_assert_int_eq(100, vec1.capacity());
    }

    //back and front
    {
        vector<int> vec1{1, 2, 3};
        mu_assert_int_eq(1, vec1.front());
        mu_assert_int_eq(3, vec1.back());
        const vector<int> vec2{5, 6, 7};
        mu_assert_int_eq(5, vec2.front());
        mu_assert_int_eq(7, vec2.back());
    }
    //insert
    {
        vector<int> vec1{1, 2, 3};
        vec1.insert(vec1.end(), 4);
        for (int i = 0; i < 4; ++i)
            mu_check(vec1.at(i) == i + 1);

        vector<int> vec2{1};
        vec2.insert(vec2.end(), 10, 3);
        mu_check(vec2.at(0) == 1);
        for (int i = 1; i < 10; ++i)
            mu_check(vec2.at(i) == 3);

        std::vector<int> vec3{1, 1, 1};
        vector<int> vec4;
        vec4.insert(vec4.begin(), vec3.begin(), vec3.end());
        for (int i = 0; i < 2; ++i)
            mu_check(vec4.at(i) == 1);

        vector<int> vec5;
        vec5.insert(vec5.begin(), {1, 1, 1});
        for (int i = 0; i < vec5.size(); ++i)
            mu_check(vec5.at(i) == 1);

        vec5.push_back(5);
        auto p = vec5.data();
        mu_check(*p == 1);
        mu_check(vec5.back() == 5);
    }
    //erase
    {
        vector<int> vec1{1, 2, 3, 4, 5};
        vec1.erase(vec1.begin());
        mu_assert_int_eq(2, *(vec1.begin()));
        vec1.erase(vec1.begin(), vec1.begin() + 3);
        mu_assert_int_eq(5, *(vec1.begin()));

        vector<int> vec2{1, 2, 3, 4, 5, 6};
        for (auto it = vec2.begin(); it != vec2.end();) {
            if (*it % 2 == 0) {
                it = vec2.erase(it);
            } else {
                ++it;
            }
        }
        mu_assert_int_eq(1, vec2.at(0));
        mu_assert_int_eq(3, vec2.at(1));
        mu_assert_int_eq(5, vec2.at(2));
    }
    //swap
    {
        vector<int> vec1{1, 2, 3};
        vector<int> vec2{4, 5, 6, 7};
        vec1.swap(vec2);
        mu_assert_int_eq(4, vec1.size());
        mu_assert_int_eq(8, vec1.capacity());
        mu_assert_int_eq(4, vec1.front());
        mu_assert_int_eq(7, vec1.back());

        mu_assert_int_eq(3, vec2.size());
        mu_assert_int_eq(6, vec2.capacity());
        mu_assert_int_eq(1, vec2.front());
        mu_assert_int_eq(3, vec2.back());

        //clear

        vec1.clear();
        mu_assert_int_eq(0, vec1.size());
        mu_assert_int_eq(8, vec1.capacity());
    }
    //push_back && emplace_back
    {
        struct base
        {
            std::string str1;
            std::string str2;
            int i1;
            base(std::string s1, std::string s2, int val = 10)
                : str1(std::move(s1))
                , str2(std::move(s2))
                , i1(val)
            {
                //                std::cout<<"base ctor"<<std::endl;
            }
        };

        vector<base> vec1;
        vec1.emplace_back("I", "am", 1);
        vec1.push_back(base("You", "are", 2));

        mu_assert_int_eq(2, vec1.size());
        mu_assert_int_eq(4, vec1.capacity());
        mu_check("I" == vec1.at(0).str1);
        mu_check("You" == vec1.at(1).str1);

        mu_check("am" == vec1.at(0).str2);
        mu_check("are" == vec1.at(1).str2);

        mu_check(1 == vec1.at(0).i1);
        mu_check(2 == vec1.at(1).i1);
    }
    //assign && pop_back
    {
        vector<std::string> vec1;
        vec1.assign(5, "We");
        mu_assert_int_eq(5, vec1.size());
        mu_check(vec1.front() == "We");
        mu_check(vec1.back() == "We");

        vector<std::string> vec2;
        vec2.assign(vec1.cbegin(), vec1.cend());
        mu_assert_int_eq(5, vec2.size());
        mu_check(vec2.front() == "We");
        mu_check(vec2.back() == "We");

        vector<std::string> vec3;
        vec3.assign({"we", "are", "young"});
        mu_assert_int_eq(3, vec3.size());
        mu_check(vec3.front() == "we");
        mu_check(vec3.back() == "young");

        vec3.pop_back();
        mu_check(vec3.back() == "are");
    }
    //operators
    {
        vector<char> vec1{'a', 'b', 'c', 'd'};
        vector<char> vec2{'a', 'b', 'c', 'd'};
        vector<char> vec3{'c', 'b', 'c', 'd'};

        mu_check(vec1 == vec2);
        mu_check(vec1 <= vec2);
        mu_check(vec1 <= vec3);
        mu_check(vec1 < vec3);
        mu_check(vec1 != vec3);
        mu_check(vec3 > vec1);
        mu_check(vec3 >= vec1);
    }
}
MU_TEST_SUITE(test_vector_suite)
{
    MU_RUN_TEST(test_vector_check);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_vector_suite);
    MU_REPORT();
    return minunit_status;
}
