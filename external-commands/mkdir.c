#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

struct flags
{
    bool v;
    bool p;
};

int create_directory(const char *path, struct flags *flags);

int is_regular_file(const char *path);
int is_directory(const char *path);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "mkdir: missing operand\n");
        fprintf(stderr, "Usage: mkdir [FLAGS] [DIRECTORY]...\n");
        return 1;
    }
    bool nonFlagArgExists = false;
    struct flags *flags = malloc(sizeof(struct flags));
    flags->v = false;
    flags->p = false;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-' || argv[i][1] == '\0')
        {
            nonFlagArgExists = true;
        }
        else
        {
            int j = 1;
            for (;argv[i][j] != '\0'; j++)
            {
                if (argv[i][j] == 'v')
                {
                    flags->v = true;
                }
                if (argv[i][j] == 'p')
                {
                    flags->p = true;
                }
            }
        }
    }
    if (!nonFlagArgExists)
    {
        fprintf(stderr, "mkdir: missing operand\n");
        fprintf(stderr, "Usage: mkdir [FLAGS] [DIRECTORY]...\n");
        free(flags);
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-' || argv[i][1] == '\0')
        {
            if (create_directory(argv[i], flags) < 0)
            {
                free(flags);
                return 1;
            }
        }
    }

    free(flags);
    return 0;
}

int create_directory(const char *path, struct flags *flags)
{
    if (is_directory(path) || is_regular_file(path))
    {
        fprintf(stderr, "mkdir: cannot create directory '%s': File exists\n", path);
        return 1;
    }
    else
    {
        // loop through the path and detect if it contains subdirectories.
        char *new_path = malloc(strlen(path) + 1);
        strcpy(new_path, path);
        char *token = strtok(new_path, "/");
        char *subdir = malloc(strlen(token) + 1);
        strcpy(subdir, token);
        while (token != NULL)
        {
            token = strtok(NULL, "/");
            if (is_directory(subdir))
            {
                if (!flags->p)
                {
                    fprintf(stderr, "mkdir: cannot create directory '%s': File exists\n", subdir);
                    free(subdir);
                    free(new_path);
                    return 1;
                }

            }
            else
            {
                if (token == NULL || flags->p)
                {
                    if (mkdir(subdir, 0777) == -1)
                    {
                        if (!flags->p) 
                        {
                            fprintf(stderr, "mkdir: cannot create directory '%s': Not a directory\n", subdir);
                        }
                        else
                        {
                            fprintf(stderr, "mkdir: cannot create directory '%s': Permission denied\n", subdir);
                        }
                        free(subdir);
                        free(new_path);
                        return 1;
                    }
                    else
                    {
                        if (flags->v)
                        {
                            printf("mkdir: created directory '%s'\n", subdir);
                        }
                    }
                }
            }

            if (token != NULL)
            {
                subdir = realloc(subdir, strlen(subdir) + strlen(token) + 2);
                strcat(subdir, "/");
                strcat(subdir, token);
            }
        }
        free(subdir);
        free(new_path);
        return 0;
    }
}

/*----------------------------------------------- UTILITY FUNCTIONS -----------------------------------------------*/

int is_directory(const char *path)
{
    // SOURCE: https://stackoverflow.com/a/4553053/12215003
   struct stat statbuf;
   if (stat(path, &statbuf) != 0) return 0;
   return S_ISDIR(statbuf.st_mode);
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) return 0;
    return S_ISREG(path_stat.st_mode);
}