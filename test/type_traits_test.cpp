#include "minunit.h"
#include "type_traits.hpp"

using namespace pdstl;

//a fake decltype
template<typename T>
inline bool f(T i)
{
    if (is_integral<T>::value) {
        return true;
    }
    return false;
};
MU_TEST(test_type_traits_check)
{
    //is_integral
    {
        class A
        {};
        enum t : int {};
        mu_assert_int_eq(0, is_integral<A>::value);
        mu_assert_int_eq(0, is_integral<t>::value);
        mu_assert_int_eq(1, is_integral<int>::value);
        mu_assert_int_eq(1, f(123));
        mu_assert_int_eq(0, f("string"));
    }
    //_traits trick
    {
        class A
        {};
        mu_assert_int_eq(0, __type_traits<A>::has_trivial_default_constructor::value);
        mu_assert_int_eq(1, __type_traits<int>::has_trivial_default_constructor::value);
        mu_assert_int_eq(1, __type_traits<decltype(5)>::has_trivial_default_constructor::value);
        mu_assert_int_eq(1, __type_traits<decltype(1.9f)>::has_trivial_default_constructor::value);
    }
}
MU_TEST_SUITE(test_type_traits_suite)
{
    MU_RUN_TEST(test_type_traits_check);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_type_traits_suite);
    MU_REPORT();
    return minunit_status;
}
