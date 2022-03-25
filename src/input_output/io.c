#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../src.h"
#include "../main.h"
#include "input_output.h"

#define MAX_CHRUNK 2048
FILE *stream;

extern time_t week;
extern char buffer[];

void open_file_if_null()
{
    if (!stream)
    {
        stream = fopen("data_tasks.bin", "ab+");
        if (!stream)
        {
            // error(0, "internal error");
            exit(EXIT_FAILURE);
        }
    }
}

int task_len(task *tsk)
{
    int size = 0;

    size += sizeof(time_t) * 2;                     // start, end
    size += sizeof(int);                            // status
    size += (strlen(tsk->name) + 1) * sizeof(char); // name

    return size;
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

    // write name
    size = strlen(tsk->name);
    fwrite(&size, sizeof(int), 1, stream);
    if (size > 0)
    {
        fwrite(tsk->name, (size + 1) * sizeof(char), 1, stream);
    }
}

int read_task(task *tsk)
{
    int size, len;

    if (!feof(stream) && fread(&size, sizeof(int), 1, stream))
    {
        if (size == 0)
        {
            return 0;
        }

        // read priority
        fread(&tsk->start, sizeof(time_t), 1, stream);
        fread(&tsk->end, sizeof(time_t), 1, stream);

        // read status
        fread(&tsk->status, sizeof(int), 1, stream);

        // read name
        fread(&len, sizeof(int), 1, stream);
        if (len > 0)
        {
            tsk->name = (char *)alloc_mem(len * sizeof(char));
            fread(tsk->name, (len + 1) * sizeof(char), 1, stream);
        }
        return 1;
    }

    return 0;
}

void delete_task()
{
    int size;
    long pos, pos2;
    if (feof(stream))
    {
        return;
    }

    pos = ftell(stream);
    if (fread(&size, sizeof(int), 1, stream) == 0 || size <= 0)
    {
        return;
    }
    pos2 = pos + size;
    fseek(stream, pos2, SEEK_SET); // go to next task (ie: after the task to be deleted).

    while (fread(&size, sizeof(int), 1, stream) && size > 0)
    {
        fseek(stream, pos2, SEEK_SET); // go bck to beginning of the task
        fread(buffer, size, 1, stream);
        fseek(stream, pos, SEEK_SET); // go bck to previous task
        fwrite(buffer, size, 1, stream);
        pos += size;
        pos2 += size; // next task
    }

    // fill the free space with 0;
    int i = 0;
    size = 0;
    while (!feof(stream))
    {
        fwrite(&size, sizeof(int), 1, stream);
        i++;
    }

    // resize file to remove free space
    if (i >= MAX_CHRUNK)
    {
        // code
    }
}

void seek_end()
{
    int size;

    rewind(stream);
    while (fread(&size, sizeof(int), 1, stream))
    {
        if (size == 0)
        {
            fseek(stream, -1 * sizeof(int), SEEK_CUR);
            break;
        }
    }
}

void write_all_date_tasks(linklist l[])
{
    // task *tsk;

    // if (lempty(l))
    // {
    //     return;
    // }

    // open_file_if_null();

    // literator iter = lat(l, LFIRST);
    // while (iter)
    // {
    //     tsk = (task *)iter->data;
    //     // new task
    //     if (tsk->flag == TASK_NEW)
    //     {
    //         seek_end();
    //         write_task(tsk);
    //     }
    //     // modified task
    //     else if (tsk->flag == TASK_MODIFIED)
    //     {
    //         search_date(get_datetime_struct(week));
    //         delete_task();
    //         seek_end();
    //         write_task(tsk);
    //     }
    //     // removed task
    //     else if (tsk->flag == TASK_REMOVED)
    //     {
    //         search_date(get_datetime_struct(week));
    //         delete_task();
    //     }
    //     linc(&iter);
    // }
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
int read_all_date_tasks(linklist l[], time_t date, direction what)
{
    // struct tm *date_tm, *start_tm;
    // task *tsk;

    // open_file_if_null();
    // rewind(stream);

    // date_tm = get_datetime_struct(date);

    // if (what == SAME_WEEK)
    // {
    //     tsk = (task *)malloc(sizeof(task));
    //     while (read_task(tsk))
    //     {
    //         start_tm = get_datetime_struct(tsk->start);
    //         if (compare_date(date_tm, start_tm) == 0)
    //         {
    //             ladd(l, LLAST, tsk);
    //             tsk = (task *)malloc(sizeof(task));
    //         }
    //     }
    //     return 1;
    // }
    // else if (what == FUTURE)
    // {
    //     tsk = (task *)malloc(sizeof(task));
    //     while (read_task(tsk))
    //     {
    //         start_tm = get_datetime_struct(tsk->start);
    //         if (compare_date(date_tm, start_tm) > 0)
    //         {
    //             ladd(l, LLAST, tsk);
    //             tsk = (task *)malloc(sizeof(task));
    //         }
    //     }
    //     return 1;
    // }
    // else
    // {
    //     tsk = (task *)malloc(sizeof(task));
    //     while (read_task(tsk))
    //     {
    //         start_tm = get_datetime_struct(tsk->start);
    //         if (compare_date(date_tm, start_tm) < 0)
    //         {
    //             ladd(l, LLAST, tsk);
    //             tsk = (task *)malloc(sizeof(task));
    //         }
    //     }
    //     return 1;
    // }
    return 0;
}