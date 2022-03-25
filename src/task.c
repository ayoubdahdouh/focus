#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "main.h"
#include "src.h"

#define PATH "data_tasks.bin"
char buffer[BUFFER_SIZE];

int is_tasks_l_changed = 0;
linklist tasks_l[N]; // 7 day of the wekk
time_t week;

void set_task(int num)
{
}

int manage()
{
    time_t time_tmp;
    int code, prev_week;
    const char menu[] = "1- add\n"
                        "2- remove\n"
                        "3- modify\n"
                        "4- copy\n"
                        "5- save\n"
                        "6- choose another week\n"
                        "7- exit\n\n"
                        "(1 by default)>> ";
    printf("\t  _            _       _ \n"
           "\t | |_ ___   __| | ___ | |\n"
           "\t | __/ _ \\ / _` |/ _ \\| |\n"
           "\t | || (_) | (_| | (_) | |\n"
           "\t  \\__\\___/ \\__,_|\\___/|_|\n");
    // printf("**********************************\n");

    for (int i = 0; i < N; i++)
    {
        tasks_l[i] = lopen();
    }

    // tasks_l = lopen();
    is_tasks_l_changed = 0;

    prev_week = week = seek_monday(time(NULL));

    read_all_date_tasks(tasks_l, week, SAME_WEEK);

    while (code != 7)
    {
        printf("--------------------------------------------\n");
        print_week(tasks_l);
        printf("--------------------------------------------\n");
        code = choose_from_menu(menu, 1, 7, 1);
        switch (code)
        {
        case 1:
            add_task();
            break;
        case 2:
            remove_task();
            break;
        case 3:
            modify_task();
            break;
        case 4:
            copy_task();
            break;
        case 5:
            save_modification();
            break;
        case 6:
            // choose a day
            time_tmp = choose_date();
            if (time_tmp == (time_t)-1) // failed to choose date
            {
                continue;
            }
            week = seek_monday(time_tmp);
            if (prev_week != week)
            {
                prev_week = week;
                /* code: save change if exist and empty list and free memory */
                read_all_date_tasks(tasks_l, week, SAME_WEEK);
                continue;
            }
            break;
        default:
            exit_program();
            break;
        }
    }

    return 1;
}

int leap_year(int year)
{
    return (((year % 4 == 0) &&
             (year % 100 != 0)) ||
            (year % 400 == 0));
}

int check_date(int day, int month, int year)
{
    const int MAX_YEAR = 9999, MIN_YEAR = 1000;

    // check range of year,month and day
    if (year > MAX_YEAR || year < MIN_YEAR)
        return 0;
    if (month < 1 || month > 12)
        return 0;
    if (day < 1 || day > 31)
        return 0;
    // Handle feb days in leap year
    if (month == 2)
    {
        if (leap_year(year))
            return (day <= 29);
        else
            return (day <= 28);
    }
    // handle months which has only 30 days
    if (month == 4 || month == 6 ||
        month == 9 || month == 11)
        return (day <= 30);
    return 1;
}

time_t choose_date()
{
    int day, month, year;
    struct tm when;
    time_t tmp = (time_t)-1;
    int ok = 3;

    while (ok--)
    {
        printf("Enter date (MM/DD/YYYY): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // quit if empty (Escape)
        //  if (strlen(buffer) == 0)
        //  {
        //      ok = 0;
        //  }

        if (sscanf(buffer, "%2d/%2d/%4d", &day, &month, &year) == 3 && check_date(day, month, year))
        {
            when.tm_mday = day;
            when.tm_mon = month - 1;
            when.tm_year = year - 1900;
            if ((tmp = mktime(&when)) != (time_t)-1)
            {
                ok = 0;
            }
            else
            {
                printf("invalid date\n");
            }
        }
        else
        {
            printf("incorrect date format\n");
        }
    }
    return tmp;
}

// the start of week is monday
time_t seek_monday(time_t t)
{
    struct tm *tmp;

    tmp = get_datetime_struct(t);
    // monday
    if (tmp->tm_wday == 1)
    {
        return t;
    }
    else
    {
        if (tmp->tm_mday >= tmp->tm_wday)
        {
            tmp->tm_mday -= tmp->tm_wday - 1;
        }
        else
        {
            int dif = tmp->tm_wday - tmp->tm_mday - 1;
            if (tmp->tm_mon == 1)
            {
                tmp->tm_year--;
                tmp->tm_mon = 12;
                tmp->tm_mday = 31 - dif;
            }
            else
            {
                tmp->tm_mon--;
                tmp->tm_mday = month_days(tmp->tm_mon, tmp->tm_year) - dif;
            }
        }
    }
    tmp->tm_wday = 1;

    return get_datetime(tmp);
}

int month_days(int month, int year)
{
    if (month < 1 || month > 12)
    {
        return 0;
    }
    // Check for 31 Days
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        return 31;
    }
    // Check for 30 Days
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return 30;
    }
    // if (month == 2) : Check for 28/29 Days
    else
    {
        if (leap_year(year))
        {
            return 29;
        }
        else
        {
            return 28;
        }
    }
}
