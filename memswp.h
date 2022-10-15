#include <string.h>

#ifndef MEMSWP_H
#define MEMSWP_H
static void swap_byte(void* a, void* b, long count)
{
    char* x = (char*) a;
    char* y = (char*) b;

    while (count--) {
        char t = *x; *x = *y; *y = t;
        x += 1;
        y += 1;
    }
}

static void swap_word(void* a, void* b, long count)
{
    char* x = (char*) a;
    char* y = (char*) b;
    long t[1];

    while (count--) {
        memcpy(t, x, sizeof(long));
        memcpy(x, y, sizeof(long));
        memcpy(y, t, sizeof(long));
        x += sizeof(long);
        y += sizeof(long);
    }
}

void memswp(void* a, void* b, long size)
{
    long words = size / sizeof(long);
    long bytes = size % sizeof(long);
    swap_word(a, b, words);
    a = (char*) a + words * sizeof(long);
    b = (char*) b + words * sizeof(long);
    swap_byte(a, b, bytes);
}

#endif // MEMSWP_H
