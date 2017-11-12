#ifdef UNIT_TEST

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    struct Node *parent;

    unsigned id;
    unsigned height;
} Node;


typedef Node *PNode;

#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


void validate(Node *root)
{
    unsigned lh, rh;
    int bf;

    if (root == NULL) return;

    /* Check links*/
    if (root->parent) assert((root->parent->left == root) || (root->parent->right == root));
    if (root->left) assert(root->left->parent == root);
    if (root->right) assert(root->right->parent == root);

    /* Check invariants. */
    if (root->left) assert(root->left->id <= root->id);
    if (root->right) assert(root->right->id >= root->id);

    /* Check heights. */
    lh = root->left ? root->left->height : 0;
    rh = root->right ? root->right->height : 0;

    assert(root->height == (lh > rh ? lh : rh) + 1);

    /* Check balance.*/
    bf = rh - lh;
    assert(bf >= -1);
    assert(bf <= 1);

    /* Check subtrees. */
    validate(root->left);
    validate(root->right);
}


void dumpTree(Node *root, int depth)
{
    if (root == NULL) return;
    
    /* The tree will appear rotated by 90 degrees. */
    dumpTree(root->right, depth + 1);
    printf("%*s %u (Node: %p, Left: %p, Right: %p, Parent: %p)\n", 4*depth, "", root->id, root, root->left, root->right, root->parent);
    dumpTree(root->left, depth + 1);
}


#define NODE PNode

#define DEFINE_STUFF
#define NEED_INSERT

#include "binarytree.h"

static unsigned state = 0;

unsigned getRand()
{
    state = (1103515245*state + 12345) % 0x7FFFFFFF;
    return state;
}


int main()
{
    int i;
    Node *root = NULL;

    state = 0;

    for (i = 0; i < 0x10000; i++)
    {
        unsigned toInsert = getRand() % 0x10000;
        /*printf("Inserting: %u\n", toInsert);*/
        Node *node = insertUnique(&root, toInsert);
        assert(node);
        assert(node->id == toInsert);

        /*dumpTree(root, 0);
        printf("==============================\n");*/
        validate(root);
        /*fgetc(stdin);*/
    }
}

#endif