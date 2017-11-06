
#include "templateheader.h"

#ifndef NODE
    #error Define the node handle type
#endif

#ifndef LEFT
    #error Define the macro to query the left child.
#endif

#ifndef RIGHT
    #error Define the macro to query the right child.
#endif

#ifndef LESS
    #error LESS is not defined. For search trees we need to be able to compare nodes.
#endif

#ifndef EQUAL
    #error EQUAL is not defined. For search trees we need to be able to compare nodes.
#endif

#ifndef KEY
    #error KEY is undefined, it is needed to get the key of a node.
#endif

#ifndef KEY_TYPE
    #error KEY_TYPE is not defined. It is needed to know what kind of key is used.
#endif


#ifdef DECLARE_STUFF
SPECIFIER NODE FN(find)(NODE root, KEY_TYPE key);
#endif

#ifdef DEFINE_STUFF
SPECIFIER NODE FN(find)(NODE root, KEY_TYPE key)
{
    if (root == NULL) return NULL;
    if (LESS(key, KEY(root)))  return FN(find)(LEFT(root), key);
    if (EQUAL(key, KEY(root))) return root;
    return FN(find)(RIGHT(root), key);
}
#endif


#undef NODE
#undef LEFT
#undef RIGHT
#undef LESS
#undef EQUAL
#undef KEY
#undef KEY_TYPE

#include "templatefooter.h"