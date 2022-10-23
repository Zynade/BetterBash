#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#define MAX_TOKENS_BUFFER_SIZE 1000
#define ARGUMENT_BUFFER_LENGTH 1024

void shell_loop(void);
char *read_line(void);
int tokenize_line(char *, char **);
int shell_execute(int, char **);

char PROGRAM_PATH[1024];

void cd(int argc, char *argv[]);
void echo(int argc, char *argv[]);
void pwd(void);

int start_thread(char *command, char *argv[]);
int start_process(char *command, char *argv[]);

/*--------------------------------------------------- MAIN ---------------------------------------------------*/

int main(void)
{
    assert(getcwd(PROGRAM_PATH, sizeof(PROGRAM_PATH)) != NULL);
    shell_loop();
    return 0;
}

/*----------------------------------------------- SHELL FUNCTIONS -----------------------------------------------*/

void shell_loop(void)
{
    /* Runs the main shell loop
    Logic: read a line from stdin, tokenize it into an argv vector, and then execute the instruction before freeing up allocated memory.
    */
    char *line = NULL;
    char **argv = malloc(MAX_TOKENS_BUFFER_SIZE * sizeof(char *));
    int return_code = 1;
    while (true)
    {
        printf("$ ");
        line = read_line();
        int argc = tokenize_line(line, argv);
        if (argc == -1) continue; // The user entered a line that could not be parsed, so prompt them to enter another instruction.
        return_code = shell_execute(argc, argv);
        if (return_code < 0) break;
    }
    free(argv);
}

char *read_line(void)
{
    /*
    Reads a line from stdin and returns it as a string.
    */
    char *line = NULL;
    size_t buffer_size = 0;
    if (getline(&line, &buffer_size, stdin) == -1)
    {
        printf("\nshell: EOF detected, terminating shell.\n");
        exit(0);
    }
    return line;
}

int tokenize_line(char *line, char **args)
{
    /* 
    Tokenizes a line into an argv vector.
    Returns the number of tokens in the line.
    */

    int i = 0;
    int argc = 0;

    char *str = line;
    char *delim = " \n";
    char *save_ptr;
    char *token = strtok_r(str, delim, &save_ptr);

    while (token != NULL)
    {
        argc++;
        if (argc > MAX_TOKENS_BUFFER_SIZE)
        {
            fprintf(stderr, "shell: your instruction contains more arguments than permitted (max: %d)", MAX_TOKENS_BUFFER_SIZE);
            return -1;
        }
        args[i++] = token;
        token = strtok_r(NULL, delim, &save_ptr);
    }
    args[i] = NULL;
    return argc;
}

int shell_execute(int argc, char *argv[])
{
    /*
    Executes a command.
    Returns the return code of the command.

    Supports the following commands:
        in-built: cd, echo, pwd
        external: ls, cat, date, rm, mkdir
    */

    if (argc < 1)
    {
        return 0;
    }
    char *command = argv[0];

    if (strcmp(command, "cd") == 0)
    {
        cd(argc, argv);
    }
    else if (strcmp(command, "echo") == 0)
    {
        echo(argc, argv);
    }
    else if (strcmp(command, "pwd") == 0)
    {
        pwd();
    }
    else
    {
        // External command
        if (strlen(command) < 2)
        {
            fprintf(stderr, "shell: command not found: %s\n", command);
            return 1;
        }

        char mode;
        if (strcmp(argv[argc - 1], "&t") == 0) 
        {
            argv[argc - 1] = NULL;
            mode = 't';
        }
        else mode = 'p';

        if (mode == 't')
        {
            // Start a thread
            if (argc < 2) // Thread instructions must have at least two arguments: the program, and "&t"
            {
                fprintf(stderr, "shell: command not found: %s\n", command);
                return 1;
            }
            return start_thread(command, argv);
        }
        else if (mode == 'p')
        {
            // Start a process
            return start_process(command, argv);
        }
    }
    return 0;
}

/*-------------------------------------------- PROCESSES & THREADS --------------------------------------------*/

int start_thread(char *command, char *argv[])
{
    /*
    Starts a thread.
    Returns the return code of the thread.
    */
    char path[1024];
    strcpy(path, PROGRAM_PATH);
    strcat(path, "/external-commands/");
    strcat(path, command);
    char *arg = argv[1];
    for (int i = 1; arg != NULL; i++)
    {
        strcat(path, " ");
        strcat(path, arg);
        arg = argv[i+1];
    }
    return system(path);
}

int start_process(char *command, char *argv[])
{
    /*
    Starts a process.
    Returns the return code of the process.
    */

    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process. Execute the command.
        char path[1024];
        strcpy(path, PROGRAM_PATH);
        strcat(path, "/external-commands/");
        strcat(path, command);
        
        if (execv(path, argv) == -1)
        {
            fprintf(stderr, "shell: command not found: %s\n", command);
        }
        exit(0);
    }
    else if (pid < 0)
    {
        fprintf(stderr, "shell: error forking");
        return -1;
    }
    else
    {
        // Parent process. Wait for the child to complete.
        int status;
        waitpid(pid, &status, 0);
        return 0;
    }
}

/*----------------------------------------------- BUILT-IN COMMANDS -----------------------------------------------*/

void cd(int argc, char *argv[])
{
    /*
    Changes the current working directory of the current process.
    */

    if (argc < 2)
    {
        fprintf(stderr, "cd: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(argv[1]) != 0)
        {
            perror("cd");
        }
    }
}
void echo(int argc, char *argv[])
{
    /*
    Prints the arguments to stdout.
    */

    if (argc == 1) 
    {
        printf("\n");
        return;
    }
    bool flag_n = false;
    bool is_first_output = true;
    char buffer[ARGUMENT_BUFFER_LENGTH];
    for (int i = 1; i < argc; i++)
    {
        strcpy(buffer, argv[i]);
        if (i == 1 && buffer[0] == '-' && buffer[1] == 'n')
        {
            flag_n = true;
        }
        else
        {
            char *p = buffer;
            if (
                (buffer[0] == '\'' && buffer[strlen(buffer) - 1] == '\'') || 
                (buffer[0] == '\"' && buffer[strlen(buffer) - 1] == '\"')
                )
            {
                p[strlen(p) - 1] = '\0';
                p++;
            }
            if (is_first_output)
            {
                printf("%s", p);
                is_first_output = false;
            }
            else printf(" %s", p);
        }
    }
    if (!flag_n) printf("\n");
    return;
}
void pwd(void)
{
    /*
    Prints the current working directory to stdout.
    */
   
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd");
    }
}