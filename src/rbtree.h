#ifndef RBTREE_H
#define RBTREE_H

struct RBTree;

struct RBTree *rbtree_new();
void rbtree_free(struct RBTree *tree);
void rbtree_insert(int val, struct RBTree *tree);
int rbtree_find(int val, struct RBTree *tree);
void rbtree_remove(int val, struct RBTree *tree);

#endif