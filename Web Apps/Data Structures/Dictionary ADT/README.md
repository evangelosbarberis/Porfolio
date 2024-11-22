Evangelos Barberis
ebarberi
CSE 101 Winter 2024 PA8
README.md

# Introduction
A continuation of PA7's Dictionary ADT, howver with Red and Black Tree implementation added to it. Additional helper functions and imrpooved implementation was added to DIctionary.cpp, and Dictionary.hy includes new helper functions. Order.cpp reamained unchanged from PA7.

# Files

## Dictionary,h and Dictionary.cpp
Dictionary.h is the header file for PA8 functions, overall really similar to Dictionary ADT from PA7 however with red and black tree implementation and functions added. Dictionary.cpp is the implementation file, please refer to notes from there for more specific information.

## DictionaryTest.cpp
Tester file for Dictionary.h functions, remains similar to one prescribed in handout, just slight changes to test every function in my file occordingly.

## WordFrequency.cpp
This file reads User input file, parses the user input, and places those strings into keys. It reads the user inputs and counts the frequency the word is read!

## Order.cpp
Client view, takes input val of strings, and inserts into a dict. Produces a key in alphaebtical order, and a representation of keys in preorder. - Remains unchanged from PA7

## Makefile
Makes the executable, had to add some functionality to it to produce an Order.cpp executable and create the DictionaryTest.cpp file.