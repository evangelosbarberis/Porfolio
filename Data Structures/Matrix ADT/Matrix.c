/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA4
* Matrix.c
* main functions for the Matrix ADT
* functions allow us to create a Matrix as well as manipulate it, and do Arithmetic OPS
***/ 
#include<stdio.h>
#include<stdlib.h>
#include "List.h"
#include "Matrix.h"
// structs --------------------------------------------------------------------
// define cruical elements of the Matrix operations here
typedef struct EntryObj {
    int column;
    double value;
} EntryObj;

typedef EntryObj* Entry;
typedef struct MatrixObj {
    int size;
    int NNZ;
    List* body;
} MatrixObj;

// Constructors-Destructors ---------------------------------------------------
// basic files to construct the types needed for Matrix ADT

// newEntry()
// returns newEntry object - empty
Entry newEntry(int column, double value) {
    //allocation
    Entry E = malloc(sizeof(EntryObj));
    //set column and Val 
    E->column = column;
    E->value = value;
    return E;
}
// freeEntry()
// free memory allocated
void freeEntry(Entry* pE) {
    //if not null
    if (pE != NULL && *pE != NULL) {
        free(*pE);// free memory of the entry
        *pE = NULL;
    }
}
// makeZero()
// sets M to zero matrix
void makeZero(Matrix M) {
    if (M == NULL) {
        exit(1);
    }
    // iterate ove reach row
    for (int i = 0; i < M->size; i++) {
        // clear list
        clear(M->body[i]);
    }
    M->NNZ = 0;
}
// newMatrix()
// retruns N*N matrix in zero state
Matrix newMatrix(int n) {
    Matrix M = malloc(sizeof(MatrixObj));
    // italize objects 
    M->size = n;
    M->NNZ = 0;
    M->body = malloc(sizeof(List) * n);
    // iterate over eqch row
    for (int i = 0; i < n; i++) {
        //create a list of each row
        M->body[i] = newList();
    }
    return M;
}
// freeMatrix()
// free heap memory
void freeMatrix(Matrix* pM) {
    // if not null
    if (pM != NULL && *pM != NULL) {
        // set to zero
        makeZero(*pM);
        // iterate through each row and free
        for (int i = 0; i < (*pM)->size; i++) {
            freeList(&(*pM)->body[i]);
        }
        free((*pM)->body);
        free(*pM);
        *pM = NULL;
    }
}


// Access functions -----------------------------------------------------------

// size()
// return size
int size(Matrix M) {
    if (M == NULL) {
        exit(1);
    }
    return M->size;
}
// NNZ()
// return NNZ
int NNZ(Matrix M) {
    if (M == NULL) {
        exit(1);
    }
    return M->NNZ;
}
// equals()
// Return 1 in equal, else 0
int equals(Matrix A, Matrix B) {
    if (A == NULL || B == NULL) {
        exit(1);
    }
    // it sizes are zero false
    if (A->size != B->size) return 0;
    // else iterate through each row of matrices
    for (int i = 0; i < A->size; i++) {
        // get current rows
        List rowA = A->body[i];
        List rowB = B->body[i];
        // if lengths are diff false
        if (length(rowA) != length(rowB)) return 0;
        // go through each entry
        moveFront(rowA);
        moveFront(rowB);
        // if col and val diff false
        while (index(rowA) >= 0) {
            Entry entryA = (Entry)get(rowA);
            Entry entryB = (Entry)get(rowB);
            if (entryA->column != entryB->column || entryA->value != entryB->value)
                return 0;
            moveNext(rowA);
            moveNext(rowB);
        }
    }
    // else true
    return 1;
}


// changeEntry()
// change i and j to x val
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x) {
    if (M == NULL) {
        exit(1);
    }
    // get row for index
    List row = M->body[i - 1];
    moveFront(row);
    while (index(row) >= 0 && ((Entry)get(row))->column < j)
        moveNext(row);
    
    // if found update or delete
    if (index(row) >= 0) {
        Entry entry = (Entry)get(row);
        if (entry->column == j) {
            if (x != 0.0) {
                entry->value = x; // update the new val if not zero
            } else {
                delete(row); // else delete that val
                freeEntry(&entry);
                M->NNZ--;
            }
        } else if (x != 0.0) {
            insertBefore(row, newEntry(j, x)); // if not update instert before
            M->NNZ++;
        }
    } else if (x != 0.0) {
        append(row, newEntry(j, x)); // if no entry and no zero append!
        M->NNZ++;
    }
}
// Matrix Arithmetic operations ------------------------------------------------

//Copy Matrix
Matrix copy(Matrix A){
    if(A == NULL){
        exit(1);
    }
    // new matrix with same size as A
    Matrix M = newMatrix(size(A));
    // iterate over each val of A
    for(int i = 0; i < size(A); i++){
        List row = A->body[i];
        moveFront(row); // front of row 
        // iterate over each entry in row
        while(index(row) >= 0){
            Entry oldEntry = (Entry)get(row);
            Entry newEntry = malloc(sizeof(EntryObj)); // allocate memory
            newEntry->column = oldEntry->column; // copy
            newEntry->value = oldEntry->value; // copy val
            append(M->body[i], newEntry); // apend to corresponding index
            moveNext(row); // contine to next row
        }
    }
    M->NNZ = A->NNZ; // set counts the same
    return M;
}


// transpose()
// return new object as transpose of A
Matrix transpose(Matrix A){
    if(A == NULL){
        exit(1);
    }
    // new matrix same size
    Matrix M = newMatrix(size(A));
    // iterate over each row
    for(int i = 0; i < size(A); i++){
        List row = A->body[i]; // current row
        moveFront(row); // front
        // iterate over each entry in row
        while(index(row) >= 0){
            Entry oldEntry = (Entry)get(row);
            // change entry to transpose
            changeEntry(M, oldEntry->column, i+1, oldEntry->value);
            moveNext(row); // continue to next
        }
    }
    return M;
}


// scalarMult()
// return new Matrix obj representing A
Matrix scalarMult(double x, Matrix A){
    if(A == NULL){
    exit(1);
   }
   // new with same size
   Matrix M = newMatrix(size(A));
   // iterate over each row
   for(int i = 0; i < size(A); i++){
      List row = A->body[i];
      moveFront(row);
      // iterate over each entry of the row
      while(index(row) >= 0){
        Entry oldEntry = (Entry)get(row);
        // multiply each entry by scalr x
        changeEntry(M, i+1, oldEntry->column, x * oldEntry->value);
        moveNext(row);
    }
   }
   return M;
}

// sum()
// returns Matrix obj representing A+B
Matrix sum(Matrix A, Matrix B) {
    if (A == NULL || B == NULL) {
        exit(1);
    }
    if (size(A) != size(B)) {
        exit(1);
    }
    // if equal retunr scalar by 2
    if (equals(A, B)) {
        return scalarMult(2.0, A);
    }
    // make new same size
    Matrix resultMatrix = newMatrix(size(A));
    // iterate over each row 
    for (int i = 0; i < size(A); i++) {
        List listA = A->body[i];
        List listB = B->body[i];
        List sumList = resultMatrix->body[i];
        moveFront(listA);
        moveFront(listB);
        // iterate over entrys
        while(index(listA) >= 0 || index(listB) >= 0){
            Entry entryA = index(listA) >= 0 ? (Entry)get(listA) : NULL; // get current
            Entry entryB = index(listB) >= 0 ? (Entry)get(listB) : NULL; // get current if not null
            // colm indexs
            int columnA = entryA != NULL ? entryA->column : -1;
            int columnB = entryB != NULL ? entryB->column : -1;
            // if entryB Null
            if(entryA != NULL && (entryB == NULL || columnA < columnB)){
                append(sumList, newEntry(columnA, entryA->value));
                resultMatrix->NNZ++;
                moveNext(listA);
                // it entryA Null
            } else if (entryB != NULL && (entryA == NULL || columnB < columnA)){
                append(sumList, newEntry(columnB, entryB->value));
                resultMatrix->NNZ++; // NNZ increases
                moveNext(listB);
                // if both not null
            } else if (entryA != NULL && entryB != NULL) {
                // calculate sum
                double s = entryA->value + entryB->value;
                // if not zero
                if (s != 0) {
                    append(sumList, newEntry(columnA, s));
                    resultMatrix->NNZ++; // increase NNZ
                }
                moveNext(listA);
                moveNext(listB);
            }
        }
    }
    return resultMatrix;
}
// diff()
// returns new object as A-B
Matrix diff(Matrix A, Matrix B){
    if(A == NULL || B == NULL){
        exit(1);
    }
    if(size(A) != size(B)){
        exit(1);
    }
    Matrix M = newMatrix(size(A));
    // iterate here for each row
    for(int i = 0; i < size(A); i++){
        List rowA = A->body[i];
        List rowB = B->body[i];
        moveFront(rowA);
        moveFront(rowB);
        // iterate over each entry
        while(index(rowA) >= 0 || index(rowB) >= 0){
            // get the current idexs and entry
            Entry entryA = index(rowA) >= 0 ? (Entry) get(rowA) : NULL;
            Entry entryB = index(rowB) >= 0 ? (Entry) get(rowB) : NULL;
            // col index
            int colA = entryA == NULL ? -1 : entryA->column;
            int colB = entryB == NULL ? -1 : entryB->column;
            // if B null or smaller
            if(entryA != NULL && (colB == -1 || colA < colB)){
                //i+1, colA
                changeEntry(M, i+1, colA, entryA->value);
                moveNext(rowA);
                // if B larger
            }else if(entryB != NULL && (colA == -1 || colA > colB)){
                // i+1, colB
                changeEntry(M, i+1, colB, -entryB->value);
                moveNext(rowB);
                //else
            }else if(entryA != NULL && entryB != NULL){
                // diff between A and B
                changeEntry(M, i+1, colA, entryA->value - entryB->value);
                moveNext(rowA);
                moveNext(rowB);
            }
        }
    }
    return M;
}

// product()
// return new object as AB
Matrix product(Matrix A, Matrix B){
    if(A == NULL || B == NULL){
        exit(1);
    }
    if(size(A) != size(B)){
        exit(1);
   }
   Matrix M = newMatrix(size(A));
   Matrix T = transpose(B); // trans b to get colunm as rows
   // iterate over each row
   for(int i = 0; i < size(A); i++){
    // iterate over each col
      for(int j = 0; j < size(A); j++){
        List rowA = A->body[i];
        List rowB = T->body[j];
        moveFront(rowA);
        moveFront(rowB);
        double dotProduct = 0.0; // italize dot product
        // ittearte over each entry of current row
        while(index(rowA) != -1 && index(rowB) != -1){
            Entry entryA = (Entry)get(rowA);
            Entry entryB = (Entry)get(rowB);
            if(entryA->column < entryB->column){
                moveNext(rowA);
            }else if(entryA->column > entryB->column){
                moveNext(rowB);
            }else{
               // Columns match, contribute to dot product
                dotProduct += (entryA->value * entryB->value);
                moveNext(rowA);
                moveNext(rowB);
                }
            }
         // only change if nonzero, changeEntry in result Matrix
        if(dotProduct != 0.0){
            changeEntry(M, i+1, j+1, dotProduct);
            }
        }
    }
   freeMatrix(&T);
   return M;
}


// Other operations ------------------------------------------------------------
// printMatrix()
// prints Matrix to out!
void printMatrix(FILE* out, Matrix M){
    if( M==NULL ){
      exit(1);
   }
   // iterate over each row
    for(int i = 0; i < M->size; i++){
        // check if curr non-empty
        List row = M->body[i];
        if(length(row) > 0){
            // print row num
        fprintf(out, "%d: ", i + 1);
        moveFront(row);
        // iterate over entry of row
        while(index(row) >= 0){
            Entry entry = (Entry)get(row);
            // print col index
            fprintf(out, "(%d, %.1f) ", entry->column, entry->value);
            moveNext(row);
        }
        fprintf(out, "\n");
      }
   }
}
