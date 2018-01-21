#include "templateheader.h"

#ifndef T
    #error You must define the element type.
#endif

#ifndef ALLOC_CTX
    #error You must define the data type of the allocator context (something which stores the state of the allocator).
#endif

/* This should insert the free block to the free list.  */
#ifndef ADD_FREE_BLOCK
    /* ADD_FREE_BLOCK(ctx, ptrToFree) */
    #error You need to provide a way to add free block to the list.
#endif

/* Removes the free block from the free list. */
#ifndef REMOVE_FREE_BLOCK
    /* REMOVE_FREE_BLOCK(ctx) */
    #error You need to provide a way to remove a free block from the list.
#endif


/* A macro to query the head ptr of the */
#ifndef GET_FIRST_PTR
    #define GET_FIRST_PTR(ctx) (ctx)->head
#endif

/* A macro to query the head ptr of the */
#ifndef SET_FIRST_PTR
    #define SET_FIRST_PTR(ctx, ptr) (ctx)->head = (ptr)
#endif

#ifdef DECLARE_STUFF
    /* Initializes the allocator with no pages to allocate. */
    SPECIFIER void FN(init)(ALLOC_CTX ctx);

    /* Adds a new page to the allocator so it can be used to make future allocations from it. */
    SPECIFIER void FN(addPage)(ALLOC_CTX ctx, T *pageAddr, size_t nElements)

    /* Allocates elements from the allocator, returns NULL, when there are no free nodes. */
    SPECIFIER T* FN(allocate)(ALLOC_CTX ctx);

    /* Frees the given element and inserts it into the free list. */
    SPECIFIER void FN(deallocate(ALLOC_CTX ctx, T *elem);
#endif /* DECLARE_STUFF */


#ifdef DEFINE_STUFF
    #include <stddef.h>

    SPECIFIER void FN(init)(ALLOC_CTX ctx)
    {
        SET_FIRST_PTR(ctx, NULL);
    }


    SPECIFIER void FN(addPage)(ALLOC_CTX ctx, T *pageAddr, size_t nElements)
    {
        while (nElements --> 0)
        {
            ADD_FREE_BLOCK(ctx, pageAddr + nElements);
        }
    }


    SPECIFIER T* FN(allocate)(ALLOC_CTX ctx)
    {
        T *head = GET_FIRST_PTR(ctx);

        if (head == NULL) return head;

        head = GET_FIRST_PTR(ctx);
        REMOVE_FREE_BLOCK(ctx);

        return head;
    }


    SPECIFIER void FN(deallocate)(ALLOC_CTX ctx, T *elem)
    {
        ADD_FREE_BLOCK(ctx, elem);
    }
#endif /* DEFINE_STUFF */


#include "templatefooter.h"
#undef T
#undef ALLOC_CTX
#undef ADD_FREE_BLOCK
#undef REMOVE_FREE_BLOCK
#undef GET_FIRST_PTR
#undef SET_FIRST_PTR

