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
