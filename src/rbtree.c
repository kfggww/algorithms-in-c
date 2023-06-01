#include <stdlib.h>
#include "rbtree.h"

struct RBTNode
{
    struct RBTNode *parent;
    struct RBTNode *left;
    struct RBTNode *right;
    int color;
    int value;
};

struct RBTree
{
    struct RBTNode *root;
};

enum RBTColor
{
    kBlack,
    kRed,
};

typedef struct RBTNode RBTNode;
typedef struct RBTree RBTree;

static inline int is_black(RBTNode *node)
{
    return node == NULL || node->color == kBlack;
}

static inline int is_red(RBTNode *node)
{
    return !is_black(node);
}

static inline RBTNode *left(RBTNode *node)
{
    return node != NULL ? node->left : NULL;
}

static inline RBTNode *right(RBTNode *node)
{
    return node != NULL ? node->right : NULL;
}

static inline RBTNode *parent(RBTNode *node)
{
    return node != NULL ? node->parent : NULL;
}

static inline int is_left_child(RBTNode *node)
{
    if (node == NULL || parent(node) == NULL)
        return 0;

    return node == parent(node)->left;
}

static inline int is_right_child(RBTNode *node)
{
    if (node == NULL || parent(node) == NULL)
        return 0;

    return node == parent(node)->right;
}

static RBTNode *rbtnode_new1(int val, int color)
{
    RBTNode *node = malloc(sizeof(*node));
    node->value = val;
    node->color = color;
    node->parent = node->left = node->right = NULL;

    return node;
}

static RBTNode *rbtnode_new2(int val, int color, RBTNode *parent)
{
    RBTNode *node = malloc(sizeof(*node));
    node->value = val;
    node->color = color;
    node->parent = parent;
    node->left = node->right = NULL;

    return node;
}

static void flip_color2red(RBTNode *node)
{

    if (!(is_black(node) && is_red(left(node)) && is_red(right(node))))
        return;

    node->color = kRed;
    node->left->color = kBlack;
    node->right->color = kBlack;
}

static void rotate_left(RBTNode *v, RBTNode *p, RBTNode *g)
{
    if (v == NULL || p == NULL || p->right != v || p->parent != g)
        return;

    int is_left = is_left_child(p);

    if (v->left != NULL)
        v->left->parent = p;

    p->right = v->left;
    p->parent = v;

    v->left = p;
    v->parent = g;

    if (g != NULL)
    {
        if (is_left)
            g->left = v;
        else
            g->right = v;
    }
}

static void rotate_right(RBTNode *v, RBTNode *p, RBTNode *g)
{
    if (v == NULL || p == NULL || p->left != v || p->parent != g)
        return;

    int is_left = is_left_child(p);

    if (v->right != NULL)
        v->right->parent = p;

    p->left = v->right;
    p->parent = v;

    v->right = p;
    v->parent = g;

    if (g != NULL)
    {
        if (is_left)
            g->left = v;
        else
            g->right = v;
    }
}

/**
 * Insert val into rbtree node root, return the root of that tree after being modified.
 */
static RBTNode *rbtnode_insert(int val, RBTNode *root)
{
    if (root == NULL)
    {
        return rbtnode_new1(val, kBlack);
    }

    /* Insert new red node to root */
    RBTNode *v = NULL;
    RBTNode *node = root;
    while (node->value != val)
    {
        if (val < node->value)
        {
            if (node->left == NULL)
            {
                node->left = rbtnode_new2(val, kRed, node);
            }
            node = node->left;
        }
        else
        {
            if (node->right == NULL)
            {
                node->right = rbtnode_new2(val, kRed, node);
            }
            node = node->right;
        }
    }
    v = node;

    /* Rebalace after insertion */
    int need_rebalance = 1;

    RBTNode *p = NULL;
    RBTNode *s = NULL;
    RBTNode *g = NULL;
    RBTNode *gp = NULL;
    while (need_rebalance)
    {
        p = parent(v);
        s = is_left_child(v) ? right(p) : left(p);
        g = parent(p);
        gp = parent(g);

        if (is_red(s) && is_red(v))
        {
            flip_color2red(p);
            v = p;
        }
        else if (is_right_child(v) && is_red(v))
        {
            if (is_black(p))
            {
                rotate_left(v, p, g);
                v->color = kBlack;
                p->color = kRed;
                need_rebalance = 0;
            }
            else
            {
                rotate_left(v, p, g);
                rotate_right(v, g, gp);
                p->color = kBlack;
            }
        }
        else if (is_left_child(v) && is_red(v) && is_red(p))
        {
            rotate_right(p, g, gp);
            v->color = kBlack;
            v = p;
        }
        else
        {
            need_rebalance = 0;
        }
    }

    /* Get the root of rbtree */
    while (parent(v) != NULL)
    {
        v = v->parent;
    }
    if (is_red(v))
        v->color = kBlack;

    return v;
}

static RBTNode *min(RBTNode *root)
{
    while (root != NULL && root->left != NULL)
    {
        root = root->left;
    }

    return root;
}

static RBTNode *max(RBTNode *root)
{
    while (root != NULL && root->right != NULL)
    {
        root = root->right;
    }

    return root;
}

static RBTNode *rbtnode_remove(int val, RBTNode *root)
{
    if (root == NULL)
        return NULL;

    RBTNode *node = root;
    while (node != NULL && val != node->value)
    {
        if (val < node->value)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    if (node == NULL || val != node->value)
        return root;

    RBTNode *successor = min(node->right);
    if (successor == NULL)
    {
        successor = max(node->left);
    }

    RBTNode *v = NULL;
    RBTNode *s = NULL;
    RBTNode *j = NULL;
    RBTNode *p = NULL;
    RBTNode *g = NULL;
    RBTNode *gp = NULL;

    int need_rebalance = 1;
    int need_free_v = 1;

    if (successor == NULL)
    {
        v = node;
    }
    else if (left(successor) == NULL && right(successor) == NULL)
    {
        node->value = successor->value;
        v = successor;
    }
    else if (left(successor) == NULL)
    {
        node->value = successor->value;
        successor->value = right(successor)->value;
        v = right(successor);
    }
    else if (right(successor) == NULL)
    {
        node->value = successor->value;
        successor->value = left(successor)->value;
        v = left(successor);
    }

    if (is_red(v))
    {
        if (is_left_child(v))
            parent(v)->left = NULL;
        else if (is_right_child(v))
            parent(v)->right = NULL;

        if (v != NULL)
        {
            free(v);
        }
        return root;
    }

    int double_black = 1;

    while (need_rebalance)
    {
        p = parent(v);
        g = parent(p);
        gp = parent(g);
        s = is_left_child(v) ? right(p) : left(p);
        j = left(s);

        if (need_free_v)
        {
            need_free_v = 0;
            if (is_left_child(v))
                p->left = NULL;
            else if (is_right_child(v))
                p->right = NULL;
            if (v != NULL)
            {
                free(v);
                v = NULL;
            }
        }

        if (double_black)
        {

            if (p != NULL && is_black(p) && is_black(s) && is_black(j))
            { /* 2,6 */
                if (is_right_child(s))
                {
                    rotate_left(s, p, g);
                    p->color = kRed;
                    v = s;
                }
                else
                {
                    s->color = kRed;
                    v = p;
                }
            }
            else if (is_red(p) && is_black(s) && is_black(j))
            { /* 4,7 */
                if (is_right_child(s))
                {
                    rotate_left(s, p, g);
                    v = s;
                }
                else
                {
                    s->color = kRed;
                    p->color = kBlack;
                    v = p;
                }
                need_rebalance = 0;
            }
            else if (is_black(p) && is_black(s) && is_red(j))
            { /* 3,9 */
                if (is_right_child(s))
                {
                    rotate_right(j, s, p);
                    rotate_left(j, p, g);
                    j->color = kBlack;
                    v = j;
                }
                else
                {
                    rotate_right(s, p, g);
                    j->color = kBlack;
                    v = s;
                }
                need_rebalance = 0;
            }
            else if (is_black(p) && is_red(s))
            { /* 8 */
                rotate_right(s, p, g);
                left(p)->color = kRed;
                need_rebalance = 0;
            }
            else if (is_red(p) && is_red(j))
            { /* 5,10 */
                if (is_right_child(s))
                {
                    rotate_right(j, s, p);
                    rotate_left(j, p, g);
                    p->color = kBlack;
                    v = j;
                }
                else
                {
                    rotate_right(s, p, g);
                    s->color = kRed;
                    j->color = kBlack;
                    p->color = kBlack;
                    v = s;
                }
                double_black = 0;
            }
        }
        else
        {
            if (is_red(s) && is_red(v))
            {
                flip_color2red(p);
                v = p;
            }
            else if (is_right_child(v) && is_red(v))
            {
                if (is_black(p))
                {
                    rotate_left(v, p, g);
                    v->color = kBlack;
                    p->color = kRed;
                    need_rebalance = 0;
                }
                else
                {
                    rotate_left(v, p, g);
                    rotate_right(v, g, gp);
                    p->color = kBlack;
                }
            }
            else if (is_left_child(v) && is_red(v) && is_red(p))
            {
                rotate_right(p, g, gp);
                v->color = kBlack;
                v = p;
            }
            else
            {
                need_rebalance = 0;
            }
        }

        if (v == NULL || v->parent == NULL)
            need_rebalance = 0;
    }

    while (v != NULL && v->parent != NULL)
    {
        v = v->parent;
    }
    if (is_red(v))
        v->color = kBlack;

    return v;
}

static RBTNode *rbtnode_find(int val, RBTNode *root)
{
    if (root == NULL)
        return NULL;

    while (root != NULL && val != root->value)
    {
        if (val < root->value)
            root = root->left;
        else
            root = root->right;
    }

    return root;
}

struct RBTree *rbtree_new()
{
    struct RBTree *tree = malloc(sizeof(*tree));
    tree->root = NULL;
    return tree;
}

void rbtree_free(struct RBTree *tree)
{
    if (tree == NULL)
        return;

    RBTNode *ltree = left(tree->root);
    RBTNode *rtree = right(tree->root);

    if (ltree)
        ltree->parent = NULL;
    if (rtree)
        rtree->parent = NULL;

    RBTNode *node = min(ltree);
    while (node != NULL)
    {
        ltree = rbtnode_remove(node->value, ltree);
        node = min(ltree);
    }

    node = min(rtree);
    while (node != NULL)
    {
        rtree = rbtnode_remove(node->value, rtree);
        node = min(rtree);
    }

    if (tree->root != NULL)
        free(tree->root);
    free(tree);
}

void rbtree_insert(int val, struct RBTree *tree)
{
    if (tree == NULL)
        return;

    tree->root = rbtnode_insert(val, tree->root);
}

int rbtree_find(int val, struct RBTree *tree)
{
    RBTNode *node = rbtnode_find(val, tree->root);
    return node != NULL;
}

void rbtree_remove(int val, struct RBTree *tree)
{
    tree->root = rbtnode_remove(val, tree->root);
}

int rbtnode_check(RBTNode *root)
{
    if (root == NULL)
        return 0;

    RBTNode *l = left(root);
    RBTNode *r = right(root);

    if ((l != NULL && l->parent != root) || (r != NULL && r->parent != root))
        return 1;

    return rbtnode_check(l) || rbtnode_check(r);
}

int rbtree_check(RBTree *tree)
{
    return rbtnode_check(tree != NULL ? tree->root : NULL);
}