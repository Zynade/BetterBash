# basic-shell-

# Pseudocode

```py
Entry point:
    main()
        -calls shell_loop()
    
    shell_loop()
        -reads a line of input from stdin
        -parse and split the line into an array of arguments
        -call shell_execute(args)

    shell_execute(args):
        mode = args[0][:-2]
        if args[0] is inbuilt:
            function(args)
        else:
            if mode is thread:
                // pthread commands to execute the instructions
                start_thread(args)
            else:
                // pid(), fork() etc commands 
                start_process(args)
```
