#include <stdio.h>
#include <string.h>
#include "operations.h"
#include "../main.h"
#include "../src.h"


void add_task()
{
    int i;
    task *tsk;
    struct tm *start;
    int day;
    tsk = new_task(&day);
    if (tsk)
    {
        start = get_datetime_struct(tsk->start);
        i = search_time_in_list(day,start);
        ladd(tasks_l[day], i, tsk);
        tsk->flag = 1;
        is_tasks_l_changed = 1;
    }
}

task *new_task(int *day)
{
    int n;
    task *tsk;

    tsk = (task *)alloc_check(sizeof(task));

    printf("Add a new task to the week.\n\n");

    *day = choose_week_day();
    if (*day == -1)
    {
        return NULL;
    }

    // start time
    tsk->start = choose_time("start (HH:MM): ");
    if (!tsk->start)
    {
        return NULL;
    }

    // end time
    tsk->end = choose_time("end (HH:MM): ");
    if (!tsk->end)
    {
        return NULL;
    }

    // read name
    do
    {
        printf("title: ");
        n = read_line(buffer, NAME_SIZE);
    } while (n == 0);
    tsk->name = (char *)alloc_check((n + 1) * sizeof(char));
    strncpy(tsk->name, buffer, n + 1);

    return tsk;
}