// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define INFINITY 99999

// ----------------------------- 
// Helper function prototypes
// ----------------------------- 
static ShortestPaths newShortestPaths(int nV, Vertex v);
static ItemPQ newItemPQ(Vertex v, int dist);
static ShortestPaths relax(ShortestPaths s, PQ q, Graph g, int v, int w);
static PredNode *newPredNode(Vertex v, PredNode *next);
static void freePred(PredNode *node);

// ----------------------------- 
// Interface functions
// ----------------------------- 

// Use Dijkstra to fill 'ShortestPaths' structure
ShortestPaths dijkstra(Graph g, Vertex v) {
    assert(g != NULL);
    ShortestPaths s = newShortestPaths(numVerticies(g), v);
    // dist and pred initialisations
    int i;
    for (i = 0; i < s.noNodes; i++) {
        s.dist[i] = INFINITY;
        s.pred[i] = NULL;
    }
    s.dist[v] = 0;
    // Add every vertex to priority queue
    PQ q = newPQ();
    for (i = 0; i < s.noNodes; i++) {
        ItemPQ a = newItemPQ(i, s.dist[i]);
        addPQ(q, a);
    }
    // Loop to find shortest paths 
    while (!PQEmpty(q)) {
        ItemPQ it = dequeuePQ(q);
        for (i = 0; i < s.noNodes; i++)
            if (adjacent(g, it.key, i))
                s = relax(s, q, g, it.key, i);
    }
    // Set distance of nodes with no path to 0
    for (i = 0; i < s.noNodes; i++)
        if (s.dist[i] == INFINITY)
            s.dist[i] = 0;
    freePQ(q);
    return s;
}

// Display ShortestPaths struct
// Based on code from testDijkstra.c given in Assignment 2 of COMP2521 in 
// Semester 1 2018
void showShortestPaths(ShortestPaths s) {
    int i;
    // print source node
    printf("Source Node %d\n", s.src);
    // print distances
    printf("  Distance\n");
    for (i = 0; i < s.noNodes; i++) {
        if (i == s.src)
            printf("    %d : X\n", i);
        else
            printf("    %d : %d\n", i, s.dist[i]);
    }
    // print predecessors
    printf("  Preds\n");
    for (i = 0; i < s.noNodes; i++) {
        printf("    %d ; ", i);
        PredNode *curr = s.pred[i];
        while (curr != NULL) {
            printf("[%d]->", curr->v);
            curr = curr->next;
        }
        printf("NULL\n");
    }
}

// Free memory associated with the ShortestPaths struct
void freeShortestPaths(ShortestPaths s) {
    int i;
    for (i = 0; i < s.noNodes; i++) {
        free(s.pred[i]);
    }
    free(s.dist);
}

// Make new ShortestPaths
static ShortestPaths newShortestPaths(int nV, Vertex v) { 
    ShortestPaths s;
    s.noNodes = nV;
    s.src = v;
    s.dist = malloc(nV*sizeof(int));
    assert(s.dist != NULL);
    s.pred = malloc(nV*sizeof(PredNode *));
    assert(s.pred != NULL);
    return s;
}

// Make a new ItemPQ
static ItemPQ newItemPQ(Vertex v, int dist) {
    ItemPQ new;
    new.key = v;
    new.value = dist;
    return new;
}

// Relax v to w
static ShortestPaths relax(ShortestPaths s, PQ q, Graph g, int v, int w) {
    AdjList curr;
    int alt;
    for (curr = inIncident(g, w); curr != NULL; curr = curr->next) {
        if (curr->w == v) break;
    }
    alt = s.dist[v] + curr->weight;
    if (alt < s.dist[w]) {
        s.dist[w] = alt;
        updatePQ(q, newItemPQ(w, s.dist[w]));
        freePred(s.pred[w]); // remove previous predecessors
        s.pred[w] = newPredNode(v, NULL); // replace with new predecessor
    } else if (alt == s.dist[w]) {
        s.pred[w] = newPredNode(v, s.pred[w]); // add to predecessor list
    }
    return s;
}

// Make a new PredNode pointer
static PredNode *newPredNode(Vertex v, PredNode *next) {
    PredNode *new = malloc(sizeof(PredNode));
    new->v = v;
    new->next = next;
    return new;
}

// Delete PredNode list
static void freePred(PredNode *node) {
    PredNode *curr, *tmp;
    curr = node;
    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        free(tmp);
    }
}
