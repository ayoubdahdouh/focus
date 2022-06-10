#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "task.h"
#include "global.h"
#include "operations/operations.h"
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
    const char menu[] = "\n1:add / 2:remove / 3:rename / 4:set / 5:save / 6:quit >> ";

    has_changed = 0;
    tasks = lopen();

    read_tasks(tasks);

    while (code != 7)
    {
        // system("clear");
        printf("----------------------------------------------------\n");
        print_tasks(tasks);
        code = choose_from_menu(menu, 1, 7, 6);
        switch (code)
        {
        case 1:
            add_task(tasks, &has_changed);
            break;
        case 2:
            remove_task(tasks, &has_changed);
            break;
        case 3:
            rename_task(tasks, &has_changed);
            break;
        case 4:
            set_task(tasks, &has_changed);
            break;
        case 5:
            save_task(tasks, &has_changed);
            break;
        default:
            quit_app(tasks, &has_changed);
            break;
        }
    }

    return 1;
}
