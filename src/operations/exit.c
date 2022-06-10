#include <stdio.h>
#include <stdlib.h>
#include "operations.h"
#include "../tools.h"
#include "../print.h"

void quit_app(linklist tasks, int *has_changed)
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
            save_task(tasks, has_changed);
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