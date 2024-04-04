Evangelos Neo Barberis
CSE101 - PA3
Graph ADT - DFS (DEPTH FIRST SEARCH ALGORTHIM)

# Introduction
This programming implements an undirected graph, by using a adjacency list representation. We used this to create a program that performs a Depth First Search, in order to find the strongly connected components of a graph.

In short, what we essentially created in simple terms is a algorthim that finds the strongly connected points of a graph, by using a GRAPH ADT.

## Files Used

### List.h and List.c
Essentially List.h and List.c have been used throughout the entire assignments provided to us so far. 

List.h is the file that defines the functions for our LIST ADT. These functions are used to help us create the adjacency list for our GRAPH ADT and for holding the vertices withing the client portion.

List.c is the file that implements the defintions and functions defined in List.h. Both of these files have not been altered in anyway shape or form since PA1. 

### Graph.h and Graph.c
Graph.h has not been altered by me in anyway, it comes directly from the PA3 Project description. However, in short this file essentially provides an interface for the GRAPH ADT, and declares the functions used for DFS and other graph operations

Graph.c, now this is the bread and butter of this whole assignment. In this file, we implemented the entire GRAPH ADT. It declared the functions relevant for the Graph.h file.

### FindComponents.c 
This was the final cherry on top, this was the client module. We used the Graph and List ADT to read a user in file, to create a graph and perform DFS to find its strongly connected components. Then we sent that out to an output file the user makes.

## DFS Implementation
This is just some notes for how the DFS function works. To begin, the DFS finctions relies on a helper function that we created called graph_visit, that performs the main functionality of DFS. We used a varaible called t (time) that was used to track the discovery times and finish times, which was tracked during the exectution of graph_visit. 

The graph_visit functions recursivly explores the vertices of the graph. The discovert and finish times assigned by this function help us classify the edges in the graph, and creating a forest using DFS.

Pseudo Code for the DFS search:
1. For each vertex u in the graph:
    a. Mark u = WHITE
    b. Parent[u] = NIL
    c. discover[u] and finish[u] = UNDEF

2. Initialize t as 0
3. For each vertex u in list S:
    a. If u is unvisited, call the recursive helper function         graph_vist on u
4. In graph_visit(u):
    a. u = GREY
    b. discoveru to the current value of t
    c. For each adjacent vertex v of u:
        i. If v is unvisited, set parent[v] to u and call graph_visit on v
    d. u = BLACK
    e. finish[u] = current value of t

