#include "alloc_test.cpp"



int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_alloc_suite);
    MU_REPORT();
    return minunit_status;
}
