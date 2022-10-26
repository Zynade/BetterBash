# Documentation

## Table of Contents

- [Built-in Commands](#built-in-commands)
  - [cd](#cd)
  - [echo](#echo)
  - [pwd](#pwd)
  - [exit](#exit)
- [External Commands](#external-commands)
    - [ls](#ls)
    - [cat](#cat)
    - [date](#date)
    - [rm](#rm)
    - [mkdir](#mkdir)

***
## cd
### Usage: `$ cd [directory]`
### Flags:
- `~` or `-` - Navigate to the home directory
- `-s` - Navigate to the BetteBash home directory
### Edge cases handled:
- If no directory is specified, the user is taken to their home directory.
- If the directory does not exist, the user is notified.
- If the directory is not a directory, the user is notified.
- If the user does not have permission to access the directory, the user is notified.
- If the user passes more than one directory, the user is notified.
***
## echo
### Usage: `$ echo [FLAGS] [STRING]` 
### Flags supported:
- `-n`: do not append the newline character
- `--help`: print a useful instruction message
### Edge cases handled:
- If the user passes a flag that is not supported, the user is notified.
- If the user passes no arguments, standard Bash behaviour is followed.
### Assumptions:
- The user will not pass more than one flag.
- The user will not pass multi-line strings.
***
## pwd
### Usage: `$ pwd [FLAGS]`
### Flags supported:
- `-L`: Use the logical path. Use PWD from environment, even if it contains symlinks. This is the default option.
- `-P`: Use the physical path. Resolve all symlinks.
### Edge cases handled:
- If the user passes a flag that is not supported, the user is notified.
- If the user passes too many arguments, the user is notified.

***
## exit
### Usage: `$ exit`
***
## ls
### Usage: `$ ls [FLAGS] [DIRECTORY]`
if `DIRECTORY` is not specified, print the contents of the current working directory.
### Flags supported:
- `-a`: do not ignore hidden files, i.e., entries starting with "."
- `-r`: reverse order while sorting
### Edge cases handled:
- If the directory does not exist, print an error message.
- If the directory is not a directory, print an error message.
- If the directory is not readable, print an error message.
- If the directory path is an absolute path instead of relative to the current directory, handle it accordingly.
### Assumptions:
- The user will not try to list multiple directories in the same command.
***
## cat
### Usage: `$ cat [FLAGS] [FILE]...`
if `FILE` is `-`, or no `FILE` is provided, use `stdin` as the file pointer.
### Flags supported:
- `-n`: number all output lines
- `-E`: display $ at end of each line
### Edge cases handled:
- If the file does not exist, print an error message.
- If the file is a directory, print an error message.
***
## date
### Usage: `$ date [FLAGS]`
### Flags supported:
- `-u`: print Coordinated Universal Time
- `-R`: print date and time in RFC 5322 format
### Edge cases handled:
- If the flag is invalid, print an error.
- If the date passed is invalid, print an error.
***
## rm
### Usage: `$ rm [FLAGS] [FILE]...`
### Flags supported:
- `-r`: remove directories and their contents recursively
- `-i`: prompt for confirmation before every removal 
### Edge cases handled: 
- If the file does not exist, print an error message.
- If the file is a directory and the -r flag is not passed, print an error message.
- If the file is named `-`, invoking `rm -` does not lead to an error.
- If the file is named '.' or '..', invoking `rm .` or `rm ..` does not lead to an error.
- If there is no file argument, print an error message.
***
## mkdir
### Usage: `$ mkdir [FLAGS] [DIRECTORY]...`
### Flags supported:
- `-v`: Verbose output. Print a message for each created directory
- `-p`: No error if existing. make parent directories as needed.
### Edge cases handled:
- If the directory already exists, print an error message.
- If the directory is named `-`, invoking `mkdir -` does not lead to an error.
- If the directory is named '.' or '..', invoking `mkdir .` or `mkdir ..` does not lead to an error.
- If the directory is a regular file, print an error message.
- If there is no directory argument, print an error message.
- If the user tries to created nested directories without passing the -p flag, print an error message.
***