/* Common stuff for unittesting.*/

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

#if 0
static void dumpTree(Node *root, int depth)
{
    if (root == NULL) return;
    
    /* The tree will appear rotated by 90 degrees. */
    dumpTree(root->right, depth + 1);
    dumpNode(root, depth);
    dumpTree(root->left, depth + 1);
}
#endif


static void validate(Node *root)
{
    unsigned lh, rh;
    int bf;

    if (root == NULL) return;
    
    /* Check links*/
    if (root->parent) assert((root->parent->left == root) || (root->parent->right == root));
    if (root->left)
    {
        if (root->left->parent != root)
        {
            printf("Problematic node: ");
            dumpNode(root, 0);
            dumpNode(root->left, 0);
        }
        assert(root->left->parent == root);
    }
    if (root->right) assert(root->right->parent == root);

    /* Check invariants. */
    if (root->left) assert(root->left->id <= root->id);
    if (root->right) assert(root->right->id >= root->id);

    /* Check heights. */
    lh = root->left ? root->left->height : 0;
    rh = root->right ? root->right->height : 0;

    /*if (root->height != (lh > rh ? lh : rh) + 1)
    {
        printf("Problematic node: ");
        dumpNode(root, 0);
        printf("Problematic height: %d\n", root->height);
        printf("Problematic left: %d\n", lh);
        printf("Problematic right: %d\n", rh);
    }*/
    assert(root->height == (lh > rh ? lh : rh) + 1);

    /* Check balance.*/
    bf = rh - lh;
    assert(bf >= -1);
    assert(bf <= 1);

    /* Check subtrees. */
    validate(root->left);
    validate(root->right);
}