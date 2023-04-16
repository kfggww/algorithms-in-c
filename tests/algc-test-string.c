#include "algcstring.h"
#include <stdio.h>

int main(int argc, char **argv) {
    char *s = argv[1];
    char *pattern = argv[2];
    char *pos = kmp_find(s, pattern);
    printf("s: %s, pattern: %s, found: %s\n", s, pattern, pos);
    return 0;
}
