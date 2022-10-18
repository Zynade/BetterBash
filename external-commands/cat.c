#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

int echo_from_file(char *file_name, bool flag_n, bool flag_E);

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        fprintf(stderr, "Usage: cat [OPTION] [FILE]\n");
        return 1;
    }

    if (strcmp(&argv[0][1], ".") == 0 && strcmp(&argv[0][2], "/") == 0)
    {
        argv[0] = argv[0] + 2;
    }

    bool flag_n = false;
    bool flag_E = false;
    bool found_non_flag_arg = false;

    // check for flags
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            char *argument = argv[i];
            if (argument[0] == '-')
            {
                if (strlen(argument) > 1)
                {
                    int j = 1;
                    for (; argument[j] != '\0'; j++)
                    {
                        if (argument[j] == 'n')
                        {
                            flag_n = true;
                        }
                        if (argument[j] == 'E')
                        {
                            flag_E = true;
                        }
                    }
                }
                else found_non_flag_arg = true;
            }
            else found_non_flag_arg = true;
        }
    }
    char cwd[1024];
    char file_name[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("ls");
    }
    if (argc == 1 || !found_non_flag_arg)
    {
        strcpy(file_name, "/dev/stdin");
        if (echo_from_file(file_name, flag_n, flag_E) == 1)
        {
            return 1;
        }
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            strcpy(file_name, cwd);
            strcat(file_name, "/");
            if (strcmp(argv[i], "-") == 0)
            {
                strcpy(file_name, "/dev/stdin");
            }
            else if (argv[i][0] != '-')
            {
                strcat(file_name, argv[i]);
            }
            if (echo_from_file(file_name, flag_n, flag_E) == 1)
            {
                return 1;
            }
        }
    }
    return 0;
}

int echo_from_file(char *file_name, bool flag_n, bool flag_E)
{
    FILE *fp = fopen(file_name, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    if (fp == NULL)
    {
        perror(file_name);
        return 1;
    }
    int line_number = 1;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (flag_n)
        {
            printf("     %d  ", line_number);
        }
        if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0'; // remove the newline character
        printf("%s", line);
        if (flag_E)
        {
            printf("$");
        }
        printf("\n");
        line_number++;
    }
    return 0;
}