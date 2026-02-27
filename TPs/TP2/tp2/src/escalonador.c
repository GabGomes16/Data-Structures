#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../include/escalonador.h"

// Função para inicializar o MinHeap
MinHeap* inicializaHeap(int capacidadeInicial) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->heap = (Evento*)malloc(capacidadeInicial * sizeof(Evento));
    minHeap->capacidade = capacidadeInicial;
    minHeap->tamanho = 0;
    return minHeap;
}

// Função para trocar dois eventos
void troca(Evento* a, Evento* b) {
    Evento temp = *a;
    *a = *b;
    *b = temp;
}

// Função para ajustar para cima (heapify-up)
void ajustaParaCima(MinHeap* minHeap, int index) {
    while (index > 0) {
        int pai = (index - 1) / 2;

        // Verificar critério principal (inicio) e desempate (paciente.id)
        if (minHeap->heap[pai].inicio < minHeap->heap[index].inicio ||
           (minHeap->heap[pai].inicio == minHeap->heap[index].inicio &&
            minHeap->heap[pai].paciente->id <= minHeap->heap[index].paciente->id)) {
            break;
        }

        // Trocar os elementos se a ordem estiver incorreta
        troca(&minHeap->heap[pai], &minHeap->heap[index]);
        index = pai;
    }
}

// Função para ajustar para baixo (heapify-down)
void ajustaParaBaixo(MinHeap* minHeap, int index) {
    int menor = index;
    int esquerda = 2 * index + 1;
    int direita = 2 * index + 2;

    // Verificar o filho à esquerda
    if (esquerda < minHeap->tamanho) {
        if (minHeap->heap[esquerda].inicio < minHeap->heap[menor].inicio ||
           (minHeap->heap[esquerda].inicio == minHeap->heap[menor].inicio &&
            minHeap->heap[esquerda].paciente->id < minHeap->heap[menor].paciente->id)) {
            menor = esquerda;
        }
    }

    // Verificar o filho à direita
    if (direita < minHeap->tamanho) {
        if (minHeap->heap[direita].inicio < minHeap->heap[menor].inicio ||
           (minHeap->heap[direita].inicio == minHeap->heap[menor].inicio &&
            minHeap->heap[direita].paciente->id < minHeap->heap[menor].paciente->id)) {
            menor = direita;
        }
    }

    // Se o menor não for o índice atual, trocar e continuar ajustando
    if (menor != index) {
        troca(&minHeap->heap[index], &minHeap->heap[menor]);
        ajustaParaBaixo(minHeap, menor);
    }
}

// Função para expandir a capacidade do heap
void expandirHeap(MinHeap* minHeap) {
    minHeap->capacidade *= 2;
    minHeap->heap = (Evento*)realloc(minHeap->heap, minHeap->capacidade * sizeof(Evento));
    if (!minHeap->heap) {
        fprintf(stderr, "Erro ao realocar memória!\n");
        exit(EXIT_FAILURE);
    }
}

// Função para inserir um evento no MinHeap
void insereEvento(MinHeap* minHeap, Procedimento * procedimento, Fila * filas, Paciente * paciente, double inicio) {
    if (minHeap->tamanho == minHeap->capacidade) {
        expandirHeap(minHeap); // Expande o heap quando atinge a capacidade
    }

    Evento novoEvento = {procedimento, filas, paciente, inicio};
    minHeap->heap[minHeap->tamanho] = novoEvento;
    ajustaParaCima(minHeap, minHeap->tamanho);
    minHeap->tamanho++;
}

// Particionamento do Quicksort (critério: ID do evento)
int particiona(Evento* heap, int low, int high) {
    int pivot = heap[high].paciente->id; // Usar o último elemento como pivô
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (heap[j].paciente->id < pivot) { // Ordenação crescente pelo ID
            i++;
            troca(&heap[i], &heap[j]);
        }
    }

    troca(&heap[i + 1], &heap[high]);
    return i + 1;
}

// Implementação do Quicksort (recursiva)
void quicksort(Evento* heap, int low, int high) {
    if (low < high) {
        int p = particiona(heap, low, high); // Obtém o índice do pivô
        quicksort(heap, low, p - 1);        // Ordena à esquerda do pivô
        quicksort(heap, p + 1, high);       // Ordena à direita do pivô
    }
}

// Função para ordenar eventos pelo ID usando Quicksort
void ordenaID(MinHeap* minHeap, int tamanhoAtual) {
    quicksort(minHeap->heap, 0, tamanhoAtual); // Ordena do início até o tamanho atual
}

// Função para inserir evento no heap e ordenar pelo ID
void insereEventoID(MinHeap* minHeap, Procedimento* procedimento, Fila* filas, Paciente* paciente, double inicio, int id) {
    if (minHeap->tamanho == minHeap->capacidade) {
        expandirHeap(minHeap); // Expande o heap quando atinge a capacidade
    }

    Evento novoEvento = {procedimento, filas, paciente, inicio, id};
    minHeap->heap[minHeap->tamanho] = novoEvento;

    ordenaID(minHeap, minHeap->tamanho); // Ordena o heap pelo ID após inserir
    minHeap->tamanho ++;
}

// Função para retirar o próximo evento do MinHeap
Evento retiraProximoEvento(MinHeap* minHeap) {
    if (minHeap->tamanho == 0) {
        printf("Heap está vazio!\n");
         return (Evento){NULL, NULL, NULL, -1.0, -1.0};
    }

    Evento proximoEvento = minHeap->heap[0];
    minHeap->heap[0] = minHeap->heap[minHeap->tamanho - 1];
    minHeap->tamanho--;
    ajustaParaBaixo(minHeap, 0);
    return proximoEvento;
}

// Função para finalizar o MinHeap
void finalizaHeap(MinHeap* minHeap) {
    free(minHeap->heap);
    free(minHeap);
}

// Função para exibir os eventos do heap
void exibeHeap(MinHeap* minHeap) {
    for (int i = 0; i < minHeap->tamanho; i++) {
        Evento evento = minHeap->heap[i];
        
        // Calcular tempo de chegada do paciente em segundos (inicio)
        double tempo_inicio_horas = calcularTempoChegada(evento.paciente);
        time_t tempo_inicio = (time_t)(tempo_inicio_horas * 3600); // Converter para segundos
        
        // Calcular tempo de fim (já está em horas, então multiplicamos por 3600 para converter em segundos)
        time_t tempo_fim = (time_t)(evento.inicio * 3600); // Converter para segundos
        
        // Converter os tempos para estruturas de data e hora
        struct tm tm_inicio = *localtime(&tempo_inicio);
        struct tm tm_fim = *localtime(&tempo_fim);

        // Obter valores individuais para os campos de data e hora
        char str_inicio[100], str_fim[100];
        
        // Formatar data para início
        snprintf(str_inicio, sizeof(str_inicio), "%s %s  %d %02d:%02d:%02d %d", 
                 tm_inicio.tm_wday == 0 ? "Sun" : tm_inicio.tm_wday == 1 ? "Mon" :
                 tm_inicio.tm_wday == 2 ? "Tue" : tm_inicio.tm_wday == 3 ? "Wed" :
                 tm_inicio.tm_wday == 4 ? "Thu" : tm_inicio.tm_wday == 5 ? "Fri" : "Sat",
                 tm_inicio.tm_mon == 0 ? "Jan" : tm_inicio.tm_mon == 1 ? "Feb" :
                 tm_inicio.tm_mon == 2 ? "Mar" : tm_inicio.tm_mon == 3 ? "Apr" :
                 tm_inicio.tm_mon == 4 ? "May" : tm_inicio.tm_mon == 5 ? "Jun" :
                 tm_inicio.tm_mon == 6 ? "Jul" : tm_inicio.tm_mon == 7 ? "Aug" :
                 tm_inicio.tm_mon == 8 ? "Sep" : tm_inicio.tm_mon == 9 ? "Oct" :
                 tm_inicio.tm_mon == 10 ? "Nov" : "Dec",
                 tm_inicio.tm_mday <= 9 ? tm_inicio.tm_mday : tm_inicio.tm_mday, // Dia sem zero à esquerda
                 tm_inicio.tm_hour, tm_inicio.tm_min, tm_inicio.tm_sec, tm_inicio.tm_year + 1900);

        // Formatar data para fim
        snprintf(str_fim, sizeof(str_fim), "%s %s  %d %02d:%02d:%02d %d", 
                 tm_fim.tm_wday == 0 ? "Sun" : tm_fim.tm_wday == 1 ? "Mon" :
                 tm_fim.tm_wday == 2 ? "Tue" : tm_fim.tm_wday == 3 ? "Wed" :
                 tm_fim.tm_wday == 4 ? "Thu" : tm_fim.tm_wday == 5 ? "Fri" : "Sat",
                 tm_fim.tm_mon == 0 ? "Jan" : tm_fim.tm_mon == 1 ? "Feb" :
                 tm_fim.tm_mon == 2 ? "Mar" : tm_fim.tm_mon == 3 ? "Apr" :
                 tm_fim.tm_mon == 4 ? "May" : tm_fim.tm_mon == 5 ? "Jun" :
                 tm_fim.tm_mon == 6 ? "Jul" : tm_fim.tm_mon == 7 ? "Aug" :
                 tm_fim.tm_mon == 8 ? "Sep" : tm_fim.tm_mon == 9 ? "Oct" :
                 tm_fim.tm_mon == 10 ? "Nov" : "Dec",
                 tm_fim.tm_mday <= 9 ? tm_fim.tm_mday : tm_fim.tm_mday, // Dia sem zero à esquerda
                 tm_fim.tm_hour, tm_fim.tm_min, tm_fim.tm_sec, tm_fim.tm_year + 1900);

        // Exibir o evento com o formato solicitado
        printf("%d %s %s  %.2f  %.2f  %.2f\n", 
               evento.paciente->id, 
               str_inicio, // Data e hora de início
               str_fim, // Data e hora de término
               evento.paciente->tempo_chegada - tempo_inicio_horas, // Tempo de permanência no hospital
               evento.paciente->tempo_chegada - tempo_inicio_horas - evento.paciente->tempo_espera_total, // Horas em atendimento
               fabs(evento.paciente->tempo_espera_total)); // Horas de espera (valor absoluto apenas pra não exibir -0.0)
    }
}