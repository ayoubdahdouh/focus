#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "main.h"
#include "linklist.h"

#define NAME_SIZE 256

extern int is_tasks_l_changed;
extern char buffer[];
extern linklist tasks_l;
extern time_t tasks_d;

void print_tasks(linklist l)
{
    const char COLOR_RED[] = "\e[0;31m", COLOR_GREEN[] = "\e[0;32m", COLOR_YELLOW[] = "\e[1;33m",
               COLOR_BLUE[] = "\e[0;34m", COLOR_NC[] = "\e[0m";
    struct tm *start, *end, *now, *previous_date = NULL;
    task *tmp_task;
    int tmp;
    int cnt = 1;

    if (!lempty(l))
    {
        now = get_datetime_struct(time(NULL));
        literator iter = lat(l, 0);

        while (iter)
        {
            tmp_task = (task *)iter->data;
            start = get_datetime_struct(tmp_task->start);
            end = get_datetime_struct(tmp_task->end);

            // print day
            if (!previous_date || compare_date(previous_date, start) != 0)
            {
                printf("%d/%d/%d:\n", now->tm_mday, now->tm_mon, now->tm_year);
                previous_date = start;
            }

            // print tasks
            tmp = compare_date(start, now);
            if (tmp < 0)
            {
                if (tmp_task->status)
                {
                    printf("\t%d- [ %sDONE%s ] %02d:%02d - %d:%02d : %s\n", cnt++, COLOR_GREEN, COLOR_NC,
                           start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
                else
                {
                    printf("\t%d- [ %sNOT DONE%s ] %02d:%02d - %d:%02d : %s\n", cnt++, COLOR_RED, COLOR_NC,
                           start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
            }
            else if (tmp == 0)
            {
                if (compare_time(start, now) <= 0)
                {
                    if (compare_time(end, now) >= 0)
                    {
                        printf("\t%d- [ %sNOW%s ] %02d:%02d - %02d:%02d : %s\n", cnt++, COLOR_BLUE, COLOR_NC,
                               start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                    }
                    else
                    {
                        if (tmp_task->status)
                        {
                            printf("\t%d- [ %sDONE%s ] %02d:%02d - %d:%02d : %s\n", cnt++, COLOR_GREEN, COLOR_NC,
                                   start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                        }
                        else
                        {
                            printf("\t%d- [ %sNOT DONE%s ] %02d:%02d - %d:%02d : %s\n", cnt++, COLOR_RED, COLOR_NC,
                                   start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                        }
                    }
                }
                else
                {
                    printf("\t%d- [ %sNOT YET%s ] %02d:%02d - %02d:%02d : %s\n", cnt++, COLOR_YELLOW, COLOR_NC,
                           start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
            }
            else
            {
                printf("\t%d- [ %sNOT YET%s ] %02d:%02d - %02d:%02d : %s\n", cnt++, COLOR_YELLOW, COLOR_NC,
                       start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
            }
            linc(&iter);
        }
    }
    else
    {
        printf("\n\tNo tasks!\n");
    }
    printf("\n");
}

time_t choose_time(const char message[])
{
    int hour, minute;
    struct tm when, *date_tm;
    time_t tmp = (time_t)-1;
    int ok = 3;

    date_tm = get_datetime_struct(tasks_d);
    when.tm_year = date_tm->tm_year;
    when.tm_mon = date_tm->tm_mon;
    when.tm_mday = date_tm->tm_mday;
    when.tm_min = 0;
    when.tm_sec = 0;
    while (ok--)
    {
        printf("%s", message);
        fgets(buffer, BUFFER_SIZE, stdin);

        if (!strchr(buffer, ':'))
        {
            if (sscanf(buffer, "%d", &hour) == 1 &&
                hour >= 0 && hour <= 23)
            {
                when.tm_hour = hour;
                tmp = get_datetime(&when);
                ok = 0;
            }
            else
            {
                printf("incorrect time format\n");
            }
        }
        else
        {
            if (sscanf(buffer, "%d:%d", &hour, &minute) == 2 &&
                hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59)
            {
                when.tm_hour = hour;
                when.tm_min = minute;
                tmp = get_datetime(&when);
                ok = 0;
            }
            else
            {
                printf("incorrect time format\n");
            }
        }
    }
    return tmp;
}

task *new_task()
{
    int n;
    task *tsk;

    tsk = (task *)alloc_check(sizeof(task));

    printf("Add a new task to the day.\n\n");

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
        printf("name*: ");
        n = read_line(buffer, NAME_SIZE);
    } while (n == 0);
    tsk->name = (char *)alloc_check((n + 1) * sizeof(char));
    strncpy(tsk->name, buffer, n + 1);

    return tsk;
}
int search_time_in_list(struct tm *date)
{
    int i = 0;
    struct tm *start;

    literator iter = lat(tasks_l, LFIRST);

    while (iter)
    {
        start = get_datetime_struct(((task *)iter->data)->start);
        if (compare_time(date, start) >= 0)
        {
            break;
        }
        else
        {
            linc(&iter);
            i++;
        }
    }
    // date at the tail of the list
    if (i > 0 && !iter)
    {
        return i - 1;
    }
    else
    {
        return i;
    }
}

void add_task()
{
    int i;
    task *tsk;
    struct tm *start;

    tsk = new_task();
    start = get_datetime_struct(tsk->start);
    i = search_time_in_list(start);
    ladd(tasks_l, i, tsk);
    tsk->flag = 1;
    is_tasks_l_changed = 1;
}

void remove_task()
{
    int num;
    int ok = NUMBER_OF_TRY;

    if (tasks_l->count > 0)
    {
        print_tasks(tasks_l);
        printf("\ntype in a task number: ");
        while (ok && (scanf("%d", &num) != 1 || num <= 0 || num > tasks_l->count))
        {
            printf("incorrect task number, retry\n");
            ok--;
        }
        if (ok != 0)
        {
            ldel(tasks_l, num - 1);
            is_tasks_l_changed = 1;
        }
        else
        {
            printf("you have tried %d times\nabort.\n\n", NUMBER_OF_TRY);
        }
    }
}

// choose non null text
// int choose_att_name(task *tsk)
// {
//     int n;
//     int ok = NUMBER_OF_TRY;

//     while (ok)
//     {
//         ok--;
//         printf("name: ");
//         n = read_line(buffer, NAME_SIZE);
//         if (n > 0)
//         {
//             break;
//         }
//         else
//         {
//             printf("false\n");
//         }
//     }
//     if (ok)
//     {
//         tsk->name = (char *)alloc_check((n + 1) * sizeof(char));
//         strncpy(tsk->name, buffer, n + 1);
//         return 1;
//     }
//     else
//     {
//         return 0;
//     }
// }

void choose_text(char **dst, const char msg[])
{
    int n;

    if (dst)
    {
        printf("%s", msg);
        n = read_line(buffer, BUFFER_SIZE);
        if (n > 0)
        {
            *dst = (char *)alloc_check((n + 1) * sizeof(char));
            strncpy(*dst, buffer, n + 1);
        }
    }
}

void choose_priority(int *pr)
{
    int n;
    printf("priority from 1 to 5 (1 by default): ");
    n = read_line(buffer, BUFFER_SIZE);
    if (n == 0)
    {
        *pr = 1;
    }
    else
    {
        *pr = atoi(buffer);
        if (*pr < 1)
        {
            *pr = 1;
            printf("the priority is set to the minimum value 1\n");
        }
        else if (*pr > 5)
        {
            *pr = 5;
            printf("the priority is set to the maximum value 5\n");
        }
    }
}

int choose_status(int *st)
{
    int n;
    int ok = NUMBER_OF_TRY;

    while (ok)
    {
        ok--;
        printf("is the task done? (\"y\" for yes and \"n\" for no): ");
        n = read_line(buffer, BUFFER_SIZE);
        if (n > 0 && (buffer[0] == 'y' || buffer[0] == 'n'))
        {
            break;
        }
        else
        {
            printf("false\n");
        }
    }
    if (ok)
    {
        if (buffer[0] == 'y')
        {
            *st = 1;
        }
        else
        {
            *st = 0;
        }
        return 1;
    }
    else
    {
        printf("you have tried %d times\nabort.\n\n", NUMBER_OF_TRY);
        return 0;
    }
}

int choose_number(char const msg[])
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
            printf("incorrect input, please retry\n");
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

    now = get_datetime_struct(tasks_d);
    printf("Modify the task on %d/%d/%d.\n", now->tm_mday, now->tm_mon, now->tm_year);

    print_tasks(tasks_l);
    if (tasks_l->count <= 0)
    {
        n = choose_number("type in a task number: ");
        if (n != -1)
        {
            tmp = lget(tasks_l, n - 1);

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
                choose_text(&tmp->name, "name: ");
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
void copy_task() {}
void clone_task() {}
void save_modification()
{
    // ask to save change
    if (is_tasks_l_changed)
    {
        write_all_date_tasks(tasks_l);
        is_tasks_l_changed = 0;
        return;
    }
}

void exit_opera()
{
    int ok = 3;

    if (!is_tasks_l_changed)
    {
        return;
    }
    while (ok--)
    {

        // ask to save change
        printf("Do you want to save the changes ? [y/n] ");
        read_line(buffer, BUFFER_SIZE);
        if (buffer[0] != 'y' && buffer[0] != 'n')
        {
            printf("please select \"y\" for yes and \"n\" for no.\n\n");
        }
        else
        {
            if (buffer[0] == 'y')
            {
                save_modification();
                printf("changes saved.\n");
            }
            else
            {
                printf("abort.\n");
            }
            lclear(tasks_l);
            ok = 0;
        }
    }
}