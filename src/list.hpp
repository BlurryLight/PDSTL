#ifndef LIST_H
#define LIST_H
#include <cstddef> //for std::nullptr_t
#include "iterator.hpp"
#include <initializer_list>
#include "allocator.h"

/*
 * This list is implemented by a doubley-linked list,
 * which refuses to support any constructor with allocators
 */
namespace pdstl {

    template <typename T,typename Alloc = Allocator<T> > class list;

/*
 * inspired by EASTL
 * Seperating ListNode from ListNodeBase is to make it easier to track listnodes' values in a debugger
 */
    struct ListNodeBase
    {
        ListNodeBase* nPrev;
        ListNodeBase* nNext;
        inline void remove() //remove nodeself from its list
        {
            this->nPrev->nNext = this->nNext;
            this->nNext->nPrev = this->nPrev;
            this->nPrev = nullptr;
            this->nNext = nullptr;
        }
        inline void insertAsPrev(ListNodeBase* pNext) //inserts nodeself before a node in an existed list
        {
            this->nPrev = pNext->nPrev;
            pNext->nPrev = this;
            this->nNext = pNext;
        }
        inline void insertAsNext(ListNodeBase* pPrev) //inserts nodeself after a node in an existed list
        {
            this->nNext = pPrev->nNext;
            pPrev->nNext = this;
            this->nPrev = pPrev;
        }
    };

    template <typename T>
    struct ListNode : public ListNodeBase
    {
        T nValue;
        bool operator==( const ListNode& other)
        {
            return (nValue == other.nValue) &&(nPrev == other.nPrev) && (nNext == other.nNext);
        }
    };

    template <typename T>
//    struct ListIterator : public iterator<bidirectional_iterator_tag,T>  //std::iterator has been deprecated in C17
//    It is more recommanded to typedef iterator_tag
    struct ListIterator
    {
        typedef size_t              size_type;
        typedef T                   value_type;
        typedef ptrdiff_t           difference_type;
        typedef ListNode<T>         node_type;
        typedef T*                  pointer;
        typedef T&                  reference;
        typedef const T&            const_reference;
        typedef bidirectional_iterator_tag iterator_category;

    public:
        node_type* nNode;
    public:
        explicit ListIterator(node_type* ptr = nullptr):nNode(ptr){}


    };








}//pdstl
#endif
