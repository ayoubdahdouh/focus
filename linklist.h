#ifndef __LIST_H__
#define __LIST_H__

#include <limits.h>

typedef struct node
{
    void *data;
    struct node *next;
} * lnode, *literator;

typedef struct linklist
{
    int count; // number of elements in LIST
    lnode first;
    lnode last;
} * linklist;

#define LINKLISTSIZ sizeof(struct linklist)
#define NODESIZ sizeof(struct node)
#define ITERATORSIZ NODESIZ
#define LLAST -1
#define LFIRST -2

/*******************
 * list operations 
 *******************/

// create a new list
linklist lopen();
// delete the list
// NOTE: the function does not free the memory pointed by "data", it frees only the memory of the node.
void lclose(linklist l);

// Add the element "data" at the "at" position.
void ladd(linklist l, long int at, void *data);
// Delete the element at the "at" position.
// NOTE: the function does not free the memory pointed by "data", it frees only the memory of the node.
void ldel(linklist l, long int at);

// Get the element at the "at" position.
void *lget(linklist l, long int at);
// Modify the element in the "at" position.
void *lset(linklist l, long int at, void *data);

// check if list is empty
int lempty(linklist l);
// Reset the list by deleting all nodes in it.
// NOTE: the function does not free the memory pointed by "data", it frees only the memory of the node.
void lclear(linklist l);

// Sort the list using the QuickSort algorithm.
void lsort(linklist l, long int from, long int to, int selector(void *, void *));

// display the list
void lprint(linklist l);

/*******************
 * Iterator functions
 *******************/

// Create an iterator
literator lat(linklist l, int at);
// Increment by 1 the iterator "i".
void linc(literator *i);

#endif