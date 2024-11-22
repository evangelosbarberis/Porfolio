/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA4
* Sparse.c
* Main file for this programming assignment
* Performs Matrix Operations on Sparce Matrices
* Matrix Calc
* essentially an output file!!! Just prints and executes results
***/ 

#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"

int main(int argc, char* argv[]) {
    // intalize vals and matrices
    FILE *in, *out;
    int n, a, b, i, row, col;
    double value;
    Matrix A, B, C, D, E, F, G, H, I;

    // Check command line for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // open and read files
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if (in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if (out == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // read in
    fscanf(in, "%d %d %d", &n, &a, &b);

    // Initialize Matrices A and B
    A = newMatrix(n);
    B = newMatrix(n);

    // Load Matrix A
    for(i = 0; i < a; i++) {
        fscanf(in, "%d %d %lf", &row, &col, &value);
        changeEntry(A, row, col, value);
    }

    // Load Matrix B
    for(i = 0; i < b; i++) {
        fscanf(in, "%d %d %lf", &row, &col, &value);
        changeEntry(B, row, col, value);
    }

    // Perform calculations
    C = scalarMult(1.5, A);
    D = sum(A, B);
    E = sum(A, A);
    F = diff(B, A);
    G = diff(A, A);
    H = transpose(A);
    I = product(A, B);
    Matrix J = product(B, B);

    // Print results
    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);
    fprintf(out, "\n");

    fprintf(out, "B has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);
    fprintf(out, "\n");

    fprintf(out, "(1.5)*A =\n");
    printMatrix(out, C);
    fprintf(out, "\n");

    fprintf(out, "A+B =\n");
    printMatrix(out, D);
    fprintf(out, "\n");

    fprintf(out, "A+A =\n");
    printMatrix(out, E);
    fprintf(out, "\n");

    fprintf(out, "B-A =\n");
    printMatrix(out, F);
    fprintf(out, "\n");

    fprintf(out, "A-A =\n");
    printMatrix(out, G);
    fprintf(out, "\n");

    fprintf(out, "Transpose(A) =\n");
    printMatrix(out, H);
    fprintf(out, "\n");

    fprintf(out, "A*B =\n");
    printMatrix(out, I);
    fprintf(out, "\n");

    fprintf(out, "B*B =\n");
    printMatrix(out, J);
    fprintf(out, "\n");

    // Free matrices
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);

    // Close files
    fclose(in);
    fclose(out);

    return 0;
}
