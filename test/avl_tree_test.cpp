#include "avl_tree_impl.hpp"
#include "minunit.h"
#include <algorithm> //std

using namespace pdstl;

MU_TEST(test_avl_tree_node_check)
{
    {
        auto root = new TreeNode<int>(2);
        root->lChild = new TreeNode<int>(1);
        root->rChild = new TreeNode<int>(3);
        root->lChild->parent = root;
        root->rChild->parent = root;
        root->DisPlay();
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
    {
        AVLTree<int> it;
        mu_assert_int_eq(1, *it.insert(1));
        mu_assert_int_eq(2, *it.insert(2));
        mu_assert_int_eq(3, *it.insert(3));
        mu_assert_int_eq(4, *it.insert(4));
    }

    {
        AVLTree<char> it;
        char test = 'a';
        mu_check(*it.insert(test) == 'a');
        for (int i = 1; i != 10; ++i)
            it.insert(static_cast<char>('a' + i));
#ifdef DEBUG_FLAG
        it.root->lChild->DisPlay();
#endif
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
