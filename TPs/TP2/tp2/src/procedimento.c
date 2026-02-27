#include <stdio.h>
#include <stdlib.h>
#include "procedimento.h"

// Inicializa um procedimento com o tempo médio e número de unidades
void inicializar_procedimento(Procedimento* procedimento, float tempo_medio, int numero_unidades) {
    procedimento->tempo_medio = tempo_medio;
    procedimento->numero_unidades = numero_unidades;
    procedimento->ocupacao_unidades = malloc(sizeof(double) * numero_unidades);

    if (!procedimento->ocupacao_unidades) {
        perror("Erro ao alocar memória para unidades");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numero_unidades; i++) {
        procedimento->ocupacao_unidades[i] = 0.0;  // Inicializa todas as unidades como ociosas
    }

    //printf("[DEBUG] Procedimento inicializado: Tempo Médio = %f, Número de Unidades = %d\n", tempo_medio, numero_unidades);
}

// Função para carregar os procedimentos do arquivo
void carregar_arquitetura(FILE* arquivo, Procedimento* procedimentos) {
    for (int i = 0; i < 6; i++) {
        float tempo_medio;
        int numero_unidades;
        if (fscanf(arquivo, "%f %d", &tempo_medio, &numero_unidades) != 2) {
            fprintf(stderr, "Erro ao ler os dados do procedimento %d.\n", i);
            exit(EXIT_FAILURE);
        }
        inicializar_procedimento(&procedimentos[i], tempo_medio, numero_unidades);
        //printf("[DEBUG] Procedimento %d carregado: Tempo Médio = %f, Número de Unidades = %d\n", i, tempo_medio, numero_unidades);
    }
    //printf("[DEBUG] Arquitetura carregada com sucesso\n");
}

// Libera a memória associada a um procedimento
void liberar_procedimento(Procedimento* procedimento) {
    free(procedimento->ocupacao_unidades);
    procedimento->ocupacao_unidades = NULL;

    //printf("[DEBUG] Memória liberada para procedimento\n");
}

double tempo_minimo_termino(Procedimento* procedimento) {
    double min = procedimento->ocupacao_unidades[0];
    for (int i = 1; i < procedimento->numero_unidades; i++) {
        if (procedimento->ocupacao_unidades[i] < min) {
            min = procedimento->ocupacao_unidades[i];
        }
    }
    return min;
}

void atualiza_tempo_termino(Procedimento* procedimento, double tempo) {
    if (procedimento == NULL || procedimento->ocupacao_unidades == NULL || procedimento->numero_unidades <= 0) {
        fprintf(stderr, "Erro: Procedimento ou vetor inválido.\n");
        return;
    }

    // Encontrar o índice da unidade com o menor tempo
    int indice_min = 0;
    for (int i = 1; i < procedimento->numero_unidades; i++) {
        if (procedimento->ocupacao_unidades[i] < procedimento->ocupacao_unidades[indice_min]) {
            indice_min = i;
        }
    }
    
    procedimento->ocupacao_unidades[indice_min] = tempo;
}