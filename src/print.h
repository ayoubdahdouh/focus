#ifndef PRINT_H
#define PRINT_H

#include "global.h"

// print.c
void print_tasks(linklist tasks);
void open_file();
void truncate_file();
int task_len(task *t);
void write_task(task *t);
int read_task(task *t);
void read_tasks(linklist tasks);

#endif