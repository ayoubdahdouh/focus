#ifndef TOOLS_H
#define TOOLS_H

void *alloc_mem(int size);
void free_mem(void *ptr);
int read_line(char str[], int n);
int choose_from_menu(const char msg[], int min, int max, int dft);
int choose_number(char const msg[], int min, int max);
void choose_text(char **dst, const char msg[]);

#endif