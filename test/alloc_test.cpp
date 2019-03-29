#include "../include/minunit.h"

#include <string>
#include <vector>
#include <iostream>
#include "../src/allocator.h"


MU_TEST(test_alloc_check) {

   pdstl::Allocator<int> p;
   int* a = p.allocate(1);
   p.construct(a,19);
    mu_check(*a==19);
    p.deallocate(a,1);

   pdstl::Allocator<std::string> a2;
   decltype (p) ::rebind<std::string>::other a2_1;

   std::string* s = a2.allocate(2);
   a2.construct(s,"test1");
   a2.construct(s + 1,"test2");

    mu_check(s[0]=="test1");
    mu_check(s[1]=="test2");
   a2.destroy(s);
   a2.destroy(s + 1);
   a2.deallocate(s,2);

   std::vector<std::string,pdstl::Allocator<std::string,512>> vec{"1","2","3"};
   mu_check(vec[0] == "1");
   mu_check(vec[1] == "2");
   mu_check(vec[2] == "3");
   mu_check(vec[0] != "2");
   vec.push_back("abc");
   mu_check(*(vec.cend() - 1) == "abc");

}
MU_TEST_SUITE(test_alloc_suite) {
    MU_RUN_TEST(test_alloc_check);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_alloc_suite);
    MU_REPORT();
    return minunit_status;
}

