#include <stdio.h>
#include <string.h>
#include "operations.h"
#include "../src.h"

static int day;
void modify_task()
{
    int n;
    struct tm *now;
    task *tmp;
    const char attribute_msg[] = "Choose an attribute to modify ?\n\n"
                                 "1- start time\n"
                                 "2- end time\n"
                                 "3- name\n"
                                 "4- status\n"
                                 "5- <=\n\n"
                                 "(5 by default)>> ";

    now = get_datetime_struct(week);
    printf("Modify the task on %d/%d/%d.\n", now->tm_mday, now->tm_mon, now->tm_year);

    day = choose_week_day();

    print_week(tasks_l);
    if (tasks_l[day]->count <= 0)
    {
        n = choose_number("task number: ", 1, tasks_l[day]->count);
        if (n != -1)
        {
            tmp = lget(tasks_l[day], n - 1);

            n = choose_from_menu(attribute_msg, 1, 5, 5);
            if (n == 1)
            {
                tmp->start = choose_time("start (HH:MM): ");
                if (!tmp->start)
                {
                    return;
                }
                tmp->flag = 2;
                is_tasks_l_changed = 1;
            }
            else if (n == 2)
            {
                tmp->end = choose_time("end (HH:MM): ");
                if (!tmp->end)
                {
                    return;
                }
                tmp->flag = 2;
                is_tasks_l_changed = 1;
            }
            else if (n == 3)
            {
                choose_text(&tmp->name, "title: ");
                tmp->flag = 2;
                is_tasks_l_changed = 1;
            }
            else if (n == 4)
            {
                choose_status(&tmp->status);
                tmp->flag = 2;
                is_tasks_l_changed = 1;
            }
        }
    }
}