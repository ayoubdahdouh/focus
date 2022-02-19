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

int compare_date(struct tm *date1, struct tm *date2)
{
    int tmp;
    tmp = date1->tm_year > date2->tm_year;
    if (tmp)
    {
        return tmp;
    }
    else
    {
        tmp = date1->tm_mon > date2->tm_mon;
        if (tmp)
        {
            return tmp;
        }
        else
        {
            return date1->tm_mday > date2->tm_mday;
        }
    }
}

int compare_time(struct tm *date1, struct tm *date2)
{
    int tmp;
    tmp = date1->tm_hour > date2->tm_hour;
    if (tmp)
    {
        return tmp;
    }
    else
    {
        tmp = date1->tm_min > date2->tm_min;
        if (tmp)
        {
            return tmp;
        }
        else
        {
            return date1->tm_sec > date2->tm_sec;
        }
    }
}
