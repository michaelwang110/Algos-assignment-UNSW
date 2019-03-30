// PQ ADT interface for Ass2 (COMP2521)
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


#define SIZE 99999

struct PQRep {
    ItemPQ *items; // array of items
    int *time; // array to track when an item has been added or updated
    int counter; // number of times addPQ or updatePQ has been used
    int nitems; // #items in array
    int nslots; // maximum #items in array
};

// ----------------------------- 
// Helper function prototypes
// -----------------------------
static void fixUp(ItemPQ a[], int i);
static void fixDown(ItemPQ a[], int i, int N);
static bool lessCounter(PQ q, int k1, int k2);
static bool greater(ItemPQ it1, ItemPQ it2);
static void swap(ItemPQ a[], int i, int j);

// -----------------------------
// Interface functions
// -----------------------------

// Creates new priority queue, that can store items of type ItemPQ
PQ newPQ() {
    PQ new = malloc(sizeof(struct PQRep));
    assert(new != NULL);
    new->items = malloc((SIZE+1)*sizeof(ItemPQ));
    assert(new->items != NULL);
    new->time = malloc(SIZE*sizeof(int));
    assert(new->time != NULL);
    new->counter = 0;
    new->nitems = 0;
    new->nslots = SIZE;
    return new;
}

// Adds item (ItemPQ) to the priority queue. If an item with 'key' exists, it's
// 'value' is updated
void addPQ(PQ q, ItemPQ it) {
    // update item's position on FIFO
    q->counter++;
    q->time[it.key] = q->counter;
    // Update item's 'value' if same 'key'
    int i;
    for (i = 1; i <= q->nitems; i++) {
        if (q->items[i].key == it.key) {
            if (it.value < q->items[i].value) {
                q->items[i].value = it.value;
                fixUp(q->items, i);
            } else if (it.value > q->items[i].value) {
                q->items[i].value = it.value;
                fixDown(q->items, i, q->nitems);
            }
            return;
        }
    }   
    // add item with new 'key'
    assert(q->nitems < q->nslots);
    q->nitems++;
    q->items[q->nitems] = it;
    fixUp(q->items, q->nitems);
}

// Removes and return the item (ItemPQ) with the smallest 'value'
ItemPQ dequeuePQ(PQ q) {
    // maintain FIFO if multiple 'keys' have minimum 'value'
    int i;
    for (i = 1; i <= q->nitems; i++)
        if (q->items[1].value == q->items[i].value)
            if (lessCounter(q, q->items[i].key, q->items[1].key))
                swap(q->items, 1, i);
    // overwrite root item by last item
    ItemPQ top = q->items[1];
    q->items[1] = q->items[q->nitems];
    q->nitems--;
    // move root to correct position
    fixDown(q->items, 1, q->nitems);
    return top;
}

// Updates item with a given 'key' value to the given 'value'
void updatePQ(PQ q, ItemPQ it) {
    int i;
    for (i = 1; i <= q->nitems; i++) {
        if (q->items[i].key == it.key) {
                q->counter++;
                q->time[it.key] = q->counter;
            if (it.value < q->items[i].value) {
                q->items[i].value = it.value;
                fixUp(q->items, i);
            } else if (it.value > q->items[i].value) { 
                q->items[i].value = it.value;
                fixDown(q->items, i, q->nitems);
            }
            return;
        }
    }
}

// Check if priority queue is empty
int PQEmpty(PQ q) {
    if (q->nitems == 0) return 1;
    return 0;
}

// Display priority queue from front to back as 2(2) > 3(1) > 4(3) > 5(0) > ...
void showPQ(PQ q) {
    if (q->nitems == 0) {
        printf("Priority Queue is empty\n");
        return;
    }
    printf("Priority Queue:\n[START] ");
    int i;
    for (i = 1; i < q->nitems; i++)
        printf("%d(%d) > ", q->items[i].value, q->items[i].key);
    printf("%d(%d) [END]", q->items[q->nitems].value, q->items[q->nitems].key);
    printf("\n");
}

// Free memory associated with the Priority Queue
void freePQ(PQ q) {
    free(q->items);
    free(q->time);
    free(q);
}

// Force value at a[i] into correct position upwards
static void fixUp(ItemPQ a[], int i) {
    while (i > 1 && greater(a[i/2], a[i])) {
        swap(a, i, i/2);
        i = i/2;
    }
}

// Force value at a[i] into correct position downwards
static void fixDown(ItemPQ a[], int i, int N) {
    while (2*i <= N) {
        // compute address of left child
        int j = 2*i;
        // choose smaller of two children
        if (j < N && greater(a[j], a[j+1])) j++;
        if(!greater(a[i], a[j])) break;
        swap(a, i, j);
        // move one level down the heap
        i = j;
    }
}

// Return 1 if key k1 added/updated to PQ before key k2, otherwise return 0
static bool lessCounter(PQ q, int k1, int k2) {
    if (q->time[k1] < q->time[k2]) return true;
    return false;
}

// Return true if i is greater than j, otherwise return false
static bool greater(ItemPQ it1, ItemPQ it2) {
    if (it1.value > it2.value) return true;
    return false;
}

// Swap two ItemPQs
static void swap(ItemPQ a[], int i, int j) {
    ItemPQ tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}
