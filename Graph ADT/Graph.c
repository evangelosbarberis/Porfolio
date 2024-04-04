/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA1 
* Graph.c
* holds main functionallity of the BFS algorithim 
* uses functions from Graph.h and List.h
***/ 

#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Graph.h"
#define WHITE 1
#define GREY 2
#define BLACK 3
#define NIL -1
#define INF -2

// intialze newGraph vals
typedef struct GraphObj{
    List* nhbrs; // array of lists, represents neighbors of each node
    int* col; // from readings - color, representing node discovery
    int* parent; // parent node
    int* dist; //distance between nodes
    int order; // order of vertices in graph
    int size; // size of graph
    int src; // source node, starting point
} GraphObj;


/*** Constructors-Destructors ***/

//create a new graph
// graphs should include int vals, as per request from PA2 assignment

Graph newGraph(int n){
    // allocate memory for graph
    Graph G = malloc(sizeof(GraphObj));
    // allocate memory for each node in graph at 1
    // +1 matches index
    G->nhbrs = calloc(n + 1, sizeof(List));
    G->col = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->dist = calloc(n + 1, sizeof(int));
    G->order = n;
    G->size = 0;
    //NIL as PA2 instructs state,
    // NIL for an empty node
    G -> src = NIL;
    for (int i = 0; i < n + 1; i++){
        G->nhbrs[i] = newList(); // adds neighbors to a new list
        G->col[i] = WHITE; // default to white, since unknown
        G->parent[i] = NIL; // default to NIL, parent node unkown
        G->dist[i] = INF; // D = Inifity, since target unkown 
    }

    return G;
}


void freeGraph (Graph* pG){
    //check if Null, if do nothing, if so free data
    if (pG != NULL && *pG != NULL){
        // free each node in graph
        for (int i = 1; i <= (*pG)->order; i++){
          freeList(&((*pG)->nhbrs[i]));        
        }
        // free memory for graph, and arrays used
        free((*pG)->nhbrs);
        free((*pG)->col);
        free((*pG)->parent);
        free((*pG)->dist);
        free(*pG);
        *pG = NULL;
    }
}

/*** Access functions ***/

// returns order / number of vertices in graph
int getOrder(Graph G){
    return G->order;
}


// returns size of graph
int getSize(Graph G){
    return G->size;
}

//returns source node of graph
int getSource(Graph G){
    return G->src;
}

//gets and returns parent node of node u
int getParent(Graph G, int u){
    // check if u is in graph, if not return NIL
    if (u < 0 || u > G->order){
        return NIL;
    }
    return G->parent[u];
}


//gets and returns distance of U
int getDist(Graph G, int u){
    //check if in range, if not return infiity
    if (u < 0 || u > G->order){
        return INF;
    }
    return G->dist[u];
}

// hard one to conceptualize
// get path by checking first if NIL, if so return nothing
void getPath(List L, Graph G, int u){
    if (getSource(G) == NIL) {
        return; // If the source is NIL, there's no path to find, return
    }
    if (u < 1 || u > getOrder(G)) {
        return; // U is out of bounds, return with no action
    }
    clear(L);
    if (u == getSource(G)) {
        append(L, u); // If u is the source, the path is just the vertex itself
    } else if (getParent(G, u) == NIL && getDist(G, u) == INF) { 
        append(L, NIL); // If there's no parent and distance is INF, append NIL to indicate no path
    } else {
        getPath(L, G, getParent(G, u)); // Recurse to construct the path
        append(L, u); // Append u to the path
    }
}




/*** Manipulation procedures ***/

void makeNull(Graph G){
    for (int i = 1; i <= G->order; i++){ // loop through vertices
        clear(G->nhbrs[i]); // delete each neighbor node
    }
    G -> size = 0; // once deleted set size to`0`
}

void addEdge(Graph G, int u, int v){
    // add both edges
    addArc(G, u, v);
    addArc(G, v, u); 
    G->size--;
}

void addArc(Graph G, int u, int v){
    if (G == NULL || u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G)){
        return; // If `u` or `v` is out of range, do nothing
    }
    List neighbor_list = G->nhbrs[u];
    moveFront(neighbor_list);
    while (index(neighbor_list) >= 0 && get(neighbor_list) < v){
        moveNext(neighbor_list);
    }
    if(index(neighbor_list) == -1){
        append(neighbor_list, v);
    } else {
        insertBefore(neighbor_list, v);
    }
    G->size++; // Increment the size for each directed edge added
}

//BREADTH FIRST SEARCH HERE!!!!!!!
//check pres
void BFS(Graph G, int s){
    if(G == NULL || s < 1 || s > getOrder(G)){ // Validate inputs
        return;
    }
    // Initialization loop: reset all nodes to default state
    for(int i = 1; i <= getOrder(G); i++){
        G->col[i] = WHITE; // Reset color to WHITE
        G->dist[i] = INF; // Reset distance to INF
        G->parent[i] = NIL; // Reset parent to NIL
    }
    // Start BFS from source node 's'
    G->src = s;
    G->col[s] = GREY;
    G->dist[s] = 0;
    List new = newList();
    append(new, s);
    // Process nodes in the queue
    while(length(new) > 0){
        int y = front(new);
        deleteFront(new); 
        List adjacent = G->nhbrs[y];
        for (moveFront(adjacent); index(adjacent) >= 0; moveNext(adjacent)){
            int j = get(adjacent);
            if (G->col[j] == WHITE) {
                G->col[j] = GREY;
                G->dist[j] = G->dist[y] + 1;
                G->parent[j] = y;
                append(new, j);
            }
        }
        G->col[y] = BLACK;
    }
    freeList(&new); // Free the list after traversal
}


/*** Other operations ***/

// print adj list!
void printGraph(FILE* out, Graph G){
    // check if G NULL
    if(G == NULL){
        return; // error handle
    }
    //iterate over vertices
    for (int i = 1; i <= getOrder(G) ; i++){
        fprintf(out, "%d: ", i);  
        // get next list
        List L = G -> nhbrs[i];
        moveFront(L);
        // iterate through adjacent list
        while(index(L) >= 0){
            fprintf(out, "%d ", get(L));  
            moveNext(L);
        }
        fprintf(out, "\n");  
    }
}
