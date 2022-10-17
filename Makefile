all: shell external-commands/ls

shell: shell.c
	gcc -o shell shell.c -g -Wall

external-commands/ls: external-commands/ls.c
	gcc -o external-commands/ls external-commands/ls.c -g -Wall

clean:
	rm shell external-commands/ls external-commands/cat external-commands/date external-commands/rm external-commands/mkdir