#include <stdio.h>
#include <stdlib.h>
#include "../include/fila.h" 

// Inicializa a fila com a capacidade fornecida
void inicializaFila(Fila* fila, int tamanho) {
    fila->elementos = malloc(sizeof(Paciente) * tamanho);
    if (!fila->elementos) {
        perror("Erro ao alocar memória para a fila");
        exit(EXIT_FAILURE);
    }
    fila->inicio = 0;
    fila->fim = -1;
    fila->tamanho = 0;
    fila->capacidade = tamanho;

    //printf("[DEBUG] Fila inicializada com capacidade %d\n", tamanho);
}

// Enfileira um paciente na fila
void enfileira(Fila* fila, Paciente paciente, double tempo_espera_total, double tempo_chegada) {
    if (fila->tamanho == fila->capacidade) {
        // Redimensionar o vetor
        fila->capacidade *= 2;
        fila->elementos = realloc(fila->elementos, sizeof(Paciente) * fila->capacidade);
        if (!fila->elementos) {
            perror("Erro ao redimensionar a fila");
            exit(EXIT_FAILURE);
        }
        //printf("[DEBUG] Capacidade da fila aumentada para %d\n", fila->capacidade);
    }

    fila->elementos[fila->tamanho] = paciente;
    fila->elementos[fila->tamanho].tempo_espera_total = tempo_espera_total;
    fila->elementos[fila->tamanho].tempo_chegada = tempo_chegada;

    //printf("[DEBUG] Enfileirando Paciente ID %d, Tempo de Chegada %.2f Tempo de espera %.2f\n", paciente.id, tempo_chegada, tempo_espera_total);
    //printf("[DEBUG] Numero de medidas hospitalares: %d\n Numero de testes de laboratorio: %d\n Numero de exames de imagem: %d\n Numero de instrumentos e medicamentos: %d\n", paciente.medidas_hospitalares, paciente.testes_laboratorio, paciente.exames_imagem, paciente.instrumentos_medicamentos);
    fila->tamanho++;
}

// Desenfileira um paciente da fila
bool desenfileira(Fila* fila, Paciente* paciente, double* tempo_espera_total, double* tempo_chegada) {
    if (filaVazia(fila)) {
        //printf("[DEBUG] Fila vazia, não é possível desenfileirar\n");
        return false;
    }

    // O primeiro elemento da fila é o que será removido
    *paciente = fila->elementos[0];
    *tempo_espera_total = fila->elementos[0].tempo_espera_total;
    *tempo_chegada = fila->elementos[0].tempo_chegada;

    // Substitui o elemento removido pelo último da fila
    fila->elementos[0] = fila->elementos[fila->tamanho - 1];
    fila->tamanho--;

    return true;
}

// Verifica se a fila está vazia
bool filaVazia(Fila* fila) {
    return fila->tamanho == 0;
}

// Finaliza a fila, liberando a memória alocada
void finalizaFila(Fila* fila) {
    free(fila->elementos);
    fila->elementos = NULL;
    fila->inicio = 0;
    fila->fim = -1;
    fila->tamanho = 0;
    fila->capacidade = 0;

    //printf("[DEBUG] Fila finalizada\n");
}