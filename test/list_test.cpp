#include "minunit.h"
#include "list.hpp"
#include <list>


using namespace pdstl;
MU_TEST(test_list_check) {
    //push_back and default ctor
    {
        pdstl::list<int> lst;
        lst.push_back(1);
        lst.push_back(2);
        lst.push_back(3);
        mu_assert_int_eq(1,*lst.at(0));
        mu_assert_int_eq(2,*lst.at(1));
        mu_assert_int_eq(3,*lst.at(2));
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


}
MU_TEST_SUITE(test_list_suite) {
    MU_RUN_TEST(test_list_check);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_list_suite);
    MU_REPORT();
    return minunit_status;
}
