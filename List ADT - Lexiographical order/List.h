/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA1 
* List.h
* Client View of List.c, has function prototypes, cleans up List.c file
***/ 

//provided breif explanation of the functions in the List.c file, and in Read.md
// functions come straight from pa1 assignment page


// included a typedef, which creates a pointer to a struct ListObj. new type = list
// this intalizes the List within each function, and allows the program to run
#include <stdbool.h>
typedef struct ListObj* List;
// Constructors-Destructors ---------------------------------------------------
List newList(void); 
void freeList(List* pL); 
// Access functions -----------------------------------------------------------
int length(List L); 
int index(List L); 
int front(List L); 
int back(List L);
int get(List L); 
bool equals(List A, List B); 
// Manipulation procedures ----------------------------------------------------
void clear(List L); 
void set(List L, int x); 
void moveFront(List L); 
void moveBack(List L); 
void movePrev(List L); 
void moveNext(List L); 
void prepend(List L, int x);
void append(List L, int x); 
void insertBefore(List L, int x);
void insertAfter(List L, int x); 
void deleteFront(List L); 
void deleteBack(List L); 
void delete(List L); 
// Other operations -----------------------------------------------------------
void printList(FILE* out, List L); 
List copyList(List L); 