#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"
#include "global.h"
#include "main.h"

extern char buffer[];

void *alloc_mem(int size)
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

void free_mem(void *ptr)
{
    if (!ptr)
    {
        free(ptr);
    }
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

int choose_number(char const msg[], int min, int max)
{
    int num;
    int ok = NUMBER_OF_TRY;

    while (ok)
    {
        ok--;
        printf("\n%s", msg);
        read_line(buffer, BUFFER_SIZE);
        if (sscanf(buffer, "%d", &num) != 1)
        {
            printf("false\n");
        }
        else if (num < min || num > max)
        {
            printf("false\n");
        }
        else
        {
            break;
        }
    }
    if (ok == 0)
    {
        printf("\nyou have tried %d times. abort.\n\n", NUMBER_OF_TRY);
        return -1;
    }
    else
    {
        return num;
    }
}

void choose_text(char **dst, const char msg[])
{
    int n;

    if (dst)
    {
        printf("%s", msg);
        n = read_line(buffer, BUFFER_SIZE);
        if (n > 0)
        {
            *dst = (char *)alloc_mem((n + 1) * sizeof(char));
            strncpy(*dst, buffer, n + 1);
        }
    }
}
