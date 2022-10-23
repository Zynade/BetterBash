cmds := ls cat rm mkdir date
FLAGS := -g -Wall -Wshadow -Wextra -Wconversion

define compile_program =
	gcc -o $(1) $(1).c $(FLAGS)
endef

all:
	@for cmd in $(cmds); do \
		$(call compile_program, $$cmd); \
	done
