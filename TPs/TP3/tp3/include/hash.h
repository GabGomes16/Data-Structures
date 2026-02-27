#ifndef HASH_H
#define HASH_H

#include "voos.h"
#include "arvore.h"

#define TAMANHO_HASH 10007

// Estrutura do nó da tabela hash
typedef struct HashNode {
    Voo voo; // O voo completo como chave
    int count; // Contagem de ocorrências
    struct HashNode* prox;
} HashNode;

// Estrutura da tabela hash
typedef struct {
    HashNode* tabela[TAMANHO_HASH];
} HashTable;

int hashFunction(Voo voo);
void inicializarHash(HashTable* hash);
void inserirHash(HashTable* hash, Voo voo);
int buscarContagemHash(HashTable* hash, Voo voo);
void liberarHash(HashTable* hash);

#endif