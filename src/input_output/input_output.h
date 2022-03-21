#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include "../main.h"

// io.h
void open_file_if_null();
int task_len(task *tsk);
void write_task(task *tsk);
int read_task(task *tsk);
void delete_task();
void seek_end();
void write_all_date_tasks(linklist l[]);
int search_date(struct tm *date);
int read_all_date_tasks(linklist l[], time_t date, direction what);

// print.h
void print_week(linklist l[]);
void print_tasks(linklist l);

#endif