cmds := ls cat rm

define compile_program =
	gcc -o $(1) $(1).c -g -Wall -Wshadow -Wextra -Wconversion
endef

all:
	@for cmd in $(cmds); do \
		$(call compile_program, $$cmd); \
	done
