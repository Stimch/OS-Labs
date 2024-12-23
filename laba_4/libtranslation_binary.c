#include <stdlib.h>
#include <stdio.h>

char* translation(long x) {
    char* result = (char*)malloc(32 * sizeof(char));
    sprintf(result, "%ld", x);
    return result;
}
