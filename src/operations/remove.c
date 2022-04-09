#include <stdio.h>
#include <string.h>
#include "operations.h"

void remove_task()
{
    int num;

    int day = choose_week_day();
    if (tasks_l[day]->count > 0)
    {
        print_tasks(tasks_l[day]);
        num = choose_number("task number: ", 1, tasks_l[day]->count);
        if (num != -1)
        {
            ldel(tasks_l[day], num - 1);
            is_tasks_l_changed = 1;
        }
        else
        {
            printf("Abort.\n\n");
        }
    }
}
