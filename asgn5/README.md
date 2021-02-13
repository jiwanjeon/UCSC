# Assignment 3

This program measures performance of 4 algorithms: Bubble Sort, Shell Sort, Quicksort and Heapsort.

## Building

This program uses the Makefile for compilation.

The Makefile has several options:

1. **all**: produce **sorting** executable
2. **clean**: remove executable
3. **format**: format source and heder files

## Running

The program accepts the following command-line options:

* -a : Employs all sorting algorithms.
* -b : Enables Bubble Sort.
* -s : Enables Shell Sort.
* -q : Enables Quicksort.
* -h : Enables Heapsort.
* -r seed : Set the random seed to seed. The default seed should be 7092016.
* -n size : Set the array size to size. The default size should be 100.
* -p elements : Print out elements number of elements from the array. The default number of elements to print out should be 100.
* -O : Creates ordered data (ascending order)
* -R : Creates reversed ordered data (descending order)
* -E : Creates data in which all elements are equal.

For example:
> ./sorting -bq -n 1000 -p 0
or
> ./sorting -bq -n 15 -r 2021
or
> ./sorting -a -n 10000 -p 0 -O
