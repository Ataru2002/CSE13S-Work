# Assignment 3 - Sorting: Putting your affairs in order

The program called sorting.c will generate a random array from a certain seed specified by the command line and sort that array using 4 different sorting algorithms: Insertion Sort, Quick Sort, Batcher Sort, and Heap Sort. Furthermore, there will also be a statistic module to calculate the performance of each sorting algorithm in terms of number of moves and comparisons

##Building the program

Run this program in other to compile all files included by sorting.c

$ make

After that run this program in order to generate the synopsis and usage for the program

$ ./sorting -H

##Running the program
After you read the program synopsis and usage. You can choose to enabled any sorting algorithm of your choice (even all 4) while also customize the size of the array, how many elements will be printed out, and the seed to generate the numbers. Here's and example of a command you can run:

$ ./sorting -a -n 100 -p 15 -r 123

##Problems encountered
When implementing Heap Sort, there was a little detail I messed up that lead to the program not sorting the array properly. Ultimately, I ended up spending half a day to fix that small mistake
Additionally, I ran scan-build and noticed that I am using "int" instead of "uint32_t" for all of my for loops and I fixed them as a result.


