#ifndef SHARED_PTR_IMPL_H
#define SHARED_PTR_IMPL_H

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
    shared_ptr_base(T* p,int c = 1) : res_ptr(p) { count = new int(c);}
    shared_ptr_base(T* p,int* c ) : res_ptr(p),count(c) {}
    shared_ptr_base(const shared_ptr_base&) = delete;
    shared_ptr_base(shared_ptr_base&&) = delete;
    shared_ptr_base operator=(shared_ptr_base&) = delete;
    shared_ptr_base operator=(const shared_ptr_base&) = delete;
    ~shared_ptr_base()
    {
        if(res_ptr)// exclude nullptr
        {
            delete res_ptr;
            delete count;
        }


    }

    friend class shared_ptr<T>;

    T* res_get()
    {
        return res_ptr;
    }
    int use_count()
    {
        return *count;
    }
    //incr or decr count
    int incr()
    {
       return  ++(*count);

    }
    int decr()
    {
       return  --(*count);
    }
    int* get_count_ptr()  const
    {
        return count;
    }
private:
    T* res_ptr;
    int* count;
};

template <typename T>
class shared_ptr
{
private:
    shared_ptr_base<T>* base_ptr;
    void release()
    {
        if(base_ptr->decr() == 0)
            base_ptr->~shared_ptr_base<T>();
    }
public:
    //ugly implementation but useful
    //it's the cost to seperate count from shared_ptr class
    void incr() const
    {
        base_ptr->incr();
    }
    void decr() const
    {
        base_ptr->decr();
    }

public:
    typedef T element_type;//cppreference

    explicit shared_ptr():base_ptr(new shared_ptr_base<T>(nullptr)){}
    explicit shared_ptr(T* p):base_ptr(new shared_ptr_base<T>(p)){}
    explicit shared_ptr(const shared_ptr& ptr):base_ptr(ptr.base_ptr)
    {
        base_ptr->incr();
    }

    template <typename U>
    explicit shared_ptr(const shared_ptr<U>& ptr,element_type* p):
        base_ptr(new shared_ptr_base<T>(p,ptr.get_count_ptr()))
    {
        base_ptr->incr();
    }

    void reset()
    {
      //  release(); //do not need to manually release
        shared_ptr().swap(*this); // cppreference
    }

    void swap(shared_ptr& other)
    {
        std::swap(this->base_ptr,other.base_ptr);
    }

    template <typename U>
    void reset(U* ptr)
    {
        if(ptr==nullptr ||this->get() == ptr) return;
        release();
        base_ptr = new shared_ptr_base<T>(ptr);
    }

    ~shared_ptr()
    {
        release();
    }
    template <typename U>
    shared_ptr(const shared_ptr<U>& ptr)
     :base_ptr(new shared_ptr_base<T>(static_cast<T*>(ptr.base_ptr->res_get()),ptr.get_count_ptr()))
    {
        ptr.incr();
    }

    unsigned long use_count() const
    {
        return base_ptr->use_count();
    }
    bool unique() const
    {
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
        if(*this == nullptr)
            return false;
        return (base_ptr->use_count() > 0);
    }

    //不符合标准
    int* get_count_ptr() const
    {
        return base_ptr->get_count_ptr();
    }
    template <typename U>
    bool owner_before(const shared_ptr<U>& other) const
    {
        if(this->get_count_ptr() == other.get_count_ptr())
            return false;
        else {
            return this->get()<other.get();
        }
    }
};

//other functions
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
template <typename T,typename U>
bool operator<=(const shared_ptr<T>& left,const shared_ptr<U>& right)
{
    return (left.get() <= right.get());
}
template <typename T,typename U>
bool operator!=(const shared_ptr<T>& left,const shared_ptr<U>& right)
{
    return (left.get() != right.get());
}
template <typename T,typename U>
bool operator>=(const shared_ptr<T>& left,const shared_ptr<U>& right)
{
    return (left.get() >= right.get());
}
template <typename T,typename U>
bool operator>(const shared_ptr<T>& left,const shared_ptr<U>& right)
{
    return (left.get() > right.get());
}
template <typename T,typename U>
bool operator<(const shared_ptr<T>& left,const shared_ptr<U>& right)
{
    return (left.get() < right.get());
}


};



#endif
