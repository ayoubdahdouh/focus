#ifndef TASK_H_
#define TASK_H_

#include <time.h>
#include "linklist.h"

#define BUFFER_SIZE 1000

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
int read_line(char str[], int n);

// operation.c
void clone_task();
void copy_task();
void modify_task();
void remove_task();
void save_modification();
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
void write_all_date_tasks();
int search_date(struct tm *date);
int read_all_date_tasks(linklist l, time_t date, direction what);

// time.c
struct tm *get_datetime_struct(time_t timer);
time_t get_datetime(struct tm *timer);
int compare_date(struct tm *date1, struct tm *date2);
int compare_time(struct tm *date1, struct tm *date2);

#endif