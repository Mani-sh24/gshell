# gshell

**gshell** is a minimal command-line shell written in C for learning purposes.  
It supports a few built-in commands and can execute external programs using `fork()` and `execvp()`.

This project was created as a learning exercise.

## Features

- Simple prompt (`>`) for user input
- Built-in commands:
  - `cd`
  - `exit`
- Support for running external commands (e.g., `ls`, `pwd`, `echo`, etc.)
- Basic command parsing and memory management

## Compilation

To compile the shell:

```
gcc -o gshell main.c
```

Replace `main.c` with the appropriate file name if different.

## Usage

To run the shell:

```
./gshell
```

You can then enter commands at the prompt:

```
> pwd
> cd /
> ls -l
> exit
```

## Built-in Commands

- `cd [directory]` — Change the current working directory.
- `exit` — Exit the shell.

Any other command is treated as an external program and is run using `execvp`.

## Reference

This shell was built by referring to:  
**Write a Shell in C** by Stephen Brennan — https://brennan.io/2015/01/16/write-a-shell-in-c/

