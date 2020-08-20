# CppND-System-Monitor

Finished project code for **System Monitor Project** in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

![System Monitor](images/monitor.png)

## NCURSES

[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output. To run this project locally in Linux/Ubuntu environment, install `ncurses` by running the following command:

`sudo apt install libncurses5-dev libncursesw5-dev`

## Make Insutructions

This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:

- `build` compiles the source code and generates an executable
- `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
- `debug` compiles the source code and generates an executable, including debugging symbols
- `clean` deletes the `build/` directory, including all of the build artifacts

To test the build, run file `[path to project directory]/build/monitor` from terminal and the shell program should start running.
