Evangelos Barberis

ebarberi

2024 Winter CSE 101

ReadMe.md 

# Introduction
A quick introduction:
Welcome to my implementation of PA4 for CSE101, this is a programming assignment that creates on implementation of Matrix ADT, that performs matrix operation (essentially a matrix calc). It creates a matrix, and only stores non-zero numbers which is abriviated as NNZ. The program is built upon the List ADT that we created in PA1, which is slightly updated for the Matrix ADT. The Matrix ADT creates the Matrix calcuations!

## Files

### Matrix.c and Matrix.h
To start Matrix.h uses the funtions as requested per PA4 handout, wixh holds the defentions we were to implement in Matrix.c.

Matrix.c is our created implementation of Matrx.h, it creates the Matrix ADT we used to Sparce and create the Matrix calculator. It creates operations on Matrices. It also introduces the Entry object, which defines and represents the Matrix elements, to learn more look at the Matrix.c file. It also uses the List ADT to represent rows of a Matrix, this in terms allow us to track the NNZ elements for sparcing!

This is where I learned more about Matrix ADT -https://www.geeksforgeeks.org/matrix/ 



### List.c and List.h
List.h is the bakcground for the List ADT, and holds all defentions for the List ADT. Something to note is that I kept most of it the same from PA1, however I updated it slightly to follow along with Matrix.c specifications. For instance the int vals were replaced with void* and some functions were removed due to us not needing them. Some of those functions include bool equals, deleted because int equals was defined in Matrix.h, and copyList was deleted because we created a Matrix Copy functionality as per Matrix.h handout. 

List.c holds the main functionality of the List ADT, and has not changed since PA1. Just updated slightly to follow the updated List.h file. But as a breif note, it creates a doubly linked list, that allows us to move accross the list as well manipulating Lists. It is the backbone for Matrix ADT so this is a very important file that allows us to create the main functionality for the Matrix files.

### Sparce.c
This is the primary file for this program, it takes user input and output files, that reads matrices from user inputed files (.txt), to execte matrix commands and opertaions. It utilizes our Matrix ADT to perform scalar multiplication, addition, transposition, and product operations. 

This is the bread and butter of our program, to find more about the functionality please refer to the file.

### Makefile
The Makefile comes directly from tha PA4 handout, and creates the executable files for our Matrix ADT and Sparse. Its a crucial file that brings functionallity to the plate.

### Test Files - MartixTest.c and ListTest.c
The MatrixTest.c and ListTest.c files are just generic files, mainly taken from the PA4 handout that test the functions from List.c and Matrix.c to assure they work accordingly. Its cruical to product testing, and to assure the program runs effectly and effectively. 