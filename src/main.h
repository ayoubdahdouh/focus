#ifndef MAIN_H
#define MAIN_H

int main(int argc, char const *argv[]);
void error(char c, char msg[]);
void version();
void help();
int parse_args(char const str[]);

#endif