#ifndef LIST_H
#define LIST_H
#include <cstddef> //for std::nullptr_t
#include "iterator.hpp"
#include <initializer_list>
#include <iterator> //std::reverse_iterator only
#include "allocator.h"

/*
 * This list is implemented by a doubley-linked list,
 * which refuses to support any constructor with allocators
 */
namespace pdstl {

//    template <typename T,typename Alloc = Allocator<T> > class list;
    //magic do not touch

    template <typename Iter>
    using isInputIterator = typename std::enable_if<
        std::is_convertible<typename iterator_traits<Iter>::iterator_category,
                            input_iterator_tag
                            >::value
        >::type;

/*
 * inspired by EASTL
 * Seperating ListNode from ListNodeBase is to make it easier to track listnodes' values in a debugger
 */

/*
 * ADDED: ListNodeBase is useless
 */
    template <typename T>
    struct ListNode
    {
        ListNode* nPrev;
        ListNode* nNext;
        T nValue;


        inline void remove() //remove nodeself from its list
        {
            this->nPrev->nNext = this->nNext;
            this->nNext->nPrev = this->nPrev;
            this->nPrev = nullptr;
            this->nNext = nullptr;
        }
        inline void insertAsPrev(ListNode* pNext) //inserts nodeself before a node in an existed list
        {
            this->nPrev = pNext->nPrev;
            pNext->nPrev->nNext = this;
            pNext->nPrev = this;
            this->nNext = pNext;
        }
        inline void insertAsNext(ListNode* pPrev) //inserts nodeself after a node in an existed list
        {
            this->nNext = pPrev->nNext;
            pPrev->nNext->nPrev = this;
            pPrev->nNext = this;
            this->nPrev = pPrev;
        }
        ListNode() : nValue(T())
        {
            nPrev = nullptr;
            nNext = nullptr;
        }
        ListNode(const T& val,ListNode* prev = nullptr,ListNode* succ = nullptr) : nValue(val)
        {
            nPrev = prev;
            nNext = succ;
        }
        bool operator==( const ListNode& other)
        {
            return (nValue == other.nValue) &&(nPrev == other.nPrev) && (nNext == other.nNext);
        }
    };


//    struct ListIterator : public iterator<bidirectional_iterator_tag,T>  //std::iterator has been deprecated in C17
//    It is more recommanded to typedef iterator_tag
    template <typename T,typename Pointer,typename Reference>
    struct ListIterator
    {
        typedef size_t              size_type;
        typedef T                   value_type;
        typedef ptrdiff_t           difference_type;
        typedef ListNode<T>         node_type;
        typedef Pointer             pointer; //Param Pointer and Reference are to create const_iterator
        typedef Reference           reference;
        typedef bidirectional_iterator_tag iterator_category;

    public:
        node_type* nNode;
    public:
        explicit ListIterator(node_type* ptr = nullptr):nNode(ptr){}
        ListIterator(const ListIterator& other) : nNode(const_cast<node_type*>(other.nNode)){}

        ListIterator& operator++()
        {
            nNode = nNode->nNext;
            return  *this;
        }

        ListIterator operator++(int)
        {
            auto tmp = *this;
            nNode = nNode->nNext;
            return  tmp;
        }

        ListIterator& operator--()
        {
            nNode = nNode->nPrev;
            return  *this;
        }
        ListIterator operator--(int)
        {
            auto tmp = *this;
            nNode = nNode->nPrev;
            return  tmp;
        }

        reference operator*() const { return nNode->nValue;}
        pointer operator->() const { return &(nNode->nValue);}
    };

    template <typename T,typename Pointer,typename Reference>
    bool operator==(const ListIterator<T,Pointer,Reference>& a,const ListIterator<T,Pointer,Reference>& b)
    {
        return a.nNode == b.nNode;
    }

    template <typename T,typename Pointer,typename Reference>
    bool operator!=(const ListIterator<T,Pointer,Reference>& a,const ListIterator<T,Pointer,Reference>& b)
    {
        return a.nNode != b.nNode;
    }

    template <typename T,typename Alloc = Allocator<ListNode<T>>>
    class list
    {
    public :
        typedef T       value_type;
        typedef Alloc    Allocator_type;
        typedef ListNode<T> node_type;
        typedef ptrdiff_t   difference_type;
        typedef size_t      size_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&                  reference;
        typedef const T&            const_reference;
        typedef ListIterator<T,T*,T&>           iterator;
        typedef ListIterator<T,const T*,const T&> const_iterator;
        typedef std::reverse_iterator<iterator>  reverse_iterator;
        typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
    private:
        Alloc dataAlloc;
        iterator head; //head is the dummy node before the list
        iterator tail; //tail plays the same role
        size_type _size;

        node_type* createNode(const T& val = T())
        {
            auto p = dataAlloc.allocate();
            dataAlloc.construct(p,ListNode<T>(val));
            return p;
        }
        node_type* deleteNode(node_type* p)
        {
            dataAlloc.destroy(p);
            dataAlloc.deallocate(p);
        }

        void init()
        {
            head.nNode = createNode();
            tail.nNode = createNode();
            head.nNode->nNext = tail.nNode;
            tail.nNode->nPrev = head.nNode;
        }

    public:
        //Member functions
        //ctors
        list() noexcept;
        explicit list(size_type n,const T& val);
        explicit list(size_type n);

        template <typename InputIt,typename = isInputIterator<InputIt>>
        list(InputIt first,InputIt last);
        list(const list& other);
        list(list&& other);
        list(std::initializer_list<T> ilnit);
        //dtor
        ~list();

        //operator =
        list& operator=(const list& other);
        list& operator=(list&& other);
        list& operator=(std::initializer_list<T> ilnit);
        //assign
        void assign(size_type n,const T& value);
        template <typename InputIt,typename = isInputIterator<InputIt>>
        void assign(InputIt first,InputIt last);
        void assign(std::initializer_list<T> ilnit);


        //element access
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

        //iterator;

        iterator       begin() ;
        const_iterator begin() const ;
        const_iterator cbegin() const ;

        iterator       end() ;
        const_iterator end() const ;
        const_iterator cend() const ;

        reverse_iterator       rbegin() ;
        const_reverse_iterator rbegin() const ;
        const_reverse_iterator crbegin() const ;

        reverse_iterator       rend() ;
        const_reverse_iterator rend() const ;
        const_reverse_iterator crend() const ;

        //capacity
        bool empty() const;
        size_type size()  const;
        //max_size      useless,refuse to implement

        //Modifiers
        void clear();
        void resize( size_type count );
        void resize( size_type count, const value_type& value );
        void swap(list& other);
        void pop_front();
        void pop_back();

        template <class... Args>
        reference emplace_front(Args&&... args);
        template <class... Args>
        reference emplace_back(Args&&... args);
        template< class... Args >
        iterator emplace( const_iterator pos, Args&&... args );

        void push_front(const T& value);
        void push_front(T&& value);

        void push_back(const T& value);
        void push_back(T&& value);

        iterator erase( const_iterator first, const_iterator last );
        iterator erase( const_iterator pos );

        iterator insert( const_iterator pos, const T& value );
        iterator insert( const_iterator pos, T&& value );
        iterator insert( const_iterator pos, size_type count, const T& value );
        template< class InputIt,typename = isInputIterator<InputIt>>
        iterator insert( const_iterator pos, InputIt first, InputIt last );
        iterator insert( const_iterator pos, std::initializer_list<T> ilist );

        //operattions
        void merge( list& other );
        void merge( list&& other );
        template <class Compare>
        void merge( list& other, Compare comp ); //Compare : a functor or lambda
        template <class Compare>
        void merge( list&& other, Compare comp );


        //some rvalue overloaded versions are too tedious to implement
        void splice( const_iterator pos, list& other );
        void splice( const_iterator pos, list& other, const_iterator it );
        void splice( const_iterator pos, list& other,
                     const_iterator first, const_iterator last);

        void remove(const T& val);
        template <typename Unarypredicate>
        void remove_if(Unarypredicate p);

        void reverse();

        void unique();
        template <typename Binarypredicate>
        void unique(Binarypredicate p);

        void sort();
        template <typename Compare>
        void sort(Compare comp);


        //bool

        bool operator== (const list& other) const;
        bool operator!= (const list& other) const;
        bool operator<= (const list& other) const;
        bool operator>= (const list& other) const;
        bool operator< (const list& other) const;
        bool operator> (const list& other) const;

    };

    template  <typename T,typename Alloc>
    list<T,Alloc>::list() noexcept
        : _size(0)
    {
        init();
    }

    template  <typename T,typename Alloc>
    list<T,Alloc>::~list()
    {
        while(head.nNode && head.nNode != tail.nNode)
        {
            auto tmp = head.nNode->nNext;
            deleteNode(head.nNode);
            head.nNode = tmp;
        }
    }

    template  <typename T,typename Alloc>
    void list<T,Alloc>::push_back(const T& val)
    {
        auto p = createNode(val);
        p->insertAsNext(tail.nNode);
        tail.nNode = p;
        _size++;
    }
    template  <typename T,typename Alloc>
    void list<T,Alloc>::push_back(T&& val)
    {
        auto p = createNode(std::move(val));
        p->insertAsPrev(tail.nNode);
        _size++;
    }









}//pdstl
#endif
