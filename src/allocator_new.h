#ifndef ALLOCATOR_NEW_H
#define ALLOCATOR_NEW_H
//This allocator is a simple wrapper for new and delete,which could be robuster than the mempool version

#include <cstddef> //std::size_t
#include <new>  //std::bad_alloc


namespace pdstl {

template <typename _T,unsigned short _GrowSize = 0> //_Growsize is to behaves like the mempool version
class Allocator
{

public:
    typedef _T              value_type;
    typedef _T*             pointer;
    typedef const _T*       const_pointer;
    typedef _T&             reference;
    typedef const _T&       const_reference;
    typedef std::size_t     size_type; //<cstddef>
    typedef std::ptrdiff_t  difference_type;
    Allocator() = default;
    //copy construct & rebind construct must exsit to work with stl containers
    Allocator( const Allocator& other) : Allocator(){}
    template <typename _U>
    Allocator(const Allocator<_U,_GrowSize> &d) : Allocator() {}

    template <typename _U>
    struct rebind
    {
        typedef Allocator<_U,_GrowSize> other;
    };

    template <typename _U>
    bool operator==(const Allocator<_U>&) const
    {
        return true;
    }
    template <typename _U>
    bool operator!=(const Allocator<_U>&) const
    {
        return false;
    }

    pointer allocate()
    {
        return static_cast<_T*>(::operator new(sizeof (_T)));
    }
    pointer allocate(size_type n ,const void* hint = 0) //hint is just a flag,useless
    {
        if( hint || n <= 0)
//            throw std::bad_alloc();
            return nullptr;
        return static_cast<_T*>(::operator new(n * sizeof (_T)));
    }
    void deallocate(pointer p,size_type /*flag*/)
    {
        if( p = nullptr)
            return;
        ::operator delete(p);
    }

    void construct(pointer p,const_reference val)
    {
        new(p)_T(val);
    }
    void destroy(pointer p)
    {
        p->~_T();
    }

};

} // namespace pdstl

#endif
