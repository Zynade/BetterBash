# BetterBash

A simplified imitation of the bash shell written in C.  

## Disclaimer

This shell is not meant to be used as a replacement for Bash. Despite the name, it is merely a hobby project I made in my spare time for fun.

This shell has been tested to work on Ubuntu, Pop!_OS, Fedora, and Artix Linux. It may or may not work on macOS or other Linux distributions.
I have no plans on adding support for Windows.

## Features

Supports the following commands:

- built-in commands:
  - [cd](documentation.md#cd)  
  - [echo](documentation.md#echo)  
  - [pwd](documentation.md#pwd)
  - [exit](documentation.md#exit)
- external commands:
  - [ls](documentation.md#ls)
  - [cat](documentation.md#cat)
  - [date](documentation.md#date)
  - [rm](documentation.md#rm)
  - [mkdir](documentation.md#mkdir)

## Installation

1. Clone the repository: `git clone https://github.com/Zynade/BetterBash.git && cd BetterBash`.
2. Run `make` to compile the program.
3. Run `./betterbash` to start the shell.

## How to use

1. Run the program: `./shell`
2. Enter a command. For example, `ls -a`
3. Press `Enter` to execute the command.
4. To exit the shell, enter `exit` or press `Ctrl + D`.

## Documentation

Refer to the [documentation](documentation.md) for an exhaustive list of commands and flags supported.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Issues

If you find any bugs, notice any inconsistencies, typos, or have any suggestions, please open an issue on the [issues page](https://github.com/Zynade/BetterBash/issues).
