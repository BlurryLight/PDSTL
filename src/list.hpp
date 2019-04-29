#ifndef LIST_H
#define LIST_H
#include <cstddef> //for std::nullptr_t
#include <iterator.hpp>
#include <initializer_list>
#include <iterator> //std::reverse_iterator only
#include <allocator.h>
#include <functional> //functors
#include <utility.hpp>

/*
 * This list is implemented by a doubley-linked list,
 * which refuses to support any constructor with allocators
 */
namespace pdstl {

//    template <typename T,typename Alloc = Allocator<T> > class list;
    //magic do not touch

    template <typename Iter>
    using isInputIterator = typename std::enable_if<
        std::is_convertible<typename pdstl::iterator_traits<Iter>::iterator_category,
                            pdstl::input_iterator_tag>::value>;

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
            if(this->nPrev != nullptr)
                this->nPrev->nNext = this->nNext;
            if(this->nNext != nullptr)
                this->nNext->nPrev = this->nPrev;
            this->nPrev = nullptr;
            this->nNext = nullptr;
        }
        inline void insertAsPrev(ListNode* pNext) //inserts nodeself before a node in an existed list
        {
            this->nPrev = pNext->nPrev;
            if(pNext->nPrev != nullptr)
                pNext->nPrev->nNext = this;
            pNext->nPrev = this;
            this->nNext = pNext;
        }
        inline void insertAsNext(ListNode* pPrev) //inserts nodeself after a node in an existed list
        {
            this->nNext = pPrev->nNext;
            if(pPrev->nNext != nullptr)
                pPrev->nNext->nPrev = this;
            pPrev->nNext = this;
            this->nPrev = pPrev;
        }

        inline void swap(ListNode* other)
        {
            auto tmp_this_nPrev = this->nPrev;
            auto tmp_this_nNext = this->nNext;

            auto tmp_other_nPrev = other->nPrev;
            auto tmp_other_nNext = other->nNext;

            this->remove();
            other->remove(); //remove nodes from list

            if(tmp_this_nPrev!=nullptr)
                other->insertAsNext(tmp_this_nPrev);

            if(tmp_other_nPrev != nullptr)
                this->insertAsPrev(tmp_other_nNext);

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

        // a good blog https://izualzhy.cn/SFINAE-and-enable_if#1-sfinae%E6%98%AF%E4%BB%80%E4%B9%88
        typedef ListIterator<T,Pointer,Reference>  iterator; //traits_helper required


        //user-defined implicit conversion
        operator ListIterator<T,const T*,const T&>()
        {
            return ListIterator<T,const T*,const T&>(this->nNode);
        }

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

    //Enable const_iterator to compare with non-const version
    template <typename T,typename PointerA,typename ReferenceA,typename PointerB,typename ReferenceB>
    bool operator==(const ListIterator<T,PointerA,ReferenceA>& a,const ListIterator<T,PointerB,ReferenceB>& b)
    {
        return a.nNode == b.nNode;
    }

    template <typename T,typename PointerA,typename ReferenceA,typename PointerB,typename ReferenceB>
    bool operator!=(const ListIterator<T,PointerA,ReferenceA>& a,const ListIterator<T,PointerB,ReferenceB>& b)
    {
        return a.nNode != b.nNode;
    }

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
        void deleteNode(node_type* p)
        {
            if(p->nNext !=nullptr)
                p->nNext->nPrev = p->nPrev;
            if(p->nPrev !=nullptr)
                p->nPrev->nNext = p->nNext;
            dataAlloc.destroy(p);
            dataAlloc.deallocate(p);
            p = nullptr;
            --_size;
        }

        void init()
        {
            _size = 0;
            head.nNode = createNode();
            tail.nNode = createNode();
            head.nNode->nNext = tail.nNode;
            tail.nNode->nPrev = head.nNode;
        }

//    protected:
    public: // for test only
        //sorting algorithms practice
        //warning : All iterators will be invalid.



        /* In place insertion  Time complexity : O(n^2) / Space complexity : O(1)*/
        template <typename Compare>
            void insertion_sort(Compare comp)
            {

                /*
                 * 新建一个dummy节点，cur遍历原链表
                 */
               auto dummy = createNode();
               auto head_node = head.nNode;
               auto tail_node = tail.nNode;
               ListNode<T>* cur = head_node->nNext;
               ListNode<T>* pre = dummy;
               while(cur  != tail_node)
               {
                   while(pre->nNext !=nullptr && comp(pre->nNext->nValue,cur->nValue))
                   {
                       pre = pre->nNext;
                   }
                   auto cur_next = cur->nNext;
                   cur->insertAsNext(pre);
                   pre = dummy;
                   cur = cur_next;
               }

               //manually attach the tail_node to the sorted list
               while(pre->nNext != nullptr)
                   pre = pre->nNext;
               cur->insertAsNext(pre);
               pre = dummy;
               head.nNode = pre;
               deleteNode(head_node);
            }


        template <typename Compare>
            void selection_sort(Compare comp)
            {
                auto head_node = head.nNode;
                auto tail_node = tail.nNode;
                ListNode<T>* cur = head_node->nNext;
                ListNode<T>* cur_next;
                ListNode<T>* foo = cur->nNext;
                ListNode<T>* candidate = head_node->nNext;
                while(cur != tail_node)
                {
                    candidate = cur;
                    cur_next = cur->nNext;

                    while(foo != tail_node)
                    {
                        if(comp(foo->nValue,candidate->nValue))
                        {
                            candidate = foo;
                        }
                        foo = foo->nNext;
                    }
                    if(candidate != cur)
                    {
                        candidate->swap(cur);
                    }

                    cur = cur_next;
                    foo = cur_next->nNext;
                }
            }


        template <typename Compare>
            void merge_sort(Compare comp)
            {
                //List<int> example{1,2,3}
                //真正的数据结构是 0(head.nNode)<->1<->2<->3<->0(tail.nNode)->nullptr
                //为了简化排序，将head.nNode和tail.nNode断开
                //成为 1->2->3->nullptr 的数据结构
                //排序完成为 3->2->1->nullptr后，再和head.nNode和tail.nNode连接上
                auto org_head_node = head.nNode;
                auto org_tail_node = head.nNode;

                auto cur_head_node = head.nNode->nNext;
                cur_head_node->nPrev = nullptr;
                org_head_node->nNext = nullptr;

                auto cur_tail_node = tail.nNode->nPrev;
                cur_tail_node->nNext = nullptr;
                org_tail_node->nPrev = nullptr;


                auto merged_head_node = _Merge_sort(cur_head_node,comp);

                merged_head_node->nPrev = head.nNode;
                head.nNode->nNext = merged_head_node;


                auto merged_tail_node = merged_head_node;
                while(merged_tail_node != nullptr && merged_tail_node->nNext != nullptr)
                    merged_tail_node = merged_tail_node->nNext;

                merged_tail_node->nNext = tail.nNode;
                tail.nNode->nPrev = merged_tail_node;


            }
        template <typename Compare>
            ListNode<T>*
            _Merge_sort(ListNode<T>* head,Compare comp)
            {
                if(head == nullptr || head->nNext ==nullptr) return head;
                //fast and slow

                ListNode<T>* fast = head;
                ListNode<T>* slow = head;
                while(fast->nNext !=nullptr && fast->nNext->nNext !=nullptr)
                {
                    slow = slow->nNext;
                    fast = fast->nNext->nNext;
                }

                ListNode<T>* head2 = slow->nNext;
                slow->nNext = nullptr;

                ListNode<T>* head1 = head;

                head1 = _Merge_sort(head1,comp);
                head2 = _Merge_sort(head2,comp);

                return _Merge(head1,head2,comp);
            }

        template <typename Compare>
            ListNode<T>* _Merge(ListNode<T>* h1,ListNode<T>* h2,Compare comp)
            {

                ListNode<T>* dummyhead = new ListNode<T>();
                ListNode<T>* mlist = dummyhead;
                while(h1 != nullptr && h2 !=nullptr)
                {
                    if(comp(h1->nValue,h2->nValue))
                    {
//                        h1->insertAsNext(mlist);
                        mlist->nNext = h1;
                        h1->nPrev = mlist;
                        h1 = h1->nNext;
                    }
                    else
                    {
                        mlist->nNext = h2;
                        h2->nPrev = mlist;
//                        h2->insertAsNext(mlist);
                        h2 = h2->nNext;
                    }
                    mlist = mlist->nNext;
                }

                if(h1 !=nullptr)
                {
                        mlist->nNext = h1;
                        h1->nPrev = mlist;
                }

                if(h2 !=nullptr)
                {
                        mlist->nNext = h2;
                        h2->nPrev = mlist;
                }
                auto tmphead = dummyhead->nNext;
                delete dummyhead;
                return tmphead;
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

        iterator       at(size_type rank); //out of standards
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
        bool empty() const { return _size== 0;}
        size_type size()  const { return _size;}
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
        template <class Compare>
        void merge( list& other, Compare comp ); //Compare : a functor or lambda


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
    {
        init();
    }

    template <typename T,typename Alloc>
    list<T,Alloc>::list(size_type n,const T& val)
    {
        init();
        while(n--)
            this->push_back(val);
    }

    template <typename T,typename Alloc>
    list<T,Alloc>::list(size_type n)
    {
        init();
        while(n--)
            this->push_back(T());
    }

    template <typename T,typename Alloc>
        template <typename InputIt,typename>
    list<T,Alloc>::list(InputIt first,InputIt last)
    {
        init();
        size_type n = 0;
        for(;first != last;++first)
        {
            this->push_back(*first);
            n++;
        }
        _size = n;
    }

    template <typename T,typename Alloc>
    list<T,Alloc>::list(const list& other)
    {
        init();
        auto last = other.cend();
        for(auto first = other.begin();first != last;++first)
        {
            this->push_back(*first);
        }
    }

    template <typename T,typename Alloc>
    void list<T,Alloc>::swap(list& other)
    {
        auto tmpSize = this->_size;
        node_type* tmpHead = head.nNode;
        node_type* tmpTail = tail.nNode;

        this->_size = other._size;
        head.nNode = other.head.nNode;
        tail.nNode = other.tail.nNode;

        other._size  = tmpSize;
        other.head.nNode = tmpHead;
        other.tail.nNode = tmpTail;
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::pop_front()
    {
        auto tmp = head.nNode->nNext;
        deleteNode(tmp);
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::pop_back()
    {
        auto tmp = tail.nNode->nPrev;
        deleteNode(tmp);
    }
    template <typename T,typename Alloc>
    list<T,Alloc>::list(list&& other)
    {
        init();
        this->swap(other);
    }

    template <typename T,typename Alloc>
    list<T,Alloc>::list(std::initializer_list<T> ilist)
    {
        init();
        auto last = ilist.end();
        for(auto first = ilist.begin();first != last;++first)
        {
            this->push_back(*first);
        }
    }

    template  <typename T,typename Alloc>
    void list<T,Alloc>::clear()
    {
        while(_size > 0)
            deleteNode(head.nNode->nNext);
    }


    template  <typename T,typename Alloc>
    list<T,Alloc>::~list()
    {
        clear();
        deleteNode(head.nNode);
        deleteNode(tail.nNode);

        //head和tail是属于list的元素，他门会被自动析构

//        ++head;
//        while(head.nNode && head.nNode != tail.nNode)
//        {
//            auto tmp = head.nNode->nNext;
//            deleteNode(head.nNode);
//            head.nNode = tmp;
        //        }
    }

    template<typename T, typename Alloc>
    list<T,Alloc>& list<T,Alloc>::operator=(const list& other)
    {
        this->assign(other.begin(),other.end());
    }

    template<typename T, typename Alloc>
    list<T,Alloc>& list<T,Alloc>::operator=(list&& other)
    {
        this->swap(other);
    }
    template<typename T, typename Alloc>
    list<T,Alloc>& list<T,Alloc>::operator=(std::initializer_list<T> ilist)
    {
        this->assign(ilist.begin(),ilist.end());
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::erase(const_iterator pos)
    {
        if(pos == end()) return iterator(nullptr); //undefined behaviour
//        pos.nNode->remove();  // safer to use private func of list : deleteNode
        node_type* tmp = pos.nNode->nNext;
        deleteNode(pos.nNode);
        return iterator(tmp);
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::insert(const_iterator pos, const T &value)
    {
       auto p = createNode(value);
       p->insertAsPrev(pos.nNode);
       _size++;
       return iterator(p);
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::insert(const_iterator pos, T&& value)
    {
       auto p = createNode(std::move(value));
       p->insertAsPrev(pos.nNode);
       _size++;
       return iterator(p);
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::insert(const_iterator pos,size_type n,const T& value)
    {
        while(n--)
        {
           auto p = createNode(value);
           p->insertAsPrev(pos.nNode);
           _size++;
        }
       return iterator((--pos).nNode);
    }

    template <typename T,typename Alloc>
        template<class InputIt, typename >
    typename list<T,Alloc>::iterator list<T,Alloc>::insert(const_iterator pos, InputIt first, InputIt last)
    {

        for(;first!=last;++first)
        {
           auto p = createNode(*first);
           p->insertAsPrev(pos.nNode);
           ++_size;
        }
       return iterator((--pos).nNode);
    }

    template <typename T,typename Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist)
    {
        auto first = ilist.begin();
        auto last = ilist.end();
        auto n = static_cast<size_type>(last - first);
        while(n--)
        {
           auto p = createNode(*first);
           p->insertAsPrev(pos.nNode);
           ++_size;
           ++first;
        }
       return iterator((--pos).nNode);
    }


    template<typename T, typename Alloc>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::erase(const_iterator first,const_iterator last)
    {
        while(first != last)
        {
            node_type* tmp = first.nNode;
            ++first;
            deleteNode(tmp);
        }
        return iterator(last.nNode);
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::assign(size_type n, const T &value)
    {
        auto i = begin();
        auto e = end();
        for (; i!= e && n > 0; --n,++i)
        {
            *i = value;
        }
        while(n--)
        {
            push_back(value);
        }
        erase(i,e);
    }

    template<typename T, typename Alloc>
        template <typename InputIt,typename>
    void list<T,Alloc>::assign(InputIt first, InputIt last)
    {
        auto i = begin();
        auto e = end();

        for(;i!=e && first!=last;++i,++first)
            *i = *first;
       //调整到合适大小
        if(first == last)
            erase(i,e);

        if(i == e)
            while(first!=last)
            {
                push_back(*first);
                ++first;
            }
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::assign(std::initializer_list<T> ilist)
    {
        auto i = begin();
        auto e = end();
        auto first = ilist.begin();
        auto last = ilist.end();

        for(;i!=e && first!=last;++i,++first)
            *i = *first;
       //调整到合适大小
        if(first == last)
            erase(i,e);

        if(i == e)
            while(first!=last)
            {
                push_back(*first);
                ++first;
            }
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::reference list<T,Alloc>::front()
    {
       return *begin();
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::const_reference list<T,Alloc>::front() const
    {
       return *begin();
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::reference list<T,Alloc>::back()
    {
        auto tmp = end();
        --tmp;
        return *tmp;
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::const_reference list<T,Alloc>::back() const
    {
        auto tmp = end();
        --tmp;
        return *tmp;
    }



    template  <typename T,typename Alloc>
    void list<T,Alloc>::push_back(const T& val)
    {
        auto p = createNode(val);
        p->insertAsPrev(tail.nNode);
        _size++;
    }

    template  <typename T,typename Alloc>
    void list<T,Alloc>::push_back(T&& val)
    {
        auto p = createNode(std::move(val));
        p->insertAsPrev(tail.nNode);
        _size++;
    }

    template  <typename T,typename Alloc>
    void list<T,Alloc>::push_front(const T& val)
    {
        auto p = createNode(val);
        p->insertAsNext(head.nNode);
        _size++;
    }

    template  <typename T,typename Alloc>
    void list<T,Alloc>::push_front(T&& val)
    {
        auto p = createNode(std::move(val));
        p->insertAsNext(head.nNode);
        _size++;
    }


    template  <typename T,typename Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::begin()
    {
        auto tmp = head;
        return (++tmp);
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::const_iterator list<T,Alloc>::begin() const
    {
        return cbegin();
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::end()
    {
        return tail;
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::const_iterator list<T,Alloc>::cbegin() const
    {
        auto tmp = head;
        ++tmp;
        ListIterator<T,const T*,const T&> chead;
        chead.nNode = tmp.nNode;
        return chead;
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::const_iterator list<T,Alloc>::cend() const
    {
        ListIterator<T,const T*,const T&> ctail;
        ctail.nNode = tail.nNode;
        return ctail;
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::reverse_iterator list<T,Alloc>::rbegin()
    {
        return reverse_iterator(end());
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::const_reverse_iterator list<T,Alloc>::rbegin() const
    {
        return const_reverse_iterator(end());
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::const_reverse_iterator list<T,Alloc>::crbegin() const
    {
        return const_reverse_iterator(end());
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::reverse_iterator list<T,Alloc>::rend()
    {
        return reverse_iterator(begin());
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::const_reverse_iterator list<T,Alloc>::rend() const
    {
        return const_reverse_iterator(begin());
    }

    template  <typename T,typename Alloc>
    typename list<T,Alloc>::const_reverse_iterator list<T,Alloc>::crend() const
    {
        return const_reverse_iterator(begin());
    }



    template  <typename T,typename Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::at(size_t rank)
    {
        auto first = this->begin();
        for(size_type i = 0;i < rank; ++i)
            ++first;
        return first;
    }

    template  <typename T, typename Alloc>
    void list<T,Alloc>::resize(size_type count)
    {
        resize(count,T());
    }


    template<typename T, typename Alloc>
    void list<T,Alloc>::resize(size_type count,const T& value)
    {
        if(count <= _size)
        {
            auto it = begin();
            while(count--)
                ++it;
            erase(it,end());
        }
        else {
            auto n = count - _size;
            insert(end(),n,value);
        }
    }

    template <typename T,typename Alloc>
    void list<T,Alloc>::merge(list& other)
    {
        merge(other,std::less<T>());
    }


    template <typename T,typename Alloc>
        template <typename Compare>
    void list<T,Alloc>::merge(list& other,Compare comp)
    {

        //head.nNode is a dummynode

        _size += other._size;
        other._size = 0;

        ListNode<T>* nodelist = head.nNode;
        //断开tail_Node的链接
        tail.nNode->remove();
        other.tail.nNode->remove();

        auto node1 = head.nNode->nNext;
        auto node2 = other.begin().nNode;
        while(node1 != nullptr && node2 != nullptr)
        {
            if(comp(node1->nValue,node2->nValue))
            {
                nodelist->nNext = node1;
                node1->nPrev = nodelist;
                node1=node1->nNext;
            }
            else
            {
                nodelist->nNext = node2;
                node2->nPrev = nodelist;
                node2=node2->nNext;
            }

            nodelist = nodelist->nNext;
        }

        if(node1 != nullptr)
        {
            nodelist->nNext = node1;
        }
        else
        {
            nodelist->nNext = node2;
        }

        auto tail_node_pre = nodelist;

        while(tail_node_pre !=nullptr && tail_node_pre->nNext != nullptr)
            tail_node_pre = tail_node_pre->nNext;

        tail_node_pre->nNext = tail.nNode;
        tail.nNode->nPrev = tail_node_pre;
    }

    template<typename T, typename Alloc>
        template <typename BinaryPredict>
    void list<T,Alloc>::unique(BinaryPredict Pred)
    {
        auto p = head.nNode;
        auto q = head.nNode->nNext;
        for (;q != tail.nNode;p = q,q = q->nNext)
        {
            if(Pred(p->nValue,q->nValue))
               {
                deleteNode(q);
                q = p;
            }
        }
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::unique()
    {
        unique([&](const T& i,const T& j)->bool{return i==j;});
//        auto p = head.nNode;
//        auto q = head.nNode->nNext;
//        for (;q != tail.nNode;p = q,q = q->nNext)
//        {
//            if(p->nValue == q->nValue)
//               {
//                deleteNode(q);
//                q = p;
//            }
//        }
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::sort()
    {
       merge_sort(std::less<T>());
    }
    template<typename T, typename Alloc>
        template <typename Compare>
    void list<T,Alloc>::sort(Compare comp)
    {
        merge_sort(comp);
    }

    template <typename T,typename Alloc>
        template<class... Args>
    typename list<T,Alloc>::reference
    list<T,Alloc>::emplace_front(Args&&... args)
    {
        auto p = dataAlloc.allocate();
        dataAlloc.construct(p,pdstl::move(ListNode<T>(pdstl::forward<Args>(args)...)));
        ++_size;
        p->insertAsNext(head.nNode);
        return p->nValue;
    }

    template <typename T,typename Alloc>
        template<class... Args>
    typename list<T,Alloc>::reference
    list<T,Alloc>::emplace_back(Args&&... args)
    {
        auto p = dataAlloc.allocate();
        dataAlloc.construct(p,pdstl::move(ListNode<T>(pdstl::forward<Args>(args)...)));
        ++_size;
        p->insertAsPrev(tail.nNode);
        return p->nValue;
    }

    template <typename T,typename Alloc>
        template<class... Args>
    typename list<T,Alloc>::iterator
    list<T,Alloc>::emplace(const_iterator pos, Args&&... args)
    {
        auto p = dataAlloc.allocate();
        dataAlloc.construct(p,pdstl::move(ListNode<T>(pdstl::forward<Args>(args)...)));
        ++_size;
        p->insertAsPrev(pos.nNode);
        return iterator(p);
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::splice(const_iterator pos,list& x)
    {
        _size+= x._size;
        x._size = 0;
        auto x_first_node = x.head.nNode->nNext;
        auto x_last_node = x.tail.nNode->nPrev;

        x_first_node->nPrev->nNext = x_last_node->nNext;
        x_last_node->nNext->nPrev = x_first_node->nPrev;


        x_first_node->nPrev = pos.nNode->nPrev;
        pos.nNode->nPrev->nNext = x_first_node;

        x_last_node->nNext = pos.nNode;
        pos.nNode->nPrev = x_last_node;
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::splice(const_iterator pos,list& x,const_iterator i)
    {
        _size++;
        x._size--;

        i.nNode->remove();
        i.nNode->insertAsPrev(pos.nNode);
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::splice(const_iterator pos,list& x,const_iterator first,const_iterator last)
    {
        if(first == last ) return;

        size_type n = static_cast<size_type>(distance(first,last));
        _size += n;
        x._size -=n;

        auto x_first_node = first.nNode;
        auto x_last_node = last.nNode->nPrev;   //[first,last)

        x_first_node->nPrev->nNext = x_last_node->nNext;
        x_last_node->nNext->nPrev = x_first_node->nPrev;

        x_first_node->nPrev = pos.nNode->nPrev;
        pos.nNode->nPrev->nNext = x_first_node;

        x_last_node->nNext = pos.nNode;
        pos.nNode->nPrev = x_last_node;
    }

    template <typename T,typename Alloc>
        template <typename UnaryPredicate>
    void list<T,Alloc>::remove_if(UnaryPredicate Pred)
    {
        auto head_node = head.nNode->nNext;
        auto tail_node = tail.nNode;
        while(head_node != tail_node)
        {
            if(Pred(head_node->nValue))
            {
                auto head_node_next = head_node->nNext;
                head_node->remove();
                deleteNode(head_node);
                head_node = head_node_next;
            }
            else {
                head_node = head_node->nNext;
            }
        }
    }
    template <typename T,typename Alloc>
    void list<T,Alloc>::remove(const T& value)
    {
        remove_if([&](const T& elem){return elem == value;});
    }

    template <typename T,typename Alloc>
    void list<T,Alloc>::reverse()
    {
        if(_size <= 1) return;

        auto cur = head.nNode;
        auto cur_next = head.nNode->nNext;

        while(cur_next!=nullptr)
        {

            cur->nNext = cur->nPrev;
            cur->nPrev = cur_next;

            cur = cur_next;
            cur_next = cur_next->nNext;
        }

        cur->nNext = cur->nPrev;
        cur->nPrev = nullptr; //cur is the new head_node and also the original tail_node

        auto tmp = head.nNode;
        head.nNode = tail.nNode;
        tail.nNode = tmp;

    }












}//pdstl
#endif
