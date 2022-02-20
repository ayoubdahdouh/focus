#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

static char program[] = "todol";
static char program_version[] = "pre-alpha";

int main(int argc, char const *argv[])
{
    int code = 0;
    int num;

    if (argc >= 2)
    {
        code = parse_args(argv[1]);
        if (code == -1)
        {
            error(0, "invalid argument");
            return 0;
        }
        else if (code == 's' && argc != 3 ||
                 code != 's' && argc != 2)
        {
            error(code, "need a number");
        }
    }

    switch (code)
    {
    case 'h':
        help();
        break;
    case 'v':
        version();
        break;
    case 's':
        num = atoi(argv[2]);
        if (num <= 0)
        {
            error('s', " task number is required");
        }
        set_task(num);
        break;
    case 'm':
        while (manage())
        {
        }

        break;
    default:
        // show today tasks
        // code
        break;
    }

    // printf("code: %c, num: %d\n", code, num);
    return 0;
}

void error(char c, char msg[])
{
    if (c)
    {
        printf("%s: option %c: %s\n", program, c, msg);
    }
    else
    {
        printf("%s: %s\n", program, msg);
    }
    exit(EXIT_FAILURE);
}

void version()
{
    printf("%s %s\n", program, program_version);
}

void help()
{
    printf("Usage:\n\t%s OPTION\n"
           "OPTION:\n"
           "\t-s NUM\n\t\tset task number NUM to done/not done.\n"
           "\t-m\tmanage tasks.\n"
           "\t-h\thelp.\n"
           "\t-v\tversion.\n"
           "\tby default, list today's tasks\n",
           program);
}

int parse_args(char const str[])
{
    if (str[0] != '-' || str[0] == '\0')
    {
        return -1;
    }

    return (str[1] != '\0' && strchr("smhv", str[1])) ? str[1] : -1;
}