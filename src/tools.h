#ifndef TOOLS_H
#define TOOLS_H

void *alloc_mem(int size);
void free_mem(void *ptr);
int read_line(char str[], int n);
int choose_from_menu(const char msg[], int min, int max, int dft);

#endif