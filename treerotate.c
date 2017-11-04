#ifdef UNIT_TEST

#include <stddef.h>
#include <assert.h>
#include <stdio.h>

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    struct Node *parent;

    char key;
} Node;

typedef Node *PNode;

#define NODE PNode
#define LEFT(x) (x)->left
#define RIGHT(x) (x)->right
#define PARENT(x) (x)->parent
#define SETLEFT(x, y) (x)->left = (y)
#define SETRIGHT(x, y) (x)->right = (y)
#define SETPARENT(x, y) (x)->parent = (y)

#include "treerotate.h"

int main()
{
    Node A, B, C, D, E;
    Node *root;

    A.key = 'A';
    A.parent = NULL;
    A.left = &B;
    A.right = &E;

    B.key = 'B';
    B.parent = &A;
    B.left = &C;
    B.right = &D;

    C.key = 'C';
    C.parent = &B;
    C.left = NULL;
    C.right = NULL;

    D.key = 'D';
    D.parent = &B;
    D.left = NULL;
    D.right = NULL;

    E.key = 'E';
    E.parent = &A;
    E.left = NULL;
    E.right = NULL;

    root = &A;

    rotateRight(&root);

    assert(root == &B);
    assert(A.parent == &B);
    assert(A.left == &D);
    assert(A.right == &E);
    assert(B.parent == NULL);
    assert(B.left == &C);
    assert(B.right == &A);
    assert(C.parent == &B);
    assert(C.left == NULL);
    assert(C.right == NULL);
    assert(D.parent == &A);
    assert(D.left == NULL);
    assert(D.right == NULL);
    assert(E.parent == &A);
    assert(E.left == NULL);
    assert(E.right == NULL);

    rotateLeft(&root);

    assert(root == &A);
    assert(A.parent == NULL);
    assert(A.left == &B);
    assert(A.right == &E);
    assert(B.parent == &A);
    assert(B.left == &C);
    assert(B.right == &D);
    assert(C.parent == &B);
    assert(C.left == NULL);
    assert(C.right == NULL);
    assert(D.parent == &B);
    assert(D.left == NULL);
    assert(D.right == NULL);
    assert(E.parent == &A);
    assert(E.left == NULL);
    assert(E.right == NULL);
}

#endif