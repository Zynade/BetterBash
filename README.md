# simple-shell
An attempt at implementing a shell.  
Permitted commands: cd, echo, pwd, ls, cat, date, rmdir

# Docs
## cd
### Usage: `$ cd [directory]`

## echo
### Usage: `$ echo [arg]` 

## ls
### Usage: `$ ls [FLAGS] [DIRECTORY]`
if `DIRECTORY` is not specified, print the contents of the current working directory.
### Flags supported:
1. `-a`: do not ignore hidden files, i.e., entries starting with "."
2. `-r`: reserve order while sorting

## cat
### Usage: `$ cat [FLAGS] [FILE]...`
if `FILE` is `-`, or no `FILE` is provided, use `stdin` as the file pointer.
### Flags supported:
1. `-n`: number all output lines
2. `-E`: display $ at end of each line

## rm
### Usage: `$rm [FLAGS] [FILE]...`
### Flags supported:
1. `-r`: remove directories and their contents recursively
2. `-i`: prompt for confirmation before every removal 

Edge cases handled: 
1. if a file is named `-`, running `rm -` does not lead to errors.
2. if user attempts to remove `.` or `..`, refuse.

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
