#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linklist.h"

void quicksort(linklist l, int from, int to, int compare(void *, void *));
int partition(linklist l, int from, int to, int compare(void *, void *));

void lerror(const char *msg)
{
    if (msg)
    {
        printf("%s\n", msg);
    }
}

linklist lopen()
{
    linklist l;

    l = (linklist)malloc(LINKLISTSIZ);
    if (!l)
    {
        lerror("[LOPEN] Memory allocation failed");
        return NULL;
    }
    l->count = 0;
    l->first = l->last = NULL;

    return l;
}

void lclose(linklist l)
{
    if (!l)
    {
        lerror("[LCLOSE] Memory allocation failed");
        return;
    }
    lnode x, y = l->first;
    while (l->count)
    {
        --l->count;
        x = y;
        y = y->next;
        free(x);
    }
    free(l);
}

int lempty(linklist l)
{
    if (l)
    {
        return (l->count == 0);
    }
    return 1;
}

void lclear(linklist l)
{
    if (!l)
    {
        lerror("[LRESET] No list is given");
        return;
    }
    while (!lempty(l))
    {
        ldel(l, LFIRST);
    }
}

void ladd(linklist l, long int at, void *data)
{
    lnode n, x, y;

    if (!l)
    {
        lerror("[LADD] No list is given");
        return;
    }
    if (!data)
    {
        lerror("[LADD] No data is given");
        return;
    }
    if (at < LFIRST || at > l->count)
    {
        lerror("[LADD] Out of bounds");
        return;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count;
    }
    n = (lnode)malloc(NODESIZ);
    n->data = data;
    n->next = NULL;

    if (l->count == 0)
    {
        l->first = n;
    }
    else if (l->count == 1)
    {
        if (at == 0)
        {
            n->next = l->first;
            l->last = l->first;
            l->first = n;
        }
        else
        {
            l->first->next = n;
            l->last = n;
        }
    }
    else
    {
        if (at == 0)
        {
            n->next = l->first;
            l->first = n;
        }
        else if (at == l->count)
        {
            l->last->next = n;
            l->last = n;
        }
        else
        {
            x = l->first;
            while (at)
            {
                --at;
                y = x;
                x = x->next;
            }
            n->next = x;
            y->next = n;
        }
    }
    l->count++;
}

void ldel(linklist l, long int at)
{
    lnode x, y;

    if (!l)
    {
        lerror("[LDEL] No list is given");
        return;
    }
    if (at < LFIRST || at >= l->count)
    {
        lerror("[LDEL] Out of bounds");
        return;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count - 1;
    }
    if (l->count == 0)
    { // if LAST's empty
        return;
    }
    else if (l->count == 1)
    {
        x = l->first;
        l->first = NULL;
    }
    else if (l->count == 2)
    {
        if (at == 0)
        {
            x = l->first;
            l->first = l->last;
            l->last = NULL;
        }
        else
        {
            x = l->last;
            l->first->next = NULL;
            l->last = NULL;
        }
    }
    else
    {
        if (at == 0)
        {
            x = l->first;
            l->first = x->next;
        }
        else
        {
            x = l->first;
            while (at)
            {
                --at;
                y = x;
                x = x->next;
            }
            y->next = x->next;
        }
    }
    free(x);
    --l->count;
}

void *lget(linklist l, long int at)
{
    lnode y;

    if (!l)
    {
        lerror("[LGET] No list is given");
        return NULL;
    }
    if (at < LFIRST || (at >= l->count))
    {
        lerror("[LGET] Out of bounds");
        return NULL;
    }
    if (lempty(l))
    {
        lerror("[LGET] The list is empty");
        return NULL;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count - 1;
    }
    y = l->first;
    while (at)
    {
        --at;
        y = y->next;
    }
    return y->data;
}

void *lset(linklist l, long int at, void *data)
{
    lnode n;
    void *tmp;

    if (!l)
    {
        lerror("[LSET] No list is given");
        return NULL;
    }
    if (at < LFIRST || (at >= l->count))
    {
        lerror("[LSET] Out of bounds");
        return NULL;
    }
    if (lempty(l))
    {
        lerror("[LSET] The list is empty");
        return NULL;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count - 1;
    }
    n = l->first;
    while (at)
    {
        --at;
        n = n->next;
    }
    tmp = n->data;
    n->data = data;
    return tmp;
}

void lsort(linklist l, long int from, long int to, int compare(void *, void *))
{
    if (!l)
    {
        return;
    }
    if ((from < LFIRST || (from >= l->count)) &&
        (to < LFIRST || (to >= l->count)))
    {
        lerror("[LSORT] Out of bounds");
        return;
    }
    if (from == LFIRST)
    {
        from = 0;
    }
    else if (from == LLAST)
    {
        from = l->count - 1;
    }
    if (to == LFIRST)
    {
        to = 0;
    }
    else if (to == LLAST)
    {
        to = l->count - 1;
    }
    if (from > to)
    {
        lerror("[LSORT] \"from\" must be less than \"to\"");
        return;
    }
    quicksort(l, from, to, compare);
}

int partition(linklist l, int from, int to, int compare(void *, void *))
{
    literator p = lat(l, to);
    literator j = lat(l, from);
    literator i = NULL;
    void *x;
    int k = from;

    while (to > from)
    {
        if (compare(j->data, p->data) < 0)
        {
            if (i)
            {
                linc(&i);
            }
            else
            {
                i = lat(l, from);
            }
            x = j->data;
            j->data = i->data;
            i->data = x;
            ++k;
        }
        linc(&j);
        --to;
    }
    if (i)
    {
        linc(&i);
    }
    else
    {
        i = lat(l, from);
    }
    x = p->data;
    p->data = i->data;
    i->data = x;
    return k;
}

void quicksort(linklist l, int from, int to, int compare(void *, void *))
{
    if (from < to)
    {
        int p = partition(l, from, to, compare);
        quicksort(l, from, p - 1, compare);
        quicksort(l, p + 1, to, compare);
    }
}

void linc(literator *i)
{
    if (i)
    {
        *i = (*i)->next;
    }
}

literator lat(linklist l, int at)
{
    literator i;

    if (!l)
    {
        lerror("[LAT] No list is given");
        return NULL;
    }
    if (at < LFIRST || (at >= l->count))
    {
        lerror("[LAT] Out of bounds");
        return NULL;
    }
    if (lempty(l))
    {
        lerror("[LAT] The list is empty");
        return NULL;
    }
    if (at == LFIRST)
    {
        at = 0;
    }
    else if (at == LLAST)
    {
        at = l->count - 1;
    }
    i = l->first;
    while (at)
    {
        --at;
        i = i->next;
    }
    return i;
}

void lprint(linklist l)
{
    if (!l)
    {
        lerror("[LSHOW]: The linklist is empty");
        return;
    }
    printf("%d nodes: ", l->count);
    for (int i = 0; i < l->count; i++)
    {
        printf("%d -> ", *(int *)lget(l, i));
    }
    printf("NULL\n\n");
}