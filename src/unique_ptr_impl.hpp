#ifndef UNIQUE_PTR_IMPL_H
#define UNIQUE_PTR_IMPL_H

#include <algorithm> //std::swap
#include "allocator.h"

#define DEBUG_FLAG
#ifdef DEBUG_FLAG
#include <iostream>
#endif

namespace pdstl
{

template <typename T> class unique_ptr;

template <typename T> class unique_ptr
{
private:
    T* res_ptr;
    inline void destory()
    {
        if(res_ptr!=nullptr) { delete res_ptr; res_ptr = nullptr;}
    }
public:
    typedef T element_type;
    explicit unique_ptr():res_ptr(nullptr){}
    explicit unique_ptr(std::nullptr_t):res_ptr(nullptr){}
    explicit unique_ptr(unique_ptr&) = delete;
    explicit unique_ptr(const unique_ptr&) = delete;

    explicit unique_ptr(T* p):res_ptr(p){}
    explicit unique_ptr(unique_ptr&& ptr):res_ptr(ptr.res_ptr)
    {
        ptr.res_ptr=nullptr;
    }
    ~unique_ptr()
    {
        destory();
    }

    T* release()
    {
        auto tmp = res_ptr;
        res_ptr = nullptr;
        return tmp;
    }
    T* get() const
    {
        return res_ptr;
    }

    void swap(unique_ptr& other)
    {
        std::swap(res_ptr,other.res_ptr);
    }
    void reset()
    {
        destory();
        res_ptr=nullptr;
    }
    void reset(T* p)
    {
        destory();
        res_ptr=p;
    }
    unique_ptr& operator=(unique_ptr&& ptr)
    {
        res_ptr = ptr.res_ptr;
        ptr.res_ptr = nullptr;
        return *this;
    }

    T& operator*() const
    {
        return *(res_ptr);
    }

    T* operator->() const
    {
        return res_ptr;
    }

    operator bool() const
    {
        return (res_ptr != nullptr);
    }
};

template <typename T>
bool operator==(const unique_ptr<T>& left,std::nullptr_t rhs)
{
    return (left.get() == rhs);
}
template <typename T,typename U>
bool operator==(const unique_ptr<T>& left,const unique_ptr<U>& right)
{
    return (left.get() == right.get());
}
template <typename T,typename U>
bool operator<=(const unique_ptr<T>& left,const unique_ptr<U>& right)
{
    return (left.get() <= right.get());
}
template <typename T,typename U>
bool operator!=(const unique_ptr<T>& left,const unique_ptr<U>& right)
{
    return (left.get() != right.get());
}
template <typename T,typename U>
bool operator>=(const unique_ptr<T>& left,const unique_ptr<U>& right)
{
    return (left.get() >= right.get());
}
template <typename T,typename U>
bool operator>(const unique_ptr<T>& left,const unique_ptr<U>& right)
{
    return (left.get() > right.get());
}
template <typename T,typename U>
bool operator<(const unique_ptr<T>& left,const unique_ptr<U>& right)
{
    return (left.get() < right.get());
}



};

#endif // UNIQUE_PTR_IMPL_H
