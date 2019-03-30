#ifndef GRAPH
	#define GRAPH
	#include "Graph.h"
#endif
#include "GraphVis.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
	// some very simple code to show you
	// how to use graph vis

  Graph g = newGraph(5);
  insertEdge(g,1,2,1);
  insertEdge(g,1,3,1);
  insertEdge(g,2,1,10);
    insertEdge(g,2,3,10);
  insertEdge(g,4,2,60);
	insertEdge(g,3,2,6);
	insertEdge(g,3,1,6);
  showGraph(g);

    int v = 2;
   AdjList new = inIncident(g, v);
   AdjList curr = new;
    printf("In Incident of %d", v);
   while (curr != NULL) {
       printf("\n %d", curr->w);
       curr = curr->next;
   }
    printf("\n");

   AdjList new3 = outIncident(g, v);
   AdjList curr3 = new3;
    printf("Out Incident of %d", v);
   while (curr3 != NULL) {
       printf("\n %d", curr3->w);
       curr3 = curr3->next;
   }
    printf("\n");
printf("Removing edges: 2 to 3\n");
printf("Removing edges: 2 to 1\n");
removeEdge(g, 2, 3);
removeEdge(g, 2, 1);


  showGraph(g);

   new = inIncident(g, v);
   curr = new;
    printf("In Incident of %d", v);
   while (curr != NULL) {
       printf("\n %d", curr->w);
       curr = curr->next;
   }
    printf("\n");


    AdjList new2 = outIncident(g, v);
    AdjList curr2 = new2;
    printf("Out Incident of %d", v);
    while (curr2 != NULL) {
        printf("\n %d", curr2->w);
        curr2 = curr2->next;
    }
    printf("\n");


	// draws the graph

  graphVis(g, DEFAULT);

	return EXIT_SUCCESS;
}