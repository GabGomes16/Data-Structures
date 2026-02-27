#ifndef ESCALONADOR_H
#define ESCALONADOR_H
#include "procedimento.h"
#include "paciente.h"
#include "fila.h"

// Estrutura de um evento
typedef struct {
    Procedimento * procedimento; // Procedimento associado ao evento
    Fila * filas; // Filas associadas ao evento
    Paciente * paciente; // Paciente associado ao evento
    double inicio; // Tempo em horas desde a data de referência
    double fim; // Tempo em horas desde a data de referência
} Evento;

// Estrutura do MinHeap
typedef struct {
    Evento* heap; // Vetor de eventos
    int capacidade; // Capacidade máxima do heap
    int tamanho; // Tamanho atual do heap
} MinHeap;

// Função para inicializar o MinHeap
MinHeap* inicializaHeap(int capacidadeInicial);

// Função para trocar dois eventos
void troca(Evento* a, Evento* b);

// Função para ajustar para cima (heapify-up)
void ajustaParaCima(MinHeap* minHeap, int index);

void ajustaParaCimaID(MinHeap* minHeap, int index);

// Função para ajustar para baixo (heapify-down)
void ajustaParaBaixo(MinHeap* minHeap, int index);

// Função para expandir a capacidade do heap
void expandirHeap(MinHeap* minHeap);

// Função para inserir um evento no MinHeap
void insereEvento(MinHeap* minHeap, Procedimento * procedimento, Fila * filas, Paciente * paciente, double inicio);

void insereEventoID(MinHeap* minHeap, Procedimento * procedimento, Fila * filas, Paciente * paciente, double inicio, int id);

// Função para retirar o próximo evento do MinHeap
Evento retiraProximoEvento(MinHeap* minHeap);

// Função para finalizar o MinHeap
void finalizaHeap(MinHeap* minHeap);

// Função para exibir os eventos do heap (apenas para teste)
void exibeHeap(MinHeap* minHeap);

#endif