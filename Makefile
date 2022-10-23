cmd_dir := ./external-commands/
cmds := ls cat rm mkdir date
FLAGS := -g -Wall -Wshadow -Wextra -Wconversion

define compile_program =
	gcc -o $(1) $(1).c $(FLAGS)
endef

all: shell
	$(MAKE) -C $(cmd_dir)

shell:
	$(call compile_program,shell)

clean:
	rm shell
	cd $(cmd_dir) && rm $(cmds)
	