/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA1 
* ListTest.c
* Tests List.c and List.h functions to make sure they work properly.
* breif summary of functions, and functionality of each function, used print statements per each function to test functionality.
***/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"



//printlist is tested in every printing of list!!!!

int main() {
    // Test list elements, c is Null
    List A = newList();
    List B = newList();
    List C = NULL;


    // test append & prepend, addd front for A and backwards for B
    //gives each list 25 elements
    for (int i = 1; i <= 25; i++) {
        append(A, i);
        prepend(B, i);
    }
    // print each list, to test contents
    printf("A = ");
    printList(stdout, A);
    printf("\n");
    printf("B = ");
    printList(stdout, B);
    printf("\n");

    //find length of each list, to test length function
    printf("Length(A) =  %d\n", length(A));
    printf("Length(B) = %d\n", length(B));

    //tests front and back functions for list A and B
    printf("Front(A) = : %d\n", front(A));
    printf("Back(A) =  %d\n", back(A));
    printf("Front(B) = : %d\n", front(B));
    printf("Back(B) =  %d\n", back(B));

    //testing index functionallity here
    moveFront(A);
    printf("A at index =  %d value = %d\n", index(A), get(A));

    moveBack(A);
    printf("A placed = index %d value =  %d\n", index(A), get(A));

    movePrev(A);
    printf("A moved prev =  index %d value = %d\n", index(A), get(A));

    moveNext(A);
    printf("A moved next to back at index %d value = %d\n", index(A), get(A));

    // set function testing
    set(A, 42);
    printf("Set cursor to 42: ");
    printList(stdout, A);
    printf("\n");

    // boolean functions testing
    // also copy list testing here to see equivlanece
    C = copyList(A);
    printf("A equals C: %s\n", equals(A, C) ? "true" : "false");
    printf("A equals B: %s\n", equals(A, B) ? "true" : "false");

    // test move functions here and insert functions
    moveFront(A);
    moveNext(A); // move cursor to second position
    insertBefore(A, 72);
    printf("Insert 72 before second element: ");
    printList(stdout, A);
    printf("\n");


    insertAfter(A, 102);
    printf("Insert 102 after cursor: ");
    printList(stdout, A);
    printf("\n");

   
    deleteFront(A);
    printf("Delete front element = ");
    printList(stdout, A);
    printf("\n");

  
    deleteBack(A);
    printf("Delete back element = ");
    printList(stdout, A);
    printf("\n");


    moveFront(A);
    moveNext(A); // move cursor to second element
    delete(A);
    printf("Delete current element (cursor) = ");
    printList(stdout, A);
    printf("\n");

    // test clear function
    clear(A);
    printf("After clear(): ");
    printList(stdout, A);
    printf("\n");

    freeList(&A);
    freeList(&B);
    freeList(&C);

    return 0;
}
