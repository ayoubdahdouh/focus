#ifndef SRC_H
#define SRC_H

// main.h
void version();
void help();
int parse_args(char const str[]);
void error(char c, char msg[]);

// com.h
void *alloc_check(int size);
int read_line(char str[], int n);
int choose_from_menu(const char msg[], int min, int max, int dft);


// task.h
int manage();
void set_task(int num);
int leap_year(int year);
int check_date(int day, int month, int year);
time_t choose_date();
time_t seek_monday(time_t t);
int month_days(int month, int year);

// time.h
int compare_date(struct tm *date1, struct tm *date2);
int compare_time(struct tm *date1, struct tm *date2);
time_t get_datetime(struct tm *timer);
struct tm *get_datetime_struct(time_t timer);



#endif