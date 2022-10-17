#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

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
    // return &linkedlist_head;
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
        fprintf(stderr, "Usage: ls\n");
        return 1;
    }

    bool flag_a = false;
    bool flag_r = false;

    // check for flags
    for (int i = 0; i < argc; i++)
    {
        char *argument = argv[i];
        if (argument[0] == '-')
        {
            for (int i = 0; argument[i] != '\0'; i++)
            {
                if (argument[i] == 'a')
                {
                    flag_a = true;
                }
                if (argument[i] == 'r')
                {
                    flag_r = true;
                }
            }
        }
    }

    // get cwd
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("ls");
    }

    // print the files in the current directory
    // if flag_a is true, print all files
    // if flag_r is true, print the files in reverse order
    // otherwise, print only the non-hidden files

    // Loop through all files in cwd and add them to a doubly linked list
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(cwd)) != NULL)
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
    if (!flag_r)
    {
        struct linkedlist_node *current = linkedlist_head;
        while (current != NULL)
        {
            if (flag_a || current->data->d_name[0] != '.')
            {
                printf("%s  ", current->data->d_name);
            }
            current = current->next;
        }
    }
    else
    {
        struct linkedlist_node *current = linkedlist_tail;
        while (current != NULL)
        {
            if (flag_a || current->data->d_name[0] != '.')
            {
                printf("%s  ", current->data->d_name);
            }
            current = current->prev;
        }
    }
    printf("\n");
    closedir(dir);
    free_linkedlist(linkedlist_head);

    return 0;
}