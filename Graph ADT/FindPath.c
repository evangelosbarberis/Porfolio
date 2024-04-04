/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA1 
* FindPath.c
* finds the path for the BFS
* finds the distance
***/ 
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"


int main(int argc, char* argv[]) {
    FILE *in, *out;
    int n, u, v;

    // pre - conditions = to manage input and output files
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open files
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if (in == NULL) {
        printf("Unable to open file \"%s\" for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if (out == NULL) {
        printf("Unable to open file \"%s\" for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // Read vertices and initalize the graph
    fscanf(in, "%d", &n);
    Graph G = newGraph(n);

    // edges and graph construction
    while(fscanf(in, "%d %d", &u, &v) == 2) {
        if (u == 0 && v == 0) {
            break; // End of the first part of the input file
        }
        addEdge(G, u, v); // add edge to graph
    }

    // print adjacency list
    printGraph(out, G);

    // part 2 - pairs and vertices
    while(fscanf(in, "%d %d", &u, &v) == 2) {
        if (u == 0 && v == 0) {
            break; // End of the second part of the input file
        }
        
        BFS(G, u); // BFS from u

        // get shortest path
        List path = newList();
        getPath(path, G, v);

        // print shortest path and distance
        // just print statements here
        int dist = getDist(G, v);
        if (dist != INF) {
            fprintf(out, "\nThe distance from %d to %d is %d\n", u, v, dist);
            fprintf(out, "A shortest %d-%d path is: ", u, v);
            printList(out, path);
            fprintf(out, "\n");
        } else {
            fprintf(out, "\nThe distance from %d to %d is infinity\n", u, v);
            fprintf(out, "No %d-%d path exists\n", u, v);
        }

        freeList(&path);  // free the path list
    }

    
    fclose(in);
    fclose(out);

    // free graph from memory
    freeGraph(&G);

    return 0;
}
