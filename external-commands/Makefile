cmds := ls cat rm

define compile_program =
	gcc -o $(1) $(1).c -g -Wall
endef

all:
	# @echo "Building all commands"
	# $(MAKE) shell
	@for cmd in $(cmds); do \
		$(call compile_program, $$cmd); \
	done
