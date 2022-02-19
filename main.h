#ifndef TASK_H_
#define TASK_H_

typedef struct Task
{
    time_t start;
    time_t end;
    int status; // 1:done, 0:not done
    char *name;
    char *details;
    int priority;
} task;

typedef enum Direction
{
    PAST = -1,
    SAMEDAY = 0,
    FUTURE = 1
} direction;

void error(char c, char msg[]);

// task.c
void manage();
void set_task(int num);

// commun.c
void *alloc_check(int size);
void read_line(char str[], int n);
int compare_date(struct tm *date1, struct tm *date2);
int compare_time(struct tm *date1, struct tm *date2);

#endif