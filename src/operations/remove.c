#include <stdio.h>
#include <string.h>
#include "operations.h"

static int day;
void remove_task()
{
    int num;

    day = choose_week_day();
    if (tasks_l[day]->count > 0)
    {
        print_week(tasks_l);
        num = choose_number("task number: ", 1, tasks_l[day]->count);
        if (num != -1)
        {
            ldel(tasks_l[day], num - 1);
            is_tasks_l_changed = 1;
        }
        else
        {
            printf("you have tried %d times\nabort.\n\n", NUMBER_OF_TRY);
        }
    }
}
