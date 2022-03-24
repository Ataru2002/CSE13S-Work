# Assignment 2 - Numerical Integration

The program called integrate.c is used to calculate the approximated value of an integrated value from the 10 functions specified, the program also use mathematical expressions that are coded in and not from a standard library

##Building the program

Run this program in other to compile 3 files integrate.c, mathlib.c, and functions.c at the same time

$ make

After that run this program in order to generate the synopsis and usage for the program

$ ./integrate -H

##Running the program
After you read the integrate.c synopsis and usage, you will choose any kind of function that you want to integrate while also set the low value of the interval, the high value of the interval, and the number of partitions. Here's an example of a command:

$ ./integrate -a -p 0.0 -q 1.0 -n 1000

##Problems encountered
When doing the integration for function sin(x) / x, we encoutered the problem where x becomes equal to 0 which result in a division by 0. I fix this problem by calculating the limit of sin(x) / x as x goes to 0, which is found to be 1.
 
