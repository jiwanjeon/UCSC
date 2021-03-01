# Assignment 6

This program implements the Bloom Filter, Hash Table, Linked List and Bit Vector ADTs. Program reads "badspeak.txt" and "newspeak.txt" files, insert a words from these files to Bloom Filter and Hash Table and then reads and parses input stream and tests all word to see if a word has been added to the Bloom filter. A message containing words from a hash table and not having a translation ("badspeak") is treated as a thoughtcrime. Those words that have a translation ("oldspeak") should be considered as subject to replacement with "newspeak".

## Building

This program uses the Makefile for compilation.

The Makefile has several options:

1. **all**: produce **banhammer** executable
2. **clean**: remove executable
3. **format**: format source and heder files

## Running

The program accepts the following command-line options:

* -h size   : specifies that the hash table will have size entries (the default will be 10000).
* -f size   : specifies that the Bloom filter will have size entries (the default will be 2^20).
* -m        : will enable the move-to-front rule.

For example:
> ./banhammer -h 1000 -f 1048576 -m

The program accepts input from the keyboard. Therefore, you must use input redirection to read from a file.
> ./banhammer < input.txt
