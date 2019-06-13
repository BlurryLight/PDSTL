#include "avl_tree_impl.hpp"
#include "minunit.h"
#include <algorithm> //std
#include <chrono>
#include <iostream>
#include <vector>

using namespace pdstl;

MU_TEST(test_avl_tree_node_check)
{
    {
        auto root = new TreeNode<int>(2);
        root->lChild = new TreeNode<int>(1);
        root->rChild = new TreeNode<int>(3);
        root->lChild->parent = root;
        root->rChild->parent = root;
#ifdef DEBUG_FLAG
        root->Display();
#endif
        root->update_n();
        root->update_height();
        mu_assert_int_eq(2, root->data);
        mu_assert_int_eq(1, root->lChild->data);
        mu_assert_int_eq(3, root->rChild->data);
        mu_assert_int_eq(3, root->n);
        mu_assert_int_eq(2, root->height);
        mu_assert_int_eq(0, root->get_imbalance_factor());
        mu_assert_int_eq(1, root->prev()->data);
        mu_assert_int_eq(3, root->succ()->data);
        mu_assert_int_eq(2, root->lChild->succ()->data);
        mu_check(nullptr == root->rChild->succ());
    }
}

MU_TEST(test_avl_tree_check)
{
    //insert and rotate
    {
        AVLTree<int> it;
        mu_assert_int_eq(1, *it.insert(1));
        mu_assert_int_eq(2, *it.insert(2));
        mu_assert_int_eq(3, *it.insert(3));
        mu_assert_int_eq(4, *it.insert(4));
    }

    //insert char and rotate
    {
        AVLTree<char> it;
        char test = 'a';
        mu_check(*it.insert(test) == 'a');
        for (int i = 1; i != 10; ++i)
            it.insert(static_cast<char>('a' + i));
#ifdef DEBUG_FLAG
        it.root->lChild->Display();
        it.root->Print();
#endif
    }
    //at
    {
        AVLTree<int> it;
        for (int i = 0; i != 10; ++i)
            it.insert(i);
        for (int i = 0; i != 10; ++i) {
            typename AVLTree<int>::const_iterator itt = it.at(i);
            mu_assert_int_eq(i, *itt);
        }
    }
    //operator []
    {
        AVLTree<int> it;
        for (int i = 0; i != 10; ++i)
            it.insert(i);
        for (int i = 0; i != 10; ++i) {
            mu_assert_int_eq(i, it[i]);
        }
    }
    //find
    {
        AVLTree<int> it;
        for (int i = 0; i != 10; ++i)
            it.insert(i);
        for (int i = 0; i != 10; ++i) {
            mu_assert_int_eq(i, *it.find(i));
        }
    }

    //erase
    {
        AVLTree<int> it;
        for (int i = 0; i != 10; ++i)
            it.insert(i);
        for (int i = 0; i != 5; ++i) {
            mu_assert_int_eq(i + 1, *it.erase(it.at(0)));
        }
        //erase(find())
        mu_assert_int_eq(9, *it.erase(it.find(8)));
    }
    //empty && size
    {
        AVLTree<int> it;
        mu_check(it.empty());
        mu_assert_int_eq(0, it.size());
        it.insert(5);
        mu_assert_int_eq(1, it.size());
        mu_check(!it.empty());
        it.erase(it.find(5));
        mu_check(it.empty());
    }
    //remove
    {
        AVLTree<int> it;
        it.insert(3);
        it.insert(2);
        it.insert(1);
        it.remove(3);

        mu_assert_int_eq(2, it.size());
        mu_assert_int_eq(1, *it.at(0));
        mu_assert_int_eq(2, *it.at(1));
    }

    //swap
    {
        AVLTree<int> it;
        it.insert(3);
        it.insert(2);
        it.insert(1);

        AVLTree<int> it2;
        it2.swap(it);
        mu_check(it.empty());
        mu_check(it2.size() == 3);
    }

    //deep copy
    {
        AVLTree<int> it;
        it.insert(3);
        it.insert(2);
        it.insert(1);
        AVLTree<int> it2;
        it2 = it;

        mu_check(it[0] == it2[0]);
        mu_check(it[1] == it2[1]);
        mu_check(it[2] == it2[2]);
        mu_check(&it[0] != &it2[0]);
    }

    //large number
    {
        std::vector<int> values(1000000);
        for (int i = 0; i < values.size(); i++)
            values[i] = i + 1;
        std::random_shuffle(values.begin(), values.end());
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        AVLTree<int> it;
        for (int i = 0; i < values.size(); i++)
            it.insert(values[i]);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                  << std::endl;
    }
}

MU_TEST_SUITE(test_avl_tree_suite)
{
    MU_RUN_TEST(test_avl_tree_node_check);
    MU_RUN_TEST(test_avl_tree_check);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_avl_tree_suite);
    MU_REPORT();
    return minunit_status;
}
