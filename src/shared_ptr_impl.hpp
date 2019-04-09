#ifndef SHARED_PTR_IMPL_H
#define SHARED_PTR_IMPL_H

#include "allocator.h"
#include "utility.hpp"
#include <functional>



#define DEBUG_FLAG
#ifdef DEBUG_FLAG
#include <iostream>
#endif

namespace pdstl
{
template <typename T> class shared_ptr;
template <typename T> class weak_ptr;
template <typename T> class shared_ptr_base;
template <typename T>
class shared_ptr_base
{
public :
    typedef std::function<void(T*)> deleter_type;
    shared_ptr_base()  = delete;
    shared_ptr_base(T* p,int c = 1,deleter_type d = deleter_type(_default_deleter<T>()))
        : res_ptr(p),_deleter(d) { count = new int(c);}
    shared_ptr_base(T* p,int *c,deleter_type d = deleter_type(_default_deleter<T>()))
        : res_ptr(p),_deleter(d),count(c){}
//    shared_ptr_base(T* p,int* c ) : res_ptr(p),count(c) {}
    shared_ptr_base(const shared_ptr_base&) = delete;
    shared_ptr_base(shared_ptr_base&&) = delete;
    shared_ptr_base operator=(shared_ptr_base&) = delete;
    shared_ptr_base operator=(const shared_ptr_base&) = delete;
    ~shared_ptr_base()
    {
        if(res_ptr)// exclude nullptr
        {
//            delete res_ptr;
            _deleter(res_ptr);
            delete count;
            res_ptr = nullptr;
            count = nullptr;
        }
    }

    friend class shared_ptr<T>;
    friend class weak_ptr<T>;

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
    template <typename U>
    class _default_deleter
    {
    public:
        void operator()(const U* ptr){
            if(ptr) delete ptr;
        }
        void operator()( U* ptr){
            if(ptr) delete ptr;
        }
    };
    T* res_ptr;
    int* count;
    deleter_type _deleter;
};

template <typename T>
class shared_ptr
{
private:
    shared_ptr_base<T>* base_ptr;
    template <class Y> friend class weak_ptr;
    void release()
    {
        if(base_ptr == nullptr) return;
        if(base_ptr->decr() == 0)
           {
            base_ptr->~shared_ptr_base<T>();
            delete base_ptr;
            base_ptr = nullptr;
            }

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
    explicit shared_ptr(const weak_ptr<T>& p):base_ptr(p.base_ptr){base_ptr->incr();}
    explicit shared_ptr(const shared_ptr& ptr):base_ptr(ptr.base_ptr)
    {
        base_ptr->incr();
    }
    template <class U,class Deleter>
    explicit shared_ptr(U* ptr,Deleter d):base_ptr(new shared_ptr_base<U>(ptr,new int(1),d)){}

    template <class Deleter>
    explicit shared_ptr(std::nullptr_t,Deleter d):base_ptr(new shared_ptr_base<T>(nullptr,new int(1),d)){}

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
    template <typename U,class deleter>
    void reset(U* ptr,deleter p)
    {
        if(ptr==nullptr ||this->get() == ptr) return;
        shared_ptr(ptr,p).swap(*this); // cppreference
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

template <typename T,typename... Args>
shared_ptr<T> make_shared(Args&&... args)
{
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
}



template <typename T>
class weak_ptr
{
private:
    template <class Y> friend class shared_ptr;
    shared_ptr_base<T>* base_ptr;
public:
    explicit weak_ptr():base_ptr(new shared_ptr_base<T>(nullptr)){}
    explicit weak_ptr(const weak_ptr& ptr):base_ptr(ptr.base_ptr){}
    explicit weak_ptr(const shared_ptr<T>& ptr):base_ptr(ptr.base_ptr){}
    explicit weak_ptr(weak_ptr&& ptr):base_ptr(ptr.base_ptr){ptr.base_ptr=nullptr;}
//    explicit weak_ptr(weak_ptr&& ptr):base_ptr(ptr.base_ptr){ptr.release();}
    ~weak_ptr(){}
    void reset()
    {
        base_ptr = nullptr;
    }
    void swap(weak_ptr& ptr)
    {
        auto tmpptr = ptr.base_ptr;
        ptr.base_ptr = base_ptr;
        base_ptr = ptr.base_ptr;
    }
    weak_ptr& operator=(const weak_ptr& ptr)
    {
//        this->base_ptr = ptr.base_ptr;
        //swap写法是cppreference要求的,上面一行是它的实际作用
        weak_ptr(ptr).swap(*this);
        return *this;
    }
    weak_ptr& operator=(const shared_ptr<T>& ptr)
    {
//        this->base_ptr = ptr.base_ptr;
        weak_ptr(ptr).swap(*this);
        return *this;
    }
    weak_ptr& operator=(weak_ptr&& ptr)
    {
//        this->base_ptr = ptr.base_ptr;
        weak_ptr(pdstl::move(ptr)).swap(*this);
        return *this;
    }
    int use_count() const
    {
        if(base_ptr == nullptr || base_ptr->res_ptr == nullptr)
            return 0;
        return base_ptr->use_count();
    }
    bool expired() const
    {
        if(this->use_count() == 0)
            return true;
        return false;
    }
    shared_ptr<T> lock() const
    {
       return  expired() ? shared_ptr<T>() : shared_ptr<T>(*this);
    }




};


};



#endif
