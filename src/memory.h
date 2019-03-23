#ifndef MEMORY_H
#define MEMORY_H

#include <algorithm> //std::swap
#include "allocator.h"

#define DEBUG_FLAG
#ifdef DEBUG_FLAG
#include <iostream>
#endif

namespace pdstl
{
template <typename T> class shared_ptr;
template <typename T> class shared_ptr_base;
template <typename T>
class shared_ptr_base
{
public :
    shared_ptr_base()  = delete;
    shared_ptr_base(T* p,unsigned long c = 1) : res_ptr(p),count(c) {}
    shared_ptr_base(const shared_ptr_base&) = delete;
    shared_ptr_base(shared_ptr_base&&) = delete;
    shared_ptr_base operator=(shared_ptr_base&) = delete;
    shared_ptr_base operator=(const shared_ptr_base&) = delete;
    ~shared_ptr_base()
    {
        if(res_ptr)// exclude nullptr
            delete res_ptr;
    }

    friend class shared_ptr<T>;

    T* res_get()
    {
        return res_ptr;
    }
    unsigned long use_count()
    {
        return count;
    }
private:
    T* res_ptr;
    unsigned long count;
};

template <typename T>
class shared_ptr
{
private:
    shared_ptr_base<T>* base_ptr;
    void release()
    {
        if(--base_ptr->count == 0)
            base_ptr->~shared_ptr_base<T>();
    }
public:
    typedef T element_type;//cppreference

    explicit shared_ptr():base_ptr(new shared_ptr_base<T>(nullptr)){}
    explicit shared_ptr(T* p):base_ptr(new shared_ptr_base<T>(p)){}
    explicit shared_ptr(const shared_ptr& ptr):base_ptr(ptr.base_ptr)
    {
        ++base_ptr->count;
    }

    void reset()
    {
        release();
        base_ptr = new shared_ptr_base<T>(nullptr);
    }
    template <typename U>
    void reset(U* ptr)
    {
        release();
        base_ptr = new shared_ptr_base<T>(reinterpret_cast<T*>(ptr));
    }

    ~shared_ptr()
    {
        release();
    }
    template <typename U>
    shared_ptr(const shared_ptr<U>& r)
    {
        base_ptr = new shared_ptr_base<T>(static_cast<T*>(r.base_ptr->res_get()),r.base_ptr->use_count());
    }

    unsigned long use_count() const
    {
        if(!base_ptr)
            return 0;
        return base_ptr->use_count();
    }
    bool unique() const
    {
        if(!base_ptr)
            return false;
        return (base_ptr->use_count() == 1);
    }
    T& operator*() const
    {
        return *(base_ptr->res_ptr);
    }

    T* operator->() const
    {
        return base_ptr->res_ptr;
    }
    T* get() const
    {
        return base_ptr->res_get();
    }
    operator bool() const
    {
        return (base_ptr->use_count() > 0);
    }
};


template <typename T>
bool operator==(const shared_ptr<T>& left,std::nullptr_t rhs)
{
    return (left.get() == rhs);
}
template <typename T,typename U>
bool operator==(const shared_ptr<T>& left,const shared_ptr<U>& right)
{
    return (left.get() == right.get());
}

};



#endif
