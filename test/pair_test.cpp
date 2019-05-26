#include "minunit.h"
#include "pair.hpp"
#include "vector.hpp"
#include <algorithm> //std

using namespace pdstl;

MU_TEST(test_pair_check)
{
    {
        auto p1 = pdstl::make_pair(1, 2);
        mu_assert_int_eq(1, p1.first);
        mu_assert_int_eq(2, p1.second);
        auto p2 = pdstl::make_pair(1, 2);
        mu_check(p1 == p2);
        auto p3 = pdstl::make_pair(2, 2);
        mu_check(p2 <= p3);
        mu_check(p2 < p3);
        mu_check(p3 >= p3);
        mu_check(p3 > p2);
        mu_check(p3 > p1);
        auto p4(p3);
        mu_assert_int_eq(2, p4.first);
        mu_assert_int_eq(2, p4.second);

        pdstl::vector<pair<int, int>> pair_vec;
        for (int i = 0; i != 9; ++i) {
            pair_vec.emplace_back(0, i);
        }
        std::sort(pair_vec.begin(),
                  pair_vec.end(),
                  [=](const pair<int, int> &p1, const pair<int, int> &p2) { return p1 > p2; });
        for (int i = 0; i != 9; ++i) {
            mu_check(pair_vec.at(i).second == (8 - i));
        }
    }
}
MU_TEST_SUITE(test_pair_suite)
{
    MU_RUN_TEST(test_pair_check);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_pair_suite);
    MU_REPORT();
    return minunit_status;
}
