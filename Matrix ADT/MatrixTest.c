/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA4
* MatrixTest.c
* Test file to test functionality of Matrix.c and Matrix.h
* Comes mostly from the test file provided to us in PA4 examples
***/ 
#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
int main() {
    int n = 3;
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);
    Matrix C, D, E, F, G, H;
    // populate Matrices A and B
    changeEntry(A, 1, 1, 1); changeEntry(B, 1, 1, 1);
    changeEntry(A, 1, 2, 2); changeEntry(B, 1, 2, 0);
    changeEntry(A, 1, 3, 3); changeEntry(B, 1, 3, 1);
    changeEntry(A, 2, 1, 4); changeEntry(B, 2, 1, 0);
    changeEntry(A, 2, 2, 5); changeEntry(B, 2, 2, 1);
    changeEntry(A, 2, 3, 6); changeEntry(B, 2, 3, 0);
    changeEntry(A, 3, 1, 7); changeEntry(B, 3, 1, 1);
    changeEntry(A, 3, 2, 8); changeEntry(B, 3, 2, 1);
    changeEntry(A, 3, 3, 9); changeEntry(B, 3, 3, 1);
    // Print information about Matrix A
    printf("%d\n", NNZ(A));
    printMatrix(stdout, A);
    printf("\n");
    // Print information about Matrix B
    printf("%d\n", NNZ(B));
    printMatrix(stdout, B);
    printf("\n");
    // Scalar multiplication of A
    C = scalarMult(1.5, A);
    printf("%d\n", NNZ(C));
    printMatrix(stdout, C);
    printf("\n");
    // A+B
    D = sum(A, B);
    printf("%d\n", NNZ(D));
    printMatrix(stdout, D);
    printf("\n");
    // diff A and itself
    E = diff(A, A);
    printf("%d\n", NNZ(E));
    printMatrix(stdout, E);
    printf("\n");
    // trans of B
    F = transpose(B);
    printf("%d\n", NNZ(F));
    printMatrix(stdout, F);
    printf("\n");
    // BB
    G = product(B, B);
    printf("%d\n", NNZ(G));
    printMatrix(stdout, G);
    printf("\n");
    // copy of A
    H = copy(A);
    printf("%d\n", NNZ(H));
    printMatrix(stdout, H);
    printf("\n");
    // see if equal 
    printf("%s\n", equals(A, H) ? "true" : "false");
    printf("%s\n", equals(A, B) ? "true" : "false");
    printf("%s\n", equals(A, A) ? "true" : "false");
    // reset to zero
    makeZero(A);
    printf("%d\n", NNZ(A));
    printMatrix(stdout, A);
    // free from memory
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    return 0;
}