cmd_dir := ./external-commands/
cmds := ls cat rm

define compile_program =
	gcc -o $(1) $(1).c -g -Wall
endef

all: shell
	# @echo "Building all commands"
	# build shell
	$(MAKE) -C $(cmd_dir)

shell:
	$(call compile_program,shell)

clean:
	rm shell
	cd $(cmd_dir) && rm $(cmds)