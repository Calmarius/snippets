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

#include "treecommon.h"


#define NODE PNode

#define DEFINE_STUFF
#define NEED_INSERT
#define NEED_CLEAR
#define NEED_INORDER_ITERATE

#include "binarytree.h"

static unsigned state = 0;

unsigned getRand()
{
    state = (1103515245*state + 12345) % 0x7FFFFFFF;
    return state;
}


#define STEPS 0x1000

void showProgress(unsigned x, unsigned max)
{
    static int prevPercent = -1;
    int percent = 100*x / max;

    if (prevPercent != percent)
    {
        printf("\r%d%% done         ", percent);
        fflush(stdout);
        prevPercent = percent;
    }
    if (percent == 100) printf("\n");
}


static int countNodes(Node *root)
{
    if (!root) return 0;
    /*printf("%d\n", root->id);*/

    return countNodes(root->left) + countNodes(root->right) + 1;
}

int main()
{
    unsigned i;
    Node *root = NULL;
    unsigned long seed = time(NULL);

    printf("Seed is: %ld\n", seed);

    state = seed;

    for (i = 0; i < STEPS; i++)
    {
        unsigned toInsert = getRand() % STEPS;
        Node *node = insertUnique(&root, toInsert);

        showProgress(i + 1, STEPS);

        assert(node);
        assert(node->id == toInsert);

        validate(root);
    }

    printf("There are %d nodes.\n", countNodes(root));

    clear(&root);

    /* Test inorder traversal */
    for (i = 0; i < 100; i++)
    {
        insertUnique(&root, i);
    }
    {
        Node *n;

        for (i = 0; i < 100; i++)
        {
            if (i == 0)
            {
                n = begin(root);
            }
            else
            {
                n = next(n);
            }

            assert(n->id == i);
        }
    }

    clear(&root);

    return 0;
}

#endif