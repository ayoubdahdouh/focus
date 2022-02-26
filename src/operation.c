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

int operation_menu()
{
    int n, code;

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
        n = read_line(buffer, BUFFER_SIZE);
        if (n == 0)
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

    // read details
    printf("details: ");
    n = read_line(buffer, DETAILS_SIZE);
    if (n > 0)
    {
        tsk->details = (char *)alloc_check((n + 1) * sizeof(char));
        strncpy(tsk->details, buffer, n + 1);
    }

    // read priority
    printf("priority from 1 to 5 (1 by default): ");
    n = read_line(buffer, BUFFER_SIZE);
    if (n == 0)
    {
        tsk->priority = 1;
    }
    else
    {
        tsk->priority = atoi(buffer);
        if (tsk->priority < 1)
        {
            tsk->priority = 1;
            printf("the priority is set to the minimum value 1\n");
        }
        else if (tsk->priority > 5)
        {
            tsk->priority = 5;
            printf("the priority is set to the maximum value 5\n");
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
int task_attribute_menu()
{
    int n, code;

    do
    {
        printf("Choose an attribute to modify ?\n\n"
               "1- start time\n"
               "2- end time\n"
               "3- name\n"
               "4- details\n"
               "5- status\n"
               "6- priority\n"
               "7- <=\n\n"
               "(7 by default)>> ");
        n = read_line(buffer, BUFFER_SIZE);
        if (n == 0)
        {
            code = 7;
        }
        else
        {
            code = atoi(buffer);
        }
    } while (code < 1 && code > 7);
    return code;
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

void modify_task()
{
    int num;
    struct tm *now;
    task *tmp;

    now = localtime(&tasks_d);
    printf("Modify the task on %d/%d/%d.\n", now->tm_mday, now->tm_mon, now->tm_year);

    if (tasks_l->count <= 0)
    {
        printf("\n\tNo tasks on %d/%d/%d\n\n", now->tm_mday, now->tm_mon, now->tm_year);
    }
    else
    {
        print_tasks(tasks_l);
        num = choose_number("type in a task number: ");
        if (num != -1)
        {
            tmp = lget(tasks_l, num - 1);
            int code;

            code = task_attribute_menu();
            if (code == 1)
            {
                tmp->start = choose_time("start (HH:MM): ");
                if (!tmp->start)
                {
                    return;
                }
            }
            else if (code == 2)
            {
                tmp->end = choose_time("end (HH:MM): ");
                if (!tmp->end)
                {
                    return;
                }
            }
            else if (code == 3)
            {
                choose_text(&tmp->name, "name: ");
            }
            else if (code == 4)
            {
                choose_text(&tmp->details, "details: ");
            }
            else if (code == 5)
            {
                choose_status(&tmp->status);
            }
            else if (code == 6)
            {
                choose_priority(&tmp->priority);
            }
        }
    }
}
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