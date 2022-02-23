#include <time.h>
#include <string.h>
#include "main.h"

time_t get_datetime(struct tm *timer)
{
    if (timer)
    {
        timer->tm_year -= 1900;
        timer->tm_mon -= 1;
        return mktime(timer);
    }
    else
    {
        return time(NULL);
    }
}

struct tm *get_datetime_struct(time_t timer)
{
    struct tm *tmp;

    tmp = (struct tm *)alloc_check(sizeof(struct tm));
    if (timer <= 0)
    {
        time(&timer);
    }
    memcpy(tmp, localtime(&timer), sizeof(struct tm));
    tmp->tm_year += 1900;
    tmp->tm_mon += 1;

    return tmp;
}

int compare_date(struct tm *date1, struct tm *date2)
{
    int tmp;
    tmp = date1->tm_year - date2->tm_year;
    if (tmp)
    {
        return tmp;
    }
    else
    {
        tmp = date1->tm_mon - date2->tm_mon;
        if (tmp)
        {
            return tmp;
        }
        else
        {
            return date1->tm_mday - date2->tm_mday;
        }
    }
}

int compare_time(struct tm *date1, struct tm *date2)
{
    int tmp;
    tmp = date1->tm_hour - date2->tm_hour;
    if (tmp != 0)
    {
        return tmp;
    }
    else
    {
        tmp = date1->tm_min - date2->tm_min;
        if (tmp)
        {
            return tmp;
        }
        else
        {
            return date1->tm_sec - date2->tm_sec;
        }
    }
}
