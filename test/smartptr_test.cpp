#include "../include/minunit.h"
#include "../src/memory.h"

using namespace pdstl;

MU_TEST(test_shared_ptr_check)
{
    //empty nullptr
    pdstl::shared_ptr<int> ptr;
    mu_check(ptr == nullptr);
    mu_check(ptr.unique() == true);
    mu_check(ptr.use_count() == 1);
    mu_check(ptr.get() == nullptr);

    auto ptr2(ptr);
    mu_check(ptr.unique() == false);

    //value shared_ptr
    //sub scope to test destructor
    {
        static int released_times = 0;
        struct bulk
        {
            int val;
            bulk(int x):val(x){}
            void incr()
            {
                val++;
            }
            void decr()
            {
                val--;
            }
            ~bulk()
            {
                released_times++;
            }
        };
        shared_ptr<bulk> ptr3(new bulk(123));
        mu_assert_int_eq(123,ptr3->val);
        ptr3->incr();
        mu_assert_int_eq(124,ptr3->val);
        (*ptr3).incr();
        mu_assert_int_eq(125,ptr3->val);
        ptr3->decr();
        mu_assert_int_eq(124,ptr3->val);
        //fail test
        //mu_fail("this is a fail test");

        //copy
        auto ptr4(ptr3);
        mu_check(ptr4 == ptr3);
        mu_assert_int_eq(true,ptr4);
        ptr4.reset();
        ptr3.reset();
        mu_assert_int_eq(1,released_times); //ensure that bulk has been released only once

    }
    //reset
    shared_ptr<int> ptr5(new int(48));
    shared_ptr<char> ptr6;
    ptr6.reset(new int(49));
    ptr5.reset();
    mu_check(ptr5 == nullptr);
    mu_check((*ptr6) == '1');
    ptr5.reset(ptr6.get());
    mu_assert_int_eq(49,*ptr5);

}

MU_TEST_SUITE(test_smartptr_suite)
{
    MU_RUN_TEST(test_shared_ptr_check);
}
int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_smartptr_suite);
    MU_REPORT();
    return minunit_status;
}
