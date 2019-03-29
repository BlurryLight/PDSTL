#include "minunit.h"
#include "iterator.hpp"
#include <iterator>


using namespace pdstl;


//difficult to unit test traits table
template <typename Iter>
int num(Iter it)
{
    return num(it,iterator_traits<Iter>::iterator_category());
}
template <typename Iter>
int num(Iter it,random_access_iterator_tag)
{
    return 0;
}
template <typename Iter>
int num(Iter it,input_iterator_tag)
{
    return 1;
}
template <typename Iter>
int num(Iter it,output_iterator_tag)
{
    return 2;
}
class Myit : public iterator<input_iterator_tag,int,std::ptrdiff_t,int*,int&>
{
};
MU_TEST(test_iterator_traits_check) {


}
MU_TEST_SUITE(test_iterator_traits_suite) {
    MU_RUN_TEST(test_iterator_traits_check);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_iterator_traits_suite);
    MU_REPORT();
    return minunit_status;
}
