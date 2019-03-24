#include "../include/minunit.h"
#include "../src/memory.h"
#include <memory>

using namespace pdstl;

MU_TEST(test_shared_ptr_check)
{
    {
        //empty nullptr
        pdstl::shared_ptr<int> ptr;
        mu_check(ptr == nullptr);
        mu_check(ptr.unique() == true);
        mu_check(ptr.use_count() == 1);
        mu_check(ptr.get() == nullptr);

        auto ptr2(ptr);
        mu_check(ptr.unique() == false);
    }
    //value shared_ptr
    //sub scope to test destrucor
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
    {
        //reset
        shared_ptr<int> ptr5(new int(48));
        ptr5.reset();
        mu_check(ptr5 == nullptr);
        mu_assert_int_eq(0,ptr5);
        ptr5.reset(new int(49));
        mu_assert_int_eq(49,*ptr5);
        mu_assert_int_eq(1,ptr5);
    }

    //swap
    {
        shared_ptr<int> p(new int(18));
        shared_ptr<int> q(new int(100));
        auto p1(p);
        auto p2(p);
        auto p3(p);
        mu_assert_int_eq(4,p.use_count());
        p.swap(q);
        mu_assert_int_eq(4,q.use_count());
        mu_assert_int_eq(1,p.use_count());
        mu_assert_int_eq(100,*p);
        mu_assert_int_eq(18,*q);
    }
    //alias constructor
    {
       struct data
       {
           int d;
       };
       shared_ptr<data> object(new data());
       shared_ptr<int> d(object,&object->d);
       mu_assert_int_eq(2,object.use_count());
       mu_assert_int_eq(2,d.use_count());
       d.reset();
       mu_assert_int_eq(1,object.use_count());
       mu_assert_int_eq(1,d.use_count());
    }
    //comparing symbols
    {
       struct data
       {
           int d1;
           int d2;
           data(int a,int b):d1(a),d2(b){}
       };

       shared_ptr<data> ptr1(new data(1,2));
       shared_ptr<int> ptr2(ptr1,&ptr1->d1);
       shared_ptr<int> ptr3(ptr1,&ptr1->d2);
       mu_check(ptr2 < ptr3);
       mu_check(ptr2 <= ptr3);
       mu_check(ptr3 >= ptr2);
       mu_check( !ptr2.owner_before(ptr3));
       mu_check( !ptr3.owner_before(ptr2));
       mu_check(!(ptr2 == ptr3));

    }

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
