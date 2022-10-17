# CPP-Monopoly

## Sources

* [Stack Overflow](https://stackoverflow.com/)
* [Geeks for Geeks](https://www.geeksforgeeks.org/)
* [Tutorials Point](https://www.tutorialspoint.com/index.htm)
* [Techie Delight](https://www.techiedelight.com/)

## Languages used

* C++
* Header++

## Setup

First, you must activate ANSI escape sequences.

Open your command prompt by pressing `Windows+R` then type in `cmd`.

After that, enter this:

`REG ADD HKCU\CONSOLE /f /v VirtualTerminalLevel /t REG_DWORD /d 1`

The command prompt will respond with `The operation completed successfully`

Then, open the `main.exe` file in the folder `src`.

## C++

C++ is the language primarily used in this project.
It was made due to the original language C lacking classes and other features.
C is considered the "Mother of all programming languages," due to being used in the compilers of most languages.
C does not have many of the features that we take for granted today:

* Strings
* Classes
* For each
* Namespaces
* Try catch
* Dynamic arrays (vectors)
* Operator Overloading

One of the biggest things that C/C++ lack is garbage collection (C++ has *some* garbage collection, but C has none).
Garbage collection is where the program scans the stack to check for pointers to objects in the heap.
If an object is not in use, it deletes it and frees up the memory.
In C, you have to do that manually.

C++ is also notoriously complicated (mostly due to implementing pointers, which I'll go over later).
It is also known for being extremely fast.
C++ is about 10 times faster than Python.
The tradeoff, though, is that C++ takes very long to compile.
When you run a Python script, it almost automatically runs, however C++ can take a very long time to compile.
Another thing that C/C++ include is header files (.h/.hpp)
Header files are where we declare functions, and .cpp files are where we initialize them.
I'll go over that later.
C++ starts running from the `main.cpp` file in `src`, so we'll go there and trace through each file.

## #include

The first things we have in `main.cpp` is this:

``` cpp
#include "player.cpp"
#include "board.cpp"
#include "functions.hpp"
#include "card.cpp"
#include "cardManagment.cpp"

#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include <windows.h>
```

The first batch of `#include` statements use quotes to get files in the current directory.
These are files that I have created.
The next batch use brackets, which means that the compiler searches for things in libraries (things that I have not made).
When we write `#include` statements, think of it as completely pasting whatever the contents of the file are in that location.

Next, we have two functions, but lets skip that and just go to the `main` function.

``` cpp
functions::clear();
```

This is the first line of our `main` function.
Here, we are calling the `clear()` function from the `functions` namespace.
The `functions` namespace is initialized in the `functions.cpp` file.
First however, lets go to the `functions.hpp` file, where `clear()` is declared.
To start, we have these two lines:

``` cpp
#ifndef SRC_FUNCTIONS_H_
#define SRC_FUNCTIONS_H_
```

And at the end we have this:

``` cpp
#endif
```

These are what are called `include guards`

When a `.hpp` file is included, it will first run the `#ifndef`.
That will check if `SRC_FUNCTIONS_H_` has been defined yet.
Since it has not been defined yet, it continue.
Then it `define`s `SRC_FUNCTIONS_H_`.
At the end of the file, it stops the `#ifndef`.
Then, when the `.hpp` file is included again, it will fail the `#ifndef` check, so the file will not run.
This is so that the functions within a header file will not be defined multiple times, but can be used multiple times.
