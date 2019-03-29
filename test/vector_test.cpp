#include "minunit.h"
#include "vector.hpp"



using namespace pdstl;

MU_TEST(test_vector_check) {
    vector<int> i(3,5);
    mu_assert_int_eq(5,i[1]);


}
MU_TEST_SUITE(test_vector_suite) {
    MU_RUN_TEST(test_vector_check);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_vector_suite);
    MU_REPORT();
    return minunit_status;
}
