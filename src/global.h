#ifndef GLOBAL_H
#define GLOBAL_H

#include <time.h>
#include "linklist.h"

#define PROGRAM "focus"
#define PROGRAM_VERSION "pre-alpha"

#define TASK_OK 0
#define TASK_NEW 1
#define TASK_MODIFIED 2
#define TASK_REMOVED 3
#define TASK_LIST 4


typedef struct _task
{
    int id;
    int status; // 1:done, 0:not done
    char *title;
    int flag;
} task;

#include "linklist.h"

#define BUFFER_SIZE 1000
#define NUMBER_OF_TRY 3
#define NAME_SIZE 256

extern char buffer[];
extern char filename[];

#endif