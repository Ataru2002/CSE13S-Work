#!/bin/bash


make clean && make collatz   # Rebuild the collatz executable.

#counting array for the Frequency
freq=() 
#initialize the freq array with size 225 and an initial value for each element of 0
for i in {0..225}
do
	freq[$i]=0
done


#Loop for both the Collatz Sequence Length and the Collatz Sequence Length Histogram
iter=2
> /tmp/collatz.dat
for i in {2..10000}
do
	./collatz -n $iter > test.sh
	# Counting the lines in test.sh
	cnt=`wc --lines < test.sh`
	printf "%i\n" $cnt >> /tmp/collatz.dat
	# Tallying up the lengths 
	((freq[$cnt]++))
        > test.sh	
	((iter++))
done

#For loop to print the value of freq array into the file to be ready for gnuplot
> /tmp/collatzFreq.dat
for i in {1..225}
do
	echo ${freq[$i]} >> /tmp/collatzFreq.dat
done

#For loop to find the maximum value of each collatz sequence starting from numbers from 2 to 10000
iter=2
> /tmp/collatzMX.dat
for i in {2..10000}
do
	./collatz -n $iter > test.sh
	# Sort and get first line for the maximum value of each collatz sequence
	mx=`sort -rn test.sh | head -n 1`
	printf "%i\n" $mx >> /tmp/collatzMX.dat
	>test.sh
	((iter++))
done


# This is the heredoc that is sent to gnuplot.
# This gnuplot is for the Collatz Sequence Length dotted graph
gnuplot <<END
    set terminal pdf
    set output "collatz.pdf"
    set title "Collatz Sequence Length"
    set xlabel "n"
    set ylabel "length"
    set zeroaxis
    plot "/tmp/collatz.dat" with dots
END

#This gnuplot is for the Collatz Sequence Maximum Value dotted graph
gnuplot <<END
    set terminal pdf
    set output "collatzMX.pdf
    set title "Collatz Sequence Maximum Value"
    set xlabel "n"
    set ylabel "value"
    set yrange [0: 100000]
    set zeroaxis
    plot "/tmp/collatzMX.dat" with dots
END

#This gnuplot is for the Collatz Sequence Length historgram
gnuplot << END
   set terminal pdf
   set output "collatzHis.pdf"
   set title "Collatz Sequence Length Histogram"
   set xlabel "length"
   set ylabel "frequency"
   set xrange [0:225]
   set yrange [0:200]
   plot "/tmp/collatzFreq.dat" with impulses
END


