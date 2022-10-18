# simple-shell
An attempt at implementing a shell.  
Permitted commands: cd, echo, pwd, ls, cat, date, rmdir

# Docs
## cd
### Usage: `$ cd [directory]`

## echo
### Usage: `$ echo [arg]` 

## ls
### Usage: `$ ls <flags> <path>`
<.> represents an optional argument.

### Flags supported:
1. `-a`: do not ignore hidden files, i.e., entries starting with "."
2. `-r`: reserve order while sorting

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
                // pthread commands to execute the instructions
                start_thread(args)
            else:
                // pid(), fork() etc commands 
                start_process(args)
```
