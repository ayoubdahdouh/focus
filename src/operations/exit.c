#include <stdio.h>
#include "operations.h"
#include "../src.h"

void exit_program()
{
    int ok = 3;

    if (!is_tasks_l_changed)
    {
        return;
    }
    while (ok--)
    {

        // ask to save change
        printf("Do you want to save the changes ? [y/n] ");
        read_line(buffer, BUFFER_SIZE);
        if (buffer[0] != 'y' && buffer[0] != 'n')
        {
            printf("please select \"y\" for yes and \"n\" for no.\n\n");
        }
        else
        {
            if (buffer[0] == 'y')
            {
                save_modification();
                printf("changes saved.\n");
            }
            else
            {
                printf("abort.\n");
            }
            lclear(tasks_l);
            ok = 0;
        }
    }
}