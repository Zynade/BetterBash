# simple-shell
An attempt at implementing a shell.  
Permitted commands: cd, echo, pwd, ls, cat, date, rmdir, mkdir

# Docs
## cd
### Usage: `$ cd [directory]`
***
## echo
### Usage: `$ echo [arg]` 
***
## pwd
### Usage: `$ pwd`
***
## ls
### Usage: `$ ls [FLAGS] [DIRECTORY]`
if `DIRECTORY` is not specified, print the contents of the current working directory.
### Flags supported:
1. `-a`: do not ignore hidden files, i.e., entries starting with "."
2. `-r`: reserve order while sorting
### Edge cases handled:
1. If the directory does not exist, print an error message.
2. If the directory is not a directory, print an error message.
3. If the directory is not readable, print an error message.

***
## cat
### Usage: `$ cat [FLAGS] [FILE]...`
if `FILE` is `-`, or no `FILE` is provided, use `stdin` as the file pointer.
### Flags supported:
1. `-n`: number all output lines
2. `-E`: display $ at end of each line
### Edge cases handled:
1. If the file does not exist, print an error message.
2. If the file is a directory, print an error message.
***
## rm
### Usage: `$ rm [FLAGS] [FILE]...`
### Flags supported:
1. `-r`: remove directories and their contents recursively
2. `-i`: prompt for confirmation before every removal 
### Edge cases handled: 
1. If the file does not exist, print an error message.
2. If the file is a directory and the -r flag is not passed, print an error message.
3. If the file is named `-`, invoking `rm -` does not lead to an error.
4. If the file is named '.' or '..', invoking `rm .` or `rm ..` does not lead to an error.
5. If there is no file argument, print an error message.
***
## mkdir
### Usage: `$mkdir [FLAGS] [DIRECTORY]...`
### Flags supported:
1. `-v`: Verbose output. Print a message for each created directory
2. `-p`: No error if existing. make parent directories as needed.
### Edge cases handled:
1. If the directory already exists, print an error message.
2. If the directory is named `-`, invoking `mkdir -` does not lead to an error.
3. If the directory is named '.' or '..', invoking `mkdir .` or `mkdir ..` does not lead to an error.
4. If the directory is a regular file, print an error message.
5. If there is no directory argument, print an error message.
6. If the user tries to created nested directories without passing the -p flag, print an error message.
***
# Pseudocode

```py
Entry point:
    main()
        -calls shell_loop()
    
    shell_loop()
        -reads a line of input from stdin
        -parse and split the line into an array of arguments
        -call shell_execute(args)

    shell_execute(args):
        mode = args[0][:-2]
        if args[0] is inbuilt:
            function(args)
        else:
            if mode is thread:
                start_thread(args)
            else:
                start_process(args)
```
