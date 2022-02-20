#ifndef TASK_H_
#define TASK_H_

#include <time.h>
#include "linklist.h"

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

// main.c
void error(char c, char msg[]);
void version();
void help();
int parse_args(char const str[]);

// task.c
void set_task(int num);
int manage();
int main_menu();
void show_upcoming();
void show_history();
int leap_year(int year);
int check_date(int day, int month, int year);
time_t choose_date();

// commun.c
void *alloc_check(int size);
void read_line(char str[], int n);
int compare_date(struct tm *date1, struct tm *date2);
int compare_time(struct tm *date1, struct tm *date2);

// operation.c
void clone_task();
void copy_task();
void modify_task();
void remove_task();
void add_task();
task *new_task();
time_t choose_time(const char message[]);
int operation_menu();
void print_tasks(linklist l);

// io.c
void open_file_if_null();
void write_task(task *tsk);
int read_task(task *tsk);
int task_len(task *tsk);
void write_tasks_list();
int search_date(time_t date);
int read_tasks_by_date(linklist l, direction when);

#define BUFFER_SIZE 1000

#endif