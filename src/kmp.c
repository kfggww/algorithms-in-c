#include <stdlib.h>
#include <string.h>

static int *build_next(const char *pattern) {
    size_t n = strlen(pattern);

    int *next = (int *)malloc(sizeof(int) * n);
    next[0] = 0;

    for (int i = 1; i < n; i++) {
        int j = next[i - 1];
        while (1) {
            if (pattern[j] == pattern[i]) {
                next[i] = next[i - 1] + 1;
                break;
            } else {
                if (j == 0) {
                    next[i] = 0;
                    break;
                }
                j = next[j - 1];
            }
        }
    }

    return next;
}

char *kmp_find(const char *s, const char *pattern) {
    if (s == NULL || pattern == NULL)
        return NULL;

    size_t ns = strlen(s);
    size_t np = strlen(pattern);
    if (ns < np)
        return NULL;

    int *next = build_next(pattern);
    int i = 0, j = 0;
    while (i < ns && j < np) {
        if (s[i] == pattern[j]) {
            i++;
            j++;
        } else {
            if (j == 0)
                i++;
            else
                j = next[j - 1];
        }
    }

    free(next);
    return (char *)(j < np ? NULL : (s + i - j));
}