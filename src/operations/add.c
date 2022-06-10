#include <stdio.h>
#include <string.h>
#include "operations.h"
#include "../tools.h"
#include "../print.h"

void add_task(linklist tasks, int *has_changed)
{
    int size = 0;
    task *t = (task *)alloc_mem(sizeof(task));
    task *tmp = (task *)lget(tasks, LLAST);
    // id

    t->id = tmp->id + 1;
    // flag
    t->flag = TASK_NEW;
    // name
    while (size == 0)
    {
        printf("title: ");
        size = read_line(buffer, NAME_SIZE);
    }
    t->title = (char *)alloc_mem((size + 1) * sizeof(char));
    strncpy(t->title, buffer, size + 1);

    ladd(tasks, LLAST, t);
    *has_changed = 1;
}