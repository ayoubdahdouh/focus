#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "main.h"
#include "linklist.h"

#define PATH "data_tasks.bin"

char buffer[BUFFER_SIZE];

int is_tasks_l_changed = 0;
linklist tasks_l;
time_t tasks_d;

void set_task(int num)
{
}

int manage()
{
    int code;

    tasks_l = lopen();

    code = main_menu();
    if (code == 1 || code == 2)
    {
        if (code == 2)
        {
            tasks_d = choose_date();
            if (tasks_d == (time_t)-1) // failed to choose date
            {
                return 1;
            }
        }
        else
        {
            time(&tasks_d);
        }
        // code
        // read_tasks_by_date(tasks_l, SAMEDAY);

        // lire tasks of today
        code = operation_menu();
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
            // ask to save change
            if (is_tasks_l_changed)
            {
                int ok = 3;
                do
                {
                    // system("clear");
                    printf("Do you want to save the changes ? [y/n] ");
                    read_line(buffer, BUFFER_SIZE);
                    if (strcmp(buffer, "y") && strcmp(buffer, "n") && ok > 1)
                    {
                        printf("please select \"y\" for yes and \"n\" for no.\n\n");
                    }
                    else
                    {
                        // code
                        // if yes save, if not discard
                        break;
                    }
                } while (--ok);

                if (ok == 0)
                {
                    printf("discard the changes\n");
                    lclear(tasks_l);
                }
            }
            break;
        }
    }
    else if (code == 3)
    {
        show_upcoming();
    }
    else if (code == 4)
    {
        show_history();
    }
    else
    {

        return 0;
    }

    lclear(tasks_l);
    return 1;
}

int main_menu()
{
    int code;
    // system("clear");
    do
    {
        printf("******** WELCOME ********\n"
               "1- today\n"
               "2- choose a day\n\n"
               "3- show upcoming\n"
               "4- show history\n"
               "5- Exit\n\n"
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
    } while (code < 1 && code > 5);
    return code;
}

void show_upcoming() {}
void show_history() {}

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
