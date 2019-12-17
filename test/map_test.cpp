#include "iterator.hpp"
#include "map.hpp"
#include "minunit.h"

using namespace pdstl;

MU_TEST(test_map_check) {
    {
        pdstl::map<int,int> test;
        mu_assert_int_eq(test.size(),0);
    }
    {
        pdstl::map<int,int> test;
        auto p = test.insert({1,2});
        mu_assert_int_eq(test.size(),1);
        mu_assert_int_eq(int(p.second),1);

        auto q = test.insert({1,3});
        mu_assert_int_eq(test.size(),1);
        mu_assert_int_eq(int(q.second),false);
    }

}
MU_TEST_SUITE(test_map_suite)
{
    MU_RUN_TEST(test_map_check);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_map_suite);
    MU_REPORT();
    return minunit_status;
}
