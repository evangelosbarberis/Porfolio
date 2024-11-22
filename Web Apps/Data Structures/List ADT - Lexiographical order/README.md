Before I began, this is my first time writing a README.MD file!!! So, I am a bit new to this!

Anywho, a breif introduction to my project!

# Introduction
Hi, I am Evangelos Barberis! My project that I am writing about is essentially a short sorting algorthim that takes a user input via file, and a user destination file, in which the program would sort words in there into lexiographical order (alphabetical order). This project is part of the UCSC CSE101 course. There were a total of three main files that I (semi)-wrote for this project (by semi I mean functions and functionality were included), that included a header file called List.h, a List.c file, and a file that held the main alogrothim called Lex.c. Additonally, I included a Makefile that was used to compile the project, and a ListTest.c file that was used to test the functionality of List functions. I will dive into a breif synopsis of each file and the functionality of each function!

## List.h
List.h is a very self explanatory file, which was gievn to us in whole in the project guidliness! This file is a header file that holds the client view of the functions in the List.c file. It is basically a bridge amongst all files, it holds the main defintions an macros to be shared amongst all project files. In terms of this project, the header file held all the main defintions in regards to the list functionallity, since our main alogrithim file, Lex.c, uses it to sort through a list and create a new list of sorted words.

## List.c
List.c is essentially the bread to List.h's butter, since it holds all the functions functionallty. This file for me took the longest in time, and also to conceptually understand, since it created a doubly linked list for us to jump around object to object in order to create main functionallity of its definitions. I think the biggest hurrdle for me was defintily the conceptual idea of a cursor in terms of creating a program that allows for us to use a cursor to move object to object, and allow us to delete items and add items bi-directionally. But needless to say, I got it at the end of the day!
Its quite a lengthy file, however its the most important file in the project, since it holds the main functionality of the project, and sets up the doubly linked list for us to use.

## Lex.c
Lex.c is the main alogorthim for this prorgam. For this I essentially took a user input file, and stored its items into an array of strings, and used the Linked list from List.c to sort the array of strings into lexiographical order, and then stored the sorted array of strings into a new file, which of course the user inputs! This main file incompanies dynamic memory allocation, as we allocate memory and free memory for our list, an an insertion alogorthim that was implemented with a linked list!
What is an insertion alogrithim? - https://www.geeksforgeeks.org/insertion-sort/ - where I got better knowldege of how to implement. 

## Makefile
The makefile was given to me for this project, but essentially it creates the executable file, which in this case was called Lex. It also creates the binary files for Lex.c and List.c! We use this make file to compile our prorgam, and execute it, using that excutable file.
I am still learning the syntax of Makefile, but it sets up the basic rules of execution and building process.

## ListTest.c
This file is where we test our List.c funtonallity. Essentially I created 3 lists, 1 empty, and 2 others with a differnet order, one deceneding and one increasing. I took these files and sent them through each function, printed the outcome, and observed whether or not it worked correctly. 
I mainly used the test cases provided to us in the testing script, but this method helped me see if my List.c file functions were working correctly as I went on!

### Thanks
Thanks for reading this! I hope you enjoyed my project! I know I suck an explaning things, but I actually had a bit of fun creating this and learning more about ADT's in the process, it was honestly a good refresher and learning expirence! Thanks!