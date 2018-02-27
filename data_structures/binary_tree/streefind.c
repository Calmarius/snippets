#ifdef UNIT_TEST

#include <assert.h>
#include <stdio.h>

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    unsigned key;
} Node;

typedef Node *PNode;

#define NODE PNode
#define KEY(node) ((node)->key)
#define DEFINE_STUFF
#define NEED_FIND

#include "binarytree.h"

void traverse(int depth, Node *root)
{
    if (root == NULL) return;
    traverse(depth + 1, root->left);
    printf("%*s %d\n", depth * 4, "", root->key);
    traverse(depth + 1, root->right);
}

int main()
{
    Node nodes[32];
    unsigned i;
    Node *root = &nodes[15];

    for (i = 0; i < 31; i++)
    {
        Node *n = &nodes[i];
        int diff;

        n->key = i + 1;
        diff = n->key & (~n->key + 1);
        diff >>= 1;
        if (diff)
        {
            n->left = &nodes[i - diff];
            n->right = &nodes[i + diff];
        }
        else
        {
            n->left = NULL;
            n->right = NULL;
        }
    }

    for (i = 1; i <= 31; i++)
    {
        Node *n = find(root, i);
        assert(n);
        assert(n->key == i);
    }
    assert(find(root, 32) == NULL);
    assert(find(root, 0) == NULL);
    assert(find(root, 6969) == NULL);
    assert(find(root, 666) == NULL);

    return 0;
}

#endif