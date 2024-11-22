/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA3 
* Graph.c
* Creates the functions for Graph.h
* Creates the main DFS ADT, and uses it to implement strong components
* Very important file, bread and butter of PA3
* Notice some functions are the same from PA2
***/ 

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"

// GraphObj structure, same as PA2 updated variables for DFS
typedef struct GraphObj {
    List* nhbrs; // array of lists, represents neighbors of each node
    int* col; // from readings - col, representing node discovery
    int *parent; // parent node
    int *disc; // time for discovery
    int *fin; // time for finish
    int order; // order of verices in graph
    int size; // size of graph
} GraphObj;


/*** Constructors-Destructors ***/

// create newGraph, initalize values 
Graph newGraph(int n) {
    // allocate memory for graph
    Graph G = malloc(sizeof(GraphObj));
    // allocate memory for each node in graph at 1
    // +1 matches index
    G->nhbrs = calloc(n + 1, sizeof(List));
    G->col = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->disc = calloc(n + 1, sizeof(int));
    G->fin = calloc(n + 1, sizeof(int));
    G->order = n;
    G->size = 0; //  size starts empty
    for (int i = 1; i <= n; i++) {
        G->nhbrs[i] = newList(); // adds neighbors to a new list
        G->col[i] = WHITE; // default to white, since unknown
        G->parent[i] = NIL; // default to NIL, parent node unkown
        G->disc[i] = UNDEF; // Not yet DEFINED
        G->fin[i] = UNDEF; // not yet defined
    }
    return G;
}

// same as PA2, slightly different elements for the new GRAPGH DFS ADT
void freeGraph(Graph* pG) {
    // free each element in graph
    for (int i = 1; i <= (*pG)->order; i++) {
        freeList(&((*pG)->nhbrs[i]));
    }
    // free memory for graph and arrays used
    free((*pG)->nhbrs);
    free((*pG)->col);
    free((*pG)->parent);
    free((*pG)->disc);
    free((*pG)->fin);
    free(*pG);
    *pG = NULL;
}

/*** Access functions ***/

// same as PA2
// returns order / number of vertices in graph
int getOrder(Graph G) {
    return G->order;
}

// Same as PA2
// returns size of graph
int getSize(Graph G) {
    return G->size;
}

// same as PA2 
//gets and returns parent node of node u
int getParent(Graph G, int u) {
    // check if u is in graph, if not return NIL
    if (u < 1 || u > getOrder(G)) {
        return NIL;
    }
    return G->parent[u];
}

int getDiscover(Graph G, int u) {
    // check if u is in graph, if not return UNDEF
    if (u < 1 || u > getOrder(G)) {
        return UNDEF;
    }
    // return the discovery time of vertex u
    return G->disc[u];
}

int getFinish(Graph G, int u) {
    // check if u is in graph, if not return UNDEF
    if (u < 1 || u > getOrder(G)) {
        return UNDEF;
    }
    // return the finish time of vertex u
    return G->fin[u];
}

/*** Manipulation procedures ***/

void addArc(Graph G, int u, int v) {
    // check if valid 
    if (u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G)) {
        return;
    }
    List L = G->nhbrs[u]; // list for neighnors
    moveFront(L); // move to front
    // traverse through list to find the posoition of v
    while (index(L) >= 0 && get(L) < v) {
        moveNext(L);
    }
    //if already in neighbor list add it
    if (index(L) == -1 || get(L) != v) {
        if (index(L) == -1) {
            append(L, v);
        } else {
            insertBefore(L, v);
        }
        G->size++; // increase the number of edges in Graph
    }
}

// Helper function to insert vertices in sorted order, for addEdge :)
void sorted_order(List L, int v) {
    // iterate over L
    for(moveFront(L); index(L) >= 0; moveNext(L)) {
        // if greater than V insert before
        if(get(L) > v) {
            insertBefore(L, v);
            return;
        }
    }
    // if greater than append
    append(L, v);
}

void addEdge(Graph G, int u, int v) {
    // Check for valid vertices
    if (u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G)) {
        return;
    }
    
    // Insert v into the adjacency list of u, and u into the adjancency list of v
    sorted_order(G->nhbrs[u], v);
    sorted_order(G->nhbrs[v], u);
    // Increment the size since one undirected edge is added
    G->size++;
}


// helper function for DFS function
void graph_visit(Graph G, List S, int u, int *t) {
    G->col[u] = GREY; // being visited
    G->disc[u] = ++(*t); // discovery time increased
    List L = G->nhbrs[u]; // get adjacency of u
    for (moveFront(L); index(L) >= 0; moveNext(L)) {
        int v = get(L); // neighbor of v
        if (G->col[v] == WHITE) { // if unvisted
            G->parent[v] = u; // u becomes parent
            graph_visit(G, S, v, t); // recursivlty visit v
        }
    }
    // mark as visited and increase finish time
    G->col[u] = BLACK;
    G->fin[u] = ++(*t);
    prepend(S, u); // S acts like a stack by adding finished vertices to the front
}

// main functionallity, Depth first search here
void DFS(Graph G, List S) {
    // if s does not have all G, end
    if (length(S) != getOrder(G)) {
        return;
    }
    // initalization of colors
    for (int i = 1; i <= getOrder(G); i++) {
        G->col[i] = WHITE;
        G->parent[i] = NIL;
        G->disc[i] = UNDEF;
        G->fin[i] = UNDEF;
    }
    // time starts at zero
    int t = 0;
    List stack = newList(); // Use a temporary List to act as a stack
    moveFront(S);
    // iterate through verticeis in s
    while (index(S) >= 0) {
        int u = get(S);
        // if univiusted
        if (G->col[u] == WHITE) {
            graph_visit(G, stack, u, &t); // Use stack to perform DFS
        }
        moveNext(S);
    }
    clear(S); // Clear the original List S
    moveFront(stack);
    while (index(stack) >= 0) { // Rebuild S with vertices in order of decreasing fin ts
        append(S, get(stack));
        moveNext(stack);
    }
    freeList(&stack); // Free the temporary stack
}


/*** Other operations ***/

//returns transpose of G
Graph transpose(Graph G) {
    // new graph
    Graph T = newGraph(getOrder(G));
    // iteratev over each vertex in G
    for (int i = 1; i <= getOrder(G); i++) {
        List L = G->nhbrs[i];
        // iterate over all neignbors of i
        for (moveFront(L); index(L) >= 0; moveNext(L)) {
            int v = get(L);
            addArc(T, v, i); // reverse the direction of the edge
        }
    }
    return T;
}

Graph copyGraph(Graph G) {
    // newgrapgh with same order as G
    Graph C = newGraph(getOrder(G));
    // iterate over each vertex it G
    for (int i = 1; i <= getOrder(G); i++) {
        List L = G->nhbrs[i];
        // itterate over each neigbor
        for (moveFront(L); index(L) >= 0; moveNext(L)) {
            addArc(C, i, get(L));
        }
    }
    return C;
}

// cleaned up PA2 implementation
void printGraph(FILE* out, Graph G) {
    // iterate opver each and every vertex
    for (int i = 1; i <= getOrder(G); i++) {
        // print and the adjacencyt list
        fprintf(out, "%d: ", i);
        printList(out, G->nhbrs[i]);
    }
}


