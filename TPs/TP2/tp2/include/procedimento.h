#ifndef PROCEDIMENTOS_H
#define PROCEDIMENTOS_H
#include "fila.h"
#include "paciente.h"

// Estrutura de um procedimento
typedef struct {
    double tempo_medio; // Tempo médio de execução do procedimento
    int numero_unidades; // Número de unidades disponíveis
    double* ocupacao_unidades; // Vetor que mantém o tempo de ocupação de cada unidade
} Procedimento;

// Função para calcular o tempo de chegada de um paciente
double calcularTempoChegada(const Paciente* paciente);

// Função para inicializar um procedimento com o tempo médio e número de unidades
void inicializar_procedimento(Procedimento* procedimento, float tempo_medio, int numero_unidades);

// Função para carregar os procedimentos do arquivo
void carregar_arquitetura(FILE* arquivo, Procedimento* procedimentos);

// Libera a memória associada a um procedimento
void liberar_procedimento(Procedimento* procedimento);

// Função para calcular o tempo mínimo de término de um procedimento
double tempo_minimo_termino(Procedimento* procedimento);

// Função para atualizar o tempo de término de um procedimento
void atualiza_tempo_termino(Procedimento* procedimento, double tempo);

#endif