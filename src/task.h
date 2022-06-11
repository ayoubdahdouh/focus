#ifndef TASK_H
#define TASK_H

#include "global.h"

int manage();
void taskset(linklist tasks, int *has_changed);
void taskadd(linklist tasks, int *has_changed);
void taskdel(linklist tasks, int *has_changed);
void taskmod(linklist tasks, int *has_changed);
void taskmov(linklist tasks, int *has_changed);
void taskcpy(linklist tasks, int *has_changed);
void tasksav(linklist tasks, int *has_changed);
void quitapp(linklist tasks, int *has_changed);

#endif