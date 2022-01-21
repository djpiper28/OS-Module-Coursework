# read-me

## Requirements

A POSIX compliant OS is required along with some build tools. Below is a list of tools used with a
breif explanation of their purpose.

## Performance

A lot of malloc calls can be made, especially because I made a linked list that stores `void *` and
assing an allocated `int *` to each node when it is populated. However this is fine because a linked list
that takes `void *` is reusable which is important for any code. For a high performance application, a 
linked list is not always a good idea and an array can often be faster as there will be less malloc calls.

### Tools

| Tool | Usage |
|---|---|
| python3 | Running automated memory checks with valgrind |
| valgrind | Memory checking |
| gdb | Debugger |
| gnu make | Automated building and testing |
| gcc | C compiler |
| astyle | Code formatter |
| git | Version control |

## Compiling

The command `make` compiles an executable file called `dlist`.

| Command | Usage |
|---|---|
| `make` or `make main-src` | make the production executable |
| `make test` | make the tests then run them |
| `make format` | formats the source code |
| `make clean` | remove all build files, debug files and, temporary files |
| `make step1` | make and run step_1.c |
| `make step2` | make and run step_2.c |
| `make step3` | make and run step_3.c |


## Testing

The command `make test` compiles the tests and runs them. An error will appear if a test fails.
Running the tests will also generate code coverage reports using gcovr, an html and an xml report
are made.

I made my own testing framework that uses a few macros to allow for unit testing. I could have used
gtest or cppunit however, I decided that I wanted to play with the `__LINE__` and `__FILE__` macros.

Testing will produce some debug files that are used by gcovr, `make clean` removes these files.

### Valgrind Memory Checks

The tests are run twice, once with and once without valgrind - this is to check sanity. The memory
checks are forr open file descriptors, memory leaks and, out of bound reads/writes. If there are any
issues you probably want to use gdb to investigate the flow of the program and the value of variables.

I wrote a python script for the automation of checking valgrind for memory leaks as memory that leaks
during runtime can lead to disaster.

## Source Code

Source code is written in ANSI C99 (enforced using `-std=gnu99` as a GCC argument). It includes 
all of the .h and .c files. Files are formatted with the Kerningham and Ritchie style with astyle.

### Compiler Flags

The `-Wno-unused-variable` flag is used because I have function pointers to allow for my doubly linked
list to store `void *` which is freed automatically by the user-defined function given to each node.
Normally this is not an issue but `-Werror` causes an unused variables (intentional or not) to cause the
compiler to error and stop.

### Build System

I decided to use GNU Make instead of CMake because the project is small and, I want to use GNU Make more
as it is still a very popular tool. Whilst CMake is more popular than CMake, I have used it in the past
and it seems overly complicated for the purpose of a small project. Custom scripts seemed easier for this
rather than configuring other people's CMake scripts.

### File Structure

| Folder | Contents |
|---|---|
| `./` | Build scripts and, readme |
| `./src/` | Source code for the program |
| `./test/` | Unit tests for the source code |
| `./.git/` | VCS Files |

Source code in `./src/` and `./test/` have definitions in their respective `.h` header files except for
the files with the main functions (`./src/main.c` and, `./test/test.c`).
