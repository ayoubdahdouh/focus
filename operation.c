#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "main.h"
#include "linklist.h"

#define NAME_SIZE 100
#define DETAILS_SIZE 1000
extern char buffer[];

extern linklist tasks_l;
extern time_t tasks_d;

void print_tasks(linklist l)
{
    int n = 1;
    const char COLOR_RED[] = "\e[0;31m", COLOR_GREEN[] = "\e[0;32m", COLOR_YELLOW[] = "\e[1;33m",
               COLOR_BLUE[] = "\e[0;34m", COLOR_NC[] = "\e[0m";
    struct tm *start, *end, *now, *previous_date = NULL;
    task *tmp_task;
    int tmp;
    time_t time_now;

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
                    printf("\t[ %sDONE%s ] %d:%d - %d:%d : %s", COLOR_GREEN, COLOR_NC,
                           start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
                else
                {
                    printf("\t[ %sNOT DONE%s ] %d:%d - %d:%d : %s", COLOR_RED, COLOR_NC,
                           start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
            }
            else if (tmp == 0)
            {
                if (compare_time(start, now) >= 0 && compare_time(end, now) <= 0)
                {
                    printf("\t[ %sNOW%s ] %d:%d - %d:%d : %s", COLOR_BLUE, COLOR_NC,
                           start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
                else
                {
                    printf("\t[ NOT YET ] %d:%d - %d:%d : %s",
                           start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
                }
            }
            else
            {
                printf("[ NOT YET ] %d:%d - %d:%d : %s",
                       start->tm_hour, start->tm_min, end->tm_hour, end->tm_min, tmp_task->name);
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

    // system("clear");
    do
    {
        // print_tasks(tasks_l);
        // code
        printf("Choose an operation ?\n\n"
               "1- add\n"
               "2- remove\n"
               "3- modify\n"
               "4- copy\n\n"
               "5- save modification\n"
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
    int priority;
    task *tsk;

    tsk = (task *)alloc_check(sizeof(task));

    tsk->name = alloc_check(NAME_SIZE * sizeof(char));
    tsk->details = alloc_check(DETAILS_SIZE * sizeof(char));

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
        printf("task name*: ");
        read_line(tsk->name, NAME_SIZE);
    } while (strlen(tsk->name) == 0);

    // read details
    printf("task details: ");
    read_line(tsk->details, DETAILS_SIZE);

    // read priority
    printf("task priority between 1-3 (1 by default): ");
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

void add_task()
{
    task *tsk;
    struct tm *now, *start;
    int tmp;

    tsk = new_task();

    // now = localtime(&tasks_d);

    // literator iter = lat(tasks_l, LFIRST);
    // while (iter)
    // {
    //     start = localtime(&((task*)iter->data)->start);
    //     tmp=compare_date(now,start);

    //     if (tmp>0)
    //     {
    //         break;
    //     }
    //     else if (tmp==0)
    //     {
    //         /* code */
    //     }
        
        
    // }

    // ladd(tasks_l, )
}

void remove_task() {}
void modify_task() {}
void copy_task() {}
void clone_task() {}
