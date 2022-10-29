#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <pthread.h>

#define MAX_TOKENS_BUFFER_SIZE 1000
#define ARGUMENT_BUFFER_LENGTH 1024

struct thread_args
{
    char *command;
    char **argv;
};
struct thread_ret_val
{
    int status;
};

void shell_loop(void);
char *read_line(void);
int tokenize_line(char *, char **);
int shell_execute(int, char **);

char PROGRAM_PATH[1024];

void cd(int argc, char *argv[]);
void echo(int argc, char *argv[]);
void pwd(int argc, char *argv[]);

void *start_thread(void *thread_arg);
int start_process(char *command, char *argv[]);

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
        char *username = getenv("USER");
        if (username == NULL)
        {
            username = "user";
        }

        char *hostname = malloc(1024);
        if (gethostname(hostname, 1024) != 0)
        {
            hostname = "host";
        }

        // get the current directory without full path
        char *cwd = malloc(1024);
        assert(getcwd(cwd, 1024) != NULL);
        char *current_dir = strrchr(cwd, '/');
        if (current_dir == NULL)
        {
            current_dir = cwd;
        }
        else
        {
            current_dir = current_dir + 1;
        }

        printf(ANSI_COLOR_YELLOW "%s@%s" ANSI_COLOR_RESET " :: " ANSI_COLOR_BLUE "/%s/" ANSI_COLOR_RED " $ " ANSI_COLOR_RESET, username, hostname, current_dir);
        free(hostname);
        free(cwd);

        line = read_line();
        int argc = tokenize_line(line, argv);
        if (argc == -1) continue;
        return_code = shell_execute(argc, argv);
        if (return_code < 0) break;
    }
    free(line);
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
        printf("\nBetterBash: EOF detected, terminating shell.\n");
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
            fprintf(stderr, "BetterBash: your instruction contains more arguments than permitted (max: %d)", MAX_TOKENS_BUFFER_SIZE);
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
        in-built: cd, echo, pwd, exit
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
        pwd(argc, argv);
    }
    else if (strcmp(command, "exit") == 0)
    {
        return -1;
    }
    else
    {
        // External command
        if (strlen(command) < 2)
        {
            fprintf(stderr, "BetterBash: command not found: %s\n", command);
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
                fprintf(stderr, "BetterBash: command not found: %s\n", command);
                return 1;
            }
            pthread_t thread;
            struct thread_args *thread_arg = malloc(sizeof(struct thread_args));
            struct thread_ret_val *rvals = malloc(sizeof(struct thread_ret_val));
            thread_arg->command = command;
            thread_arg->argv = argv;

            pthread_create(&thread, NULL, start_thread, thread_arg);
            pthread_join(thread, (void **) &rvals);
            int status = rvals->status;
            free(thread_arg);
            free(rvals);
            return status;
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

void *start_thread(void *thread_arg)
{
    /*
    Starts a thread.
    Returns the return code of the thread.
    */
    struct thread_args *thread_argument = (struct thread_args *) thread_arg;
    struct thread_ret_val *rvals = malloc(sizeof(struct thread_ret_val));
    char *command = thread_argument->command;
    char **argv = thread_argument->argv;
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
    int status = system(path);
    rvals->status = status;
    return (void *) rvals;
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
            fprintf(stderr, "BetterBash: command not found: %s\n", command);
        }
        exit(0);
    }
    else if (pid < 0)
    {
        fprintf(stderr, "BetterBash: error forking");
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

    // get the home directory path
    if (argc > 2)
    {
        fprintf(stderr, "BetterBash: cd: too many arguments\n");
        return;
    }
    if (argc < 2 || strcmp(argv[1], "~") == 0 || strcmp(argv[1], "-") == 0)
    {
        char *home = getenv("HOME");
        if (home == NULL)
        {
            fprintf(stderr, "BetterBash: cd: could not get home directory path");
            return;
        }
        if (chdir(home) != 0)
        {
            perror("cd");
        }
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", home, 1);
    }
    else if (strcmp(argv[1], "-s") == 0)
    {
        char *shell_path = PROGRAM_PATH;
        if (shell_path == NULL)
        {
            fprintf(stderr, "BetterBash: cd: could not get shell directory path");
            return;
        }
        if (chdir(shell_path) != 0)
        {
            perror("cd");
        }
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", shell_path, 1);
    }
    else
    {
        if (chdir(argv[1]) != 0)
        {
            perror("cd");
        }
        char *new_pwd = malloc(1024);
        if (new_pwd == NULL)
        {
            fprintf(stderr, "BetterBash: cd: could not allocate memory");
            return;
        }
        strcpy(new_pwd, getenv("PWD"));
        strcat(new_pwd, "/");
        strcat(new_pwd, argv[1]);

        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", new_pwd, 1);
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
        if (strcmp(buffer, "--help") == 0)
        {
            printf("\n\tUsage: echo [FLAGS] [STRING] \n");
            printf("\tEcho the STRING(s) to standard output.\n");
            printf("\tSupported flags:\n");
            printf("\t\t-n:\tdo not output the trailing newline\n");
            printf("\n");
            return;
        }
        if (i == 1 && buffer[0] == '-' && buffer[1] != '\0')
        {
            if (buffer[1] == 'n')
            {
                flag_n = true;
            }
            else
            {
                fprintf(stderr, "BetterBash: echo: invalid option -- '%c'\n", buffer[1]);
                return;
            }
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
void pwd(int argc, char *argv[])
{
    /*
    Prints the current working directory to stdout.
    */
    if (argc > 2)
    {
        fprintf(stderr, "BetterBash: pwd: too many arguments\n");
        return;
    }

    if (argc == 1 || (argv[1][0] == '-' && argv[1][1] == 'L'))
    {
        char *cwd = malloc(1024);
        cwd = getenv("PWD");
        if (cwd == NULL)
        {
            perror("pwd");
            return;
        }
        printf("%s\n", cwd);
        // free(cwd);
    }
    else 
    {
        if (argv[1][0] == '-' && argv[1][1] == 'P')
        {     
            char *cwd = getcwd(NULL, 0);
            if (cwd == NULL)
            {
                perror("pwd");
                return;
            }
            printf("%s\n", cwd);
            free(cwd);
        }
        else if (argv[1][0] == '-')
        {
            fprintf(stderr, "BetterBash: pwd: invalid option -- '%c'\n", argv[1][1]);
        }
        else
        {
            fprintf(stderr, "BetterBash: pwd: too many arguments\n");
        }
    }
    return;
}