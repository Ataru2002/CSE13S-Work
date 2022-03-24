# Assignment 1 - Getting Acquainted with Unix and C

This program called plot.sh will generate 3 graphs: the Collatz Sequence Length dotted graph, the Collatz Sequence Maximum Value dotted graph, and the Collatz Sequence Length Histogram

##Building the program

Run this command to build collatz.c, the provided program used in plot.sh:

$ cc -o collatz collatz.c

Make sure the program works by running this command:

$ ./collatz -n (Any number you want, this will be the starting number of the your collatz sequence)

After you run this command you should see a line of numbers with the last number always being 1

##Running the program

in the command line type ./plot.sh to generate all 3 graphs.
After running the program, you should see 3 graphs with file names collatz.pdf being the collatz sequence length dotted graph, collatzMX.pdf being the collatz sequence maximum value dotted graph, collatzHis.pdf being the collatz sequencelength histogram. You will also see a file name test.sh, don't worry about this file since this is the file used to generate data from collatz.c C program
