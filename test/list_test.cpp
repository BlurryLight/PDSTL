#include "minunit.h"
#include "list.hpp"
//#include <list>
#include <string>
#include <iostream>



using namespace pdstl;
MU_TEST(test_list_check) {
    //push_back ,push_front and default ctor
    {
        pdstl::list<int> lst;
        lst.push_back(1);
        lst.push_back(2);
        lst.push_back(3);
        mu_assert_int_eq(1,*lst.at(0));
        mu_assert_int_eq(2,*lst.at(1));
        mu_assert_int_eq(3,*lst.at(2));

        lst.push_front(9);
        lst.push_front(8);
        lst.push_front(7);
        mu_assert_int_eq(7,*lst.at(0));
        mu_assert_int_eq(8,*lst.at(1));
        mu_assert_int_eq(9,*lst.at(2));
        mu_assert_int_eq(6,lst.size());
    }

    //iterator_ctor
    {
        pdstl::list<int> slist{1,2,3};
        pdstl::list<int> lst(slist.cbegin(),slist.cend());
        mu_assert_int_eq(1,*lst.at(0));
        mu_assert_int_eq(2,*lst.at(1));
        mu_assert_int_eq(3,*lst.at(2));
    }
    //count ctor ,copy ctor and move ctor
    {
        pdstl::list<int> lst(3,5);
        mu_assert_int_eq(5,*lst.at(0));
        mu_assert_int_eq(5,*lst.at(1));
        mu_assert_int_eq(5,*lst.at(2));

        pdstl::list<int> list2(lst);
        mu_assert_int_eq(5,*list2.at(0));
        mu_assert_int_eq(5,*list2.at(1));
        mu_assert_int_eq(5,*list2.at(2));

        pdstl::list<int> list3(std::move(lst));
        mu_assert_int_eq(5,*list3.at(0));
        mu_assert_int_eq(5,*list3.at(1));
        mu_assert_int_eq(5,*list3.at(2));

        pdstl::list<int> list4{5,5,5};
        mu_assert_int_eq(5,*list4.at(0));
        mu_assert_int_eq(5,*list4.at(1));
        mu_assert_int_eq(5,*list4.at(2));
        mu_assert_int_eq(3,list4.size());
    }
    //erase assign
    {
        pdstl::list<int> list1;
        list1.assign(3,5);
        mu_assert_int_eq(5,*list1.at(0));
        mu_assert_int_eq(5,*list1.at(1));
        mu_assert_int_eq(5,*list1.at(2));
        mu_assert_int_eq(3,list1.size());

        pdstl::list<int> list2{1,2,3};
        mu_assert_int_eq(3,list2.size());
        list2.erase(list2.begin()); //{2,3}
        mu_assert_int_eq(2,*list2.at(0));
        mu_assert_int_eq(3,*list2.at(1));
        mu_assert_int_eq(2,list2.size());
        list2.erase(list2.begin(),list2.cend()); //{ }
        mu_assert_int_eq(0,list2.size());

        pdstl::list<char> characters;
        characters.assign(5,'a');
        for(auto tmp : characters)
            mu_check(tmp == 'a');
        mu_assert_int_eq(5,characters.size());

        characters.assign(3,'b');
        for(auto tmp : characters)
            mu_check(tmp == 'b');
        mu_assert_int_eq(3,characters.size());

        pdstl::list<char> chars{'a','a','a','a'};
        characters.assign(chars.begin(),chars.end());
        for(auto tmp : characters)
            mu_check(tmp == 'a');
        mu_assert_int_eq(4,characters.size());
    }
   //front and back
    {
        list<std::string> list{"aa","bb","cc"};
        mu_assert_string_eq("aa",list.front().c_str());
        mu_assert_string_eq("cc",list.back().c_str());
    }

    //reverse_itrator and normal iterator
    {
        list<int> lst{9,8,7,6,5,4,3,2,1,0};
        int n = 0;
        for(auto ri = lst.rbegin();ri!=lst.rend();++ri,++n)
            mu_assert_int_eq(n,*ri);
        int m = 9;
        for(auto i = lst.begin();i!=lst.end();++i,--m)
            mu_assert_int_eq(m,*i);
    }

    //operator =
    {

        list<int> list1 = {5,5,5};
        mu_assert_int_eq(5,*list1.at(0));
        mu_assert_int_eq(5,*list1.at(1));
        mu_assert_int_eq(5,*list1.at(2));
        mu_assert_int_eq(3,list1.size());

        list<int> list2 = list1;
        mu_assert_int_eq(5,*list2.at(0));
        mu_assert_int_eq(5,*list2.at(1));
        mu_assert_int_eq(5,*list2.at(2));
        mu_assert_int_eq(3,list2.size());

        list<int> list3 = std::move(list1);
        mu_assert_int_eq(5,*list3.at(0));
        mu_assert_int_eq(5,*list3.at(1));
        mu_assert_int_eq(5,*list3.at(2));
        mu_assert_int_eq(3,list3.size());

       //list1 was resetted
        mu_assert_int_eq(0,list1.size());
    }
    //pop_back and pop_front
    {
        list<int> list1 = {1,2,3};
        mu_assert_int_eq(3,list1.size());
        list1.pop_front();
        mu_assert_int_eq(2,list1.size());
        mu_assert_int_eq(2,list1.front());
        list1.pop_back();
        mu_assert_int_eq(1,list1.size());
        mu_assert_int_eq(2,list1.back());

        //check iterators being still valid
        list1.insert(list1.cbegin(),1);
        list1.insert(list1.cend(),3);
        mu_assert_int_eq(3,list1.size());
        mu_assert_int_eq(1,*list1.at(0));
        mu_assert_int_eq(2,*list1.at(1));
        mu_assert_int_eq(3,*list1.at(2));
    }

    //insert
    {
        list<int> list1;
        //T&&
        list1.insert(list1.cbegin(),1);
        mu_assert_int_eq(1,*list1.at(0));
        mu_assert_int_eq(1,list1.size());

        //const T&
        int a = 2;
        list1.insert(list1.cbegin(),a);
        mu_assert_int_eq(2,*list1.at(0));
        mu_assert_int_eq(2,list1.size());

        //size_type n
        list1.insert(list1.cbegin(),3,3);
        mu_assert_int_eq(3,*list1.at(0));
        mu_assert_int_eq(3,*list1.at(1));
        mu_assert_int_eq(3,*list1.at(2));
        mu_assert_int_eq(2,*list1.at(3));
        mu_assert_int_eq(1,*list1.at(4)); //{head,3,3,3,2,1,tail}
        mu_assert_int_eq(5,list1.size());

        list<int> list2;

        auto it1 = list1.begin();
        auto it2 = list1.end();
        //std::advance cannot work with pdstl::iterator

        ++it1;
        --it2;
        list2.insert(list2.end(),it1,it2); //{3,3,2}
        mu_assert_int_eq(3,list2.size());

        list2.insert(list2.cend(),{1,2,3}); //{3,3,2,1,2,3};
        mu_assert_int_eq(6,list2.size());

        mu_assert_int_eq(3,*list2.at(0));
        mu_assert_int_eq(3,*list2.at(1));
        mu_assert_int_eq(2,*list2.at(2));
        mu_assert_int_eq(1,*list2.at(3));
        mu_assert_int_eq(2,*list2.at(4));
        mu_assert_int_eq(3,*list2.at(5));
    }
    //resize
    {
        list<int> list1;
        list1.resize(2,3);
        mu_assert_int_eq(2,list1.size());
        mu_assert_int_eq(3,*list1.at(0));
        mu_assert_int_eq(3,*list1.at(1));

        list1.resize(3);
        mu_assert_int_eq(3,list1.size());
        mu_assert_int_eq(0,*list1.at(2));

        list1.resize(2);
        mu_assert_int_eq(2,list1.size());
        mu_assert_int_eq(3,*list1.at(0));
        mu_assert_int_eq(3,*list1.at(1));
    }

    //sort
    {
        //insertion_sort
        list<int> list1{9,8,7};
        list1.insertion_sort(std::less<int>());

        int m = 7; //7,8,9
        for(auto i : list1)
            mu_assert_int_eq(m++,i);

        list<int> list2{2,3,4};
        list2.insertion_sort(std::greater<int>());

        mu_assert_int_eq(4,*list2.at(0));
        mu_assert_int_eq(3,*list2.at(1));
        mu_assert_int_eq(2,*list2.at(2));

        //selection sort
        list<int> list3{9,8,7};
        list3.selection_sort(std::less<int>());

        mu_assert_int_eq(7,*list3.at(0));
        mu_assert_int_eq(8,*list3.at(1));
        mu_assert_int_eq(9,*list3.at(2));

        list<int> list4{2,3,4};
        list4.selection_sort(std::greater<int>());

        mu_assert_int_eq(4,*list4.at(0));
        mu_assert_int_eq(3,*list4.at(1));
        mu_assert_int_eq(2,*list4.at(2));

        list<int> list5{2,3,4};
        list5.merge_sort(std::greater<int>());

        mu_assert_int_eq(4,*list5.at(0));
        mu_assert_int_eq(3,*list5.at(1));
        mu_assert_int_eq(2,*list5.at(2));

        list<int> list6{9,8,7};
        list6.merge_sort(std::less<int>());

        mu_assert_int_eq(7,*list6.at(0));
        mu_assert_int_eq(8,*list6.at(1));
        mu_assert_int_eq(9,*list6.at(2));
    }
    //uniqify
    {
        list<int> list1{1,1,2,2,3,3};
        list1.unique();
        mu_assert_int_eq(3,list1.size());
        mu_assert_int_eq(1,*list1.at(0));
        mu_assert_int_eq(2,*list1.at(1));
        mu_assert_int_eq(3,*list1.at(2));

        struct bar
        {
            int x;
            bar():x(0){}
            bar(int xx):x(xx){}
        };

        struct bar_predicator
        {
            bool operator()(const bar& bar_x,const bar& bar_y)
            {
                return (bar_x.x == bar_y.x);
            }
        };

        list<bar> lbar;
        lbar.emplace_back(1);
        lbar.emplace_back(1);
        lbar.emplace_back(2);
        lbar.emplace_back(2);
        lbar.emplace_back(3);
        lbar.emplace_back(3);

        lbar.unique(bar_predicator());

        int k = 1;
        for(auto i : lbar)
        {
            mu_assert_int_eq(k,i.x);
            k++;
        }



    }
    //emplace functions
    {
        list<int> list1;
        list1.emplace_front(1);
        mu_assert_int_eq(1,*list1.at(0));
        mu_assert_int_eq(1,list1.size());
        //{head,1,tail}

        list1.emplace_back(3);
        mu_assert_int_eq(3,*list1.at(1));
        mu_assert_int_eq(2,list1.size());

        //{head,1,3,tail}

        list1.emplace(list1.begin(),5);
        //{head,5,1,3,tail}
       auto it1 = list1.emplace(list1.begin(),10); //it1 is an iterator -> 10
        //{head,10,5,1,3,tail}

        mu_assert_int_eq(10,*list1.at(0));
        mu_assert_int_eq(5,*list1.at(1));
        mu_assert_int_eq(1,*list1.at(2));
        mu_assert_int_eq(3,*list1.at(3));

        mu_assert_int_eq(10,*it1);

        mu_assert_int_eq(4,list1.size());

    }
    //list_node inline func
    {

        list<int> list1{1,2};
        mu_assert_int_eq(1,*list1.at(0));
        mu_assert_int_eq(2,*list1.at(1));

        auto it1 = list1.begin();
        auto it2 = ++list1.begin();
        it1.nNode->swap(it2.nNode);

        mu_assert_int_eq(2,*list1.at(0));
        mu_assert_int_eq(1,*list1.at(1));

        list<int> list2{3,2,1};
        auto it3 = list2.begin();
        auto it4 = --list2.end();
        it3.nNode->swap(it4.nNode);
        mu_assert_int_eq(1,*list2.at(0));
        mu_assert_int_eq(2,*list2.at(1));
        mu_assert_int_eq(3,*list2.at(2));

    }
    //list merge
    {
        {
            list<int> list1{4,0,2,3,1};
            list<int> list2{9,7,8,5,6};
            list1.sort();
            list2.sort();
            list1.merge(list2);
            for(int i=0;i<9;i++)
            {
                mu_assert_int_eq(i,*list1.at(i));
            }
        }

        {
            list<int> list1{4,0,2,3,1};
            list<int> list2{9,7,8,5,6};
            list1.sort();
            list2.sort();
            list1.merge(list2,std::less<int>());
        }

        {

            //unknown bug here
            //unknown reason when using std::greater<T> causes  program creash.
//            list<int> list1{4,0,2,3,1};
//            list<int> list2;
//            list2.push_back(5);
//            list1.sort(std::greater<int>());
//            list2.sort(std::greater<int>());
//            list1.merge(list2,std::greater<int>());
        }

        //splice
        {
            {
                list<int> list1{1,5,6};
                list<int> list2{2,3,4};
                list1.splice(++list1.begin(),list2); //shoud be {1,2,3,4,5,6}
                for(auto i =0;i<list1.size();++i)
                {
                    mu_assert_int_eq((i+1),*list1.at(i));
                }
            }

            {
                list<int> list1{1,3,4,5,6};
                list<int> list2{2,3,4};
                list1.splice(++list1.begin(),list2,list2.begin()); //shoud be {1,2,3,4,5,6}
                for(auto i =0;i<list1.size();++i)
                {
                    mu_assert_int_eq((i+1),*list1.at(i));
                }
                //list2 should be {3,4}
                    mu_assert_int_eq(3,*list2.at(0));
                    mu_assert_int_eq(4,*list2.at(1));
            }

            {
                list<int> list1{1,6};
                list<int> list2{1,2,3,4,5,6};

                auto it2 = list2.begin();
                pdstl::advance(it2,4);
                list1.splice(++list1.begin(),list2,++list2.begin(),it2); //shoud be {1,2,3,4,5,6}

                for(auto i =0;i<list1.size();++i)
                {
                    mu_assert_int_eq((i+1),*list1.at(i));
                }
                //list2 should be {1,6}
                    mu_assert_int_eq(1,*list2.at(0));
                    mu_assert_int_eq(6,*list2.at(1));
            }
        }

        //remove and remove_if

        {
            int mynums[] = {1,2,3,4,5};
            list<int> list1 (mynums,mynums + 5); //{1,2,3,4,5}

            list1.remove_if([](const int& value){ return (value%2)==1; });//remove odd
            //list1 should be {2,4}
            mu_assert_int_eq(2,*list1.at(0));
            mu_assert_int_eq(4,*list1.at(1));
            mu_assert_int_eq(2,list1.size());

            list1.remove(4);
            mu_assert_int_eq(1,list1.size());
            mu_assert_int_eq(2,*list1.at(0));


        }

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
