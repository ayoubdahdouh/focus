#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "operations.h"
#include "../main.h"
#include "../src.h"
#include "../linklist.h"

time_t choose_time(const char message[])
{
    int hour, minute;
    struct tm when, *date_tm;
    time_t tmp = (time_t)-1;
    int ok = 3;

    date_tm = get_datetime_struct(week);
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

int choose_number(char const msg[], int min, int max)
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
            printf("false\n");
        }
        else if (num < min || num > max)
        {
            printf("false\n");
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

int search_time_in_list(int day, struct tm *date)
{
    int i = 0;
    struct tm *start;

    literator iter = lat(tasks_l[day], LFIRST);

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

int search_day(const char *day)
{
    int i;
    char *tab[] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};
    for (i = 0; i < 7; i++)
    {
        if (strcmp(tab[i], day) == 0)
        {
            break;
        }
    }
    return (i != 7) ? -1 : i + 1;
}
int choose_week_day()
{
    // read week-s day
    int day = -1;
    int ok = 3;
    while (ok)
    {
        printf("day name (monday-sunday): ");
        if (read_line(buffer, NAME_SIZE) == 0)
        {
            printf("false");
        }
        day = search_day(buffer) == -1;
        if (day == -1)
        {
            printf("false");
        }

        ok--;
    }
    return day;
}