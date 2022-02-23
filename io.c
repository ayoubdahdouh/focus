#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "main.h"
#include "linklist.h"

FILE *stream;

extern time_t tasks_d;

void open_file_if_null()
{
    if (!stream)
    {
        if (!stream)
        {
            stream = fopen("data_tasks.bin", "ab+");
        }
        if (!stream)
        {
            error(0, "internal error");
            exit(EXIT_FAILURE);
        }
    }
}

void write_task(task *tsk)
{
    int size;

    size = task_len(tsk);

    // write length of tsk
    fwrite(&size, sizeof(int), 1, stream);

    // write star and end
    fwrite(&tsk->start, sizeof(time_t), 1, stream);
    fwrite(&tsk->end, sizeof(time_t), 1, stream);

    // write status
    fwrite(&tsk->status, sizeof(int), 1, stream);

    // write priority
    fwrite(&tsk->priority, sizeof(int), 1, stream);

    // write name
    size = strlen(tsk->name);
    fwrite(&size, sizeof(int), 1, stream);
    if (size > 0)
    {
        fwrite(tsk->name, size * sizeof(char), 1, stream);
    }

    // write details
    size = strlen(tsk->details);
    fwrite(&size, sizeof(int), 1, stream);
    if (size > 0)
    {
        fwrite(tsk->details, size * sizeof(char), 1, stream);
    }
}

int read_task(task *tsk)
{
    int size, len;

    if (!feof(stream))
    {
        fread(&size, sizeof(int), 1, stream);

        // read priority
        fread(&tsk->start, sizeof(time_t), 1, stream);
        fread(&tsk->end, sizeof(time_t), 1, stream);

        // read status
        fread(&tsk->status, sizeof(int), 1, stream);

        // read priority
        fread(&tsk->priority, sizeof(int), 1, stream);

        // read name
        fread(&len, sizeof(int), 1, stream);
        if (len > 0)
        {
            tsk->name = (char *)alloc_check(len * sizeof(char));
            fread(tsk->name, len * sizeof(char), 1, stream);
        }

        // read details
        fread(&len, sizeof(int), 1, stream);
        if (len > 0)
        {
            tsk->details = (char *)alloc_check(len * sizeof(char));
            fread(tsk->details, len * sizeof(char), 1, stream);
        }
        return 1;
    }

    return 0;
}

int task_len(task *tsk)
{
    int size = 0;

    size += sizeof(time_t) * 2;                  // start, end
    size += sizeof(int) * 2;                     // status, priority
    size += strlen(tsk->name) * sizeof(char);    // name
    size += strlen(tsk->details) * sizeof(char); // details

    return size;
}

void write_all_date_tasks(linklist l)
{
    int nb = 0;
    task *last;
    FILE *tp;
    struct tm *date_last;
    long pos;

    last = (task *)lget(l, LLAST);
    date_last = localtime(&last->start);
    date_last->tm_mday += 1;
    search_date(date_last);

    tp = tmpfile();

    pos = ftell(stream);

    while (!feof(stream))
    {
        fputc(fgetc(stream), tp);
        nb++;
    }
    fseek(stream, -1 * pos, SEEK_END);

    date_last->tm_mday -= 1;
    search_date(date_last);

    literator iter;
    iter = lat(l, LFIRST);
    while (iter)
    {
    }
}

// find the first task of date.
// if no task has found, set the cursor
// to the position where should be a tasks of this date
// file: ----- date< ----- date(cursor) ----- date> -----
int search_date(struct tm *date)
{
    int found = 0, size, tmp;
    time_t start;
    struct tm *start_tm;

    rewind(stream);

    while (!found && !feof(stream))
    {
        // read task length
        fread(&size, sizeof(int), 1, stream);

        // read start
        fread(&start, sizeof(time_t), 1, stream);

        start_tm = localtime(&start);

        tmp = compare_date(date, start_tm);
        if (tmp == 0)
        {
            found = 1;
        }
        else if (tmp > 0)
        {
            // go to next task
            fseek(stream, size - sizeof(int) - sizeof(time_t), SEEK_CUR);
        }
        else
        {
            // set the cursor to the position where
            // should be a tasks of this date
            fseek(stream, -sizeof(int) - sizeof(time_t), SEEK_CUR);
            break;
        }
    }
    return found;
}

// what = SAMEDAY: same to date
// what = FUTURE: in the future
// what = PAST: in the past
int read_all_date_tasks(linklist l, time_t date, direction what)
{
    struct tm *date_tm, *start_tm;
    task *tmp_task;

    date_tm = localtime(&tasks_d);

    // if found
    if (search_date(date_tm))
    {

        tmp_task = (task *)malloc(sizeof(task));
        if (what == SAMEDAY)
        {
            while (read_task(tmp_task))
            {
                start_tm = localtime(&tmp_task->start);
                if (compare_date(date_tm, start_tm) == 0)
                {
                    ladd(l, LLAST, tmp_task);
                    tmp_task = (task *)malloc(sizeof(task));
                }
                else
                {
                    break;
                }
            }
        }
        else if (what == FUTURE)
        {
            // code
        }
        else
        {
            // code
        }
        // ?
        return 1;
    }
    return 0;
}