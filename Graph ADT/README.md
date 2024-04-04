pa2 - Programming assignment 2
Evangelos Barberis
CSE 101

# Pre
This program essentially creates a BFS alogrithim, that depicts the progress of the BFS alogrithim. It uses color:
Black - visited and neighbors explored
White - unvisited
Grey - neighbors to be explored

Nil- Invalid
inf - infinity - distance automatically since no target

It itaalizes everything at white, or nil (essentially NULL). If there is no path from a source node to a destination node, it will return INF. And, Nil represents if a value is not present or there is no relation between nodes, which may happen if the node is the source, or there is no source node!

# Introduction
This program effectively creates a BFS algorithim that creates a tree, and explores its children. We used the alogorithim to find the shorest path from vertices to vertices. Some files we included are from PA1, which create the LIST ADT that we need in order to store the trees in.

## List.h and List.c
Both of these files came from pa1, which are here to create a List ADT, and hold the functions relevant for us to create a tree, and make List manipulations. These files combined allow for us to use its doubly linked list functions in order to create a lists of adjacent vertices and paths (need for us to find).

## Graph.h and Graph.c
Both of these files hold the key elements on BFS functions, and its creation. In these files, we created a graph, free-ed a graph, returned it order, source, parent and distance, as well as finding its path. One crucial element to this file was the BFS function which we created the Breadth First Search (BFS) algorithm. 

For the BFS function, we essentially uses a node "s", and uses a queue to store the nodes that we need to visit, and a list to store the nodes that we have visited. We then start the BFS algorithm, and start by adding the source node to the queue, and then we start the while loop. In this while loop, we check if the queue is empty, if it is, we break out of the loop, if it is not, we pop the first element from the queue, and add it to the list, and then we add its children to the queue. We then repeat this process until the queue is empty.

## FindPath.c
FindPath.c reads through the user input file and prints its results to a user out file, as provided. The first part of the file depictsthe edges connectinmg vertices, and the second part depicts the pairs of vertices in order to calculate the shortest path using BFS. After, it writes the shortest path and adjaceny list to an output file. 

## GraphTest.c
This was a very simple and fast made file, that was made to test each function in Graph.c to make sure everything runs accordingly. Nothing to special, very simple, and tests the structure and integrity of each function!

### Makefile
This is where the program is compiled, only thing that was altered from the given Makefile was creating a GraphTest.c execulatble. Other than that it comes from the one provided.