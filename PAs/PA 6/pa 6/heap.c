    #include <stdio.h>
#include <stdlib.h>
#include "heap.h"

// Cria um novo heap
Heap* NovoHeap(int maxsize) {
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->tamanho = 0;
    h->dados = (int*)malloc(maxsize * sizeof(int));
    return h;
}

// Deleta o heap
void DeletaHeap(Heap* h) {
    free(h->dados);
    free(h);
}

// Funções para acessar ancestrais e sucessores
int GetAncestral(Heap* h, int posicao) {
    return (posicao - 1) / 2;
}

int GetSucessorEsq(Heap* h, int posicao) {
    return 2 * posicao + 1;
}

int GetSucessorDir(Heap* h, int posicao) {
    return 2 * posicao + 2;
}

// Verifica se o heap está vazio
int Vazio(Heap* h) {
    return h->tamanho == 0;
}

// Insere um elemento no heap
void Inserir(Heap* h, int x) {
    int i = h->tamanho++;
    h->dados[i] = x;

    HeapifyPorCima(h, i);
}

// Remove o menor elemento do heap
int Remover(Heap* h) {
    if (Vazio(h)) {
        return -1; // Heap vazio
    }
    int min = h->dados[0];
    h->dados[0] = h->dados[--h->tamanho];
    HeapifyPorBaixo(h, 0);
    return min;
}

// Heapify para restaurar a propriedade do heap (subindo)
void HeapifyPorCima(Heap* h, int posicao) {
    int pai = GetAncestral(h, posicao);
    while (posicao > 0 && h->dados[posicao] < h->dados[pai]) {
        int temp = h->dados[posicao];
        h->dados[posicao] = h->dados[pai];
        h->dados[pai] = temp;

        posicao = pai;
        pai = GetAncestral(h, posicao);
    }
}

// Heapify para restaurar a propriedade do heap (descendo)
void HeapifyPorBaixo(Heap* h, int posicao) {
    int menor = posicao;
    int esq = GetSucessorEsq(h, posicao);
    int dir = GetSucessorDir(h, posicao);

    if (esq < h->tamanho && h->dados[esq] < h->dados[menor]) {
        menor = esq;
    }
    if (dir < h->tamanho && h->dados[dir] < h->dados[menor]) {
        menor = dir;
    }

    if (menor != posicao) {
        int temp = h->dados[posicao];
        h->dados[posicao] = h->dados[menor];
        h->dados[menor] = temp;

        HeapifyPorBaixo(h, menor);
    }
}
