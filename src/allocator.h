#ifndef ALLOCATOR_H
#define ALLOCATOR_H


#ifdef ALLOCATOR_MEMPOOL
    #include "allocator_mempool.h"
#else
	#include "allocator_new.h"
#endif

#endif
	
