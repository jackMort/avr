#!/bin/bash
make clean
make run&

while true;
do 
	cat plot.cnf | gnuplot && sleep 10
done
