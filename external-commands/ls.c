#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

bool is_ELF_exectuable(char *file_path);

/*----------------------------------------------- LINKED LIST IMPLEMENTATION -----------------------------------------------*/

struct linkedlist_node {
    struct dirent *data;
    struct linkedlist_node *next;
    struct linkedlist_node *prev;
};

struct linkedlist_node *linkedlist_head = NULL;
struct linkedlist_node *linkedlist_tail = NULL;

void dll_insert(struct dirent *data)
{
    struct linkedlist_node *new_node = malloc(sizeof(struct linkedlist_node));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    if (linkedlist_head == NULL)
    {
        linkedlist_head = new_node;
        linkedlist_tail = new_node;
    }
    else
    {
        linkedlist_tail->next = new_node;
        new_node->prev = linkedlist_tail;
        linkedlist_tail = new_node;
    }
}

void sort_linkedlist(struct linkedlist_node *head)
{
    // Alphabetically sort the linkedlist based on the name of the file.
    // Uses Bubble Sort.
    struct linkedlist_node *current = head;
    struct linkedlist_node *index = NULL;
    struct dirent *temp;
    if (head == NULL)
    {
        return;
    }
    else
    {
        while (current != NULL)
        {
            index = current->next;
            while (index != NULL)
            {
                if (strcmp(current->data->d_name, index->data->d_name) > 0)
                {
                    temp = current->data;
                    current->data = index->data;
                    index->data = temp;
                }
                index = index->next;
            }
            current = current->next;
        }
    }
}

void free_linkedlist(struct linkedlist_node *head)
{
    struct linkedlist_node *current = head;
    struct linkedlist_node *next = NULL;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
}

/*----------------------------------------------- MAIN -----------------------------------------------*/

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        fprintf(stderr, "Usage: ls [OPTIONS] [DIRECTORY]\n");
        return 1;
    }

    if (strcmp(&argv[0][1], ".") == 0 && strcmp(&argv[0][2], "/") == 0)
    {
        argv[0] = argv[0] + 2;
    }

    bool flag_a = false;
    bool flag_r = false;

    // check for flags
    for (int i = 0; i < argc; i++)
    {
        char *argument = argv[i];
        if (argument[0] == '-')
        {
            int j = 1;
            for (; argument[j] != '\0'; j++)
            {
                if (argument[j] == 'a')
                {
                    flag_a = true;
                }
                if (argument[j] == 'r')
                {
                    flag_r = true;
                }
            }
        }
    }

    // Get the file path
    char path[1024];
    if (getcwd(path, sizeof(path)) == NULL)
    {
        perror("ls");
    }
    if (argc >= 2)
    {
        int index = 1;
        bool is_path_to_be_changed = false;
        while (index < argc) 
        {
            // Find the index of the first argument that isn't a flag or a mode.
            if (argv[index][0] != '&' && argv[index][0] != '-') 
            {
                is_path_to_be_changed = true;
                break;
            }
            index++; 
        }
        if (is_path_to_be_changed && index != argc)
        {
            if (argv[index][0] != '/')
            {
                strcat(path, "/");
                strcat(path, argv[index]);
            }
            else
            {
                strcpy(path, argv[index]);
            }
        }
    }

    // Loop through all files in the path and add them to a doubly linked list
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            dll_insert(ent);
        }
    }
    else
    {
        perror("ls");
    }
    sort_linkedlist(linkedlist_head);

    // Print the files in the linked list
    if (!flag_r)
    {
        struct linkedlist_node *current = linkedlist_head;
        while (current != NULL)
        {
            char *file_path = malloc(strlen(path) + strlen(current->data->d_name) + 2);
            strcpy(file_path, path);
            if (file_path[strlen(file_path) - 1] != '/')
            {
                strcat(file_path, "/");
            }
            strcat(file_path, current->data->d_name);

            if (flag_a || current->data->d_name[0] != '.')
            {
                // if the file type is a directory, print it in blue
                if (current->data->d_type == DT_DIR)
                {
                    printf(ANSI_COLOR_BLUE "%s  " ANSI_COLOR_RESET, current->data->d_name);
                }
                // if the file type is an ELF executable, print it in green
                else if (is_ELF_exectuable(file_path))
                {
                    printf(ANSI_COLOR_GREEN "%s  " ANSI_COLOR_RESET, current->data->d_name);
                }
                // if the file type is a symbolic link, print it in cyan
                else if (current->data->d_type == DT_LNK)
                {
                    printf(ANSI_COLOR_CYAN "%s  " ANSI_COLOR_RESET, current->data->d_name);
                }
                // if the file type is a regular file, print it in the default colour
                else
                {
                    printf("%s  ", current->data->d_name);
                }
            }
            free(file_path);
            current = current->next;
        }
    }
    else
    {
        struct linkedlist_node *current = linkedlist_tail;
        while (current != NULL)
        {
            char *file_path = malloc(strlen(path) + strlen(current->data->d_name) + 2);
            strcpy(file_path, path);
            if (file_path[strlen(file_path) - 1] != '/')
            {
                strcat(file_path, "/");
            }
            strcat(file_path, current->data->d_name);

            if (flag_a || current->data->d_name[0] != '.')
            {
                // if the file type is a directory, print it in blue
                if (current->data->d_type == DT_DIR)
                {
                    printf(ANSI_COLOR_BLUE "%s  " ANSI_COLOR_RESET, current->data->d_name);
                }
                // if the file type is an ELF executable, print it in green
                else if (is_ELF_exectuable(file_path))
                {
                    printf(ANSI_COLOR_GREEN "%s  " ANSI_COLOR_RESET, current->data->d_name);
                }
                // if the file type is a symbolic link, print it in cyan
                else if (current->data->d_type == DT_LNK)
                {
                    printf(ANSI_COLOR_CYAN "%s  " ANSI_COLOR_RESET, current->data->d_name);
                }
                // if the file type is a regular file, print it in the default colour
                else
                {
                    printf("%s  ", current->data->d_name);
                }
            }
            current = current->prev;
        }
    }
    printf("\n");
    closedir(dir);
    free_linkedlist(linkedlist_head);

    return 0;
}

bool is_ELF_exectuable(char *file_path)
{
    /*
    This function checks if the file at the given path is an ELF executable.
    It does this by checking the first 4 bytes of the file to see if they match the ELF magic number.
    */
    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        return false;
    }
    unsigned char buffer[4];
    fread(buffer, 1, 4, fp);
    fclose(fp);
    if (buffer[0] == 0x7f && buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F')
    {
        return true;
    }
    return false;
    
}
