#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(){
    ItemPQ a;
    PQ q = newPQ();
    assert(PQEmpty(q));
    showPQ(q);
    int i;
    for (i = 0; i < 10; i++) {
        a.key = i;
        a.value = 12 - i;
        addPQ(q, a);
    }
    assert(!PQEmpty(q));
    
    printf("\n===== original priority queue =====\n");
    showPQ(q);

    printf("\n===== change value of key 5 to 1 =====\n");
    a.key = 5;
    a.value = 1;
    updatePQ(q, a);
    showPQ(q);
    
    printf("\n===== add item key 3 with value 1 =====\n");
    a.key = 3;
    a.value = 1;
    addPQ(q, a);
    showPQ(q);

    printf("\n===== change value of key 5 to 13 =====\n");
    a.key = 5;
    a.value = 13;
    updatePQ(q, a);
    showPQ(q);

    printf("\n===== update key 0 value to 6 =====\n");
    a.key = 0;
    a.value = 6;
    updatePQ(q, a);
    showPQ(q);

    printf("\n===== update key 9 value to 6 =====\n");
    a.key = 9;
    a.value = 6;
    updatePQ(q, a);
    showPQ(q);

    printf("\n===== update key 2 value to 6 =====\n");
    a.key = 2;
    a.value = 6;
    updatePQ(q, a);
    showPQ(q);

    printf("\n===== update key 3 value to 6 =====\n");
    a.key = 3;
    a.value = 6;
    updatePQ(q, a);
    showPQ(q);

    printf("\n===== update key 5 value to 6 =====\n");
    a.key = 5;
    a.value = 6;
    updatePQ(q, a);
    showPQ(q);

    printf("\n===== add key 11, 12, 13, 14 all value 6 =====\n");
    for (i = 11; i < 15; i++) {
        a.key = i;
        a.value = 6;
        addPQ(q, a);
    }

    printf("\n===== iteratively dequeue until empty =====\n");
    while(!PQEmpty(q)) {
        printf("\n");
        a=dequeuePQ(q);
        showPQ(q);
        printf("Removed: %d(%d)\n", a.value, a.key);
        printf("\n");
    }

    assert(PQEmpty(q));
    freePQ(q);
  return EXIT_SUCCESS;
}
