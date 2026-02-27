#include "../include/hash.h"

// Função de hash que combina múltiplos atributos do voo para calcular um índice na tabela hash
// Entrada: voo - estrutura do tipo Voo contendo os atributos do voo
// Saída: índice calculado para a tabela hash
int hashFunction(Voo voo) {
    return (voo.id * 31 + voo.org_val * 17 + voo.dst_val * 13) % TAMANHO_HASH;
}

// Inicializa a tabela hash, definindo todas as posições como NULL
// Entrada: hash - ponteiro para a tabela hash a ser inicializada
void inicializarHash(HashTable* hash) {
    if (!hash) return;
    for (int i = 0; i < TAMANHO_HASH; i++) {
        hash->tabela[i] = NULL;
    }
}

// Insere um voo na tabela hash. Se o voo já existir, incrementa a contagem de ocorrências
// Entrada: hash - ponteiro para a tabela hash
//          voo - estrutura do tipo Voo a ser inserida
void inserirHash(HashTable* hash, Voo voo) {
    if (!hash) return;

    int index = hashFunction(voo);
    HashNode* atual = hash->tabela[index];

    // Percorrer a lista encadeada para verificar se já existe
    while (atual) {
        if (atual->voo.id == voo.id && 
            strcmp(atual->voo.org, voo.org) == 0 &&
            strcmp(atual->voo.dst, voo.dst) == 0) {
            atual->count++; // Voo já existe, incrementa a contagem
            return;
        }
        atual = atual->prox;
    }

    // Se o voo não existir, cria um novo nó
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    if (!novo) {
        fprintf(stderr, "Erro ao alocar memória para HashNode\n");
        exit(EXIT_FAILURE);
    }
    novo->voo = voo;
    novo->count = 1; // Primeira ocorrência
    novo->prox = hash->tabela[index]; // Insere no início da lista encadeada
    hash->tabela[index] = novo;
}

// Busca a contagem de ocorrências de um voo na tabela hash
// Entrada: hash - ponteiro para a tabela hash
//          voo - estrutura do tipo Voo a ser buscada
// Saída: número de ocorrências do voo (0 se não encontrado)
int buscarContagemHash(HashTable* hash, Voo voo) {
    if (!hash) return 0;

    int index = hashFunction(voo);
    HashNode* atual = hash->tabela[index];

    while (atual) {
        if (atual->voo.id == voo.id && 
            strcmp(atual->voo.org, voo.org) == 0 &&
            strcmp(atual->voo.dst, voo.dst) == 0) {
            return atual->count; // Retorna a contagem do voo
        }
        atual = atual->prox;
    }
    return 0; // Voo não encontrado
}

// Libera a memória alocada para a tabela hash
// Entrada: hash - ponteiro para a tabela hash a ser liberada
void liberarHash(HashTable* hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode* atual = hash->tabela[i];
        while (atual) {
            HashNode* temp = atual;
            atual = atual->prox;
            free(temp);
        }
        hash->tabela[i] = NULL;
    }
}
