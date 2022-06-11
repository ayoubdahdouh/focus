#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "task.h"
#include "print.h"
#include "tools.h"

#define PATH "data_tasks.bin"
char buffer[BUFFER_SIZE];

int num_removed = 0;
int has_changed = 0;
linklist tasks;

int manage()
{
    int code;
    const char menu[] = "\n┌┈┈[ 1:add | 2:remove | 3:rename | 4:copy | 5:move | 6:set | 7:save | 8:quit ]\n├┈┈[command] "; //╰

    has_changed = 0;
    tasks = lopen();

    read_tasks(tasks);

    while (code != 7)
    {
        // system("clear");
        printf("\n\n"); // ----------------------------------------------------
        print_tasks(tasks);
        code = choose_from_menu(menu, 1, 8, 8);
        switch (code)
        {
        case 1:
            taskadd(tasks, &has_changed);
            break;
        case 2:
            taskdel(tasks, &has_changed);
            break;
        case 3:
            taskmod(tasks, &has_changed);
            break;
        case 4:
            taskcpy(tasks, &has_changed);
            break;
        case 5:
            taskmov(tasks, &has_changed);
            break;
        case 6:
            taskset(tasks, &has_changed);
            break;
        case 7:
            tasksav(tasks, &has_changed);
            break;
        default:
            quitapp(tasks, &has_changed);
            break;
        }
    }

    return 1;
}

void taskadd(linklist tasks, int *has_changed)
{
    int level, num, size = 0;
    task *t, *tmp;

    t = (task *)alloc_mem(sizeof(task));

    num = choose_from_menu("├┈┈[number] ", 0, tasks->count, 0);

    int ok = 0;
    while (!ok)
    {
        level = choose_from_menu("├┈┈[level] ", 0, 10, 0);

        if (level > 0)
        {
            int i = num - 1;
            while (i > 0 && !ok)
            {
                tmp = (task *)lget(tasks, i - 1);
                if (tmp->level == level - 1)
                {
                    ok = 1;
                }
            }
            if (!ok)
            {
                printf("├┈┈[%sError%s] %sNo parent%s\n", WARNING, NOCOLOR, WARNING, NOCOLOR);
            }
            else
            {
                t->level = level;
            }
        }
        else
        {
            ok = 1;
            t->level = level;
        }
    }

    t->level = level;
    t->flag = TASK_NEW;
    while (size == 0)
    {
        printf("├┈┈[title] ");
        size = read_line(buffer, NAME_SIZE);
    }
    t->title = (char *)alloc_mem((size + 1) * sizeof(char));
    strncpy(t->title, buffer, size + 1);

    if (num > 0)
    {
        ladd(tasks, num - 1, t);
    }
    else
    {
        ladd(tasks, LLAST, t);
    }

    printf("╰┈┈[%sOK%s] ", SUCCESS, NOCOLOR);
    *has_changed = 1;
}

void taskdel(linklist tasks, int *has_changed)
{
    int id;

    if (tasks->count > 0)
    {
        id = choose_number("task number: ", 1, tasks->count);
        if (id != -1)
        {
            ldel(tasks, id - 1);
            // task *t = lget(tasks, id - 1);
            // t->flag = TASK_REMOVED;
            *has_changed = 1;
        }
        else
        {
            printf("Abort.\n\n");
        }
    }
    else
    {
        printf("List is empty...");
    }
}

void taskmod(linklist tasks, int *has_changed)
{
    int id;
    task *tmp;

    if (tasks->count > 0)
    {
        id = choose_number("task number: ", 1, tasks->count);
        if (id != -1)
        {
            tmp = lget(tasks, id - 1);
            choose_text(&tmp->title, "title: ");
            tmp->flag = TASK_MODIFIED;
            *has_changed = 1;
        }
    }
    else
    {
        printf("List is empty...");
    }
}

task *taskclone(task *t)
{
    int len;
    task *tmp;

    tmp = (task *)alloc_mem(sizeof(task));
    tmp->level = t->level;
    tmp->status = t->status;
    len = strlen(t->title) + 1;
    tmp->title = (char *)alloc_mem(len * sizeof(char));
    strncpy(tmp->title, t->title, len);
    return tmp;
}
void taskcpy(linklist tasks, int *has_changed)
{
    int from, to;
    task *tmp;

    if (tasks->count > 0)
    {
        from = choose_number("from: ", 1, tasks->count);
        if (from != -1)
        {
            to = choose_number("to: ", 1, tasks->count);
            if (to != -1)
            {
                tmp = taskclone(lget(tasks, from - 1));
                ladd(tasks, to - 1, tmp);
                *has_changed = 1;
            }
        }
    }
    else
    {
        printf("List is empty...");
    }
}

void taskmov(linklist tasks, int *has_changed)
{
    int from, to;
    task *tmp;

    if (tasks->count > 0)
    {
        from = choose_number("from: ", 1, tasks->count);
        if (from != -1)
        {
            to = choose_number("to: ", 1, tasks->count);
            if (to != -1)
            {
                tmp = taskclone(lget(tasks, from - 1));
                ldel(tasks, from - 1);
                ladd(tasks, to - 1, tmp);
                *has_changed = 1;
            }
        }
    }
    else
    {
        printf("List is empty...");
    }
}

void tasksav(linklist tasks, int *has_changed)
{
    // ask to save change
    if (*has_changed)
    {
        task *t;

        printf("truncate file...\n");
        truncate_file();

        literator it = lat(tasks, LFIRST);
        while (it)
        {
            t = (task *)it->data;
            write_task(t);
            t->flag = TASK_OK;
            printf("wirte task %d...\n", t->level);
            linc(&it);
        }
        *has_changed = 0;

        printf("changes saved.\n");
    }
}

void taskset(linklist tasks, int *has_changed)
{
    int id;
    task *tmp;

    if (tasks->count > 0)
    {
        id = choose_number("task number: ", 1, tasks->count);
        if (id != -1)
        {
            tmp = lget(tasks, id - 1);
            tmp->status = 1;
            *has_changed = 1;
        }
    }
    else
    {
        printf("List is empty...");
    }
}

void quitapp(linklist tasks, int *has_changed)
{
    int ok = 3;

    exit(0);
    if (!*has_changed)
    {
        return;
    }
    while (ok)
    {
        ok--;
        printf("Do you want to save the changes ? [y/n] ");
        read_line(buffer, BUFFER_SIZE);
        if (buffer[0] == 'y')
        {
            tasksav(tasks, has_changed);
            printf("changes saved.\n");
            lclear(tasks);
            ok = 0;
        }
        else if (buffer[0] != 'n')
        {
            printf("changes abort.\n");
            lclear(tasks);
            ok = 0;
        }
    }
}