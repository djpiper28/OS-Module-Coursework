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

## Assignment Feedback

I chose to omit a linked list structure that contains the list, head, tail and, the size as that
information is derived from the list nodes. The addition of a list structure may be useful for asserting
thread safety however, for the assignment I saw no need as the entire program can be single threaded.
Instead, I created a virt\_mem structure which stores the head, tail, left, right, size and, free nodes of
the virtual memory that is being simulated.

The implmentation notes in the PDF file seemed patronising as there was a lot of poor code showing me how
to do it. I have written a fair bit of C so, I decided that I would devise an implementation from the 
bottom up with TDD, a more prodcedural approach and lastly, with custom tools for testing.

Whilst it is true that I should follow the specification for the program, as a coursework the task seemed
like another lab sheet. I heard that last year students got to create their own shell, I have not made a 
shell before however I imagine that unlike a doubly linked list it requires a lot of thought. The lab sheet
style of the assignment seems like a poor choice as it removes the thinking which is the core part of
programming. Approaching a problem in a C style is a skill which is entirely missed here. OO programmers will
end up trying to write OO and, throw exceptions which is not what C is about. 

Furthermore, the tools which are recommended are not perfect and, not used properly. Valgrind is a specialist
tool for checking for memory errors, race conditions and, various other bugs. However without compiling an
executable with debug information the tool is useless. The tool without flags to track leak sources is also
useless. Valgrind is a dumb tool, it needs information to give usable output. A stack filled with question
marks is useless. Whereas, a stack that has function names, line numbers and, file names is useful as the
error's cause can be found. Valgrind is also not a debugger. Segmentation faults or sigabrts can be hard to 
debug, especially when the contents of memory are a mystery. I think that students should be taught to use
gdb so that they can create breakpoints, observe and change memory and, step through their code. The `-g`
flag should be used with gcc so that debug information is in the executable.

Overall the hardest part of C is writing a part of a program from the ground up with foresight of how the
parts fit together. The software engineering coursework had very flawed, incomplete UML which required a
knowledge of design patterns to fix however, here the solution is given on a golden plate. Lastly, the 
code that I wrote is far longer then it should be however the code I wrote handles failed
mallocs, reallocs and, IO operations. From following the OS module lab sheets I do not feel that these
essential skills were taught to people. Error handling in C is complex however not being taught it does not
allow people to learn how to. In C there is jump based (goto) error handling, error codes and, signals. Each
of these is a nuanced skill and important to learn as various organisations use different standards. For
example `libpng` uses jump based error handling and, `stdio` uses error codes.

