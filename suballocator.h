#include "templateheader.h"

#ifndef T
    #error You must define the element type.
#endif

#ifndef ALLOC_CTX
    #error You must define the data type of the allocator context (something which stores the state of the allocator).
#endif

/* A macro to query the head ptr of the */
#ifndef GET_HEAD_PTR
    #define GET_HEAD_PTR(ctx) (ctx)->head
#endif

/* A macro to query the head ptr of the */
#ifndef SET_HEAD_PTR
    #define SET_HEAD_PTR(ctx, ptr) (ctx)->head = (ptr)
#endif


/* The number of elements in a page. */
#ifndef PAGE_ELEMENTS
    #define PAGE_ELEMENTS 100
#endif

/* A function to request new memory. It returns the pointer to the new page or NULL.  */
#ifndef GET_PAGE
    #define GET_PAGE() malloc(sizeof(T)*PAGE_ELEMENTS)
#endif

#ifndef MEMCPY
    #define MEMCPY(a, b, n) memcpy((a), (b), (n))
#endif

#ifdef DECLARE_STUFF
    SPECIFIER T* FN(allocate)(ALLOC_CTX ctx);
    SPECIFIER void FN(deallocate(ALLOC_CTX ctx, T *elem);
    SPECIFIER void FN(init)(ALLOC_CTX ctx);
#endif


#ifdef DEFINE_STUFF
    #include <stddef.h>

    SPECIFIER void FN(init)(ALLOC_CTX ctx)
    {
        SET_HEAD_PTR(ctx, NULL);
    }


    SPECIFIER T* FN(allocate)(ALLOC_CTX ctx)
    {
        T *head = GET_HEAD_PTR(ctx);

        if (head == NULL)
        {
            /* Need to allocate new page. */
            T *newPage = GET_PAGE();
            size_t n = PAGE_ELEMENTS;

            if (newPage == NULL) return NULL;

            while (n --> 0)
            {
                MEMCPY(newPage + n, &head, sizeof(T*));
                head = newPage + n;
            }

            SET_HEAD_PTR(ctx, head);
        }

        {
            T *newHead;

            MEMCPY(&newHead, head, sizeof(T*));
            SET_HEAD_PTR(ctx, newHead);
        }

        return head;
    }


    SPECIFIER void FN(deallocate)(ALLOC_CTX ctx, T *elem)
    {
        T *head = GET_HEAD_PTR(ctx);

        MEMCPY(elem, head, sizeof(T*));
        SET_HEAD_PTR(ctx, elem);
    }

#endif


#include "templatefooter.h"
#undef T
#undef ALLOC_CTX
#undef GET_HEAD_PTR
#undef SET_HEAD_PTR
#undef PAGE_ELEMENTS
#undef GET_PAGE
#undef END_PTR
#undef MEMCPY

