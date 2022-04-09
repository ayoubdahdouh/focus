#include <stdio.h>
#include "../main.h"
#include "../src.h"
#include "input_output.h"

void print_week(linklist l[])
{
    char *days_of_the_week[N] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    for (int i = 0; i < N; i++)
    {
        printf("%10s :\n", days_of_the_week[i]);
        print_tasks(l[i]);
    }
}

void print_tasks(linklist l)
{
    const char COLOR_RED[] = "\e[0;31m", COLOR_GREEN[] = "\e[0;32m", COLOR_YELLOW[] = "\e[1;33m",
               COLOR_BLUE[] = "\e[0;34m", COLOR_NC[] = "\e[0m";
    struct tm *start, *end, *now;
    task *tmp_task;
    int tmp;
    int cnt = 1;

    if (!lempty(l))
    {
        now = get_datetime_struct(time(NULL));
        literator iter = lat(l, LFIRST);

        while (iter)
        {
            tmp_task = (task *)iter->data;
            start = get_datetime_struct(tmp_task->start);
            end = get_datetime_struct(tmp_task->end);

            // print day
            // if (!previous_date || compare_date(previous_date, start) != 0)
            // {
            //     printf("%d/%d/%d:\n", now->tm_mday, now->tm_mon, now->tm_year);
            //     previous_date = start;
            // }

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
    // else
    // {
    //     printf("\tempty\n");
    // }
}
