#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rbtree.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    struct RBTree *tree = rbtree_new();
    int v = 0;

    for (v = 1; v <= 10; v++)
    {
        rbtree_insert(v, tree);
        if (rbtree_check(tree))
        {
            printf("%d\n", v);
        }
    }

    for (v = 1; v <= 10; v++)
    {
        rbtree_remove(v, tree);
        if (rbtree_check(tree))
        {
            printf("%d\n", v);
        }
    }

    for (int i = 0; i < 100; i++)
    {
        v = rand() % 100;
        rbtree_insert(v, tree);
        if (rbtree_check(tree))
        {
            printf("%d", v);
        }
    }

    for (v = 0; v < 100; v++)
    {
        if (rbtree_find(v, tree))
            rbtree_remove(v, tree);
        if (rbtree_check(tree))
        {
            printf("%d", v);
        }
    }

    rbtree_free(tree);
    return 0;
}
