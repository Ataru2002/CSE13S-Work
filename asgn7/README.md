# Assignment 7 - Author Identification 

The program will try to identify the author with the most similarity to the subject text

##Building the program

Run this program to compile all the files correctly

$ make

##Running the program

When you run the program it's important to know what the commands are. To do that, each program will consist of a synopsis that will help. The command for the synopsis is -h:

./identify -h

After you read the synopsis you will need to come up with a message then run ./identify. An example is:

./identify -k 10 < (a text)

After running the program you should see a list of authors that are the closest to the subject text, for the code above a list of top 10 authors should be displayed

##Problems encountered

When running valgrind it took me a while to fix the memory issue because the program is taking a considerable time to run. Furthermore, running scan-build display an error showing the variables in metric names are not used, I fixed this issue by specifically running clang when running scan-build
