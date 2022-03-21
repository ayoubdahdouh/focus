#include <stdio.h>
#include "operations.h"

void save_modification()
{
    // ask to save change
    if (is_tasks_l_changed)
    {
        write_all_date_tasks(tasks_l);
        is_tasks_l_changed = 0;
        printf("changes saved.\n");
    }
}