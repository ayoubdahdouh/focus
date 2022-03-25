#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "src.h"

extern char buffer[];

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

int choose_from_menu(const char msg[], int min, int max, int dft)
{
    int n;

    printf("%s", msg);
    n = read_line(buffer, BUFFER_SIZE);
    if (n > 0 && sscanf(buffer, "%d", &n) == 1)
    {
        return (n < min || n > max) ? dft : n;
    }
    return dft;
}

