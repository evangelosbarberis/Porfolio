/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA3
* Find Components.c
* Client Module
* Uses Graph ADT to implement strong components
* Please refer to Graph.c for more information and README.md on implementation details
***/ 
#include<stdio.h>
#include<stdlib.h>
#include "Graph.h"

int main(int argc, char * argv[]){
    FILE *in, *out;
    int n, u, v;
    Graph G;

    // same as PA2, read in the graph from a file
    // Check command line for correct number of arguments
    if(argc != 3){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // open files for reading and writing 
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if(in == NULL){
        exit(1);
    }
    if(out == NULL){
        exit(1);
    }

    // Read order
    // create a newGraph G
    fscanf(in, "%d", &n);
    G = newGraph(n);

    // read in the graph
    while(fgetc(in) != EOF){
        fscanf(in, "%d %d", &u, &v);
        if(u == 0 && v == 0) break;
        addArc(G, u, v);
    }

    // Print Graph Adjacency List
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);
    fprintf(out, "\n");

    // Create new list, perform DFS, transpose, then perform DFS again
    List new = newList();
    for(int i = 1; i<= n; i++) append(new, i);
    DFS(G, new);
    Graph T = transpose(G);
    DFS(T, new);

    // count, for the total number of strongly connected components
    int count = 0;
    for(moveFront(new); index(new) >= 0; moveNext(new)){
        if(getParent(T, get(new)) == NIL) count++;
    }

    fprintf(out, "G contains %d strongly connected components:\n", count);

    // print those strongly connected components here
    int strongly_conect = 0; //count of strongly connected components
    List stack = newList(); //create stack
    for(moveBack(new); index(new) >= 0; movePrev(new)){
        int x = get(new);
        prepend(stack, x); //put current vertex on stack
        if(getParent(T, x) == NIL) {
            strongly_conect++;
            fprintf(out, "Component %d: ", strongly_conect);
            for(moveFront(stack); index(stack) >= 0; moveNext(stack)){ //iterate through stack
                fprintf(out, "%d ", get(stack)); //pirnt elements in stack
            }
            fprintf(out, "\n");
            clear(stack);
        }
    }

    // Free objects from memory
    
    freeList(&new);
    freeList(&stack);
    freeGraph(&G);
    freeGraph(&T);


    //close files
    fclose(in);
    fclose(out);

    return(0);
}