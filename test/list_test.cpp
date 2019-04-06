#include "minunit.h"
#include "list.hpp"
#include <string>


using namespace pdstl;
MU_TEST(test_list_check) {
    //push_back ,push_front and default ctor
    {
        pdstl::list<int> lst;
        lst.push_back(1);
        lst.push_back(2);
        lst.push_back(3);
        mu_assert_int_eq(1,*lst.at(0));
        mu_assert_int_eq(2,*lst.at(1));
        mu_assert_int_eq(3,*lst.at(2));

        lst.push_front(9);
        lst.push_front(8);
        lst.push_front(7);
        mu_assert_int_eq(7,*lst.at(0));
        mu_assert_int_eq(8,*lst.at(1));
        mu_assert_int_eq(9,*lst.at(2));
        mu_assert_int_eq(6,lst.size());
    }

    //iterator_ctor
    {
        pdstl::list<int> slist{1,2,3};
        pdstl::list<int> lst(slist.cbegin(),slist.cend());
        mu_assert_int_eq(1,*lst.at(0));
        mu_assert_int_eq(2,*lst.at(1));
        mu_assert_int_eq(3,*lst.at(2));
    }
    //count ctor ,copy ctor and move ctor
    {
        pdstl::list<int> lst(3,5);
        mu_assert_int_eq(5,*lst.at(0));
        mu_assert_int_eq(5,*lst.at(1));
        mu_assert_int_eq(5,*lst.at(2));

        pdstl::list<int> list2(lst);
        mu_assert_int_eq(5,*list2.at(0));
        mu_assert_int_eq(5,*list2.at(1));
        mu_assert_int_eq(5,*list2.at(2));

        pdstl::list<int> list3(std::move(lst));
        mu_assert_int_eq(5,*list3.at(0));
        mu_assert_int_eq(5,*list3.at(1));
        mu_assert_int_eq(5,*list3.at(2));

        pdstl::list<int> list4{5,5,5};
        mu_assert_int_eq(5,*list4.at(0));
        mu_assert_int_eq(5,*list4.at(1));
        mu_assert_int_eq(5,*list4.at(2));
        mu_assert_int_eq(3,list4.size());
    }
    //erase assign
    {
        pdstl::list<int> list1;
        list1.assign(3,5);
        mu_assert_int_eq(5,*list1.at(0));
        mu_assert_int_eq(5,*list1.at(1));
        mu_assert_int_eq(5,*list1.at(2));
        mu_assert_int_eq(3,list1.size());

        pdstl::list<int> list2{1,2,3};
        mu_assert_int_eq(3,list2.size());
        list2.erase(list2.begin()); //{2,3}
        mu_assert_int_eq(2,*list2.at(0));
        mu_assert_int_eq(3,*list2.at(1));
        mu_assert_int_eq(2,list2.size());
        list2.erase(list2.begin(),list2.cend()); //{ }
        mu_assert_int_eq(0,list2.size());

        pdstl::list<char> characters;
        characters.assign(5,'a');
        for(auto tmp : characters)
            mu_check(tmp == 'a');
        mu_assert_int_eq(5,characters.size());

        characters.assign(3,'b');
        for(auto tmp : characters)
            mu_check(tmp == 'b');
        mu_assert_int_eq(3,characters.size());

        pdstl::list<char> chars{'a','a','a','a'};
        characters.assign(chars.begin(),chars.end());
        for(auto tmp : characters)
            mu_check(tmp == 'a');
        mu_assert_int_eq(4,characters.size());
    }
   //front and back
    {
        list<std::string> list{"aa","bb","cc"};
        mu_assert_string_eq("aa",list.front().c_str());
        mu_assert_string_eq("cc",list.back().c_str());
    }

    //reverse_itrator and normal iterator
    {
        list<int> lst{9,8,7,6,5,4,3,2,1,0};
        int n = 0;
        for(auto ri = lst.rbegin();ri!=lst.rend();++ri,++n)
            mu_assert_int_eq(n,*ri);
        int m = 9;
        for(auto i = lst.begin();i!=lst.end();++i,--m)
            mu_assert_int_eq(m,*i);
    }

    //operator =
    {

        list<int> list1 = {5,5,5};
        mu_assert_int_eq(5,*list1.at(0));
        mu_assert_int_eq(5,*list1.at(1));
        mu_assert_int_eq(5,*list1.at(2));
        mu_assert_int_eq(3,list1.size());

        list<int> list2 = list1;
        mu_assert_int_eq(5,*list2.at(0));
        mu_assert_int_eq(5,*list2.at(1));
        mu_assert_int_eq(5,*list2.at(2));
        mu_assert_int_eq(3,list2.size());

        list<int> list3 = std::move(list1);
        mu_assert_int_eq(5,*list3.at(0));
        mu_assert_int_eq(5,*list3.at(1));
        mu_assert_int_eq(5,*list3.at(2));
        mu_assert_int_eq(3,list3.size());

       //list1 was resetted
        mu_assert_int_eq(0,list1.size());
    }
    //pop_back and pop_front
    {
        list<int> list1 = {1,2,3};
        mu_assert_int_eq(3,list1.size());
        list1.pop_front();
        mu_assert_int_eq(2,list1.size());
        mu_assert_int_eq(2,list1.front());
        list1.pop_back();
        mu_assert_int_eq(1,list1.size());
        mu_assert_int_eq(2,list1.back());

        //check iterators being still valid
        list1.insert(list1.cbegin(),1);
        list1.insert(list1.cend(),3);
        mu_assert_int_eq(3,list1.size());
        mu_assert_int_eq(1,*list1.at(0));
        mu_assert_int_eq(2,*list1.at(1));
        mu_assert_int_eq(3,*list1.at(2));
    }

    //insert
    {
        list<int> list1;
        //T&&
        list1.insert(list1.cbegin(),1);
        mu_assert_int_eq(1,*list1.at(0));
        mu_assert_int_eq(1,list1.size());

        //const T&
        int a = 2;
        list1.insert(list1.cbegin(),a);
        mu_assert_int_eq(2,*list1.at(0));
        mu_assert_int_eq(2,list1.size());

        //size_type n
        list1.insert(list1.cbegin(),3,3);
        mu_assert_int_eq(3,*list1.at(0));
        mu_assert_int_eq(3,*list1.at(1));
        mu_assert_int_eq(3,*list1.at(2));
        mu_assert_int_eq(2,*list1.at(3));
        mu_assert_int_eq(1,*list1.at(4)); //{head,3,3,3,2,1,tail}
        mu_assert_int_eq(5,list1.size());

        list<int> list2;

        auto it1 = list1.begin();
        auto it2 = list1.end();
        //std::advance cannot work with pdstl::iterator

        ++it1;
        --it2;
        list2.insert(list2.end(),it1,it2); //{3,3,2}
        mu_assert_int_eq(3,list2.size());

        list2.insert(list2.cend(),{1,2,3}); //{3,3,2,1,2,3};
        mu_assert_int_eq(6,list2.size());

        mu_assert_int_eq(3,*list2.at(0));
        mu_assert_int_eq(3,*list2.at(1));
        mu_assert_int_eq(2,*list2.at(2));
        mu_assert_int_eq(1,*list2.at(3));
        mu_assert_int_eq(2,*list2.at(4));
        mu_assert_int_eq(3,*list2.at(5));
    }


}
MU_TEST_SUITE(test_list_suite) {
    MU_RUN_TEST(test_list_check);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_list_suite);
    MU_REPORT();
    return minunit_status;
}
