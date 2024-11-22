/***
* Evangelos Neo barberis
* ebarberi
* 2024 Winter CSE101 PA3
* GraphTest.c
* Tests the Graph.c functions!
* similar to handout provided to us
***/ 


#include<stdio.h>
#include<stdlib.h>
#include"List.h"
#include"Graph.h"

int main(int argc, char* argv[]){
   int n=8;
   List S = newList(); // new list
   Graph G = newGraph(n); // new grapgh with 8 vertices, n

   // addArc here
   printf("addArc\n");
   addArc(G, 1,2);
   addArc(G, 1,3);
   addArc(G, 2,3);
   addArc(G, 2,4);
   addArc(G, 2,5);
   addArc(G, 2,6);
   addArc(G, 3,4);
   addArc(G, 3,7);
   addArc(G, 4,3);
   addArc(G, 4,8);
   addArc(G, 5,1);
   addArc(G, 5,6);
   addArc(G, 6,7);
   addArc(G, 7,6);
   addArc(G, 7,8);
   addArc(G, 8,8);
   printGraph(stdout, G);
   printf("\n");

   // addEdge here
   printf("addEdge\n");
   addEdge(G, 1,5);
   addEdge(G, 2,6);
   addEdge(G, 2,3);
   addEdge(G, 3,7);
   addEdge(G, 4,4);
   addEdge(G, 5,5);
   addEdge(G, 6,6);
   addEdge(G, 7,7);
   addEdge(G, 8,8);
   printGraph(stdout, G);
   printf("\n");

   // georOrder and getSize
   printf("getOrder:\n");
   printf("%d\n", getOrder(G));
   printf("getSize:\n");
   printf("%d\n", getSize(G));
   printf("\n");

   // getDiscover, getFinish, and getParent 
   printf("DFS:\n");
   for(int i = 1; i<=n; i++) append(S, i);
   DFS(G, S);
   printf("\n");
   printf("x:  d  f  p\n");
   for(int i=1; i<=n; i++) {
      printf("%d: %2d %2d %2d\n", i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
   }
   printf("\n");

   // transpose and copyGrapgh
   Graph T, C;
   printf("Transpose:\n");
   T = transpose(G);
   printGraph(stdout, T);
   printf("\n");
   printf("copyGrapgh\n");
   C = copyGraph(G);
   printGraph(stdout, C);
   printf("\n");

   // DFS, see if able to run twice
   printf("DFS:\n");
   for(int i = 1; i<=n; i++) append(S, i);
   DFS(G, S);
   printf("\n");
   printf("x:  d  f  p\n");
   for(int i=1; i<=n; i++) {
      printf("%d: %2d %2d %2d\n", i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
   }
   printf("\n");

   // free allocated memory for lists and graphs
   freeList(&S);
   freeGraph(&G);
   freeGraph(&T);
   freeGraph(&C);
   return(0);
}