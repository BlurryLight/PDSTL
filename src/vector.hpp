#ifndef VECTOR_H
#define VECTOR_H

#define DEBUG_FLAG

#include <cstddef>
#ifdef DEBUG_FLAG
#include <iostream>
#endif
#include "allocator.h"
#include "iterator.hpp"
#include <iterator> //reverse_iterator only : will be replaced after reverse_iterator implementation
#include <initializer_list> //std::initializer_list<T>

namespace pdstl {
template <typename T,class Alloc=Allocator<T> >
class vector
{
private:
    T* _arrStart;
    T* _arrEnd;
    T* _StorageEnd;
    size_t resrv_size;
    size_t vec_size;
    Allocator<T> dataAlloc;
    inline void reallocate();

    //magic do not touch

    template <typename Iter>
    using isInputIterator = typename std::enable_if<
        std::is_convertible<typename std::iterator_traits<Iter>::iterator_category,
                            std::input_iterator_tag
                            >::value
        >::type;

public:
    typedef T                                               value_type;
    typedef value_type&                                     reference;
    typedef value_type*                                     pointer;
    typedef const value_type&                               const_reference;
    typedef const T*                                        const_pointer;
    typedef value_type*                                     iterator;
    typedef const value_type*                               const_iterator;
    typedef Alloc                                           allocator_type;
    typedef size_t                                          size_type;
    typedef ptrdiff_t                                       difference_type;
    typedef random_access_iterator_tag                      iterator_category;
    typedef std::reverse_iterator<iterator>                 reverse_iterator;
    typedef std::reverse_iterator<const_iterator>           const_reverse_iterator;

    //default ctor/copy ctor/move ctor/d ctor
    vector() noexcept;
    vector(size_type n);
    vector(size_type n,const T& val);
    //magic do not touch
    template <typename InputIt, typename = isInputIterator<InputIt>>
    vector(InputIt first,InputIt last);

    vector(const vector& other);
    vector(vector&& other);

    vector(std::initializer_list<T> init);
    vector<T>& operator= (const vector<T>& other);
    vector<T>& operator= (vector<T>&& other);
    vector<T>& operator= (std::initializer_list<T> init);
    ~vector() noexcept;

    //iterators

    iterator begin();
    const_iterator cbegin() const;
    iterator end();
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator crend() const;

    //bool

    bool operator== (const vector& other) const;
    bool operator!= (const vector& other) const;
    bool operator<= (const vector& other) const;
    bool operator>= (const vector& other) const;
    bool operator< (const vector& other) const;
    bool operator> (const vector& other) const;

    //element

    reference operator[](size_type rank);
    const_reference operator[](size_type rank) const;
    reference at(size_type rank);//safe way
    const_reference at(size_type rank) const;//safe way
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    pointer data();//return the underlying array
    const_pointer data() const;

    //capacity
    bool empty() const;
    size_type size() const;
    size_type max_size() const;
    size_type capacity() const;
    void reserve(size_type new_cap);
    void shrink_to_fit();

    //modifiers
    void resize(size_type n);
    void resize(size_type n,const T& val);
    void push_back(const T& val);

    template <typename... Args>
    void emplace_back(Args&&... args);
    template <typename... Args>
    iterator emplace_back(const_iterator pos,Args&&... args);

    void push_back(T&& val);
    void pop_back();
    void swap(vector& other);
    void clear();
    iterator insert(const_iterator pos,const T& val);
    iterator insert(const_iterator pos,T&& val);
    iterator insert(const_iterator pos,size_type n,const T& val);
    template <typename InputIt>
    iterator insert(const_iterator pos,InputIt first,InputIt last);
    iterator insert(const_iterator pos,std::initializer_list<T> ilist);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first,const_iterator last);

};

template <typename T,class Alloc>
vector<T,Alloc>::vector() noexcept
{
    _arrStart= Allocator<T>::allocate(4);
    _arrEnd = _arrStart;
    _StorageEnd = _arrStart + 4;
}

template <typename T,class Alloc>
vector<T,Alloc>::vector(typename vector<T,Alloc>::size_type n)
{
     resrv_size = n * 2; //reserve factor = 2
    _arrStart= dataAlloc.allocate(resrv_size);
    for(size_type i = 0;i<n;++i)
        dataAlloc.construct(_arrStart + i,T());
    _arrEnd = _arrStart + n;
    _StorageEnd = _arrStart + resrv_size;
}

template <typename T,class Alloc>
vector<T,Alloc>::vector(size_type n,const T& val)
{
     resrv_size = n * 2; //reserve factor = 2
     vec_size = n;
    _arrStart= dataAlloc.allocate(resrv_size);
    for(size_type i = 0;i<n;++i)
    {
        dataAlloc.construct(_arrStart + i,val);
#ifdef DEBUG_FLAG
        std::cout<<_arrStart + i<<std::endl;
#endif
    }
    _arrEnd = _arrStart + n - 1;
    _StorageEnd = _arrStart + resrv_size - 1;
}

template <typename T,class Alloc>
vector<T,Alloc>::~vector() noexcept
{
    for(size_type i = 0;i<vec_size;++i)
    {
        dataAlloc.destroy(_arrStart + i);
#ifdef DEBUG_FLAG
        std::cout<<_arrStart + i<<" destroyed"<<std::endl;
#endif
    }
    dataAlloc.deallocate(_arrStart,resrv_size);
}

template <typename T,class Alloc>
typename vector<T,Alloc>::reference
    vector<T,Alloc>::operator[](typename vector<T,Alloc>::size_type rank)
{
    return *(_arrStart + rank);
}
template <typename T,class Alloc>
typename vector<T,Alloc>::const_reference
    vector<T,Alloc>::operator[](typename vector<T,Alloc>::size_type rank) const
{
    return *(_arrStart + rank);
}




} // namespace pdstl
#endif
