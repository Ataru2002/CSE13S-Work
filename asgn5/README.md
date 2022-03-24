# Assignment 5 - The Game of Life

The program consists of keygen.c, encrypt.c, and decrypt.c and it's main job is to perform an RSA encryption

##Building the program

Run this program to compile all the files correct

$ make

##Running the program

When you run the program it's important to know what the commands are. To do that, each program will consist of a synopsis that will help. The command for the synopsis is -h:

./keygen -h
./encrypt -h
./decrypt -h

After you read the synopsis you will start generating the keys, encrypting the messages, and decrypting the messages. Here are some examples of the commands used to perform a regular RSA encryption

./keygen -b 64 -i 1000 -v
./encrypt -i [input file] -o [output file] -v
./decrypt -i [input file] -o [output file] -v

##Problems encountered

When running valgrind it took me a while to fix the memory issue since I forgot to clear out all the mpz variables after I initialized it
Running scan-build doesn't produce much problems since all I need to fix is some small syntax mistakes.
