
#include "templateheader.h"

/* A handle that represents a node in the tree. */
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

/* Macro to set the key of a node. */
#ifndef SET_KEY
    #define SET_KEY(node, k) ((node)->id = (k))
#endif

/* Macro to define the key type.  */
#ifndef KEY_TYPE
    #define KEY_TYPE unsigned
#endif

/* Macro the query the height of a subtree. NULL nodes' height is 0. */
#ifndef HEIGHT
    #define HEIGHT(node) ((node) ? (node)->height : 0)
#endif

#ifndef HEIGHT_RECALC
    #define HEIGHT_RECALC(node) SET_HEIGHT((node), (HEIGHT(LEFT(node)) > HEIGHT(RIGHT(node)) ? HEIGHT(LEFT(node)) : HEIGHT(RIGHT(node))) + 1)
#endif

/* Macro to set the height of a subtree.  */
#ifndef SET_HEIGHT
    #define SET_HEIGHT(node, h) ((node)->height = (h))
#endif

/* Macro to allocate the node with. On failure it returns NULL.  */
#ifndef ALLOCATE_NODE
    #define ALLOCATE_NODE() (malloc(sizeof(*(NODE)(0))))
#endif

#if defined(NEED_INSERT) || defined(NEED_DELETE)
    #ifndef NEED_ROTATE
        #define NEED_ROTATE
    #endif
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
        if (D) SET_PARENT(D, A);
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

        if (D) SET_PARENT(D, B);
        SET_RIGHT(B, D);
        SET_PARENT(B, A);
        SET_LEFT(A, B);
        SET_PARENT(A, rootParent);
        *root = A;
    }
#endif

#ifdef NEED_INSERT
    SPECIFIER NODE FN(insertUnique)(NODE *root, KEY_TYPE k)
    {
        NODE newNode;
        NODE sTree;
        int needLink;

        if (*root == NULL)
        {
            newNode = ALLOCATE_NODE();
            if (!newNode) return NULL;

            SET_KEY(newNode, k);
            SET_HEIGHT(newNode, 1);
            SET_LEFT(newNode, NULL);
            SET_RIGHT(newNode, NULL);
            SET_PARENT(newNode, *root);
            *root = newNode;
            return *root;
        }

        if (EQUAL(k, KEY(*root))) return *root;

        if (LESS(k, KEY(*root)))
        {
            sTree = LEFT(*root);
            needLink = sTree == NULL;
            newNode = FN(insertUnique)(&sTree, k);
            if (needLink) SET_PARENT(newNode, *root);
            SET_LEFT(*root, sTree);
        }
        else
        {
            sTree = RIGHT(*root);
            needLink = sTree == NULL;
            newNode = FN(insertUnique)(&sTree, k);
            if (needLink) SET_PARENT(newNode, *root);
            SET_RIGHT(*root, sTree);
        }

        /* Normalize the current subtree.  */
        {
            NODE lTree = LEFT(*root);
            NODE rTree = RIGHT(*root);
            int bf = HEIGHT(rTree) - HEIGHT(lTree);

            if (bf <= -2)
            {
                int subBf = HEIGHT(RIGHT(lTree)) - HEIGHT(LEFT(lTree));
                if (subBf >= 1)
                {
                    /* LR case */
                    FN(rotateLeft)(&lTree);
                    HEIGHT_RECALC(LEFT(lTree));
                    HEIGHT_RECALC(lTree);
                    SET_LEFT(*root, lTree);
                }
                FN(rotateRight)(root);
                HEIGHT_RECALC(RIGHT(*root));
            }
            else if (bf >= 2)
            {
                int subBf = HEIGHT(RIGHT(rTree)) - HEIGHT(LEFT(rTree));
                if (subBf <= -1)
                {
                    /* RL case */
                    FN(rotateRight)(&rTree);
                    HEIGHT_RECALC(RIGHT(rTree));
                    HEIGHT_RECALC(rTree);
                    SET_RIGHT(*root, rTree);
                }
                FN(rotateLeft)(root);
                HEIGHT_RECALC(LEFT(*root));
            }

            HEIGHT_RECALC(*root);
        }

        return newNode;
    }
#endif

#endif

#undef NODE
#undef LEFT
#undef SET_LEFT
#undef RIGHT
#undef SET_RIGHT
#undef PARENT
#undef SET_PARENT
#undef LESS
#undef EQUAL
#undef KEY
#undef KEY_TYPE
#undef DECLARE_STUFF
#undef DEFINE_STUFF
#undef NEED_FIND
#undef NEED_ROTATE
#undef NEED_INSERT
#undef NEED_DELETE
#undef ALLOCATE_NODE
#undef SET_KEY
#undef SET_HEIGHT
#undef HEIGHT
#undef HEIGHT_RECALC


#include "templatefooter.h"