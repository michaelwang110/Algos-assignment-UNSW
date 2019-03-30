// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Tracks shortest paths from a src to dest and number of times paths go through v
typedef struct Betweeness {
    int counterV; // #times pass through v
    int counterSP; // #shortestpaths
} Betweeness;

// ----------------------------- 
// Helper function prototypes
// ----------------------------- 
static NodeValues newNodeValues(int nV);
static Betweeness countShortestPaths(ShortestPaths spSrc, Betweeness vB, int dest, int src, int v);

// ----------------------------- 
// Interface functions
// -----------------------------

// Centrality based on number of ties that a node directs to others
NodeValues outDegreeCentrality(Graph g){
    NodeValues new = newNodeValues(numVerticies(g));
    Vertex i;
    AdjList curr;
    // Loop through outIncident list to find outDegreeCentrality
    for (i = 0; i < numVerticies(g); i++)
        for (curr = outIncident(g, i); curr != NULL; curr = curr->next)
            new.values[i] = new.values[i] + 1;
    return new;
}

// Centrality based on number of ties directed to the node
NodeValues inDegreeCentrality(Graph g){
    NodeValues new = newNodeValues(numVerticies(g));
    Vertex i;
    AdjList curr;
    // Loop through inIncident list to find inDegreeCentrality
    for (i = 0; i < numVerticies(g); i++)
        for (curr = inIncident(g, i); curr != NULL; curr = curr->next)
            new.values[i] = new.values[i] + 1;
    return new;
}

// Centrality based on total number of ties, both directed to and directed 
// from the node
NodeValues degreeCentrality(Graph g) {
    NodeValues new = newNodeValues(numVerticies(g));
    NodeValues outDegree = outDegreeCentrality(g);
    NodeValues inDegree = inDegreeCentrality(g);
    // add outDegreeCentrality and inDegreeCentrality for each node
    int i;
    for (i = 0; i < numVerticies(g); i++)
        new.values[i] = outDegree.values[i] + inDegree.values[i];
    return new;
}

// Centrality based on sum of the length of the shortest paths between a node
// and all other nodes
NodeValues closenessCentrality(Graph g){
    NodeValues cloCen = newNodeValues(numVerticies(g));   
    double nodesReached = 0;
    Vertex i, j;
    double closeness = 0;
    double noNodes = numVerticies(g);
    for (i = 0; i < noNodes; i++) {
        ShortestPaths new = dijkstra(g, i);
        nodesReached = 0;
        closeness = 0;
        for (j = 0; j < noNodes; j++) {
            if (i != j) {
                if (new.dist[j] > 0) {
                    nodesReached++;
                    closeness = closeness + new.dist[j];
                }
            } else {
                nodesReached++;
            }
        }
        if (outIncident(g, i) == NULL) {
            cloCen.values[i] = 0;
        } else {
            cloCen.values[i] = ((nodesReached - 1)*(nodesReached - 1))/((noNodes - 1)*closeness);
        }
    }
    return cloCen;
}

// Centrality based on number of shortest paths
NodeValues betweennessCentrality(Graph g){
    int src, dest, v, nV;
    nV = numVerticies(g);
	NodeValues new = newNodeValues(nV);
    Betweeness vB;
    for (v = 0; v < nV; v++) {
        for (src = 0; src < nV; src++) {
            if (src != v) {
                ShortestPaths spSrc = dijkstra(g, src);
                for (dest = 0; dest < nV; dest++) {
                    if (dest != src && dest != v) {
                        vB.counterV = vB.counterSP = 0.000000;
                        vB = countShortestPaths(spSrc, vB, dest, src, v);
                        if (vB.counterSP != 0.000000) {
                            new.values[v] += (double)vB.counterV/(double)vB.counterSP;
                        }
                    }
                }
            }
        }
    }
    return new;
}

// Centrality based on normalising betweenessCentrality
NodeValues betweennessCentralityNormalised(Graph g){
	NodeValues new = newNodeValues(numVerticies(g));
        
    Vertex i;
    int noNodes = numVerticies(g);
    NodeValues btnCent = betweennessCentrality(g);
    for (i = 0; i < noNodes; i++) {
        new.values[i] = btnCent.values[i]/((noNodes-1)*(noNodes-2));
    }
	return new;
}

// Display all node values
void showNodeValues(NodeValues values){
    Vertex i;
    for (i = 0; i < values.noNodes; i++) {
        printf("%d: %lf\n", i, values.values[i]);
    }
}

// Free memory associated with NodeValues struct
void freeNodeValues(NodeValues values){
    free(values.values);
}

// Make a new NodeValues struct
static NodeValues newNodeValues(int nV) {
    NodeValues new;
    new.noNodes = nV;
    new.values = malloc(nV*sizeof(double));
    assert(new.values != NULL);
    for (int i = 0; i < nV; i++) {
        new.values[i] = 0;
    }
    return new;
}

// Return Betweeness struct for a given source, destination and v node
static Betweeness countShortestPaths(ShortestPaths spSrc, Betweeness vB, int dest, int src, int v) {
    PredNode *prev;
    for (prev = spSrc.pred[dest]; prev != NULL; prev = prev->next) {
        if (prev->v == src)
            vB.counterSP++;
        else if (prev->v == v && prev->v !=src)
            vB.counterV++;
        vB = countShortestPaths(spSrc, vB, prev->v, src, v); 
    }
    return vB;
}
