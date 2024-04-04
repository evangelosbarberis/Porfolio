/*** 
* Evangelos Neo barberis 
* ebarberi 
* 2024 Winter CSE101 PA1 
* Lex.c 
* Uses contents from List.c to sort words alphabetically 
* Algorithm will run through the characters in file and send them back to List.c 
***/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#define MAX_LEN 1024

// insertion sort alogorthim, sorts words in lexiographical order, prints to output file

int main(int argc, char *argv[]) {
    FILE *input, *output;  //ptrs for input and output files
    char line[MAX_LEN];  // array = stores read lines
    int lineCount = 0, i; // # of lines / loop vari
    char **array; // stores lines
    List list; // linked list to sort array

    // check command line arguments
    //open in and out files
    // != print error
    if (argc != 3 || !(input = fopen(argv[1], "r")) || !(output = fopen(argv[2], "w"))) {
        fprintf(stderr, "Error opening files\n");
         exit(1);
    }
    // read in file
    while (fgets(line, MAX_LEN, input)) lineCount++;
    // alloocation
    array = calloc(lineCount, sizeof(char*));
    // allocate for each string in array
    for (i = 0; i < lineCount; i++) array[i] = calloc(MAX_LEN, sizeof(char));

    // start over for reading in file
    rewind(input);
    // read lines and copy to array
    for (i = 0; fgets(line, MAX_LEN, input); strcpy(array[i++], line));

    // create new empty list for sorting values to be stored in
    list = newList();
    // if lines, initalize witrh index of 1st item
    if (lineCount > 0) append(list, 0);
    // go through array and put indicies into list to be used for insertions sorting
    for (i = 1; i < lineCount; i++) {
        moveFront(list);
        while (index(list) >= 0 && strcmp(array[get(list)], array[i]) < 0) {
        moveNext(list);
    }
    //if curosr is valid, insert before
        if (index(list) >= 0) {
        insertBefore(list, i);
        }   else {
         append(list, i);
        }
    }

    //go to front of list 
    moveFront(list);

// Print the sorted array, traverse the list
    while (index(list) >= 0) {
        fprintf(output, "%s", array[get(list)]);  // list index
        moveNext(list);  // move next to list
    }

// Close input and output files
    fclose(input), fclose(output);

// Free memory
    freeList(&list);
    for (i = 0; i < lineCount; free(array[i++]));
    free(array);
    return 0;
}
