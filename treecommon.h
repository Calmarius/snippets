/* Common stuff for unittesting.*/

#if 0
static void dumpNode(Node *node, int depth)
{
    if (!node)
    {
        printf("NULL node\n");
        return;
    }
    printf("%*s %u (Node: %p, Left: %p, Right: %p, Parent: %p, Height: %d)\n",
        4*depth,
        "",
        node->id,
        node,
        node->left,
        node->right,
        node->parent,
        node->height
    );
}

static void dumpTree(Node *root, int depth)
{
    if (root == NULL) return;

    /* The tree will appear rotated by 90 degrees. */
    dumpTree(root->right, depth + 1);
    dumpNode(root, depth);
    dumpTree(root->left, depth + 1);
}
#endif


static void validateSubtree(Node *root)
{
    unsigned lh, rh;
    int bf;

    if (root == NULL) return;

    /* Check links*/
    if (root->parent) assert((root->parent->left == root) || (root->parent->right == root));
    if (root->left)
    {
        assert(root->left->parent == root);
    }
    if (root->right)
    {
        assert(root->right->parent == root);
    }

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
    validateSubtree(root->left);
    validateSubtree(root->right);
}


static void validate(Node *root)
{
    if (root == NULL) return;

    assert(root->parent == NULL);

    validateSubtree(root);
}