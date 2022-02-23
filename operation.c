#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "main.h"
#include "linklist.h"

#define NUMBER_OF_TRY 3
#define NAME_SIZE 64
#define DETAILS_SIZE 256

extern int is_tasks_l_changed;
extern char buffer[];
extern linklist tasks_l;
extern time_t tasks_d;

void print_tasks(linklist l)
{
    const char COLOR_RED[] = "\e[0;31m", COLOR_GREEN[] = "\e[0;32m", /*COLOR_YELLOW[] = "\e[1;33m",*/
        COLOR_BLUE[] = "\e[0;34m", COLOR_NC[] = "\e[0m";
    struct tm *start, *end, *now, *previous_date = NULL;
    task *tmp_task;
    int tmp;
    time_t time_now;
    int cnt = 1;

    time(&time_now);

    if (!lempty(l))
    {
        now = localtime(&time_now);
        literator iter = lat(l, 0);

        while (iter)
        {
            tmp_task = (task *)iter->data;
            start = localtime(&tmp_task->start);
            end = localtime(&tmp_task->end);

            // print day
            if (!previous_date || compare_date(previous_date, start) != 0)
            {
                printf("%d/%d/%d:\n", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
                previous_date = start;
            }

            // print tasks
            tmp = compare_date(start, now);
            if (tmp < 0)
            {
                if (tmp_task->status)
                {
                    printf("\t%d- [ %sDONE%s ] %d:%d - %d:%d : %s\n", cnt++, COLOR_GREEN, COLOR_NC,
                           start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
                else
                {
                    printf("\t%d- [ %sNOT DONE%s ] %d:%d - %d:%d : %s\n", cnt++, COLOR_RED, COLOR_NC,
                                                                                 start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
            }
            else if (tmp == 0)
            {
                if (compare_time(start, now) >= 0 && compare_time(end, now) <= 0)
                {
                    printf("\t%d- [ %sNOW%s ] %d:%d - %d:%d : %s\n", cnt++, COLOR_BLUE, COLOR_NC,
                                                                            start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
                else
                {
                    printf("\t%d- [ NOT YET ] %d:%d - %d:%d : %s\n", cnt++,
                           start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
            }
            else
            {
                printf("%d- [ NOT YET ] %d:%d - %d:%d : %s\n", cnt++,
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

int operation_menu()
{

    int code;

    // system("clear");
    do
    {
        print_tasks(tasks_l);
        // code
        printf("Choose an operation ?\n\n"
               "1- add\n"
               "2- remove\n"
               "3- modify\n"
               "4- copy\n"
               "5- save\n"
               "6- exit\n\n"
               "(1 by default)>> ");
        read_line(buffer, BUFFER_SIZE);
        if (strlen(buffer) == 0)
        {
            code = 1;
        }
        else
        {
            code = atoi(buffer);
        }
    } while (code < 1 && code > 6);
    return code;
}

time_t choose_time(const char message[])
{
    int hour, minute;
    struct tm when, *date_tm;
    time_t tmp = (time_t)-1;
    int ok = 3;

    date_tm = localtime(&tasks_d);
    while (ok--)
    {
        printf("%s", message);
        fgets(buffer, BUFFER_SIZE, stdin);

        if (sscanf(buffer, "%d:%d", &hour, &minute) == 2 &&
            hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59)
        {
            when.tm_year = date_tm->tm_year;
            when.tm_mon = date_tm->tm_mon;
            when.tm_mday = date_tm->tm_mday;
            when.tm_hour = hour;
            when.tm_min = minute;
            when.tm_sec = 0;
            tmp = mktime(&when);
            ok = 0;
        }
        else
        {
            printf("incorrect time format\n");
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
        read_line(buffer, BUFFER_SIZE);
        n = strlen(buffer);
    } while (n == 0);
    if (n > NAME_SIZE)
    {
        n = NAME_SIZE - 1;
    }
    tsk->name = (char *)alloc_check(n * sizeof(char));
    strncpy(tsk->name, buffer, n);

    // read details
    printf("details: ");
    read_line(buffer, DETAILS_SIZE);
    if ((n = strlen(buffer)) > 0)
    {
        if (n > DETAILS_SIZE)
        {
            n = DETAILS_SIZE - 1;
        }

        tsk->details = (char *)alloc_check(n * sizeof(char));
        strncpy(tsk->details, buffer, n);
    }

    // read priority
    printf("priority from 1 to 5 (1 by default): ");
    read_line(buffer, BUFFER_SIZE);
    if (strlen(buffer) == 0)
    {
        tsk->priority = 1;
    }
    else
    {
        tsk->priority = atoi(buffer);
        if (tsk->priority < 1 && tsk->priority > 3)
        {
            tsk->priority = 1;
        }
    }

    return tsk;
}
int search_time_in_list(struct tm *date)
{
    int i = 0;
    struct tm *start;

    literator iter = lat(tasks_l, LFIRST);

    while (iter)
    {
        start = localtime(&((task *)iter->data)->start);
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
    start = localtime(&tsk->start);
    i = search_time_in_list(start);
    printf("\n\ni: %d\n\n---add\n", i);
    ladd(tasks_l, i, tsk);
}

void remove_task()
{
    int num;
    struct tm *now;
    int n, ok = NUMBER_OF_TRY;

    now = localtime(&tasks_d);

    n = tasks_l->count;
    if (n <= 0)
    {
        printf("\n\tNo tasks on %d/%d/%d\n\n", now->tm_mday, now->tm_mon, now->tm_year);
    }
    else
    {
        print_tasks(tasks_l);
        printf("\ntype in a task number: ");
        while (ok && (scanf("%d", &num) != 1 || num <= 0 || num > n))
        {
            printf("incorrect task number, retry\n");
            ok--;
        }
        if (ok != 0)
        {
            ldel(tasks_l, num - 1);
        }
        else
        {
            printf("you have tried %d times\nabort.\n\n", NUMBER_OF_TRY);
        }
    }
}

void modify_task() {}
void copy_task() {}
void clone_task() {}
void save_modification()
{
    if (!is_tasks_l_changed)
    {
        return;
    }
    write_all_date_tasks();
}