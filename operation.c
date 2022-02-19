#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "linklist.h"

#define NAME_SIZE 100
#define DETAILS_SIZE 1000
extern char buffer[];
extern const int buffer_size;

extern linklist tasks_l;
extern time_t tasks_d;

void print_tasks(linklist l)
{
    int n = 1;
    const char COLOR_RED[] = '\e[0;31m', COLOR_GREEN[] = '\e[0;32m', COLOR_YELLOW[] = '\e[1;33m', COLOR_BLUE[] = '\e[0;34m', COLOR_NC[] = '\e[0m';
    struct tm *start, *end, *now, *previous_date = NULL;
    task *tmp_task;
    int tmp;

    if (!lempty(l))
    {
        now = localtime(time(NULL));
        literator iter = lat(l, 0);

        while (iter)
        {
            tmp_task = (task *)iter->data;
            start = mktime(&tmp_task->start);
            end = mktime(&tmp_task->end);

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
                    printf("\t[ %sDONE%s ] %d:%d - %d:%d : %s", COLOR_GREEN, COLOR_NC, start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
                else
                {
                    printf("\t[ %sNOT DONE%s ] %d:%d - %d:%d : %s", COLOR_RED, COLOR_NC, start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
            }
            else if (tmp == 0)
            {
                if (compare_time(start, now) >= 0 && compare_time(end, now) <= 0)
                {
                    printf("\t[ %sNOW%s ] %d:%d - %d:%d : %s", COLOR_BLUE, COLOR_NC, start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
                else
                {
                    printf("\t[ NOT YET ] %d:%d - %d:%d : %s", start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
            }
            else
            {
                printf("[ NOT YET ] %d:%d - %d:%d : %s", start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
            }
            linc(&iter);
        }
    }
    else
    {
        printf("\n\tNo tasks!\n\n");
    }
}

int operation_menu()
{

    int code;

    system("clear");
    do
    {
        print_tasks(tasks_l);
        // code
        printf("Choose an operation ?\n\n"
               "1- add\n"
               "2- remove\n"
               "3- modify\n"
               "4- copy\n\n"
               "6- save modification\n\n"
               "7- exit\n\n"
               "(1 by default)>> ");
        read_line(buffer, buffer_size);
        if (strlen(buffer) == 0)
        {
            code = 1;
        }
        else
        {
            code = atoi(buffer);
        }
    } while (code < 1 && code > 7);
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
        printf(message);
        fgets(buffer, buffer_size, stdin);

        // quit if empty (Escape)
        //  if (strlen(buffer) == 0)
        //  {
        //      ok = 0;
        //  }

        if (sscanf(buffer, "%2d;%2d", &hour, &minute) == 3 &&
            hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59)
        {
            when.tm_year = date_tm->tm_year;
            when.tm_mon = date_tm->tm_mon;
            when.tm_mday = date_tm->tm_mday;
            when.tm_hour = minute;
            when.tm_min = hour;
            when.tm_sec = 0;
            if ((tmp = mktime(&when)) != (time_t)-1)
            {
                ok = 0;
            }
            else
            {
                printf("invalid time\n");
            }
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
    int priority;
    task *tsk;

    tsk = (task *)alloc_check(sizeof(task));

    tsk->name = alloc_check(NAME_SIZE * sizeof(char));
    tsk->details = alloc_check(DETAILS_SIZE * sizeof(char));

    printf("Add a new task to the day.\n\n");

    // read name
    do
    {
        printf("task name: ");
        read_line(tsk->name, NAME_SIZE);
    } while (strlen(tsk->name) == 0);
    strcpy(tsk->name, buffer);

    // read details
    printf("task details: ");
    read_line(tsk->details, DETAILS_SIZE);

    // read priority
    printf("task priority between 1-3 (1 by default): ");
    read_line(buffer, buffer_size);
    tsk->priority = atoi(buffer);
    if (tsk->priority < 1 && tsk->priority > 3)
    {
        tsk->priority = 1;
    }
    return tsk;
}

void add_task()
{
    task *tsk;

    tsk = new_task();

    // add task to the list
}

void remove_task() {}
void modify_task() {}
void copy_task() {}
void clone_task() {}
