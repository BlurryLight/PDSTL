#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#define ALLOCATOR_CHOICE 0

#if ALLOCATOR_CHOICE ==0
    #include "allocator_new.h"
#elif ALLOCATOR_CHOICE == 1
	#include "allocator_mempool.h"
#endif

#endif
	
