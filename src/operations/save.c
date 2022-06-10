#include <stdio.h>
#include "operations.h"
#include "../tools.h"
#include "../print.h"

void save_task(linklist tasks, int *has_changed)
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
            printf("wirte task %d...\n", t->id);
            linc(&it);
        }
        *has_changed = 0;

        printf("changes saved.\n");
    }
}
