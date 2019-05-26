#ifndef VECTOR_H
#define VECTOR_H

#define DEBUG_FLAG

#include <cstddef>
#ifdef DEBUG_FLAG
#include <iostream>
#endif
#include "allocator.h"
#include "iterator.hpp"
#include <cstring>          //for memmove
#include <initializer_list> //std::initializer_list<T>
#include <iterator> //reverse_iterator only : will be replaced after reverse_iterator implementation
#include <limits>

namespace pdstl {
template<typename T, class Alloc = Allocator<T>>
class vector
{
private:
    T *_arrStart;
    T *_arrEnd;
    T *_StorageEnd;
    size_t resrv_size;
    size_t vec_size;
    Alloc dataAlloc;
    //    typedef Alloc dataAlloc;
    void shrink();
    void expand();

    //magic do not touch

    template<typename Iter>
    using isInputIterator = typename std::enable_if<
        std::is_convertible<typename std::iterator_traits<Iter>::iterator_category,
                            std::input_iterator_tag>::value>::type;

public:
    typedef T value_type;
    typedef value_type &reference;
    typedef value_type *pointer;
    typedef const value_type &const_reference;
    typedef const T *const_pointer;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef Alloc allocator_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef random_access_iterator_tag iterator_category;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //default ctor/copy ctor/move ctor/d ctor
    vector() noexcept;
    vector(size_type n);
    vector(size_type n, const T &val);
    //magic do not touch
    template<typename InputIt, typename = isInputIterator<InputIt>>
    vector(InputIt first, InputIt last);

    vector(const vector &other);
    vector(vector &&other);

    vector(std::initializer_list<T> init);
    vector<T> &operator=(const vector<T> &other);
    vector<T> &operator=(vector<T> &&other);
    vector<T> &operator=(std::initializer_list<T> init);
    void assign(size_type count, const T &val);
    template<typename InputIt, typename = isInputIterator<InputIt>>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> ilist);
    ~vector() noexcept;

    //iterators

    iterator begin() { return _arrStart; }
    const_iterator cbegin() const { return _arrStart; }
    iterator end() { return _arrEnd + 1; }
    const_iterator cend() const { return _arrEnd + 1; }
    reverse_iterator rbegin() { return reverse_iterator(_arrEnd + 1); }
    const_reverse_iterator crbegin() const { return reverse_iterator(_arrEnd + 1); }
    reverse_iterator rend() { return reverse_iterator(_arrStart); }
    const_reverse_iterator crend() const { return reverse_iterator(_arrStart); }

    //bool

    bool operator==(const vector &other) const;
    bool operator!=(const vector &other) const;
    bool operator<=(const vector &other) const;
    bool operator>=(const vector &other) const;
    bool operator<(const vector &other) const;
    bool operator>(const vector &other) const;

    //element

    reference operator[](size_type rank);
    const_reference operator[](size_type rank) const;
    reference at(size_type rank);             //safe way
    const_reference at(size_type rank) const; //safe way
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    pointer data() { return _arrStart; } //return the underlying array
    const_pointer data() const { return _arrStart; }

    //capacity
    bool empty() const { return vec_size == 0; }
    size_type size() const { return vec_size; }
    size_type max_size() const { return std::numeric_limits<size_type>::max(); }
    size_type capacity() const { return resrv_size; }
    void reserve(size_type new_cap);
    void shrink_to_fit() { shrink(); }

    //modifiers
    void resize(size_type n);
    void resize(size_type n, const T &val);
    void push_back(const T &val);
    void push_back(T &&val);

    template<typename... Args>
    reference emplace_back(Args &&... args);

    void pop_back();
    void swap(vector &other);
    void clear();
    iterator insert(const_iterator pos, const T &val);
    iterator insert(const_iterator pos, T &&val);
    iterator insert(const_iterator pos, size_type n, const T &val);
    //    iterator insert(const_iterator pos,size_type n,T&& val);
    template<typename InputIt, typename = isInputIterator<InputIt>>
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
};

template<typename T, class Alloc>
vector<T, Alloc>::vector() noexcept
{
    vec_size = 0;
    resrv_size = 4;
    _arrStart = dataAlloc.allocate(resrv_size);
    _arrEnd = _arrStart;
    _StorageEnd = _arrStart + 4;
}

template<typename T, class Alloc>
vector<T, Alloc>::vector(typename vector<T, Alloc>::size_type n)
{
    resrv_size = n * 2; //reserve factor = 2
    vec_size = n;
    _arrStart = dataAlloc.allocate(resrv_size);
    for (size_type i = 0; i < n; ++i)
        dataAlloc.construct(_arrStart + i, T());
    _arrEnd = _arrStart + vec_size - 1;
    _StorageEnd = _arrStart + resrv_size;
}

template<typename T, class Alloc>
vector<T, Alloc>::vector(size_type n, const T &val)
{
    resrv_size = n * 2; //reserve factor = 2
    vec_size = n;
    _arrStart = dataAlloc.allocate(resrv_size);
    for (size_type i = 0; i < n; ++i) {
        dataAlloc.construct(_arrStart + i, val);
#ifdef DEBUG_FLAG
//        std::cout<<_arrStart + i<<std::endl;
#endif
    }
    _arrEnd = _arrStart + vec_size - 1;
    _StorageEnd = _arrStart + resrv_size - 1;
}

template<typename T, class Alloc>
vector<T, Alloc>::~vector() noexcept
{
    for (size_type i = 0; i < vec_size; ++i) {
        dataAlloc.destroy(_arrStart + i);
#ifdef DEBUG_FLAG
//        std::cout<<_arrStart + i<<" destroyed"<<std::endl;
#endif
    }
    dataAlloc.deallocate(_arrStart, resrv_size);
}
template<typename T, typename Alloc>
template<typename InputIt, typename> //magic
vector<T, Alloc>::vector(InputIt first, InputIt last)
{
    size_type n = last - first;
    resrv_size = n * 2;
    vec_size = n;
    _arrStart = dataAlloc.allocate(resrv_size);
    _arrEnd = _arrStart + vec_size - 1;
    _StorageEnd = _arrStart + resrv_size - 1;
    for (auto i = 0; i < n; ++i, ++first)
        dataAlloc.construct(_arrStart + i, *first);
}

template<typename T, typename Alloc>
vector<T, Alloc>::vector(std::initializer_list<T> ilist)
{
    resrv_size = ilist.size() * 2;
    vec_size = ilist.size();
    _arrStart = dataAlloc.allocate(resrv_size);
    _arrEnd = _arrStart + vec_size - 1;
    _StorageEnd = _arrStart + resrv_size - 1;

    size_type i = 0;
    for (auto item : ilist) {
        dataAlloc.construct(_arrStart + i,
                            item); // do not use _arrStart++. The pointer shouldn't move
        ++i;
    }
}

template<typename T, typename Alloc>
vector<T, Alloc>::vector(const vector<T, Alloc> &other)
{
    //deep copy
    vec_size = other.vec_size;
    resrv_size = other.resrv_size;
    _arrStart = dataAlloc.allocate(resrv_size);

    for (size_type i = 0; i < vec_size; ++i)
        dataAlloc.construct(_arrStart + i, other.at(i));
}

template<typename T, typename Alloc>
vector<T, Alloc>::vector(vector<T, Alloc> &&other)
{
    //deep copy
    vec_size = other.vec_size;
    resrv_size = other.resrv_size;
    _arrStart = dataAlloc.allocate(resrv_size);

    for (size_type i = 0; i < vec_size; ++i)
        dataAlloc.construct(_arrStart + i, std::move(other.at(i)));
}

template<typename T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](
    typename vector<T, Alloc>::size_type rank)
{
    return *(_arrStart + rank);
}
template<typename T, class Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::operator[](
    typename vector<T, Alloc>::size_type rank) const
{
    return *(_arrStart + rank);
}

template<typename T, class Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::at(size_type rank) const
{
    if (rank < vec_size)
        return *(_arrStart + rank);
    else {
        throw std::out_of_range("Rank out of range");
    }
}

template<typename T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::at(size_type rank)
{
    if (rank < vec_size)
        return *(_arrStart + rank);
    else {
        throw std::out_of_range("Rank out of range");
    }
}

template<typename T, typename Alloc>
void vector<T, Alloc>::shrink()
{
    if (resrv_size < 4)
        return; //minimum size
    if (vec_size * 2 > resrv_size)
        return;
    T *_oldarrStart = _arrStart;
    auto _oldresrv_size = resrv_size;
    _arrStart = dataAlloc.allocate(resrv_size /= 2);
    _StorageEnd = _arrStart + resrv_size - 1;
    _arrEnd = _arrStart + vec_size - 1;
    for (int i = 0; i < vec_size; ++i)
        dataAlloc.construct(_arrStart + i, *(_oldarrStart + i));
    dataAlloc.deallocate(_oldarrStart, _oldresrv_size);
}
template<typename T, typename Alloc>
void vector<T, Alloc>::expand()
{
    if (vec_size < resrv_size)
        return;
    T *_oldarrStart = _arrStart;
    auto _oldresrv_size = resrv_size;
    _arrStart = dataAlloc.allocate(resrv_size *= 2);
    _StorageEnd = _arrStart + resrv_size - 1;
    _arrEnd = _arrStart + vec_size - 1;
    for (int i = 0; i < vec_size; ++i)
        dataAlloc.construct(_arrStart + i, *(_oldarrStart + i));
    dataAlloc.deallocate(_oldarrStart, _oldresrv_size);
}

template<typename T, typename Alloc>
void vector<T, Alloc>::resize(size_type n)
{
    resize(n, T());
}
template<typename T, typename Alloc>
void vector<T, Alloc>::resize(size_type n, const T &val)
{
    if (n > vec_size) {
        if (n > resrv_size) {
            T *_oldarrStart = _arrStart;
            auto _oldresrv_size = resrv_size;
            resrv_size = n;
            _arrStart = dataAlloc.allocate(resrv_size);
            size_type i = 0;
            for (; i < vec_size; ++i)
                dataAlloc.construct(_arrStart + i, *(_oldarrStart + i));
            dataAlloc.deallocate(_oldarrStart, _oldresrv_size);
        }
        for (size_type i = vec_size; i < n; ++i) {
            dataAlloc.construct(_arrStart + i, val);
#ifdef DEBUG_FLAG
//                std::cout<<_arrStart + i<<std::endl;
//                std::cout<<(*this)[i]<<std::endl;
#endif
        }

    } else {
        for (size_type i = n - 1; i < vec_size; i++) {
            dataAlloc.destroy(_arrStart + i);
        }
    }
    //    shrink();
    vec_size = n;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::reserve(size_type new_cap)
{
    if (new_cap > resrv_size) {
        T *_oldarrStart = _arrStart;
        auto _oldresrv_size = resrv_size;
        _arrStart = dataAlloc.allocate(new_cap);
        _arrEnd = _arrStart + vec_size - 1;
        _StorageEnd = _arrStart + new_cap - 1;
        for (int i = 0; i < vec_size; ++i)
            dataAlloc.construct(_arrStart + i, *(_oldarrStart + i));
        dataAlloc.deallocate(_oldarrStart, _oldresrv_size);
        resrv_size = new_cap;
    }
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::front()
{
    return *(_arrStart);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::front() const
{
    return *(_arrStart);
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::back()
{
    return *(_arrEnd);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::back() const
{
    return *(_arrEnd);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(
    typename vector<T, Alloc>::const_iterator it, const T &val)
{
    expand();
    auto itt = _arrStart + (it - _arrStart);
    for (auto i = end(); i != it; i--)
        *i = *(i - 1);
    *itt = val;
    vec_size++;
    _arrEnd = _arrStart + vec_size - 1;
    return itt;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(
    typename vector<T, Alloc>::const_iterator it, T &&val)
{
    return insert(it, val);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(
    typename vector<T, Alloc>::const_iterator it, size_type n, const T &val)
{
    if (n == 0)
        return const_cast<T *>(it);

    //a little tricky
    //if you want to understand it
    //you must completely understand how the allocator with its memorypool works.
    auto _oldarrStart = _arrStart;
    if (n + vec_size > resrv_size)
        reserve(resrv_size + n);
    auto itt = _arrStart + (it - _oldarrStart);
    vec_size += n;
    //memmove
    for (auto i = end() + n; i != itt + n; i--)
        *i = *(i - 1 - n);
    //    memmove(itt + n,itt,(_arrEnd - itt) * sizeof(T));

    for (auto tmp = itt; n--; ++tmp)
        *tmp = val;
    _arrEnd = _arrStart + vec_size - 1;
    return itt;
}

template<typename T, typename Alloc>
template<typename InputIt, typename>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(
    typename vector<T, Alloc>::const_iterator it, InputIt first, InputIt last)
{
    auto n = last - first;
    if (n == 0)
        return const_cast<T *>(it);

    auto _oldarrStart = _arrStart;
    if (n + vec_size > resrv_size)
        reserve(resrv_size + n);
    auto itt = _arrStart + (it - _oldarrStart);
    vec_size += n;
    //memmove
    for (auto i = end() + n; i != itt + n; i--)
        *i = *(i - 1 - n);

    for (auto tmp = itt; first != last; ++tmp, ++first)
        *tmp = *first;
    _arrEnd = _arrStart + vec_size - 1;
    return itt;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(
    typename vector<T, Alloc>::const_iterator it, std::initializer_list<T> lst)
{
    auto n = lst.size();
    if (n == 0)
        return const_cast<T *>(it);

    auto _oldarrStart = _arrStart;
    if (n + vec_size > resrv_size)
        reserve(resrv_size + n);
    auto itt = _arrStart + (it - _oldarrStart);
    vec_size += n;
    //memmove
    for (auto i = end() + n; i != itt + n; i--)
        *i = *(i - 1 - n);

    for (auto &item : lst) {
        (*itt) = item;
        itt++;
    }
    _arrEnd = _arrStart + vec_size - 1;
    return itt;
}

//template <typename T,typename Alloc>
//typename vector<T,Alloc>::iterator
//vector<T,Alloc>::insert(typename vector<T,Alloc>::const_iterator it,size_type n,T&& val)
//{
//    return insert(it,n,val);
//}

template<typename T, typename Alloc>
void vector<T, Alloc>::push_back(const T &val)
{
    expand();
    dataAlloc.construct(_arrStart + vec_size, val);
    ++vec_size;
    ++_arrEnd;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::push_back(T &&val)
{
    expand();
    dataAlloc.construct(_arrStart + vec_size, std::move(val));
    ++vec_size;
    ++_arrEnd;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(
    typename vector<T, Alloc>::const_iterator it)
{
    auto itt = const_cast<T *>(it);
    dataAlloc.destroy(itt);
    //elegent way
    memmove(itt, itt + 1, (_arrEnd - it) * sizeof(T));
    //    for(auto tmp = itt;tmp!=end();++tmp)
    //        *tmp = *(tmp+1);
    vec_size--;
    _arrEnd = _arrStart + vec_size - 1;
    return itt;
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(const_iterator first,
                                                            const_iterator last)
{
    auto itt = const_cast<T *>(first);
    auto n = static_cast<int>(last - first);
    for (auto tmp = itt; tmp != last; ++tmp)
        dataAlloc.destroy(tmp);

    //    for(auto tmp = itt;tmp!=end();++tmp)
    //        *tmp = *(tmp+n);
    memmove(itt, last, (vec_size - n) * sizeof(T));
    vec_size -= n;
    _arrEnd = _arrStart + vec_size - 1;
    return itt;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::swap(vector &other)
{
    size_type tmpvec_size = vec_size;
    size_type tmpresrv_size = resrv_size;
    T *tmp_arrStart = _arrStart;
    T *tmp_arrEnd = _arrEnd;
    T *tmp_StorageEnd = _StorageEnd;

    vec_size = other.vec_size;
    resrv_size = other.resrv_size;
    _arrStart = other._arrStart;
    _arrEnd = other._arrEnd;
    _StorageEnd = other._StorageEnd;

    other.vec_size = tmpvec_size;
    other.resrv_size = tmpresrv_size;
    other._arrStart = tmp_arrStart;
    other._arrEnd = tmp_arrEnd;
    other._StorageEnd = tmp_StorageEnd;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::clear()
{
    for (int i = 0; i < vec_size; i++)
        dataAlloc.destroy(_arrStart + i);
    vec_size = 0;
    _arrEnd = _arrStart;
}

template<typename T, typename Alloc>
template<typename... Args>
typename vector<T, Alloc>::reference vector<T, Alloc>::emplace_back(Args &&... args)
{
    expand();
    dataAlloc.construct(_arrStart + vec_size, std::move(T(std::forward<Args>(args)...)));
    ++vec_size;
    ++_arrEnd;
    return back();
}

template<typename T, typename Alloc>
void vector<T, Alloc>::assign(size_type count, const T &val)
{
    if (count > resrv_size)
        reserve(count * 2);

    for (auto i = 0; i < count; ++i)
        dataAlloc.construct(_arrStart + i, val);
    vec_size = count;
    _arrEnd = _arrStart + vec_size - 1;
}

template<typename T, typename Alloc>
template<typename Inputit, typename>
void vector<T, Alloc>::assign(Inputit first, Inputit last)
{
    size_type count = static_cast<size_type>(last - first);
    if (count > resrv_size)
        reserve(count * 2);

    for (auto i = 0; i < count; ++i, ++first)
        dataAlloc.construct(_arrStart + i, *first);
    vec_size = count;
    _arrEnd = _arrStart + vec_size - 1;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::assign(std::initializer_list<T> ilist)
{
    size_type count = ilist.size();
    if (count > resrv_size)
        reserve(count * 2);

    size_type i = 0;
    auto it = ilist.begin();
    for (; i < count; ++i)
        dataAlloc.construct(_arrStart + i, *(it++));
    vec_size = count;
    _arrEnd = _arrStart + vec_size - 1;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::pop_back()
{
    erase(_arrEnd);
}

template<typename T, typename Alloc>
bool vector<T, Alloc>::operator==(const vector &other) const
{
    if (other.vec_size != vec_size)
        return false;
    auto it = cbegin();
    auto itt = other.cbegin();
    while (it != cend()) {
        if (*(it++) != *(itt++))
            return false;
    }
    return true;
}
template<typename T, typename Alloc>
bool vector<T, Alloc>::operator!=(const vector &other) const
{
    return !((*this) == other);
}

template<typename T, typename Alloc>
bool vector<T, Alloc>::operator<(const vector &other) const
{
    auto it = cbegin();
    auto itt = other.cbegin();
    while (it != cend() && itt != other.cend()) {
        if (*(it++) < *(itt++))
            return true;
    }
    return vec_size < other.vec_size;
}

template<typename T, typename Alloc>
bool vector<T, Alloc>::operator>(const vector &other) const
{
    auto it = cbegin();
    auto itt = other.cbegin();
    while (it != cend() && itt != other.cend()) {
        if (*(it++) > *(itt++))
            return true;
    }
    return vec_size > other.vec_size;
}

template<typename T, typename Alloc>
bool vector<T, Alloc>::operator<=(const vector &other) const
{
    return ((*this) == other || (*this) < other);
}

template<typename T, typename Alloc>
bool vector<T, Alloc>::operator>=(const vector &other) const
{
    return ((*this) == other || (*this) > other);
}

} // namespace pdstl
#endif
