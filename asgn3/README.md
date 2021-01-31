# Assignment 3

This program implements the "Game of Life".

## Building

This program uses the Makefile for compilation.

The Makefile has several options:

1. **all** or **life**: produce **life** executable
2. **clean**: remove executable and *.o files
3. **format**: format source and hedeer files

## Running

The program accepts the following command-line options:

* -t : Specify that the Game of Life is to be played on a toroidal universe.
* -s : Silence ncurses. Enabling this option means that nothing should be displayed by ncurses.
* -n generations : Specify the number of generations that the universe goes through. The default
number of generations is 100.
* -i input : Specify the input ﬁle to read in order to populate the universe. By default the input
should be stdin.
* -o output : Specify the output ﬁle to print the ﬁnal state of the universe to. By default the output
should be stdout.

For example:
> ./life < input.txt

is the simplest command line that uses input redirection and prints the result to the screen.

More complex command:
> ./life -i input.txt -n 99 -s -t -o output.txt
