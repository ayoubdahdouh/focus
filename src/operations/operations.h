#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../main.h"

// com.c
time_t choose_time(const char message[]);
void choose_priority(int *pr);
int choose_status(int *st);
int choose_number(char const msg[], int min, int max);
void choose_text(char **dst, const char msg[]);
int search_time_in_list(int day, struct tm *date);
int choose_week_day();


// add.c
void add_task();
task *new_task(int *day);

// remove.c
void remove_task();

// modify.c
void modify_task();

// save.c
void save_modification();

// exit.c
void exit_program();

// copy.c
void copy_task();

#endif