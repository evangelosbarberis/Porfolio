/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA4
* ListTest.c
* Tests List.c and List.h functions to make sure they work properly.
* breif summary of functions, and functionality of each function, used print statements per each function to test functionality.
***/ 

#include<stdio.h>
#include<stdlib.h>
#include"List.h"
int main(){
   // newList - 3
   List L = newList();
   List M = newList();
   List N = newList();
   // give vals 1 to 10
   for(int i=1; i<=10; i++){
      append(L, (void*)(long)i);
      prepend(M, (void*)(long)i);
   }
   // Print length, front, and back of Lists L and M
   printf("Length L: %d\n", length(L));
   printf("Length M: %d\n", length(M));
   printf("Front L: %ld\n", (long)front(L));
   printf("Back  L: %ld\n", (long)back(L));
   printf("Front M: %ld\n", (long)front(M));
   printf("Back  M: %ld\n", (long)back(M));
   moveFront(L);
   // traverse  get front to back
   while(index(L) != -1){
      printf("%ld ", (long)get(L));
      moveNext(L);
   }
   printf("\n");
   moveBack(L);
   // Traverse List L from back to front and print its elements
   while(index(L) != -1){
      printf("%ld ", (long)get(L));
      movePrev(L);
   }
   printf("\n");
   printList(stdout, N);
   printf("\n");
    // Clear List L and print its length
   clear(L);
   printf("List L after clear: %d\n", length(L));
   // append vals 1 to 20
   for(int i=1; i<=20; i++){
      append(L, (void*)(long)i);
   }
   moveFront(L);
   for(int i=0; i<10; i++) moveNext(L); // move cursor to 10th element
   set(L, (void*)(long)0); // overwrite the element with 0
   printf("Element at index 10 in List L: %ld\n", (long)get(L));
   moveFront(L);
   //Insert elements -1 and -2 before and after the cursor respectively
   insertBefore(L, (void*)(long)-1);
   insertAfter(L, (void*)(long)-2);
   deleteFront(L);
   deleteBack(L);
   moveFront(L);
   moveNext(L); 
   delete(L);
   printList(stdout, L);
   printf("\n");
   freeList(&L);
   freeList(&M);
   freeList(&N);
   return 0;
}