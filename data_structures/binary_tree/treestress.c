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
#define NEED_DELETE
#define NEED_FIND

#include "binarytree.h"

static unsigned state = 0;

unsigned getRand()
{
    state = (1103515245*state + 12345) % 0x7FFFFFFF;
    return state;
}


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

#define STEPS 0x1000

int main()
{
    int i;
    Node *root = NULL;
    unsigned long seed = time(NULL);

    printf("Seed is: %ld\n", seed);

    state = seed;

    for (i = 0; i < STEPS; i++)
    {
        unsigned toTouch = getRand() % STEPS;

        showProgress(i + 1, STEPS);

        if (getRand() % 2)
        {
            Node *node = insertUnique(&root, toTouch);
            assert(node);
            assert(node->id == toTouch);
            validate(root);
        }
        else
        {
            Node *node = find(root, toTouch);
            int res;

            res = deleteByKey(&root, toTouch);
            assert((node && !res) || (!node && res)); /* Check if the return value is correct */
            validate(root);
            assert(find(root, toTouch) == NULL);
        }
    }
    /* Delete all nodes at the end. */
    for (i = 0; i < STEPS; i++)
    {
        showProgress(i + 1, STEPS);
        /*printf("%u\n", i);*/
        deleteByKey(&root, i);
        validate(root);
    }
}

#endif