cmds := ls cat rm mkdir date
FLAGS := -g3 -Wall -Wshadow -Wextra -Wconversion -Wdouble-promotion -Werror -Wshadow -Wformat=2 -Wformat-overflow -Wformat-truncation -Wundef -Wconversion -fno-common

define compile_program =
	gcc -o $(1) $(1).c $(FLAGS)
endef

all:
	@for cmd in $(cmds); do \
		$(call compile_program, $$cmd); \
	done
