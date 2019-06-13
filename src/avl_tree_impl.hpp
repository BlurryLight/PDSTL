#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

//#define DEBUG_FLAG

#ifdef DEBUG_FLAG
#include <iostream>
#endif

#include "allocator.h"
#include "iterator.hpp"
#include "pair.hpp"
#include "utility.hpp"

#include <algorithm> //std::max

namespace pdstl {

#ifdef DEBUG_FLAG
static int vec_left[100] = {0}; //help work
#endif

template<typename T>
struct TreeNode
{
    T data;
    int height;
    size_t n; //total number of nodes from subtrees and itself
    TreeNode *parent;
    TreeNode *lChild;
    TreeNode *rChild;
    TreeNode()
        : data(T())
        , height(1)
        , n(1)
        , parent(nullptr)
        , lChild(nullptr)
        , rChild(nullptr)
    {}

    TreeNode(const T &val)
        : data(val)
        , height(1)
        , n(1)
        , parent(nullptr)
        , lChild(nullptr)
        , rChild(nullptr)
    {}

    TreeNode(T &&val)
        : data(move(val))
        , height(1)
        , n(1)
        , parent(nullptr)
        , lChild(nullptr)
        , rChild(nullptr)
    {}

    void update_height()
    {
        this->height = std::max(this->lChild ? lChild->height : 0, this->rChild ? rChild->height : 0)
                       + 1;
    }
    void update_n()
    {
        this->n = (this->lChild ? lChild->n : 0) + (this->rChild ? rChild->n : 0) + 1;
    }

    int get_imbalance_factor()
    {
        return (lChild ? lChild->height : 0) - (rChild ? rChild->height : 0);
    }

    TreeNode *succ()
    {
        auto tmp = this;
        if (tmp->rChild) {
            tmp = tmp->rChild;
            while (tmp->lChild)
                tmp = tmp->lChild;
        } else {
            auto next = tmp->parent;
            while (next && next->rChild == tmp) {
                next = next->parent;
                tmp = tmp->parent;
            }
            //mannual step
            tmp = tmp->parent;
        }
        return tmp;
    }

    TreeNode *prev()
    {
        auto tmp = this;
        if (tmp->lChild) {
            tmp = tmp->lChild;
            while (tmp->rChild)
                tmp = tmp->rChild;
        } else {
            auto next = tmp->parent;
            while (next && next->lChild == tmp) {
                next = next->parent;
                tmp = tmp->parent;
            }
            //mannual step
            tmp = tmp->parent;
        }
        return tmp;
    }
#ifdef DEBUG_FLAG
private:
    //debug mode
    //this function prints "pretty" Binary tree for debugging
    void DisplaySubtree(TreeNode *root, int ident)
    {
        std::fill(std::begin(vec_left), std::end(vec_left), 0);
        if (ident > 0) {
            for (int i = 0; i < ident - 1; ++i) {
                printf(vec_left[i] ? "│   " : "    ");
            }
            printf(vec_left[ident - 1] ? "├── " : "└── ");
        }

        if (!root) {
            printf("(null)\n");
            return;
        }

        std::cout << root->data << "\n";
        if (!root->lChild && !root->rChild) {
            return;
        }

        vec_left[ident] = 1;
        DisplaySubtree(root->rChild, ident + 1);
        vec_left[ident] = 0;
        DisplaySubtree(root->lChild, ident + 1);
    }

    void PrintSubtree(TreeNode *root)
    {
        if (root == nullptr)
            return;
        PrintSubtree(root->lChild);
        std::cout << root->data << "\t";
        PrintSubtree(root->rChild);
    }

public:
    void Display() { this->DisplaySubtree(this, 0); }
    void Print() { PrintSubtree(this); }
#endif
};

template<typename T, typename Pointer, typename Reference>
struct TreeIterator
{
    template<typename U, typename V>
    friend class AVLTree;

    typedef size_t size_type;
    typedef T value_type;
    typedef TreeNode<T> node_type;
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag iterator_category;
    typedef Reference reference;
    typedef Pointer pointer;

    //traits
    typedef TreeIterator<T, Pointer, Reference> iterator;
    //定义往const的转换
    operator TreeIterator<T, const T *, const T &>()
    {
        return TreeIterator<T, const T *, const T &>(this->ptr);
    }

public:
    TreeIterator(node_type *pt = nullptr)
        : ptr(pt)
    {}
    TreeIterator(const iterator &other)
        : ptr(other.ptr)
    {}
    //++i
    iterator &operator++()
    {
        ptr = ptr->succ();
        return *this;
    }

    iterator &operator++(int)
    {
        iterator tmp(*this);
        ptr = ptr->succ();
        return tmp;
    }

    iterator &operator--()
    {
        ptr = ptr->prev();
        return *this;
    }

    iterator &operator--(int)
    {
        iterator tmp(*this);
        ptr = ptr->prev();
        return tmp;
    }

    reference operator*() const { return ptr->data; }

    pointer operator->() const { return &(ptr->data); }

private:
    node_type *ptr;
    template<typename _T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
    friend bool operator==(const TreeIterator<_T, PointerA, ReferenceA> &a,
                           const TreeIterator<_T, PointerB, ReferenceB> &b);

    template<typename _T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
    friend bool operator!=(const TreeIterator<_T, PointerA, ReferenceA> &a,
                           const TreeIterator<_T, PointerB, ReferenceB> &b);
    template<typename _T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
    friend bool operator<(const TreeIterator<_T, PointerA, ReferenceA> &a,
                          const TreeIterator<_T, PointerB, ReferenceB> &b);

    template<typename _T, typename PointerA, typename ReferenceA>
    friend bool operator==(const TreeIterator<_T, PointerA, ReferenceA> &a,
                           const TreeIterator<_T, PointerA, ReferenceA> &b);
    template<typename _T, typename PointerA, typename ReferenceA>
    friend bool operator!=(const TreeIterator<_T, PointerA, ReferenceA> &a,
                           const TreeIterator<_T, PointerA, ReferenceA> &b);
    template<typename _T, typename PointerA, typename ReferenceA>
    friend bool operator<(const TreeIterator<_T, PointerA, ReferenceA> &a,
                          const TreeIterator<_T, PointerA, ReferenceA> &b);
};

template<typename T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
bool operator==(const TreeIterator<T, PointerA, ReferenceA> &a,
                const TreeIterator<T, PointerB, ReferenceB> &b)
{
    return a.ptr == b.ptr;
}

template<typename T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
bool operator!=(const TreeIterator<T, PointerA, ReferenceA> &a,
                const TreeIterator<T, PointerB, ReferenceB> &b)
{
    return a.ptr != b.ptr;
}

template<typename T, typename Pointer, typename Reference>
bool operator==(const TreeIterator<T, Pointer, Reference> &a,
                const TreeIterator<T, Pointer, Reference> &b)
{
    return a.ptr == b.ptr;
}

template<typename T, typename Pointer, typename Reference>
bool operator!=(const TreeIterator<T, Pointer, Reference> &a,
                const TreeIterator<T, Pointer, Reference> &b)
{
    return a.ptr != b.ptr;
}

template<typename T, typename PointerA, typename ReferenceA, typename PointerB, typename ReferenceB>
bool operator<(const TreeIterator<T, PointerA, ReferenceA> &a,
               const TreeIterator<T, PointerB, ReferenceB> &b)
{
    return *a < *b;
}

template<typename T, typename Pointer, typename Reference>
bool operator<(const TreeIterator<T, Pointer, Reference> &a,
               const TreeIterator<T, Pointer, Reference> &b)
{
    return *a < *b;
}

template<typename T, typename Alloc = pdstl::Allocator<TreeNode<T>>>
class AVLTree
{
    //traits
public:
    typedef T value_type;
    typedef Alloc Allocator_type;
    typedef size_t size_type;
    typedef typename Alloc::difference_type difference_type;
    typedef T &reference;
    typedef const T &const_reference;
    typedef TreeIterator<T, T *, T &> iterator;
    typedef TreeIterator<T, const T *, const T &> const_iterator;
    typedef T *pointer;
    typedef TreeNode<T> node_type;

    //interfaces
    AVLTree() noexcept;
    AVLTree(const AVLTree &other) noexcept;
    AVLTree(AVLTree &&other) noexcept;
    AVLTree &operator=(const AVLTree &other);
    ~AVLTree() noexcept;

    iterator insert(const T &val);
    iterator insert(T &&val);

    iterator at(size_type i);
    reference operator[](size_type i);
    const_reference operator[](size_type i) const;
    iterator erase(iterator it);
    void remove(const_reference value);
    iterator find(const_reference value);

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    bool empty() const { return root->lChild == nullptr; }
    size_type size() const { return root->n; }
    void swap(AVLTree &other) { std::swap(this->root, other.root); }

#ifdef DEBUG_FLAG
public:
#else
private:
#endif

    Alloc dataAlloc;
    node_type *root;
    node_type *createNode(const T &val = T())
    {
        auto p = dataAlloc.allocate();
        dataAlloc.construct(p, node_type(val));
        return p;
    }
    void clear_node_recur(node_type *root)
    {
        if (root == nullptr)
            return;

        if (root->lChild) {
            clear_node_recur(root->lChild);
        }
        if (root->rChild) {
            clear_node_recur(root->rChild);
        }
        dataAlloc.destroy(root);
        dataAlloc.deallocate(root);
        root = nullptr;
    }
    node_type *deep_copy_recur(node_type *root)
    {
        node_type *new_root = createNode(root->data);
        new_root->n = root->n;
        new_root->height = root->height;
        if (root->lChild) {
            new_root->lChild = deep_copy_recur(root->lChild);
            new_root->lChild->parent = new_root;
        }
        if (root->rChild) {
            new_root->rChild = deep_copy_recur(root->rChild);
            new_root->rChild->parent = new_root;
        }
        return new_root;
    }

    void right_rotate(node_type *root);

    void left_rotate(node_type *root);
};
template<typename T, typename Alloc>
AVLTree<T, Alloc>::AVLTree() noexcept
{
    root = createNode();
    root->n = 0;
}
template<typename T, typename Alloc>
AVLTree<T, Alloc>::AVLTree(const AVLTree &other) noexcept
{
    root = deep_copy_recur(other.root);
}
template<typename T, typename Alloc>
AVLTree<T, Alloc>::AVLTree(AVLTree &&other) noexcept
{
    root = other.root;
    other.root = createNode();
    other.root->n = 0;
}

template<typename T, typename Alloc>
AVLTree<T, Alloc> &AVLTree<T, Alloc>::operator=(const AVLTree &other)
{
    root = deep_copy_recur(other.root);
    return *this;
}
template<typename T, typename Alloc>
AVLTree<T, Alloc>::~AVLTree() noexcept
{
    clear_node_recur(root->lChild);
    clear_node_recur(root->rChild);
    root->lChild = nullptr;
    root->rChild = nullptr;
    dataAlloc.destroy(root);
    dataAlloc.deallocate(root);
}

template<typename T, typename Alloc>
void AVLTree<T, Alloc>::left_rotate(node_type *root)
{
    node_type *new_root = root->rChild;
    node_type *tmp = root->rChild->lChild;
    if (root->parent) {
        if (root == root->parent->lChild) {
            root->parent->lChild = new_root;
        } else {
            root->parent->rChild = new_root;
        }
    }

    new_root->parent = root->parent;
    new_root->lChild = root;
    root->parent = new_root;
    root->rChild = tmp;
    if (tmp)
        tmp->parent = root;
    //update informations
    //keep in order
    root->update_n();
    new_root->update_n();

    while (root) {
        root->update_height();
        root = root->parent;
    }
}

template<typename T, typename Alloc>
void AVLTree<T, Alloc>::right_rotate(node_type *root)
{
    /*
         *   root
         *  /           ->rightrotate     new_root
         *  new_root                       /     \
         *  / \                           T1      root
         * T1  T2                                /
         *                                    T2
         */
    node_type *new_root = root->lChild;
    node_type *tmp = root->lChild->rChild;
    if (root->parent) {
        if (root == root->parent->lChild) {
            root->parent->lChild = new_root;
        } else {
            root->parent->rChild = new_root;
        }
    }

    new_root->parent = root->parent;
    new_root->rChild = root;
    root->parent = new_root;
    root->lChild = tmp;
    if (tmp)
        tmp->parent = root;
    //update informations
    //keep in order
    root->update_n();
    new_root->update_n();

    while (root) {
        root->update_height();
        root = root->parent;
    }
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::iterator AVLTree<T, Alloc>::insert(const T &val)
{
    iterator res;
    node_type *parent = root; //parent will be the parent of newly inserted node
    while (true) {
        ++parent->n;
        //root is a dummy node
        //real AVL is in its left-subtree
        if (parent == root || parent->data > val) {
            if (parent->lChild) {
                parent = parent->lChild;
            } else {
                parent->lChild = createNode(val);
                parent->lChild->parent = parent;
                res = iterator(parent->lChild);
                break;
            }
        } else {
            if (parent->rChild) {
                parent = parent->rChild;
            } else {
                parent->rChild = createNode(val);
                parent->rChild->parent = parent;
                res = iterator(parent->rChild);
                break;
            }
        }
    }

    int branch_height = 1;
    while (parent) {
        if (parent == root)
            break;
        if (parent->height > branch_height)
            break;
        parent->height = branch_height + 1;
        if (parent->get_imbalance_factor() > 1) {
            //LR case
            if (parent->lChild->get_imbalance_factor() < 0) {
                left_rotate(parent->lChild);
            }
            //LL case
            right_rotate(parent);
            break;
        } else if (parent->get_imbalance_factor() < -1) {
            //RL case
            if (parent->rChild->get_imbalance_factor() > 0) {
                right_rotate(parent->rChild);
            }
            //rr case
            left_rotate(parent);
            break;
        }
        branch_height = parent->height;
        parent = parent->parent;
    }
    parent->update_height();
    return res;
}

//This T&& is not forwarding reference
//it absolutely is a RVALUE Reference
template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::iterator AVLTree<T, Alloc>::insert(T &&val)
{
    iterator res;
    node_type *parent = root; //parent will be the parent of newly inserted node
    while (true) {
        ++parent->n;
        //root is a dummy node
        //real AVL is in its left-subtree
        if (parent == root || parent->data > val) {
            if (parent->lChild) {
                parent = parent->lChild;
            } else {
                parent->lChild = createNode(pdstl::move(val));
                parent->lChild->parent = parent;
                res = iterator(parent->lChild);
                break;
            }
        }
        //right sub_tree
        else {
            if (parent->rChild) {
                parent = parent->rChild;
            } else {
                parent->rChild = createNode(pdstl::move(val));
                parent->rChild->parent = parent;
                res = iterator(parent->rChild);
                break;
            }
        }
    }

    int branch_height = 1;
    //parent now still stays above the newly inserted node
    //and it's time to update height
    //and rotate
    while (parent) {
        /*
            //         A(4)
            //       /     \
            //      B(3)   C(2)
            //  now A's height is 4 and its balance factor is 1;
            // when subtree C inserts new node,A neither needs to update Height
            // nor rotate
            */
        if (parent == root)
            break;
        if (parent->height > branch_height)
            break;
        parent->height = branch_height + 1;
        if (parent->get_imbalance_factor() > 1) {
            //LR case
            if (parent->lChild->get_imbalance_factor() < 0) {
                left_rotate(parent->lChild);
            }
            //LL case
            right_rotate(parent);
            break;
        } else if (parent->get_imbalance_factor() < -1) {
            //RL case
            if (parent->rChild->get_imbalance_factor() > 0) {
                right_rotate(parent->rChild);
            }
            //rr case
            left_rotate(parent);
            break;
        }
        branch_height = parent->height;
        parent = parent->parent;
    }
    parent->update_height();
    return res;
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::iterator AVLTree<T, Alloc>::erase(iterator it)
{
    iterator res(it);
    ++res;
    auto ptr = it.ptr;
    TreeNode<T> *q;
    if (!ptr->lChild || !ptr->rChild) {
        q = ptr;
    } else {
        q = res.ptr; //ptr has both child
    }

    TreeNode<T> *s;
    if (q->lChild) {
        s = q->lChild;
        q->lChild = nullptr;
    } else { //including q->rChild = nullptr;
        s = q->rChild;
        q->rChild = nullptr;
    }

    if (s) {
        s->parent = q->parent;
    }

    if (q == q->parent->lChild) {
        q->parent->lChild = s;
    } else {
        q->parent->rChild = s;
    }
    TreeNode<T> *q_parent = q->parent;

    if (q != ptr) //swap q and ptr
    //q is the succ of ptr
    //and ptr is the node to be deleted
    {
        q->parent = ptr->parent;
        if (q->parent->lChild == ptr) {
            q->parent->lChild = q;
        } else {
            q->parent->rChild = q;
        }
        q->lChild = ptr->lChild;
        q->rChild = ptr->rChild;
        if (q->lChild) {
            q->lChild->parent = q;
        }
        if (q->rChild) {
            q->rChild->parent = q;
        }
        q->n = ptr->n;
        q->height = ptr->height;
        ptr->lChild = nullptr;
        ptr->rChild = nullptr;
    }
    if (q_parent == ptr)
        q_parent = q; //now q has been in ptr's position
    for (auto parent = q_parent; parent; parent = parent->parent) {
        --parent->n;
    }
    for (auto parent = q_parent; parent; parent = parent->parent) {
        parent->update_height();
        if (parent == root)
            break;
        if (parent->get_imbalance_factor() > 1) { //check LR double-rotation case
            if (parent->lChild->get_imbalance_factor() < 0) {
                left_rotate(parent->lChild);
            }
            //else ll case
            right_rotate(parent);
            break;
        } else if (parent->get_imbalance_factor() < -1) {
            //check RL case
            if (parent->rChild->get_imbalance_factor() > 0) {
                right_rotate(parent->rChild);
            }
            left_rotate(parent);
            break;
        }
    }
    dataAlloc.destroy(ptr);
    dataAlloc.deallocate(ptr);
    return res;
}

template<typename T, typename Alloc>
void AVLTree<T, Alloc>::remove(const_reference value)
{
    iterator it = find(value);
    if (it == end()) {
        return;
    } else {
        this->erase(it);
    }
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::iterator AVLTree<T, Alloc>::find(const_reference value)
{
    node_type *ptr = root->lChild;
    while (ptr) {
        if (ptr->data == value)
            return iterator(ptr);
        else if (ptr->data < value) {
            ptr = ptr->rChild;
        } else {
            ptr = ptr->lChild;
        }
    }
    return end();
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::iterator AVLTree<T, Alloc>::begin()
{
    node_type *ptr = root;
    while (ptr->lChild) {
        ptr = ptr->lChild;
    }
    return iterator(ptr);
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::const_iterator AVLTree<T, Alloc>::begin() const
{
    return static_cast<const_iterator>(begin());
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::const_iterator AVLTree<T, Alloc>::cbegin() const
{
    return static_cast<const_iterator>(begin());
}
template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::iterator AVLTree<T, Alloc>::end()
{
    return iterator(root);
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::const_iterator AVLTree<T, Alloc>::end() const
{
    return const_iterator(root);
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::const_iterator AVLTree<T, Alloc>::cend() const
{
    return const_iterator(root);
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::iterator AVLTree<T, Alloc>::at(size_type i)
{
    if (i >= root->n || i < 0)
        exit(1);

    node_type *real_root = root->lChild;
    size_type index = i + 1;
    /* https://wxx5433.github.io/findget-bst-node-index.html
        * Analysis:
        * index <= n of (left subtree) search the left subtree
        * index == n of (left subtree) + 1  return current node
        * index > n of (leftsubtree) + 1   index -= n of leftsubtree + 1;
        * then search the right subtree
        */

    while (real_root) {
        int left_size = real_root->lChild ? real_root->lChild->n : 0;

        if (index <= left_size) {
            real_root = real_root->lChild;
        } else if (index == (left_size + 1)) {
            break;
        } else {
            index -= (left_size + 1);
            real_root = real_root->rChild;
        }
    }
    return iterator(real_root);
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::reference AVLTree<T, Alloc>::operator[](size_type i)
{
    return *(at(i));
}

template<typename T, typename Alloc>
typename AVLTree<T, Alloc>::const_reference AVLTree<T, Alloc>::operator[](size_type i) const
{
    return *(at(i));
}

} //namespace pdstl

#endif
