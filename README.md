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
|          a          |
+---------------------+
|          b          |
+---------------------+
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

Here, we start an `std::ifstream` called `cardData`.
We pass in the folder name to read there.

Then, we initialize an empty card to add to the vector.
The first `if` is to check if the file is valid.
Then we start a `while` loop that continues until the end of file.

Here is an example of a card:

``` txt
-NEWCARD-
TYPE=CHANCE
DESC=ADVANCE TO THE NEXT RAILROAD. If UNOWNED, you may buy it from the Bank. If OWNED, pay the owner twice the rent to which they are otherwise entitled. IF YOU PASS GO, COLLECT $200
[FLAGS]
MOVETONEXTRAILROAD
PAYDOUBLEIFOWNED
COLLECTGOCASH
[PROPERTIES]
-NEWCARD-
TYPE=CHANCE
DESC=SPEEDING FINE. PAY $15.
[FLAGS]
MONEYCHANGE
[PROPERTIES]
MONEYCHANGE=-15
```

First we say to create a new card.

``` cpp
std::string next;
std::getline(cardData, next);
```

The first line is to store the current line.
Then we use `getline` to get the first line.

After that, we check if the card is a chance card.
We do this by splitting the line that contains the TYPE.
We split it by the `=` sign, then get the last word found, which is either `CHEST` or `CHANCE`.
Here is our `split()` function:

``` cpp
std::vector<std::string> functions::split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}
```

Our arguments are a string, and a delimiter.
The `&` next to the `s` means that we will be passing in a *reference* to the string.
First we initialize a `stringstream`.
Then we create a variable that stores the current object.
After that we create a vector of all the split words.
Then, we push every item to that array, and return the vector.

Back to our functions, we then get the next line, and set the description to what we found.
Then, we loop over each flag and property and add it to their respective variables.

Flags will be used to identify what the card will do.
For example, in our card above, we known that, since `MONEYCHANGE` is in the flags, that the user's cash will be changed.

After that, we call `shuffleDecks()`:

``` cpp
std::default_random_engine rng = std::default_random_engine {};
std::shuffle(this->chanceCards.begin(), this->chanceCards.end(), rng);
std::shuffle(std::begin(this->chestCards), std::end(this->chestCards), rng);
```

This does, exactly what it says it does, shuffles both decks.
First it creates a `default_random_engine`.
Then it shuffles the deck by getting the beggining and end of the decks, as well as the `"random"` part.

Then we create a new `board` and call `createPlots`:

``` cpp
std::ifstream plotData("plotData.txt");
plot::Plot plot({}, {}, {});
if (plotData.is_open()) {
    while (plotData.good()) {
        std::string next;
        std::getline(plotData, next); // Get [FLAGS]
        std::getline(plotData, next); // Get first flag (OR STRING_PROPERTIES)
        while (next != "[STRING_PROPERTIES]") {
            plot.flags.insert(next);
            std::getline(plotData, next);
        }
        std::getline(plotData, next);
        while (next != "[INT_PROPERTIES]") {
            std::vector<std::string> split = functions::split(next, '=');
            if (split[0] == "COLORCODE") {
                if (split[1] == "WHITE")
                    split[1] = functions::ANSI_WHITE;
                else if (split[1] == "BROWN")
                    split[1] = functions::ANSI_BROWN;
                else if (split[1] == "DARK_CYAN")
                    split[1] = functions::ANSI_DARK_CYAN;
                else if (split[1] == "CYAN")
                    split[1] = functions::ANSI_CYAN;
                else if (split[1] == "ORANGE")
                    split[1] = functions::ANSI_ORANGE;
                else if (split[1] == "MAGENTA")
                    split[1] = functions::ANSI_MAGENTA;
                else if (split[1] == "YELLOW")
                    split[1] = functions::ANSI_YELLOW;
                else if (split[1] == "RED")
                    split[1] = functions::ANSI_RED;
                else if (split[1] == "BLUE")
                    split[1] = functions::ANSI_BLUE;
                else if (split[1] == "GREEN")
                    split[1] = functions::ANSI_GREEN;
            }
            plot.stringProperties.insert({{split[0], split[1]}});
            std::getline(plotData, next);
        }
        std::getline(plotData, next);
        while (next != "-NEWPLOT-") {
            std::vector<std::string> split = functions::split(next, '=');
            plot.intProperties.insert({{split[0], stoi(split[1])}});
            getline(plotData, next);
        }
        this->plots.push_back(plot);
        plot = plot::Plot{{}, {}, {}};
    }
} else {
    functions::printlnRed("FATAL ERROR. CANNOT FIND OR READ FILE NAMED PLOTDATA.TXT");
    functions::printlnRed("PLEASE CHECK THAT THE FILE EXISTS WITHIN THE SAME DIRECTORY AS MAIN.EXE AND CHECK FILE PERMISSIONS");
    functions::readStringInput("PRESS ENTER TO ABORT.");
    exit(0);
}
```

This is essentialy the exact same as the create cards file, so I won't go over it.

Since every plot is different, we need to use `FLAGS` and properties.

Then we create a new player.
The `true` within the parenthesis is a parameter, basically saying that this will be the main player.

Then we print the intro:

``` cpp
functions::printlnBlue("Welcome to Monopoly!");
functions::printlnGreen("What is your name?");
player.name = functions::readStringInput(">");
```

In order to get the name, we call this function:

``` cpp
std::string returnString;
std::cout << prompt;
std::getline(std::cin, returnString);
return returnString;
```

This essentialy gets the user input using `std::cin`.

The we clear the screen and ask the users how many computers they want to play against using these function:

``` cpp
bool functions::tryParse(std::string &input, int &output) {
    try {
        output = stoi(input);
    } catch (std::invalid_argument) {
        return false;
    }
    return true;
}

int functions::readIntInput(std::string prompt, int min, int max) {
    std::string input;
    std::cout << prompt;
    int x;
    std::getline(std::cin, input);
    while (!tryParse(input, x) || x < min || x > max) {
        std::cout << "Enter a number between " << min << " & " << max << ">";
        std::getline(std::cin, input);
    }
    return x;
}
```

First, we store the input as a string.
Then we call `tryParse`.
This will check if the user inputted an integer.
To do that it uses a `try-catch` block.
First it tries to do everything in the `try` block.
If any exeptions occur, it jumps to the `catch` block.
In the `try` block, we convert the output into an integer using `stoi`.
If it can't do that (say the input is `asdfoasj`), then we return `false`.
Otherwise, we return `true`.
Back to our `while` loop, we also check if the number is in the range specified.
Then, back in our `main` functions, we create all those computers.
You may notice that I use `unsigned char` instead of `int`.
Take this table:

| Name                         | Bits | Range                           |
|------------------------------|------|---------------------------------|
| char                         | 1    | -128 to 127 / 0 to 255          |
| unsigned char                | 1    | 0 to 255                        |
| signed char                  | 1    | -128 to 127                     |
| int / signed int             | 4    | -2147483648 to 2147483647       |
| unsigned int                 | 4    | 0 to 4294967295                 |
| short int / signed short int | 2    | -32768 to 32767                 |
| unsigned short int           | 2    | 0 to 65,535                     |
| long int / signed long int   | 8    | -2,147,483,648 to 2,147,483,647 |
| unsigned long int            | 8    | 0 to 4,294,967,295              |
| long long int                | 8    | -(2^63) to (2^63)-1             |
| unsigned long long int       | 8    | 0 to 18,446,744,073,709,551,615 |

Since I know that `i` will never be as large as an `int`, I can save 3 bytes.
In reality, it does not matter.

Then, we add that many computers to a vector.
We also set their names.
Then we clear the screen.
