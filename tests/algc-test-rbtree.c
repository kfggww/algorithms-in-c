#include <stdio.h>
#include "rbtree.h"

int main(int argc, char const *argv[])
{

    struct RBTree *tree = rbtree_new();

    for (int i = 1; i <= 3; i++)
    {
        rbtree_insert(i, tree);
    }

    return 0;
}
