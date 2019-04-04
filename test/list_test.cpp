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
        std::list<int> slist{1,2,3};
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

        pdstl::list<int> lst2(lst);
        mu_assert_int_eq(5,*lst2.at(0));
        mu_assert_int_eq(5,*lst2.at(1));
        mu_assert_int_eq(5,*lst2.at(2));

        pdstl::list<int> lst3(std::move(lst));
        mu_assert_int_eq(5,*lst3.at(0));
        mu_assert_int_eq(5,*lst3.at(1));
        mu_assert_int_eq(5,*lst3.at(2));
        mu_assert_int_eq(0,lst.size());
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
