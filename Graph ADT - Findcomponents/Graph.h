/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA3 
* Graph.h - funcions for Graph.c
* previsouly declared from the PA3 hadnout
***/ 
#ifndef _GRAPH_H_INCLUDE_
#define _GRAPH_H_INCLUDE_
#define WHITE 1
#define GREY 2
#define BLACK 3
#define NIL -1
#define INF -2
#define UNDEF -3
#include "List.h"
// Forward declaration of GraphObj
typedef struct GraphObj* Graph;
/*** Constructors-Destructors ***/
Graph newGraph(int n);
void freeGraph(Graph* pG);
/*** Access functions ***/
int getOrder(Graph G);
int getSize(Graph G);
int getParent(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getDiscover(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
/*** Manipulation procedures ***/
void addArc(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void addEdge(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void DFS(Graph G, List S);
/*** Other operations ***/
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE* out , Graph G);
#endif
