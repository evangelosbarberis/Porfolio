/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA1 
* GraphTest.c
* Tests the Graph.c functions!
***/ 
#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include"List.h"

int main(){
    // NewGraph and printGraph
    printf("New Graph and Print Graph\n");
    int verts = 5;
    Graph A = newGraph(verts);
    printGraph(stdout, A);
    printf("\n");

    // addedge addarc getSize
    printf("AddEdge addArc and getSize\n");
    for(int i = 1; i < verts; i++){
        addEdge(A, i, i+1);
    }
    addArc(A, 1, verts);
    printGraph(stdout, A);
    printf("Size after adding edges and arcs: %d\n", getSize(A));
    printf("\n");

    // BFS and getPath
    printf("BFS and getPath\n");
    BFS(A, 3);
    List path = newList();
    getPath(path, A, 5);
    printf("Path from 3 to 5: ");
    printList(stdout, path);
    printf("\n");
    clear(path);
    printf("\n");

    // distance after BFS 
    printf("Dist after BFS\n");
    for(int i = 1; i < verts; i++){
        printf("Distance from 3 to %d: %d\n", i, getDist(A, i));
    }
    printf("\n");

    // Parent
    printf("Parent after BFS\n");
    for(int i = 1; i < verts; i++){
        printf("Parent of %d: %d\n", i, getParent(A, i));
    }
    printf("\n");

    // makeNull
    printf("makeNull\n");
    makeNull(A);
    printGraph(stdout, A);
    printf("Size after makeNull: %d\n", getSize(A));
    printf("\n");

    // Free objects 
    freeList(&path);
    freeGraph(&A);

    return 0;
}

