#include <stdio.h>
#include <string.h>
#include "operations.h"
#include "../tools.h"
#include "../print.h"

void rename_task(linklist tasks, int *has_changed)
{
    int id;
    task *tmp;

    if (tasks->count > 0)
    {
        id = choose_number("task number: ", 1, tasks->count);
        if (id != -1)
        {
            tmp = lget(tasks, id-1);
            choose_text(&tmp->title, "title: ");
            tmp->flag = TASK_MODIFIED;
            *has_changed = 1;
        }
    }
    else
    {
        printf("List is empty...");
    }
}