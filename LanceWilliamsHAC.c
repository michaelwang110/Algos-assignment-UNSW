// LanceWilliamsHAC ADT interface for Ass2 (COMP2521)
#include "LanceWilliamsHAC.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Store coordinates of shortest distance in dist matrix
typedef struct shortestDist {
    int i;
    int j;
} shortestDist;

// Store distances to other clusters from the two closest clusters, ci and cj
typedef struct distIJ {
    double *ci;
    double *cj;
} distIJ;

// -----------------------------
// Helper function prototypes
// -----------------------------
static Dendrogram newDendrogram(int v, Dendrogram left, Dendrogram right);
static double **newDistMatrix(int n);
static void fillDistMatrix(Graph g, int n, double *dist[]);
static Dendrogram *newDendA(int n);
static void fillDendA(int n, Dendrogram dendA[]);
static shortestDist closestClusters(int n, double *dist[]);
static void removeEleDendA(Dendrogram dendA[], int n, Dendrogram d);
static distIJ newDistIJ(double *dist[], int n, int i, int j);
static void reduceDistMatrix(double *dist[], int n, int i, int j);
static void removeRow(double *dist[], int width, int height, int i);
static void removeColumn(double *dist[], int width, int height, int j);
static void updateDistMatrix(int method, double *dist[], distIJ IJ, int n);
static void singleUpdate(double *dist[], distIJ IJ, int n);
static void completeUpdate(double *dist[], distIJ IJ, int n);
static double min(double cik, double cjk);
static double max(double cik, double cjk);
static double absolute(double a);
static void freeDistIJ(distIJ IJ);
static void freeDistMatrix(double *dist[], int n);
static void freeDendA(Dendrogram dendA[], int n);

// -----------------------------
// Interface functions
// -----------------------------

// Finds Dendrogram using Lance-Williams algorithm for the given graph g and
// the specified method for agglomerative clustering.
// Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
// Returns 'Dendrogram' structure (binary) tree with the required information.
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    assert(g != NULL);
    // Get number of vertices in Graph g
    int N = numVerticies(g);
    // initialise dist array
    double **dist = newDistMatrix(N);
    fillDistMatrix(g, N, dist);
    // initialise dendrogram array
    Dendrogram *dendA = newDendA(N);
    fillDendA(N, dendA);
    // iterate to find clusters in Dendrogram
    int i, j, I, n;
    Dendrogram ci, cj, cij;
    n = N;
    for (I = 1; I < N; I++) {
        // find the two closest clusters, ci and cj
        shortestDist matrix = closestClusters(n, dist);
        i = matrix.i;
        j = matrix.j;
        ci = dendA[i];
        cj = dendA[j];
        // remove clusters ci and cj from dendA
        removeEleDendA(dendA, n, ci);
        removeEleDendA(dendA, n-1, cj);
        // add merged cluster cij to end of dendA 
        cij = newDendrogram(0, ci, cj);
        dendA[n-2] = cij;
        // record distance data for i and j 
        distIJ IJ = newDistIJ(dist, n, i, j);
        // reduce dimension of dist matrix
        reduceDistMatrix(dist, n, i , j);
        // update the dist matrix using Lance-Williams
        updateDistMatrix(method, dist, IJ, n);
        // free memory associated with distIJ struct
        freeDistIJ(IJ);
        // reduce n by 1
        n--;
    }
    freeDistMatrix(dist, N);
    freeDendA(dendA, N);
    return dendA[0];
}

// Free memory associated with a dendrogram
void freeDendrogram(Dendrogram d) {
    if (d == NULL) return;
    freeDendrogram(d->left);
    freeDendrogram(d->right);
    free(d);
}

// Make new Dendrogram
static Dendrogram newDendrogram(int v, Dendrogram left, Dendrogram right) {
    Dendrogram new = malloc(sizeof(DNode));
    new->vertex = v;
    new->left = left;
    new->right = right;
    return new;
}
// Allocate memory to a n x n matrix of doubles
static double **newDistMatrix(int n) {
    double **new = malloc(n*sizeof(double *));
    assert(new != NULL);
    int i;
    for (i = 0; i < n; i++) {
        new[i] = malloc(n*sizeof(double));
        assert(new[i] != NULL);
    }
    return new;
}

// Fill dist matrix with shortest distances
static void fillDistMatrix(Graph g, int n, double *dist[]) {
    assert(g != NULL);
    AdjList curr = NULL;
    int i, j;
    double a, d1, d2;
    a = 0;
    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            d1 = d2 = 1/a;
            // find the weight of edges
            for (curr = outIncident(g, i); curr != NULL; curr = curr->next) {
                if (curr->w == j) {
                    d1 = (double)1/curr->weight;
                    break;
                }
            }
            for (curr = outIncident(g, j); curr != NULL; curr = curr->next) {
                if (curr->w == i) {
                    d2 = (double)1/curr->weight;
                    break;
                }
            }
            // fill array
            if (d1 < d2) {
                dist[i][j] = dist[j][i] = d1;
            } else {
                dist[i][j] = dist[j][i] = d2;
            }
        }
    }
}

// Allocate memory to an array of Dendrograms
static Dendrogram *newDendA(int n) {
    Dendrogram *new = malloc(n*sizeof(Dendrogram));
    assert(new != NULL);
    int i;
    for (i = 0; i < n; i++)
        new[i] = NULL;
    return new;
}

// Fill Dendrogram Array with all single vertices
static void fillDendA(int n, Dendrogram dendA[]) {
    int i;
    for (i = 0; i < n; i++)
        dendA[i] = newDendrogram(i, NULL, NULL);
}

// Finds and returns dist matrix inputs for minimum distance
static shortestDist closestClusters(int n, double *dist[]) {
    int i, j;
    shortestDist matrix;
    matrix.i = 1;
    matrix.j = 0;
    double min = dist[1][0];
    // find minimum value in dist matrix
    for (i = 1; i < n; i++)
        for (j = 0; j < i; j++)
            if (min > dist[i][j] && i != j)
                min = dist[i][j];
    // find and return array inputs for min 
    for (i = 1; i < n; i++) {
        for (j = 0; j < i; j++) {
            if (dist[i][j] == min && i != j) {
                matrix.i = i;
                matrix.j = j;
                return matrix;
            }
        }
    }
    return matrix;
}

// Removes Dendrogram d from dendA
static void removeEleDendA(Dendrogram dendA[], int n, Dendrogram d) {
    // if d is the last elementi
    if (dendA[n-1] == d) {
        dendA[n-1] = NULL;
        return;
    }
    // start from last element in array and move backwards
    int i;
    Dendrogram curr, next;
    next = dendA[n-1];
    for (i = n - 2; i >= 0 && dendA[i] != d; i--) {
        curr = dendA[i];
        dendA[i] = next;
        next = curr;
    }
    // replace d by next element
    dendA[i] = next;
    dendA[n-1] = NULL;
}

// Fill previousDistInfo struct with i and j distance data
static distIJ newDistIJ(double *dist[], int n, int i, int j) {
    distIJ new;
    new.ci = malloc((n-1)*sizeof(double));
    new.cj = malloc((n-1)*sizeof(double));
    assert(new.ci != NULL && new.cj != NULL);
    int v = 0;
    for (int w = 0; w < n; w++) {
        if (w == i || w == j) {
            if (w+1 == i || w+1 == j) {
                new.ci[v] = dist[i][w+2];
                new.cj[v] = dist[j][w+2];
                w+=2; 
            } else {
                new.ci[v] = dist[i][w+1];
                new.cj[v] = dist[j][w+1];
                w++;
            }
        } else {
            new.ci[v] = dist[i][w];
            new.cj[v] = dist[j][w];
        }
        v++;
    }
    return new;
}

// Reduce dimension of dist array by removing i and j
static void reduceDistMatrix(double *dist[], int n, int i, int j) {
    removeRow(dist, n, n, i);
    removeColumn(dist, n, n-1, i);
    if (j < i) {
        removeRow(dist, n-1, n-1, j);
        removeColumn(dist, n-1, n-2, j);
    } else {
        removeRow(dist, n-1, n-1, j-1);
        removeColumn(dist, n-1, n-2, j-1);
    }
}

// Remove row i from dist matrix
static void removeRow(double *dist[], int width, int height, int i) {
    height--;
    int j, tmp;
    tmp = i;
    for (j = 0; j < width; j++) {
        while (i < height) {
            dist[i][j] = dist[i+1][j];
            i++;
        }
        i = tmp;
    }
}

// Remove column j from dist matrix
static void removeColumn(double *dist[], int width, int height, int j) {
    width--;
    int i, tmp;
    tmp = j;
    for (i = 0; i < height; i++) {
        while (j < width) {
            dist[i][j] = dist[i][j+1];
            j++;
        }
        j = tmp;
    }
}

// Update dist matrix using Lance-Williams method
static void updateDistMatrix(int method, double *dist[], distIJ IJ, int n) {
    if (method == 1)
        singleUpdate(dist, IJ, n-1);
    else
        completeUpdate(dist, IJ, n-1);
}

// Update dist matrix using single link method
static void singleUpdate(double *dist[], distIJ IJ, int n) {
    n--;
    for (int k = 0; k < n; k++) {
        dist[n][k] = min(IJ.ci[k], IJ.cj[k]);
        dist[k][n] = dist[n][k];
    }
}

// Update dist matrix using complete link method
static void completeUpdate(double *dist[], distIJ IJ, int n) {
    n--;
    for (int k = 0; k < n; k++) {
        dist[n][k] = max(IJ.ci[k], IJ.cj[k]);
        dist[k][n] = dist[n][k];
    }
}

// Single link Lance-Williams formula
static double min(double cik, double cjk) {
    return (0.5 * (cik + cjk - absolute(cik - cjk)));
}

// Complete link Lance-Williams formula
static double max(double cik, double cjk) {
    return (0.5 * (cik + cjk + absolute(cik - cjk)));
}

// Return absolute value of double
static double absolute(double a) {
    if (a < 0) return -a;
    return a;
}

// Free memory associated with the distIJ struct
static void freeDistIJ(distIJ IJ) {
    free(IJ.ci);
    free(IJ.cj);
}

// Free memory associated with the distance matrix
static void freeDistMatrix(double *dist[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        free(dist[i]);
    }
    free(dist);
}

// Free memory all memory associated with dendA except at dendA[0]
static void freeDendA(Dendrogram dendA[], int n) {
    int i;
    for (i = 1; i < n; i++) {
        freeDendrogram(dendA[i]);
    }
}
