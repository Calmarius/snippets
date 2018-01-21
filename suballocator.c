#ifdef UNIT_TEST

#include <stdlib.h>
#include <string.h>

typedef struct
{
    double x, y, z;
} Vector, *PVector;

typedef struct
{
    Vector *head;
} VectorAllocCtx, *PVAC;

#include <sys/mman.h>


#define ELEM PVector
#define LIST_TYPE PVAC
#define GET_HEAD(list) ((list)->head)
#define SET_HEAD(list, ptr) ((list)->head = (ptr))
#define VL_next(elem) (*(PVector*)(elem))
#define GET_NEXT(elem) VL_next(elem)
#define SET_NEXT(elem, next) (*(PVector*)(elem) = (next))
#define LESS(elem1, elem2) ((elem1) < (elem2))
#define PREFIX VL_
#define DEFINE_STUFF
#include "linkedlist.h"


#define ALLOC_CTX PVAC
#define T Vector
#define PREFIX V_
#define ADD_FREE_BLOCK(ctx, ptr) VL_addHead(ctx, ptr)
#define REMOVE_FREE_BLOCK(ctx) VL_removeHead(ctx)
#define DEFINE_STUFF
#include "suballocator.h"

#define ELEM_TYPE Vector

#include <stdio.h>
#include <assert.h>

#define PAGE_ELEMENTS 100

int main()
{
    VectorAllocCtx ctx;
    Vector *a, *b, *c;
    Vector pool[PAGE_ELEMENTS];

    V_init(&ctx);

    V_addPage(&ctx, pool, PAGE_ELEMENTS);

    a = V_allocate(&ctx);
    assert(a);
    b = V_allocate(&ctx);
    assert(b);
    c = V_allocate(&ctx);
    assert(c);

    a->x = 1.0;
    a->y = 2.0;
    a->z = 3.0;

    b->x = 4.0;
    b->y = 5.0;
    b->z = 6.0;

    c->x = 7.0;
    c->y = 8.0;
    c->z = 9.0;

    /* After each deallocation we check the pointer is really added to the free list. */
    V_deallocate(&ctx, b);
    assert(ctx.head == b);

    V_deallocate(&ctx, a);
    assert(ctx.head == a);

    V_deallocate(&ctx, c);
    assert(ctx.head == c);

    /* Now the allocator is empty, we should be able to allocate all elements. */

    {
        size_t n = 0;

        for (;;)
        {
            Vector *tmp = V_allocate(&ctx);

            if (tmp == NULL) break;
            n++;
        }
        assert(n == PAGE_ELEMENTS);
    }

    return 0;
}

#endif