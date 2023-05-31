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
    if (v == NULL || p == NULL || p->right != v || p->parent != g)
        return;

    int is_left = is_left_child(p);

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
        gp = parent(gp);

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

struct RBTree *rbtree_new()
{
    struct RBTree *tree = malloc(sizeof(*tree));
    tree->root = NULL;
    return tree;
}

void rbtree_free(struct RBTree *tree)
{
}

void rbtree_insert(int val, struct RBTree *tree)
{
    if (tree == NULL)
        return;

    tree->root = rbtnode_insert(val, tree->root);
}

int rbtree_find(int val, struct RBTree *tree)
{
}

void rbtree_remove(int val, struct RBTree *tree)
{
}