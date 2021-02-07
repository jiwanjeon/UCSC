# Assignment 4

This program demonstrates the Hamming Codes technique - encoding, decoding and error correction.

## Building

This program uses the Makefile for compilation.

The Makefile has several options:

1. **all**: build all executables (gen, dec and err)
2. **gen**: build generator only
3. **dec**: build decoder only
4. **err**: build error injector only
5. **clean**: remove all executables and *.o files
6. **format**: format source and hedeer files

## Running

All programs accept the following two command-line options:

* -i input : Specify the input file to read. By default the input is *__stdin__*
* -o output : Specify the output file to write. By default the output is *__stdout__*

Error injector (**err** program) accepts two other arguments:

* -e : Specify the error rate. By defualt 0.01. It must be between [0.0, 1.0]
* -s : Specify the seed. It must be a positive integer

Example usage and decoder statistics. Here diff uses the dash to represent stdin:
> ./gen -i frankenstein.txt | ./dec | diff frankenstein.txt -

Example to run all three programs (with error injection):
> ./gen -i frankenstein.txt | ./err -e 0.002 -s 2021 | ./dec -o out.txt
