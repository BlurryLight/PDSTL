#include "minunit.h"
#include "list.hpp"


using namespace pdstl;
MU_TEST(test_list_check) {


}
MU_TEST_SUITE(test_list_suite) {
    MU_RUN_TEST(test_list_check);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_list_suite);
    MU_REPORT();
    return minunit_status;
}
