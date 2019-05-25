#ifndef ALLOCATOR_MEMPOOL_H
#define ALLOCATOR_MEMPOOL_H
/*
 * Modified from Moya-lang/Allocator
 * https://github.com/moya-lang/Allocator
 */

/*
// Deprecated !!
// 比起系统默认的Allocator，它没有任何优势，反而带来无穷无尽的bug，浪费许多时间来调试。所以弃用
// Compared with the Allocator_new version,it has no advantage but tons of bugs.I've spent more than 5 hours
// on debugging it ,trying to employ it to work with  containers but obviously I failed.
*/

#include <cstddef> //std::size_t
#include <new> //std::bad_alloc
#ifdef DEBUG_FLAG
#include <iostream>
#endif

#define Memory_Alignment 8

namespace pdstl {

template <typename _T,unsigned short _GrowSize>
class MemoryPool
{
private:



    struct MemoryBlock
    {
       MemoryBlock* pBlock;
    };

    struct Buffer
    {
        Buffer* const pNext;

       static const unsigned short BlockSize = (sizeof (_T) > 4) ? (sizeof (_T) + (Memory_Alignment - 1)) & ~ (Memory_Alignment - 1) : 4; //Magic but necessary

      //  static const unsigned short BlockSize =  sizeof(_T) > sizeof (MemoryBlock) ? sizeof(_T) : sizeof(MemoryBlock); //Magic but necessary

        unsigned char data[BlockSize * _GrowSize]; //BlockSize must be static

        Buffer(Buffer* _next):pNext(_next){}

        MemoryBlock* GetBlock(unsigned short index)
        {
            return reinterpret_cast<MemoryBlock*>(&data[BlockSize * index]);
        }
    };

    Buffer* pBuffer  = nullptr;
    MemoryBlock* pBlockFree = nullptr;
    unsigned short BufferedBlocks = _GrowSize;

public:
    MemoryPool(MemoryPool&& ) = delete;
    MemoryPool(const MemoryPool& ) = delete;
    MemoryPool operator=(MemoryPool&) = delete;
    MemoryPool operator=(const MemoryPool&) = delete;
    MemoryPool() = default;

    //useless method

//    static MemoryPool& getInstance()
//    {
//        static MemoryPool instance;
//        return instance;
//    }

    _T* allocate()
    {
        if(pBlockFree)
        {
            auto tmp = pBlockFree; //记录block开始分配时的链表首地址
            auto num = (Buffer::BlockSize / sizeof (MemoryBlock));
            while(num--)    pBlockFree = pBlockFree->pBlock;
            //移动链表,这里暗含了链表是连续整块的。因为链表是由deallocate生成的,它只有在回收连续整块的内存时回收到链表里
            return reinterpret_cast<_T*>(tmp);
        }

        if(BufferedBlocks >= _GrowSize)
        {
            pBuffer = new Buffer(pBuffer); //A linked list : new one and add it to the head
            BufferedBlocks = 0;
        }

        auto atmp = BufferedBlocks;
        return reinterpret_cast<_T*>(pBuffer->GetBlock(BufferedBlocks++));
    }

    _T* allocate(size_t num)
    {
        //return continuous blocks

        if(BufferedBlocks >= _GrowSize)
        {
            pBuffer = new Buffer(pBuffer); //A linked list : new one and add it to the head
            BufferedBlocks = 0;
        }
       MemoryBlock* tmp =pBuffer->GetBlock(BufferedBlocks);
       BufferedBlocks += num; // i+= 1  equals ++i
       return reinterpret_cast<_T*>(tmp);
    }
    //single deallocate was deprecated since it cannot work with multiple deallocate
    void deallocate(_T* pointer)
    {

        deallocate(pointer,1);

    }
    void deallocate(_T* pointer, size_t n)
    {
        MemoryBlock* tmp = reinterpret_cast<MemoryBlock*>(pointer);
         auto num = (Buffer::BlockSize / sizeof (MemoryBlock)) * n;
        while(num--)
        {
            tmp->pBlock = pBlockFree;
            pBlockFree = tmp;
            tmp +=1;
        }
    }
    ~MemoryPool()
    {
        while(pBuffer)
        {
            auto tmp = pBuffer;
            pBuffer = pBuffer->pNext;
            delete tmp;
        }
    }
};

template <typename _T,unsigned short _GrowSize = 1024>
class Allocator : public MemoryPool<_T,_GrowSize>
//class Allocator
{
public:
    typedef _T              value_type;
    typedef _T*             pointer;
    typedef const _T*       const_pointer;
    typedef _T&             reference;
    typedef const _T&       const_reference;
    typedef std::size_t     size_type; //<cstddef>
    typedef std::ptrdiff_t  difference_type;

    template <typename _U>
    struct rebind
    {
        typedef Allocator<_U,_GrowSize> other;
    };

    Allocator() = default;
    //copy construct & rebind construct must exsit to work with stl containers
    Allocator( const Allocator& other) : Allocator(){}
    template <typename _U>
    Allocator(const Allocator<_U,_GrowSize> &d) : Allocator() {}

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
       return MemoryPool<_T,_GrowSize>::allocate();
    }
    pointer allocate(size_type n ,const void* hint = 0)
    {
        if(hint || n <= 0)
            throw std::bad_alloc();
        if(n == 1)
            //deprecated method
//            return MemoryPool<_T,_GrowSize>::getInstance().allocate();
            return MemoryPool<_T,_GrowSize>::allocate();
        else
            return MemoryPool<_T,_GrowSize>::allocate(n);
    }

    void deallocate(pointer p)
    {
       MemoryPool<_T,_GrowSize>::deallocate(p,1);
    }
    void deallocate(pointer p,size_type n) //n must equle to that allocated
    {
        if(n <= 0)
            throw std::bad_alloc();
        else {
            MemoryPool<_T,_GrowSize>::deallocate(p,n);
        }

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


}

#endif // ALLOCATOR_H
