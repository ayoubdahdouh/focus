#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "print.h"
#include "task.h"
#include "tools.h"

#define MAX_CHRUNK 2048
FILE *fp = NULL;
char filename[];
extern char buffer[];

void print_tasks(linklist tasks)
{

    task *t;
    int cnt = 1;

    if (!lempty(tasks))
    {
        literator iter = lat(tasks, LFIRST);

        while (iter)
        {
            t = (task *)iter->data;
            if (t->flag == TASK_REMOVED)
            {
                linc(&iter);
                continue;
            }
            for (int i = 0; i < t->level; i++)
            {
                printf("   ");
            }
            if (t->status)
            {
                printf("%d --- [%sOK%s] %s\n", cnt, SUCCESS, NOCOLOR, t->title);
            }
            else
            {
                printf("%d --- %s\n", cnt, t->title);
            }
            cnt++;
            linc(&iter);
        }
    }
    else
    {
        printf("\tLa liste est vide.\n");
    }
}

void open_file()
{
    if (!(fp = fopen(filename, "ab+")))
    {
        fseek(fp, 0, SEEK_SET);
        exit(EXIT_FAILURE);
    }
}

void truncate_file()
{
    if (fp)
    {
        fclose(fp);
    }

    remove(filename);
    open_file();
}

// write task to end of file
int task_len(task *t)
{
    int size = 0;

    size += sizeof(int) * 2;
    size += (strlen(t->title) + 1) * sizeof(char);

    return size;
}

void write_task(task *t)
{
    int size = task_len(t);

    // write length of t
    fwrite(&size, sizeof(int), 1, fp);
    // write id
    fwrite(&t->level, sizeof(int), 1, fp);
    // write status
    fwrite(&t->status, sizeof(int), 1, fp);
    // write title
    size -= sizeof(int) * 2;
    fwrite(&size, sizeof(int), 1, fp);
    if (size > 0)
    {
        fwrite(t->title, (size + 1) * sizeof(char), 1, fp);
    }

    fflush(fp);
}

// read task from the current position of file
int read_task(task *t)
{
    int size;
    if (fread(&size, sizeof(int), 1, fp) && size > 0)
    {
        // id
        fread(&t->level, sizeof(int), 1, fp);
        // status
        fread(&t->status, sizeof(int), 1, fp);
        // title
        fread(&size, sizeof(int), 1, fp);
        if (size > 0)
        {
            t->title = (char *)alloc_mem(size * sizeof(char));
            fread(t->title, (size + 1) * sizeof(char), 1, fp);
        }
        return 1;
    }

    return 0;
}

// read all tasks in the file
void read_tasks(linklist tasks)
{
    task *t;

    printf("%s\n", filename);
    open_file();
    int cnt = 0;
    while (!feof(fp))
    {
        t = (task *)alloc_mem(sizeof(task));
        if (read_task(t))
        {
            cnt++;
            ladd(tasks, LLAST, t);
        }
        else
        {
            break;
        }
    }

    printf("%d tasks has been read...\n", cnt);
}
