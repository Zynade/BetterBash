#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct flags 
{
    bool R;
    bool u;
};

#define BUFFER_LEN 1024

int main(int argc, char *argv[])
{
    struct flags *flags = malloc(sizeof(struct flags));
    flags->R = false;
    flags->u = false;
    if (argc > 1)
    {
        char argument[1024];
        for (int i = 1; i < argc; i++)
        {
            strcpy(argument, argv[i]);
            if (argument[0] == '-')
            {
                if (argument[1] == '\0')
                {
                    fprintf(stderr, "date: invalid date '%s'\n", argument);
                    free(flags);
                    return 1;
                }
                int j = 1;
                while (argument[j] != '\0')
                {
                    if (argument[j] == 'R')
                    {
                        flags->R = true;
                    }
                    else if (argument[j] == 'u')
                    {
                        flags->u = true;
                    }
                    else
                    {
                        fprintf(stderr, "date: invalid option -- '%c'\n", argument[j]);
                        free(flags);
                        return 1;
                    }
                    j++;
                }
            }
            else
            {
                fprintf(stderr, "date: invalid date '%s'\n", argument);
                free(flags);
                return 1;
            }
        }
    }

    // Get the current date and time
    time_t now = time(NULL);
    if (now == -1) 
    {
        perror("date");
        free(flags);
        return 1;
    }
    struct tm *current_time = gmtime(&now);
    if (current_time == NULL)
    {
        perror("date");
        free(flags);
        return 1;
    }

    char buffer[BUFFER_LEN];

    if (flags->R)
    {
        strftime(buffer, BUFFER_LEN, "%a, %d %b %Y %T +0000", current_time);
        puts(buffer);
    }
    else
    {
        strftime(buffer, BUFFER_LEN, "%a %b %d %T UTC %Y", current_time);
        puts(buffer);
    }
    free(flags);
    return 0;
}