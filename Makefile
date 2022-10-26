cmd_dir := ./external-commands/
cmds := ls cat rm mkdir date
FLAGS := -g3 -Wall -Wshadow -Wextra -Wconversion -Wdouble-promotion -Werror -Wshadow -Wformat=2 -Wformat-overflow -Wformat-truncation -Wundef -Wconversion -fno-common

define compile_program =
	gcc -o $(1) $(1).c $(FLAGS)
endef

all: shell
	$(MAKE) -C $(cmd_dir)

shell: shell.c
	$(call compile_program,shell)

clean:
	rm shell
	cd $(cmd_dir) && rm $(cmds)
	