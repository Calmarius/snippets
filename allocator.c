#ifdef UNIT_TEST

#include <stdlib.h>

typedef struct BoundaryTreeNode
{
    struct BoundaryTreeNode *left;
    struct BoundaryTreeNode *right;
    struct BoundaryTreeNode *parent;
    size_t height;
    void *id;
} Node, *PNode;

typedef struct
{
    Node *boundaryStorage;
} Allocator, *PAllocator;

typedef void *PVoid;

#define NODE PNode
#define PREFIX Alloc_
#define KEY_TYPE PVoid
#define DECLARE_STUFF
#define DEFINE_STUFF
#define NEED_INSERT
#define NEED_FIND
#define NEED_DELETE
#define NEED_INORDER_ITERATE
#include "binarytree.h"

int toggleBoundary(Allocator *ctx, void *addr)
{
    Node *n = Alloc_find(ctx->boundaryStorage, addr);

    if (n)
    {
        /* Found, remove it. */
        Alloc_deleteByKey(&ctx->boundaryStorage, addr);
    }
    else
    {
        /* Not found add it. */
        n = Alloc_insertUnique(&ctx->boundaryStorage, addr);
        if (n == NULL) return -1;
    }

    return 0;
}

#define ALLOC_CTX PAllocator
#define TOGGLE_BOUNDARY(ctx, addr) toggleBoundary(ctx, addr)
#define ALLOC_ITERATOR PNode
#define FIRST_BOUNDARY(ctx) Alloc_begin(ctx->boundaryStorage)
#define AI_ADDR(iterator) ((iterator)->id)
#define NULL_ITERATOR NULL
#define NEXT_BOUNDARY(ctx, iter) Alloc_next(iter)
#define SYSTEM_ALLOC(ctx, size) malloc(size)
#define DECLARE_STUFF
#define DEFINE_STUFF

#include "allocator.h"

#include <assert.h>

int main()
{
    return 0; 
}

#endif