#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1000

void shell_loop(void);
char *read_line(void);
int tokenize_line(char *, char **);
int shell_execute(int, char **);

#define MAX_TOKENS_BUFFER_SIZE 1000

char *builtin_commands[] = {"cd", "echo", "pwd"};
int num_builtin_commands = 3;

void cd(int argc, char *argv[]);
void echo(int argc, char *argv[]);
void pwd(int argc, char *argv[]);

int start_thread(char *command, char *argv[]);
int start_process(char *command, char *argv[]);

/*--------------------------------------------------- MAIN ---------------------------------------------------*/

int main(int argc, char *argv[])
{
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
        if (argc == -1) continue; // The user entered a line that could not be parsed, so prompt them to enter another line.
        return_code = shell_execute(argc, argv);
        if (return_code == -1)
        {
            break;
        }
        else if (return_code == 0)
        {
            continue;
        }
    }
    free(argv);
}

char *read_line(void)
{
    char *line = NULL;
    size_t buffer_size = 0;
    if (getline(&line, &buffer_size, stdin) == -1)
    {
        printf("\nreadline: EOF detected, terminating program.\n");
        exit(0);
        // fprintf(stderr, "shell: input too long");
    }
    return line;
}

int tokenize_line(char *line, char **args)
{
    /* 
    line: string to parse
    args: array of strings to write the tokens into
    returns: argc
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
            fprintf(stderr, "tokenize_line: your line contained more arguments than permitted (max: %d)", MAX_TOKENS_BUFFER_SIZE);
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
    // internal: cd, echo, pwd
    // external: ls, cat, date, rm, mkdir
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
        pwd(argc, argv);
    }
    else
    {
        // The command is not a built-in command, and a process or thread must be launched as appropriate.
        if (strlen(command) < 2)
        {
            fprintf(stderr, "shell: command not found: %s", command);
            return 0;
        }

        char mode;
        if (strcmp(argv[argc - 1], "&t") == 0) mode = 't';
        else mode = 'p';

        if (mode == 't')
        {
            // The command requests for a thread to execute the instruction.
            // launch a thread to execute the program!
            // start_thread(command, argv);
        }
        else if (mode == 'p')
        {
            // The command requests a new process to execute the instruction.
            // create another process to execute the program!
            return start_process(command, argv);
        }
    }
    return 0;
}

/*-------------------------------------------- PROCESSES & THREADS --------------------------------------------*/

int start_thread(char *command, char *argv[])
{
    // TODO
    return 0;
}

int start_process(char *command, char *argv[])
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        // child process
        char path[1024];
        strcpy(path, "./external-commands/");
        strcat(path, command);
        if (execv(path, argv) == -1)
        {
            fprintf(stderr, "shell: command not found: %s", command);
        }
        exit(0);
    }
    else if (pid < 0)
    {
        // error forking
        fprintf(stderr, "shell: error forking");
        return -1;
    }
    else
    {
        // parent process
        int status;
        waitpid(pid, &status, 0);
        return 0;
    }
}

/*----------------------------------------------- BUILT-IN COMMANDS -----------------------------------------------*/

void cd(int argc, char *argv[])
{
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
    if (argc < 2)
    {
        fprintf(stderr, "echo: expected argument to \"echo\"\n");
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            printf("%s ", argv[i]);
        }
        printf("\n");
    }
}
void pwd(int argc, char *argv[])
{
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