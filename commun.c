#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"

void *alloc_check(int size)
{
    void *ptr;

    ptr = malloc(size);
    if (!ptr)
    {
        error(0, "internal error");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int read_line(char str[], int n)
{
    int c = 0;
    int i = 0;

    while (i < n - 1 && (c = getchar()) != EOF && c != '\n')
    {
        str[i++] = c;
    }
    str[i] = '\0';

    // empty the buffer
    if (i == n - 1)
    {
        while (c != EOF && c != '\n')
        {
            c = getchar();
        }
    }
    return i;
}
