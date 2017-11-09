
#include "templateheader.h"

/* Pointer to the node structure used. It's called the node handle.*/
#ifndef NODE
    #error Define the node handle type
#endif

/* Macro to access the left subtree. Should return a node handle. */
#ifndef LEFT
    #define LEFT(node) ((node)->left)
#endif

/* Macro to set the left subtree. */
#ifndef SET_LEFT
    #define SET_LEFT(node, newLeft) ((node)->left = (newLeft))
#endif

/* Macro to access the right subtree. Should return a node handle. */
#ifndef RIGHT
    #define RIGHT(node) ((node)->right)
#endif

/* Macro to set the right subtree. */
#ifndef SET_RIGHT
    #define SET_RIGHT(node, newRight) ((node)->right = (newRight))
#endif

/* Macro to access the parent node.. Should return a node handle. */
#ifndef PARENT
    #define PARENT(node) ((node)->parent)
#endif

/* Macro to set the right subtree. */
#ifndef SET_PARENT
    #define SET_PARENT(node, newParent) ((node)->parent = (newParent))
#endif

/* Macro to compare the keys in the tree. It should return true if x is sorted earlier than y.*/
#ifndef LESS
    #define LESS(x, y) ((x) < (y))
#endif

/* Macro to compare keys. It should return true if x and y should compare equal.*/
#ifndef EQUAL
    #define EQUAL(x, y) ((x) == (y))
#endif

/* Macro to query the key of the node.  */
#ifndef KEY
    #define KEY(node) ((node)->id)
#endif

/* Macro to define the key type.  */
#ifndef KEY_TYPE
    #define KEY_TYPE unsigned
#endif


#ifdef DECLARE_STUFF

#ifdef NEED_FIND
    SPECIFIER NODE FN(find)(NODE root, KEY_TYPE key);
#endif

#ifdef NEED_ROTATE
    SPECIFIER void FN(rotateRight)(NODE *root);
    SPECIFIER void FN(rotateLeft)(NODE *root);
#endif

#endif

#ifdef DEFINE_STUFF
#ifdef NEED_FIND
    SPECIFIER NODE FN(find)(NODE root, KEY_TYPE key)
    {
        if (root == NULL) return NULL;
        if (LESS(key, KEY(root)))  return FN(find)(LEFT(root), key);
        if (EQUAL(key, KEY(root))) return root;
        return FN(find)(RIGHT(root), key);
    }
#endif    

#ifdef NEED_ROTATE
    SPECIFIER void FN(rotateRight)(NODE *root)
    {
        NODE A = *root;
        NODE B = LEFT(A);
        NODE D = RIGHT(B);
        NODE rootParent = PARENT(*root);
    
        SET_PARENT(A, B);
        SET_RIGHT(B, A);
        SET_PARENT(D, A);
        SET_LEFT(A, D);
        SET_PARENT(B, rootParent);
        *root = B;
    }
    
    SPECIFIER void FN(rotateLeft)(NODE *root)
    {
        NODE B = *root;
        NODE A = RIGHT(B);
        NODE D = LEFT(A);
        NODE rootParent = PARENT(*root);
    
        SET_PARENT(D, B);
        SET_RIGHT(B, D);
        SET_PARENT(B, A);
        SET_LEFT(A, B);
        SET_PARENT(A, rootParent);
        *root = A;
    }
#endif
    
#endif


#undef NODE
#undef LEFT
#undef RIGHT
#undef LESS
#undef EQUAL
#undef KEY
#undef KEY_TYPE

#include "templatefooter.h"