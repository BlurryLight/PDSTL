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
            bulk(int x)
                : val(x)
            {}
            void incr() { val++; }
            void decr() { val--; }
            ~bulk() { released_times++; }
        };
        shared_ptr<bulk> ptr3(new bulk(123));
        mu_assert_int_eq(123, ptr3->val);
        ptr3->incr();
        mu_assert_int_eq(124, ptr3->val);
        (*ptr3).incr();
        mu_assert_int_eq(125, ptr3->val);
        ptr3->decr();
        mu_assert_int_eq(124, ptr3->val);
        //fail test
        //mu_fail("this is a fail test");

        //copy
        auto ptr4(ptr3);
        mu_check(ptr4 == ptr3);
        mu_assert_int_eq(true, ptr4);
        ptr4.reset();
        ptr3.reset();
        mu_assert_int_eq(1, released_times); //ensure that bulk has been released only once
    }
    {
        //reset
        shared_ptr<int> ptr5(new int(48));
        ptr5.reset();
        mu_check(ptr5 == nullptr);
        mu_assert_int_eq(0, ptr5);
        ptr5.reset(new int(49));
        mu_assert_int_eq(49, *ptr5);
        mu_assert_int_eq(1, ptr5);
    }

    //swap
    {
        shared_ptr<int> p(new int(18));
        shared_ptr<int> q(new int(100));
        auto p1(p);
        auto p2(p);
        auto p3(p);
        mu_assert_int_eq(4, p.use_count());
        p.swap(q);
        mu_assert_int_eq(4, q.use_count());
        mu_assert_int_eq(1, p.use_count());
        mu_assert_int_eq(100, *p);
        mu_assert_int_eq(18, *q);
    }
    //alias constructor
    {
        struct data
        {
            int d;
        };
        shared_ptr<data> object(new data());
        shared_ptr<int> d(object, &object->d);
        mu_assert_int_eq(2, object.use_count());
        mu_assert_int_eq(2, d.use_count());
        d.reset();
        mu_assert_int_eq(1, object.use_count());
        mu_assert_int_eq(1, d.use_count());
    }
    //comparing symbols
    {
        struct data
        {
            int d1;
            int d2;
            data(int a, int b)
                : d1(a)
                , d2(b)
            {}
        };

        shared_ptr<data> ptr1(new data(1, 2));
        shared_ptr<int> ptr2(ptr1, &ptr1->d1);
        shared_ptr<int> ptr3(ptr1, &ptr1->d2);
        mu_check(ptr2 < ptr3);
        mu_check(ptr2 <= ptr3);
        mu_check(ptr3 >= ptr2);
        mu_check(!ptr2.owner_before(ptr3));
        mu_check(!ptr3.owner_before(ptr2));
        mu_check(!(ptr2 == ptr3));
    }
    //factory function test
    {
        struct data
        {
            int d1;
            int d2;
            data(int a, int b)
                : d1(a)
                , d2(b)
            {}
        };

        auto ptr1 = make_shared<data>(1, 2);

        mu_assert_int_eq(1, ptr1->d1);
        mu_assert_int_eq(2, ptr1->d2);
    }
    //deleter
    {
        struct test_int_deleter
        {
            void operator()(int *p)
            {
                std::cout << "lmao" << std::endl;
                delete p;
            }
        };

        shared_ptr<int> ptr1(new int(5), test_int_deleter());
        ptr1.reset();
        //should show lmao
        shared_ptr<int> ptr3;
        ptr3.reset(new int(10), test_int_deleter());
    }
}

MU_TEST(test_unique_ptr_check)
{
    //value potiner
    {
        pdstl::unique_ptr<int> p1(new int(48));
        mu_assert_int_eq(48, *p1);
        pdstl::unique_ptr<int> p2(std::move(p1));
        mu_assert_int_eq(48, *p2);
        mu_check(p1 == nullptr);

        pdstl::unique_ptr<int> p3(new int(100));
        pdstl::unique_ptr<int> p4;
        p4.reset(p3.release());
        mu_assert_int_eq(100, *p4);
        mu_check(p3 == nullptr);

        pdstl::unique_ptr<int> p5(new int(48));
        pdstl::unique_ptr<int> p6;
        p6 = std::move(p5);
        mu_assert_int_eq(48, *p6);
        mu_check(p5 == nullptr);
    }
    //comparing
    {
        int *a = new int(5);
        pdstl::unique_ptr<int> p1(a);
        pdstl::unique_ptr<int> p2(new int(5));
        mu_check(p1 == p1);
        mu_check(p1 != p2);
    }
    {
        struct data
        {
            int d1;
            int d2;
            data(int a, int b)
                : d1(a)
                , d2(b)
            {}
        };

        pdstl::unique_ptr<data> ptr1 = make_unique<data>(1, 2);
        mu_assert_int_eq(1, ptr1->d1);
        mu_assert_int_eq(2, ptr1->d2);
    }
}

MU_TEST(test_weak_ptr_check)
{
    pdstl::weak_ptr<int> ptr1;
    {
        pdstl::shared_ptr<int> ptr2(new int(40));
        ptr1 = ptr2;
        mu_assert_int_eq(40, *(ptr1.lock()));
    }
    mu_assert_int_eq(1, ptr1.expired());
    //use count
    {
        pdstl::shared_ptr<int> ptr3(new int(555));
        pdstl::weak_ptr<int> ptr2(ptr3);
        pdstl::weak_ptr<int> ptr4(ptr2);

        mu_assert_int_eq(1, ptr3.use_count());
        mu_assert_int_eq(1, ptr2.use_count());
        mu_assert_int_eq(1, ptr4.use_count());
    }

    mu_check(ptr1.lock() == nullptr);
}
MU_TEST_SUITE(test_smartptr_suite)
{
    MU_RUN_TEST(test_shared_ptr_check);
    MU_RUN_TEST(test_unique_ptr_check);
    MU_RUN_TEST(test_weak_ptr_check);
}
int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_smartptr_suite);
    MU_REPORT();
    return minunit_status;
}
