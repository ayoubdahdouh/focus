#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../global.h"

// com.c
int choose_number(char const msg[], int min, int max);
void choose_text(char **dst, const char msg[]);

// set.c
void set_task(linklist tasks, int *has_changed);

// add.c
void add_task(linklist tasks, int *has_changed);

// remove.c
void remove_task(linklist tasks, int *has_changed);

// modify.c
void rename_task(linklist tasks, int *has_changed);

// save.c
void save_task(linklist tasks, int *has_changed);

// exit.c
void quit_app(linklist tasks, int *has_changed);

#endif