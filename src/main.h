#ifndef TASK_H_
#define TASK_H_

#include <time.h>
#include "linklist.h"

#define BUFFER_SIZE 1000
#define NUMBER_OF_TRY 3

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

// main.c
void error(char c, char msg[]);
void version();
void help();
int parse_args(char const str[]);
void opera();

// task.c
void set_task(int num);
int manage();
int leap_year(int year);
int check_date(int day, int month, int year);
time_t choose_date();

// commun.c
void *alloc_check(int size);
int read_line(char str[], int n);
int choose_from_menu(const char menu[], int min, int max, int dft);

// operation.c
void add_task();
void remove_task();
void modify_task();
void copy_task();
void save_modification();
void show_upcoming();
void show_history();
void exit_program();
task *new_task();
time_t choose_time(const char message[]);
void print_tasks(linklist l);
int choose_number(char const msg[], int min, int max);

// io.c
void open_file_if_null();
void write_task(task *tsk);
int read_task(task *tsk);
int task_len(task *tsk);
void write_all_date_tasks(linklist l);
int search_date(struct tm *date);
int read_all_date_tasks(linklist l, time_t date, direction what);
void delete_task();

// time.c
struct tm *get_datetime_struct(time_t timer);
time_t get_datetime(struct tm *timer);
int compare_date(struct tm *date1, struct tm *date2);
int compare_time(struct tm *date1, struct tm *date2);

#endif