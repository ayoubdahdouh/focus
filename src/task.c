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
    int code, prev_date;
    const char menu[] = "choose an operation ?\n\n"
                        "1- choose another day\n"
                        "2- add\n"
                        "3- remove\n"
                        "4- modify\n"
                        "5- copy\n"
                        "6- save\n"
                        "7- show upcoming\n"
                        "8- show history\n"
                        "9- exit\n\n"
                        "(1 by default)>> ";

    printf("****************TODOL****************\n");
    tasks_l = lopen();
    is_tasks_l_changed = 0;

    prev_date = time(&tasks_d);
    read_all_date_tasks(tasks_l, tasks_d, SAMEDAY);

    while (code != 9)
    {
        printf("--------------------------------------------\n");
        print_tasks(tasks_l);
        code = choose_from_menu(menu, 1, 9, 1);
        switch (code)
        {
        case 2:
            add_task();
            break;
        case 3:
            remove_task();
            break;
        case 4:
            modify_task();
            break;
        case 5:
            copy_task();
            break;
        case 6:
            save_modification();
            break;
        case 7:
            show_upcoming();
            break;
        case 8:
            show_history();
            break;
        case 9:
            exit_program();
            break;
        default:
            // choose a day
            tasks_d = choose_date();
            if (tasks_d == (time_t)-1) // failed to choose date
            {
                return 0;
            }
            if (prev_date != tasks_d)
            {
                prev_date = tasks_d;
                /* code: save change if exist and empty list and free memory */
                read_all_date_tasks(tasks_l, tasks_d, SAMEDAY);
                continue;
            }
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
