#include "templateheader.h"

#include <stddef.h>
#include <stdint.h>

/**
 * An user provided handle type to the allocator.
 */
#ifndef ALLOC_CTX
    #error Provide the allocator handle type
#endif

/* Addresses and sizes will be divisible by this. Must be a power of 2.*/
#ifndef ALLOC_ALIGNMENT
    #define ALLOC_ALIGNMENT 16
#endif

/* TOGGLE_BOUNDARY(ALLOC_CTX ctx, void *addr): int.
 * 
 * Toggles boundary between the allocated and deallocated regions.
 * It must return 0 on successful toggle. -1 on failure.
 */
#ifndef TOGGLE_BOUNDARY
    #error You must provide a way to toggle boundaries in your context.
#endif

/**
 * An iterator used to iterate the boundaries.
 * 
 * For example a pointer to a node in a binary tree, or something like that.
 */
#ifndef ALLOC_ITERATOR
    #error You need to define the iterator that is used to iterate the boundaries
#endif

/**
 * FIRST_BOUNDARY(ALLOC_CTX ctx): ALLOC_ITERATOR
 * 
 * Returns an iterator to the first border.
 */
#ifndef FIRST_BOUNDARY
    #error You need to provide a way to enumerate the first border. 
#endif


/**
 * AI_ADDR(ALLOC_ITERATOR iterator): void*
 * 
 * It's a macro that should extract address from the iterator.
 */
#ifndef AI_ADDR
    #error Need a way to extract address from the allocator iterator.
#endif

/**
 * A way to know when we reached the end of the iteration.
 */
#ifndef NULL_ITERATOR
    #error Need to provide a value for the NULL boundary iterator.
#endif

/**
 * NEXT_BOUNDARY(ALLOC_CTX ctx, ALLOC_ITERATOR iterator): ALLOC_ITERATOR
 * 
 * A way to move to the next iterator. If there are no more boundaries it must return NULL_ITERATOR.
 */
#ifndef NEXT_BOUNDARY
    #error Need a way to move to the next iterator.
#endif

/**
 * The size of the pages requested of the system will be divisible by this.
 * Must be a power of 2.
 */
#ifndef ALLOC_GRANULARITY
    #define ALLOC_GRANULARITY 0x10000
#endif

/**
 * SYSTEM_ALLOC(ALLOC_CTX ctx, size_t size): void*
 * 
 * Allocates a larger page of memory from the system. if it cannot allocate it must return NULL.
 */
#ifndef SYSTEM_ALLOC
    #error Need a way to allocate memory from the system.
#endif


/**
 * The allocator works by storing only the borders between the allocated and deallocated blocks in a searchable data structure.
 * 
 * The first border marks the start of the first free block, the second marks the end of it; the third one marks the start of the second free block, the fourth marks the end of it, and so on.
 * 
 * Allocation and deallocation are identical operations: just toggle the border of the start and the end of the blocks. Wrappers are provided to avoid problems.
 * This provides automatic coalescence of free and allocated blocks.
 * 
 * This also makes it possible to deallocate a block partially. The drawback is that you will need to know the size of the block to deallocate (In case of The classic malloc/free stuff you don't need to know the size you free).
 * 
 * The allocator uses the first fit method to find the block ordered by address.
 */

#ifdef DECLARE_STUFF
    SPECIFIER void *FN(memToggle)(ALLOC_CTX ctx, void *addr, size_t size);
#endif

#ifdef DEFINE_STUFF

SPECIFIER void *FN(memToggle)(ALLOC_CTX ctx, void *addr, size_t size)
{
    void *alignedAddr = (void*)((uintptr_t)addr & ~(ALLOC_ALIGNMENT - 1));
    size_t alignedSize = (size & ~(ALLOC_ALIGNMENT - 1)) + ALLOC_ALIGNMENT;

    if (addr)
    {
        /* The simplest case is toggling the the boundaries*/
        if (TOGGLE_BOUNDARY(ctx, alignedAddr)) return NULL;
        if (TOGGLE_BOUNDARY(ctx, (char*)alignedAddr + alignedSize)) return NULL;
        return alignedAddr;
    }

    /* More complex case when we need to find where to allocate. */
    {
        ALLOC_ITERATOR itStartAddr = FIRST_BOUNDARY(ctx);
        ALLOC_ITERATOR itEndAddr;

        while (itStartAddr != NULL_ITERATOR)
        {
            ptrdiff_t blockSize;
            void *startAddr;
            void *endAddr;

            itEndAddr = NEXT_BOUNDARY(ctx, itStartAddr);
            if (itEndAddr == NULL_ITERATOR) return NULL; /* Consistency broken. */

            startAddr = AI_ADDR(itStartAddr);
            endAddr = AI_ADDR(itEndAddr);

            itStartAddr = NEXT_BOUNDARY(ctx, itEndAddr);

            if (endAddr < startAddr) return NULL; /* Consistency broken. */

            blockSize = (uintptr_t)endAddr - (uintptr_t)startAddr;

            if ((size_t)blockSize >= alignedSize)
            {
                /* This is the right block to allocate in. */
                if (TOGGLE_BOUNDARY(ctx, startAddr)) return NULL;
                if (TOGGLE_BOUNDARY(ctx, (char*)startAddr + alignedSize)) return NULL;
                return startAddr;
            }
        }
        /* Haven't found a room to fit the block in. We need to request a page to do the allocation. */
        {
            size_t pageSize = (alignedSize & ~(ALLOC_GRANULARITY - 1)) + ALLOC_GRANULARITY;
            void *newPage = SYSTEM_ALLOC(ctx, pageSize);

            if (newPage == NULL) return NULL; /* Failed to get page the allocator fails too. */

            /* Toggle a new free area beyond the allocated block. */
            if (TOGGLE_BOUNDARY(ctx, (char*)newPage + alignedSize)) return NULL;
            if (TOGGLE_BOUNDARY(ctx, (char*)newPage + pageSize)) return NULL;

            return newPage;
        }
    }
}

#endif



#include "templatefooter.h"

#undef ALLOC_CTX
#undef ALLOC_ALIGNMENT
#undef TOGGLE_BOUNDARY
#undef ALLOC_ITERATOR
#undef FIRST_BOUNDARY
#undef AI_ADDR
#undef NULL_ITERATOR
#undef NEXT_BOUNDARY
#undef ALLOC_GRANULARITY
#undef SYSTEM_ALLOC

#undef DECLARE_STUFF
#undef DEFINE_STUFF