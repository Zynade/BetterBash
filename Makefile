cmd_dir := ./external-commands/
cmds := ls cat rm

define compile_program =
	gcc -o $(1) $(1).c -g -Wall -Wshadow -Wextra -Wconversion
endef

all: shell
	$(MAKE) -C $(cmd_dir)

shell:
	$(call compile_program,shell)

clean:
	rm shell
	cd $(cmd_dir) && rm $(cmds)