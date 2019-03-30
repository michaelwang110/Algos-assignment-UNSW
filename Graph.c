// Graph ADT interface for Ass2 (COMP2521)
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Graph.h"

struct GraphRep{
    int nV; // number of vertices in graph
    int nE; // number of edges in graph
    AdjList *edges; // adjacency list representation
};

// ----------------------------- 
// Helper function prototypes
// -----------------------------
static AdjList insertLL(AdjList L, Vertex v, int weight);

// -----------------------------
// Interface functions
// -----------------------------

// Create new graph
Graph newGraph(int noNodes) {
    Graph g = malloc(sizeof(struct GraphRep));
    assert(g != NULL);
    g->nV = noNodes;
    g->nE = 0;
    Vertex i;
    g->edges = malloc(noNodes * sizeof(adjListNode));
    for (i = 0; i < noNodes; i++) {
        g->edges[i] = NULL;
    }
	return g;
}

// Return number of vertices in Graph
int numVerticies(Graph g) {
    assert(g != NULL);
    return g->nV;
}

// Insert a directed edge from 'src' to 'dest' with 'weight'
void  insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
    assert(g != NULL);
    g->edges[src] = insertLL(g->edges[src], dest, weight);
    g->nE++;
}

// Remove a directed edge from src to dest
void  removeEdge(Graph g, Vertex src, Vertex dest) {
    assert(g != NULL);
    AdjList curr, prev, tmp;
    curr = g->edges[src];
    // empty list
    if (curr == NULL) return;
    // first in list
    if (curr->w == dest) {
        tmp = curr;
        curr->next = tmp->next;
        free(tmp);
        g->nE--;
        return;
    }
    // general case
    prev = g->edges[src];
    for (curr = prev->next; curr != NULL; curr = curr->next) {
        if (curr->w == dest) {
            prev->next = curr->next;
            free(curr);
            g->nE--;
            return;
        }
        prev = prev->next;
    }
}

// Check whether there is an edge from src to dest
bool adjacent(Graph g, Vertex src, Vertex dest) {
    assert(g != NULL);
    AdjList curr;
    for (curr = g->edges[src]; curr != NULL; curr = curr->next) {
        if (curr->w == dest) return true;
    }
	return false;
}

// Return a list of adjacent vertices on outgoing edges from Vertex v
AdjList outIncident(Graph g, Vertex v) {
    assert(g != NULL);
    AdjList curr, new = NULL;
    for (curr = g->edges[v]; curr != NULL; curr = curr->next) {
        new = insertLL(new, curr->w, curr->weight);
    }
	return new;
}

// Return a list of adjacent vertices on incoming edges to Vertex v
AdjList inIncident(Graph g, Vertex v) {
    assert(g != NULL);
    AdjList curr, new = NULL;
    Vertex i;
    for (i = 0; i < g->nV; i++) {
        for (curr = g->edges[i]; curr != NULL; curr = curr->next) {
            if (curr->w == v) new = insertLL(new, i, curr->weight);
        }
    }
	return new;
}

// Display the graph
void  showGraph(Graph g) {
    assert(g != NULL);
    if (g == NULL) return;
    Vertex i;
    for (i = 0; i < g->nV; i++) {
        AdjList curr = g->edges[i];
        printf("\n Vertex: %d  :  {", i);
        while (curr != NULL) {
            printf(" %d,", curr->w);
            curr = curr->next;
        }
        printf(" }\n");
    }
}

// Free memory associated with the Graph
void  freeGraph(Graph g) {
    if (g == NULL) return;
    Vertex i;
    AdjList curr, tmp;
    for (i = 0; i < g->nV; i++) {
        curr = g->edges[i];
        while (curr != NULL) {
            tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(g->edges);
    free(g);
}

// Insert a node into an AdjList
static AdjList insertLL(AdjList L, Vertex v, int weight) {
    AdjList new = malloc(sizeof(adjListNode));
    new->w = v;
    new->weight = weight;
    if (L == NULL) {
        new->next = NULL;
    } else {
        new->next = L;
    }
    return new;
}
