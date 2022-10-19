#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

struct flags
{
    bool i;
    bool r;
};

int remove_file(const char *path, struct flags *flags);
int is_regular_file(const char *path);
int is_directory(const char *path);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "rm: missing operand\n");
        fprintf(stderr, "Usage: rm [FLAGS] [FILE]...\n");
        return 1;
    }
    bool nonFlagArgExists = false;
    
    struct flags *flags = malloc(sizeof(struct flags));
    flags->i = false;
    flags->r = false;

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
                if (argv[i][j] == 'r')
                {
                    flags->r = true;
                }
                if (argv[i][j] == 'i')
                {
                    flags->i = true;
                }
            }
        }
    }
    if (!nonFlagArgExists)
    {
        fprintf(stderr, "rm: missing operand\n");
        return 1;
    }

    // get cwd
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("rm");
        return 1;
    }

    // remove files
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], ".") == 0 || strcmp(argv[i], "..") == 0)
        {
            fprintf(stderr, "rm: refusing to remove '.' or '..' directory: skipping '%s'\n", argv[i]);
            continue;
        }
        if (argv[i][0] != '-' || argv[i][1] == '\0')
        {
            char file_name[1024];
            strcpy(file_name, "./");
            strcat(file_name, argv[i]);
            remove_file(file_name, flags);
        }
    }
    return 0;
}

int remove_file(const char *path, struct flags *flags)
{
    if (!is_regular_file(path) && !is_directory(path))
    {
        fprintf(stderr, "rm: cannot remove '%s': No such file or directory\n", path);
        return 1;
    }
    if (is_regular_file(path))
    {
        if (flags->i)
        {
            printf("rm: remove regular file '%s'? ", path);
            char c;
            scanf(" %c", &c);
            if (c != 'y')
            {
                return 0;
            }
        }
        if (remove(path) != 0)
        {
            perror("rm");
            return 1;
        }
    }
    else
    {
        if (!flags->r)
        {
            fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", path);
            return 1;
        }
        if (flags->i)
        {
            printf("rm: descend into directory '%s'? ", path);
            char c;
            scanf(" %c", &c);
            if (c != 'y')
            {   
                return 0;
            }
        }
        if (rmdir(path) != 0)
        {
            // loop through all files in path and call remove_file()
            DIR *d = opendir(path);
            if (d == NULL)
            {
                perror("rm");
                return 1;
            }
            struct dirent *dir;
            while ((dir = readdir(d)) != NULL)
            {
                if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                {
                    continue;
                }
                char file_name[1024];
                strcpy(file_name, path);
                strcat(file_name, "/");
                strcat(file_name, dir->d_name);
                if (remove_file(file_name, flags) != 0)
                {
                    return 1;
                }
            }
            closedir(d);
            if (flags->i)
            {
                printf("rm: remove directory '%s'? ", path);
                char c;
                scanf(" %c", &c);
                if (c != 'y')
                {
                    return 0;
                }
            }
            if (rmdir(path) != 0)
            {
                perror("rm");
                return 1;
            }
        }
    }
    return 0;
}

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