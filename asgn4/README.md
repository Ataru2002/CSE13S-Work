# Assignment 4 - The Game of Life

The program called life.c will utilized the inputs specified by the either the file you add for the input or the data you add in the standard input and from the inputs, simulates the Conway's Game of Life

##Building the program

Run this program in other to compile all files included by life.c

$ make

##Running the program
When you run the program you have the option to make the game of life run on a toroidal universe (-t), disabled the display (-s), customizing the number of generations (-n), the input file (-i), and the output file (-o). An example would be:

$ ./life -t -n 100 -i gospergun.txt

##Problems encountered
When running valgrind it took me a while to fix the memory leak problem since my destructor function is incorrectly implemented
Additionally, when I run scan-build I had some problems with the unsigned int because I made an if statement to specified whenever they are negative or not. I also encountered a problem with ncurses from running scan-build even though I have included the necessary links, I had to fix the problem by specifically make scan-build use clang
