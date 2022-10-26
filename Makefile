cmd_dir := ./external-commands/
cmds := ls cat rm mkdir date
FLAGS := -g3 -Wall -Wshadow -Wextra -Wconversion -Wdouble-promotion -Werror -Wshadow -Wformat=2 -Wformat-overflow -Wformat-truncation -Wundef -Wconversion -fno-common

define compile_program =
	gcc -o $(1) $(1).c $(FLAGS)
endef

all: betterbash
	$(MAKE) -C $(cmd_dir)

betterbash: betterbash.c
	$(call compile_program,betterbash)

clean:
	rm betterbash
	cd $(cmd_dir) && rm $(cmds)
	