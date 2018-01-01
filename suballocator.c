#ifdef UNIT_TEST

#include <stdlib.h>
#include <string.h>

typedef struct
{
    double x, y, z;
} Vector;

typedef struct
{
    Vector *head;
} VectorAllocCtx, *PVAC;

#define ALLOC_CTX PVAC
#define T Vector
#define PREFIX V_
#define DEFINE_STUFF
#include "suballocator.h"

int main()
{
    VectorAllocCtx ctx;
    Vector *a, *b, *c;

    V_init(&ctx);

    a = V_allocate(&ctx);
    b = V_allocate(&ctx);
    c = V_allocate(&ctx);

    a->x = 1.0;
    a->y = 2.0;
    a->z = 3.0;

    b->x = 4.0;
    b->y = 5.0;
    b->z = 6.0;

    c->x = 7.0;
    c->y = 8.0;
    c->z = 9.0;

    V_deallocate(&ctx, b);
    V_deallocate(&ctx, a);
    V_deallocate(&ctx, c);
    
    
    

    return 0;
}

#endif