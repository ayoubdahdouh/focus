#ifndef TASK_H_
#define TASK_H_

#include <time.h>
#include "linklist.h"

typedef enum Task_flag
{
    TASK_OK,
    TASK_NEW,
    TASK_MODIFIED,
    TASK_REMOVED
} task_flag;

typedef struct Task
{
    time_t start;
    time_t end;
    int status; // 1:done, 0:not done
    char *name;
    task_flag flag; // 0:ok, 1:new, 2:modified
} task;

typedef enum Direction
{
    PAST = -1,
    SAME_WEEK = 0,
    FUTURE = 1
} direction;

#include "operations/operations.h"
#include "input_output/input_output.h"
#include "linklist.h"

#define BUFFER_SIZE 1000
#define NUMBER_OF_TRY 3
#define N 7
#define NAME_SIZE 256

extern int is_tasks_l_changed;
extern char buffer[];
extern linklist tasks_l[];
extern time_t week;
// extern int day;



#endif