# CPP-Monopoly

2080 lines of code written by me.
934 lines of data written by me.
141 lines of code not written by me.

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

Then we create a `namespace` that is called `functions`.
In order to access items of a namespace, you must use `::`.
For example, if we had a namespace called `ns` and a value called `vl`, you would do `ns::vl`.
Within our namespace, we declare `clear()`.
In our `functions.cpp` file, we `include` our header file, then later initialize `clear()` like this:

``` cpp
void functions::clear() {
    std::cout << "\033[2J" << std::endl;
}
```

## ANSI Escape Codes

`std::cout` and `std::endl` are part of the `<iostream>`, which is included above.
What we do here is first create an `std::cout`.
This starts writing to the console.
Then we use a `left shift` operator to print `\033[2J`.
Then we use `std::endl` to add a `\n` to the endl of the line.
This also flushes the console in order to stop weird behavior.
What we are printing is called an `ANSI Escape code`.
The `/033` initializes the code.
Then `[2J` clears the entire console screen.
While we are here, lets go over some other ANSI sequences.

In our `functions.hpp` file, we initialize lots of variables:

``` cpp
std::string ANSI_RESET     = "\033[0m";
std::string ANSI_RED       = "\033[31m";
std::string ANSI_GREEN     = "\033[32m";
std::string ANSI_YELLOW    = "\033[33m";
std::string ANSI_BLUE      = "\033[34m";
std::string ANSI_MAGENTA   = "\033[35m";
std::string ANSI_CYAN      = "\033[36m";
std::string ANSI_WHITE     = "\033[37m";
std::string ANSI_BROWN     = "\033[38;5;52m";
std::string ANSI_DARK_CYAN = "\033[38;5;23m";
std::string ANSI_ORANGE    = "\033[38;5;202m";
std::string ANSI_PURPLE    = "\033[38;5;127m";
```

These are all ANSI Escape sequences.
The first one is used to stop all ANSI sequences.
The other ones start printing with color in the console.
In our `functions.cpp` file, we have these functions:

``` cpp
void functions::printlnRed(std::string str)     {std::cout << functions::ANSI_RED     << str << functions::ANSI_RESET << std::endl;}
void functions::printlnBlue(std::string str)    {std::cout << functions::ANSI_BLUE    << str << functions::ANSI_RESET << std::endl;}
void functions::printlnGreen(std::string str)   {std::cout << functions::ANSI_GREEN   << str << functions::ANSI_RESET << std::endl;}
void functions::printlnYellow(std::string str)  {std::cout << functions::ANSI_YELLOW  << str << functions::ANSI_RESET << std::endl;}
void functions::printlnMagenta(std::string str) {std::cout << functions::ANSI_MAGENTA << str << functions::ANSI_RESET << std::endl;}
void functions::printlnCyan(std::string str)    {std::cout << functions::ANSI_CYAN    << str << functions::ANSI_RESET << std::endl;}
```

Let's take the first functions, `printlnRed`.
It takes a string as an argument, which is what we will be printing.
Then, we start the printing process.
After that, we tell the console to start printing everything in red.
Then we print the string.
Then we stop printing in red and flush the console.

We will be using these functions a lot within this project.

Now back to our `main.cpp` file.

``` cpp
card_managment::CardManagment cardManager;
```

In our `cardManagment.hpp` file, we initialize a class called `CardManagment`.
Here we are creating a new variable called `cardManager` that is a class of `CardManagment`.
This adds the class to the stack.
You may be familiar with creating classes like this:

``` java
CardManagment cardManager = new CardManagment();
```

In C++, we *can* do this:

``` cpp
CardManagment* cardManager = new CardManagment();
```

Here we create an object on the heap.
In the stack, we have a pointer to the object on the heap, which is what the `*` is for.
Pointers are a huge hassle, so we will go over them later.

Next we call `cardManager.createCards()`:

``` cpp
std::ifstream cardData("cardData.txt");
card::Card card(true, "", {}, {});
if (cardData.is_open()) {
    while (cardData.good()) {
        std::string next;
        std::getline(cardData, next);
        card.isChance = functions::split(next, '=')[1] == "CHANCE";
        std::getline(cardData, next);
        card.description = functions::split(next, '=')[1];
        std::getline(cardData, next); // Get [FLAGS]
        std::getline(cardData, next); // Get first flag (OR PROPERTIES)
        while (next != "[PROPERTIES]") {
            card.flags.insert(next);
            std::getline(cardData, next);
        }
        std::getline(cardData, next);
        while (next != "-NEWCARD-") {
            std::vector<std::string> split = functions::split(next, '=');
            card.properties.insert({{split[0], split[1]}});
            getline(cardData, next);
        }
        card.isChance ? this->chanceCards.push_back(card) : this->chestCards.push_back(card);
        card = card::Card(true, "", {}, {});
    }
}
```

## Vectors

Since there are so many cards in Monopoly, I put all the data of the cards inside a `.txt` file.
Here, we are parsing this file to put the data into a `vector` of cards.
Think of a `vector` as an array that's size can be modified.

Take this array:

``` cpp
char[] str = {'a', 'b', 'c'};
```

Here we create an array of characters, otherwise known as a string.
This is what it looks like in memory:

``` txt
+---------------------+
|          a          | ---+
+---------------------+ <--+
|          b          | ---+
+---------------------+ <--+
|          c          |
+---------------------+
|       variable      |
+---------------------+
|                     |
+---------------------+
|                     |
+---------------------+
|                     |
+---------------------+
|                     |
+---------------------+
```

Now, lets say that `str` is a `vector`, and we add `d`.
Another variable is occupying the next space, so C++ has to manually move the memory:

``` txt
+---------------------+
|                     |
+---------------------+
|                     |
+---------------------+
|                     |
+---------------------+
|       variable      |
+---------------------+
|          a          |
+---------------------+
|          b          |
+---------------------+
|          c          |
+---------------------+
|          d          |
+---------------------+
```
