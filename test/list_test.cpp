#include "minunit.h"
#include "list.hpp"


using namespace pdstl;
MU_TEST(test_list_check) {
    {
    pdstl::list<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
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
