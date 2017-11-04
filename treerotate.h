/**
 *     A         B
 *    / \       / \ 
 *   B   E <=> C   A
 *  / \           / \
 * C   D         D   E
 *
 */
 
/**
 * Binary tree rotation is a key operation in rebalancing binary search trees.
 *
 * This templatized file is fully abstract to allow the use of it in the most wide scenarios.
 *
 */

#include "symconcat.h"

#define FN(x) CCAT(PREFIX, x)

#ifndef PREFIX
    #define PREFIX
#endif

#ifndef NODE
    #error Define the node handle type
#endif

#ifndef LEFT
    #error Define the macro to query the left child.
#endif

#ifndef RIGHT
    #error Define the macro to query the right child.
#endif

#ifndef PARENT
    #error Define the macro to query the parent.
#endif

#ifndef SETLEFT
    #error Define the macro to set the left child.
#endif

#ifndef SETRIGHT
    #error Define the macro to set the right child.
#endif

#ifndef SETPARENT
    #error Define the macro to set the parent.
#endif

void FN(rotateRight)(NODE *root)
{
    NODE A = *root;
    NODE B = LEFT(A);
    NODE D = RIGHT(B);
    NODE rootParent = PARENT(*root);

    SETPARENT(A, B);
    SETRIGHT(B, A);
    SETPARENT(D, A);
    SETLEFT(A, D);
    SETPARENT(B, rootParent);
    *root = B;
}


void FN(rotateLeft)(NODE *root)
{
    NODE B = *root;
    NODE A = RIGHT(B);
    NODE D = LEFT(A);
    NODE rootParent = PARENT(*root);

    SETPARENT(D, B);
    SETRIGHT(B, D);
    SETPARENT(B, A);
    SETLEFT(A, B);
    SETPARENT(A, rootParent);
    *root = A;
}

#undef FN
#undef LEFT
#undef RIGHT
#undef PARENT
#undef SETLEFT
#undef SETRIGHT
#undef SETPARENT
#undef PREFIX

#undef NODE