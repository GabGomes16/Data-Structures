#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "paciente.h"

// Estrutura da fila
typedef struct {
    Paciente* elementos; // Vetor dinâmico para armazenar os elementos da fila
    int inicio;              // Índice do início da fila
    int fim;                 // Índice do final da fila
    int tamanho;             // Quantidade atual de elementos na fila
    int capacidade;          // Capacidade total do vetor
} Fila;


// Inicializa a fila com a capacidade fornecida
void inicializaFila(Fila* fila, int tamanho);

// Enfileira um paciente na fila
void enfileira(Fila* fila, Paciente paciente, double tempo_espera_total, double tempo_chegada);

// Desenfileira um paciente da fila
bool desenfileira(Fila* fila, Paciente* paciente, double* tempo_espera_total, double* tempo_chegada);

// Verifica se a fila está vazia
bool filaVazia(Fila* fila);

// Finaliza a fila, liberando a memória alocada
void finalizaFila(Fila* fila);

#endif