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

void read_line(char str[], int n)
{
    int c;
    int i = 0;

    while (i < n && (c = getchar()) != EOF && c != '\n')
    {
        str[i++] = c;
    }
    if (i == n)
    {
        str[i - 1] = '\0';
        // empty the buffer
        while (c != EOF && c != '\n')
        {
            c = getchar();
        }
        return;
    }

    if (c == EOF || c == '\n')
    {
        str[i] = '\0';
    }
}
