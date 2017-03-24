Personal project to build a C repl. This project is barely functional. 


Right now it works by creating three files in /tmp, when you enter in a normal statement it gets inserted into a predefined `main` function in `clcrepl.c`, all includes are inserted into `clcincludes.c` and definitions are in `clcdefs.c`. When you enter a line into the reple `clcrepl.c` is compiled and executed with `stdout` and `stderr` pointing to the terminal.


I'm hoping to modify this project to use llvm's libclang api otherwise I'll have to do some janky File IO to accomplish what I'm trying to do.
