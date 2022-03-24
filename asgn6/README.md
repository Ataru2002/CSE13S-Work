# Assignment 6 - Huffman Coding

The program consists of encode.c and decode.c will perform a regular Huffman compression

##Building the program

Run this program to compile all the files correctly

$ make

##Running the program

When you run the program it's important to know what the commands are. To do that, each program will consist of a synopsis that will help. The command for the synopsis is -h:

./encode -h
./decode -h

After you read the synopsis you will need to come up with a message then run ./encode follow by ./decode. You can also run them simultaneously

./encode -i [input file] -o [output file] -v
./decode -i [input file] -o [output file] -v
./encode | ./decode (after that type in the message you want to compress then decompress

After running the 2 programs you will see the same message you originally typed in

##Problems encountered

When running valgrind it took me a while to fix the memory issue since I intentionally leave it there to fix it at the end. Fixing valgrind brings many instances of segmentation faults and I was able to fix them. Running scan-build doesn't produce any problems for me in this assignment
~                                                                                                                    
