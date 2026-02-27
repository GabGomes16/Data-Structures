#include <stdio.h>
#include "heap.h"
#include "heap.c"

int main() {
    int n;
    scanf("%d", &n);

    Heap* heap = NovoHeap(n);

    for (int i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
        Inserir(heap, x);
    }

    while (!Vazio(heap)) {
        printf("%d ", Remover(heap));
    }
    printf("\n");

    DeletaHeap(heap);
    return 0;
}
