/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA4
* List.c
* Completed List.c file, uses List.h functions
* Creates a List ADT and has functions to manipulate a List
* Completly same functionality since PA1 just updated with a void* as per PA4 prereqs
***/ 




#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"List.h"

//initalize list, making it doubly linked, bi-directional
typedef struct NodeObj{
    void* data;
    struct NodeObj* next;
    struct NodeObj* prev;  
} NodeObj;
//represents a node of the linked list
typedef NodeObj* Node;

typedef struct ListObj{
    Node front;
    Node back;
    Node curr;
    int length;
    int index; 
} ListObj;

//creates newnode, returns a pointer to its new node
Node newNode(void* data){
    Node N = malloc(sizeof(NodeObj));
    N->data = data;
    N->next = NULL;
    N->prev = NULL; 
    return(N);
}
//free from memory, returns NULL after freeing
void freeNode(Node* pN){
    if (pN != NULL && *pN != NULL){
        free(*pN);
        *pN = NULL;
    }
}
// Constructors-Destructors ---------------------------------------------------
//creation of new empty lits
List newList(void){
    List L;
    L = malloc(sizeof(ListObj));
    L->front = L->back = L->curr = NULL; // sets everurything to NULL
    L->length = 0; // sets length to 0, as no data in it
    L->index = -1;  // -1 index  because empty list
    return(L);
}

// free memory from linked list
void freeList(List* pL){
    if(pL!=NULL && *pL!=NULL) { 
        while(length(*pL) > 0) { 
            deleteFront(*pL); // delete elemets from front until empty
        }
        free(*pL); // free memory from list
        *pL = NULL;
    }
}
// Access functions -----------------------------------------------------------
// all have an error function, to check peramerts of each function call!
// comes from the List.h handoutr

// obtains the length of the list
int length(List L){
    if(L == NULL){
        fprintf(stderr, "Error: Length() = Null\n");
        exit(1);
    }
    return L->length;
}

// obtains the index of the current node
// returns -1 if no current node
int index(List L){
    if(L == NULL){
        fprintf(stderr, "Error: Index() = Null\n");
        exit(1);
    }
    if (L->curr == NULL) {
        return -1;
    }
    return L->index;
}

// obtains the data of the front node
void* front(List L) {
    if (L == NULL || L->length <= 0) {
        fprintf(stderr, "Error: front() = Invalid List / Empty\n");
        exit(1);
    }

    return L->front->data;
}

// same logic as front back returns back element from list
void* back(List L) {
    if (L == NULL || L->length <= 0) {
        fprintf(stderr, "Error: back() =  Invalid List / Empty\n");
        exit(1);
    }

    return L->back->data;
}

//returns the cursor element
 // exit if no cursor, or invalid list
void* get(List L) {
    if (L == NULL || L->length <= 0 || L->index < 0) {
        fprintf(stderr, "Error: get() = Invalid List / Empty / N/A Cursor\n");
        exit(1);
    }

    return L->curr->data;
}

// checks if list is empty

// Manipulation procedures ----------------------------------------------------

// clears the list
void clear(List L){
    if(L == NULL){
        fprintf(stderr, "Error: clear() =  NULL\n");
        exit(1);
    }
    // delete all nodes from list by front until empty
    while(L->length > 0) {
        deleteFront(L);
    }
}

// sets the cursor to the current node
// exit if invalid, see the error messagge
void set(List L, void* x) {
    if (L == NULL || L->length <= 0 || L->index < 0) {
        fprintf(stderr, "Error: set()  = Invalid List / Empty / N/A Cursor\n");
        exit(1);
    }

    L->curr->data = x;
}

// moves cursor to front of list
void moveFront(List L){
    if(L==NULL){
        fprintf(stderr, "Error:  moveFront() = Null\n");
        exit(1);
    }
    if(L->length > 0) { 
        L->curr = L->front; // sets to from front here
        L->index = 0; // first elemet is index 0
    }
}

// same logic as moveFront, moves cursor to back of list
void moveBack(List L){
    if(L==NULL){
        fprintf(stderr, "Error:  moveBack() = Null\n");
        exit(1);
    }
    if(L->length > 0) {
        L->curr = L->back;
        L->index = L->length - 1;  // length -1 because back is last element
    }
}

// moves cursor to previous element
void movePrev(List L){
    if(L==NULL){
        fprintf(stderr, "Error:  movePrev() = Null\n");
        exit(1);
    }
    // logic here, if index is 0, then it is already at the front
    if(L->curr != NULL && L->index != 0){
        L->curr = L->curr->prev; // sets to previous element
        L->index--; // sets index to previous index
    } else {
        L->curr = NULL;
        L->index = -1;
    }
}

// same logic as above but moves cursor to next element
void moveNext(List L){
    if(L==NULL){
        fprintf(stderr, "Error:  moveNext() = Null\n");
        exit(1);
    }
    // same preliminary logic as above
    if(L->curr != NULL && L->index != L->length-1){
        L->curr = L->curr->next; // sets to next element
        L->index++; // sets index to next index
    } else {
        L->curr = NULL;
        L->index = -1;
    }
}


// adds element to front of list
// also updates cursor to new element
void prepend(List L, void* data){
    Node N = newNode(data);
    if( L->front == NULL) { 
        L->front = L->back = N; 
    }else{ 
        N->next = L->front;// adds here
        L->front->prev = N; 
        L->front = N;
    }
    if(L->index != -1) {
        L->index++; // update index here
    }
    L->length++; // also length is increased by 1!
}

//adds new node to end of list
void append(List L, void* data){
    Node N = newNode(data);
    if( L->front == NULL) { 
        L->front = L->back = N; 
    }else{ 
        L->back->next = N; // adds here
        N->prev = L->back; 
        L->back = N;
    }
    L->length++; // adds length by 1
}

// inserts new node before current node
// if at front it prepends data
void insertBefore(List L, void* data) {
    if (L == NULL || L->length <= 0 || L->index < 0) {
        fprintf(stderr, "Error: insertBefore() = Invalid List / Empty / N/A Cursor\n");
        exit(1);
    }

    if (L->index == 0) {
        prepend(L, data); // prepend if at front
    } else {
        Node N = newNode(data);
        N->next = L->curr;
        N->prev = L->curr->prev;
        L->curr->prev->next = N;
        L->curr->prev = N;
        L->index++;
        L->length++;
    }
}

// inmsert new node after current node
// if at back it appends data 
void insertAfter(List L, void* data) {
    if (L == NULL || L->length <= 0 || L->index < 0) {
        fprintf(stderr, "Error: insertAfter() = Invalid List / Empty / N/A Cursor\n");
        exit(1);
    }

    if (L->curr == L->back) {
        append(L, data);
    } else {
        Node N = newNode(data);
        N->prev = L->curr;
        N->next = L->curr->next;
        L->curr->next->prev = N;
        L->curr->next = N;
        L->length++; // add length by 1
    }
}

// delete front
void deleteFront(List L) {
    if (L == NULL || L->length <= 0) {
        fprintf(stderr, "Error: deleteFront() = Invalid List\n");
        exit(1);
    }

    Node N = L->front;
    if (L->length > 1) {
        L->front = L->front->next;
        L->front->prev = NULL;
    } else {
        L->front = L->back = NULL;
    }

    if (L->curr == N) {
        L->curr = NULL;
        L->index = -1;
    } else if (L->curr != NULL) {
        L->index--;
    }

    L->length--;
    //release memory
    freeNode(&N);
}


// deletes back of list
// same logic as above but deletes back
void deleteBack(List L) {
    if (L == NULL || L->length <= 0) {
        fprintf(stderr, "Error: deleteBack() = Invalid List\n");
        exit(1);
    }

    Node N = L->back;
    if (L->length > 1) {
        L->back = L->back->prev;
        L->back->next = NULL;
    } else {
        L->front = L->back = NULL;
    }

    if (L->curr == N) {
        L->curr = NULL;
        L->index = -1;
    }

    L->length--;
    freeNode(&N);
}

// delets current node
void delete(List L) {
    if (L == NULL || L->length <= 0 || L->index < 0) {
        fprintf(stderr, "Error: delete() = Invalid List\n");
        exit(1);
    }

    if (L->curr == L->front) {
        deleteFront(L);
    } else if (L->curr == L->back) {
        deleteBack(L);
    } else {
        Node N = L->curr;
        N->prev->next = N->next;
        N->next->prev = N->prev;
        freeNode(&N);
        L->curr = NULL;
        L->index = -1;
        L->length--;
    }
}

// prints list to outtput file
void printList(FILE* out, List L){
    Node N = NULL;
    if(L == NULL){
        fprintf(stderr, "Error:  printList() = Null\n");
        exit(1);
    }
    for(N = L->front; N != NULL; N = N->next){ // loops through list
        fprintf(out, "%d ", *((int*)N->data));
    }
    fprintf(out, "\n"); // adds line spacing
}

