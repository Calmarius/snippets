
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

#ifndef FREE_NODE
    #define FREE_NODE(node) (free(node))
#endif

/* Macro the expand to the NULL node.*/
#ifndef NULL_NODE
    #define NULL_NODE NULL
#endif

#if defined(NEED_INSERT) || defined(NEED_DELETE)
    #ifndef NEED_ROTATE
        #define NEED_ROTATE
    #endif
#endif


#ifdef DECLARE_STUFF

#ifdef NEED_FIND
    /**
     * Find the node specified by key.
     *
     * root (in): handle of the root node of the subtree.
     * key (in): the key to find.
     * 
     * Returns the handle to node.
     */
    SPECIFIER NODE FN(find)(NODE root, KEY_TYPE key);
#endif

#ifdef NEED_ROTATE
    /** Rotates the tree to the right. 
     * 
     * root (in, out): The root before and after the rotation.
     *
     * The function assumes there is a left subtree.
     */
    SPECIFIER void FN(rotateRight)(NODE *root);
    /** Rotates the tree to the left.. 
     * 
     * root (in, out): The root before and after the rotation.
     *
     * The function assumes there is a right subtree.
     */
    SPECIFIER void FN(rotateLeft)(NODE *root);
#endif

#ifdef NEED_INSERT
    /** Inserts new element into the binary tree and returns the handle of this new node. No duplicates allowed. 
     *
     * root (in, out): The root of tree, it may change if the tree is rotated to rebalance.
     * k: the key of the new element.
     *
     * Returns a handle to the new node.
     */
    SPECIFIER NODE FN(insertUnique)(NODE *root, KEY_TYPE k);
#endif

#if defined(NEED_INSERT) || defined(NEED_DELETE)
    /** Rebalaces tree if needed.
     *
     * root (in, out): The root of the tree, it may change as a result of rotation.
     *
     * This is an internal function it shouldn't be used.
     */
    static void FN(rebalanceTree)(NODE *root);
#endif

#ifdef NEED_DELETE
    /** Deletes key from the tree.
     *
     * root (in, out): The root of the tree, it may change if the deletion causes rotation.
     * k (in): The key to delete.
     *
     * Returns zero if the node is found and deleted, returns non-zero if the element is not found.
     */
    SPECIFIER int FN(deleteByKey)(NODE *root, KEY_TYPE k);
    /** Deletes root of the subtree.
     *
     * root (in, out): The root of the tree to delete, it will be changed by another suitable element.
     *
     * This is an internal function do not use unless you know what you are doing.
     */
    static void FN(deleteRoot)(NODE *root);
#endif

#ifdef NEED_CLEAR
    /* Clears the tree, sets the root node NULL_NODE*/
    SPECIFIER int FN(clear)(NODE *root);
#endif

#endif

#ifdef DEFINE_STUFF
#ifdef NEED_FIND
    SPECIFIER NODE FN(find)(NODE root, KEY_TYPE key)
    {
        if (root == NULL_NODE) return NULL_NODE;
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
        if (D != NULL_NODE) SET_PARENT(D, A);
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

        if (D != NULL_NODE) SET_PARENT(D, B);
        SET_RIGHT(B, D);
        SET_PARENT(B, A);
        SET_LEFT(A, B);
        SET_PARENT(A, rootParent);
        *root = A;
    }
#endif

#if defined(NEED_INSERT) || defined(NEED_DELETE)
    static void FN(rebalanceTree)(NODE *root)
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
#endif

#if defined(NEED_DELETE)
    static void FN(deleteNode)(NODE *root, NODE node)
    {
        NODE replacement = NULL_NODE;
        NODE rc = node; /* Child of replacement */
        NODE rNode; /* Node where the rebalancing starts. */

        if (node == NULL_NODE) return; /* Nothing to delete, bail out. */
        rNode = PARENT(node); /* This is where the rebalancing starts if we don't find replacement. */

        /* Find a replacement. */
        if (LEFT(node) != NULL_NODE)
        {
            /* Rightmost of the left subtree is the replacement */
            replacement = LEFT(node);
            while (RIGHT(replacement) != NULL_NODE)
            {
                replacement = RIGHT(replacement);
            }
            rNode = PARENT(replacement); /* The balancing start from where the replacement was removed.*/

            /* Remove the replacement, it may have left subtree, so relink it.*/
            rc = LEFT(replacement);
            if (rc != NULL_NODE) SET_PARENT(rc, rNode);
            if (rNode == node)
            {
                SET_LEFT(rNode, rc);
            }
            else
            {
                SET_RIGHT(rNode, rc);
            }
        }
        else if (RIGHT(node) != NULL_NODE)
        {
            /* Symmetric logic for the right subtree. */
            replacement = RIGHT(node);
            while (LEFT(replacement) != NULL_NODE)
            {
                replacement = LEFT(replacement);
            }
            rNode = PARENT(replacement);

            rc = RIGHT(replacement);
            if (rc != NULL_NODE) SET_PARENT(rc, rNode);
            if (rNode == node)
            {
                SET_RIGHT(rNode, rc);
            }
            else
            {
                SET_LEFT(rNode, rc);
            }
        }

        /* The node where the balancing starts happens to be node to be removed we should replace it by the replacement..  */
        if (rNode == node) rNode = replacement;

        /* Perform the replacement*/
        if (replacement != NULL_NODE)
        {
            /* Set the replacement's pointers. */
            SET_PARENT(replacement, PARENT(node));
            SET_LEFT(replacement, LEFT(node));
            SET_RIGHT(replacement, RIGHT(node));
        }

        if (PARENT(node))
        {
            NODE p = PARENT(node);
            if (RIGHT(p) == node) SET_RIGHT(p, replacement);
            else SET_LEFT(p, replacement);
        }
        if (LEFT(node) != NULL_NODE) SET_PARENT(LEFT(node), replacement);
        if (RIGHT(node) != NULL_NODE) SET_PARENT(RIGHT(node), replacement);

        FREE_NODE(node);

        /*printf("Before rebalancing:\n");
        printf("node: "); dumpNode(node, 0);
        printf("rNode: "); dumpNode(rNode, 0);*/

        /* Perform the rebalancing, and height recalc*/
        while (rNode != NULL_NODE)
        {
            NODE p = PARENT(rNode);
            int linkId;

            if (p != NULL_NODE)
            {
                linkId = RIGHT(p) == rNode;
            }

            FN(rebalanceTree)(&rNode);

            if (p != NULL_NODE)
            {
                if (linkId) SET_RIGHT(p, rNode);
                else SET_LEFT(p, rNode);
            }
            else
            {
                break;
            }

            rNode = p;
        }
        *root = rNode;
    }


    SPECIFIER int FN(deleteByKey)(NODE *root, KEY_TYPE key)
    {
        int ret;
        NODE node;

        node = *root;

        while (node)
        {
            if (EQUAL(key, KEY(node)))
            {
                FN(deleteNode)(root, node);
                return 0;
            }
            else if (LESS(key, KEY(node)))
            {
                node = LEFT(node);
            }
            else
            {
                node = RIGHT(node);
            }
        }

        return -1;

        return ret;
    }


#endif

#ifdef NEED_CLEAR
    SPECIFIER void FN(clear)(NODE *root)
    {
        NODE current;
        /*int count = 0;*/

        /*dumpTree(*root, 0);*/

        current = *root;

        if (current == NULL_NODE) return;

        for (;;)
        {
            if (LEFT(current))
            {
                current = LEFT(current);
            }
            else if (RIGHT(current))
            {
                current = RIGHT(current);
            }                            
            else
            {
                break;
            }
        }

        while (current)
        {
            NODE toDelete = current;

            if (PARENT(current))
            {
                if (RIGHT(PARENT(current)) == current)
                {
                    /* Right child, the parent is the next to traverse */
                    current = PARENT(current);
                }
                else
                {
                    /* Left child, the parent's right subtree is not traversed yet, so traverse it, if needed..*/
                    current = PARENT(current);
                    if (RIGHT(current) != NULL_NODE) 
                    {
                        current = RIGHT(current);
                        for (;;)
                        {
                            if (LEFT(current))
                            {
                                current = LEFT(current);
                            }
                            else if (RIGHT(current))
                            {
                                current = RIGHT(current);
                            }                            
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                current = NULL_NODE;
            }
            /*printf("%d\n", toDelete->id);*/

            FREE_NODE(toDelete);
            /*count++;*/
        }

        /*printf("Count deleted: %d\n", count);*/

        *root = NULL_NODE;
    }

#endif

#ifdef NEED_INSERT

    SPECIFIER NODE FN(insertUnique)(NODE *root, KEY_TYPE k)
    {
        NODE newNode;
        NODE sTree;
        int needLink;

        if (*root == NULL_NODE)
        {
            newNode = ALLOCATE_NODE();
            if (!newNode) return NULL_NODE;

            SET_KEY(newNode, k);
            SET_HEIGHT(newNode, 1);
            SET_LEFT(newNode, NULL_NODE);
            SET_RIGHT(newNode, NULL_NODE);
            SET_PARENT(newNode, *root);
            *root = newNode;
            return *root;
        }

        if (EQUAL(k, KEY(*root))) return *root;

        if (LESS(k, KEY(*root)))
        {
            sTree = LEFT(*root);
            needLink = sTree == NULL_NODE;
            newNode = FN(insertUnique)(&sTree, k);
            if (needLink) SET_PARENT(newNode, *root);
            SET_LEFT(*root, sTree);
        }
        else
        {
            sTree = RIGHT(*root);
            needLink = sTree == NULL_NODE;
            newNode = FN(insertUnique)(&sTree, k);
            if (needLink) SET_PARENT(newNode, *root);
            SET_RIGHT(*root, sTree);
        }

        /* Normalize the current subtree.  */
        FN(rebalanceTree)(root);

        return newNode;
    }
    #endif

    #ifdef NEED_DELETE
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
#undef NEED_CLEAR
#undef ALLOCATE_NODE
#undef SET_KEY
#undef SET_HEIGHT
#undef HEIGHT
#undef HEIGHT_RECALC


#include "templatefooter.h"