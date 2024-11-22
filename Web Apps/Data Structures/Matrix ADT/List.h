/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA4
* List.h
* List.h holds all relevant functions for List.c
* edited only the int's to void* as per Matrix.h needs
***/

//provided breif explanation of the functions in the List.c file, and in Read.md
// functions come straight from pa1 assignment page


// included a typedef, which creates a pointer to a struct ListObj. new type = list
// this intalizes the List within each function, and allows the program to run

typedef struct ListObj* List;
// Constructors-Destructors ---------------------------------------------------
List newList(void);
void freeList(List* pL);
// Access functions -----------------------------------------------------------
int length(List L);
int index(List L);
void* front(List L);
void* back(List L);
void* get(List L);
// Manipulation procedures ----------------------------------------------------
void clear(List L);
void set(List L, void* x);
void moveFront(List L);
void moveBack(List L);
void movePrev(List L);
void moveNext(List L);
void prepend(List L, void* data);
void append(List L, void* data);
void insertBefore(List L, void* data);
void insertAfter(List L, void* data);
void deleteFront(List L);
void deleteBack(List L);
void delete(List L);
// Other operations -----------------------------------------------------------
void printList(FILE* out, List L);

