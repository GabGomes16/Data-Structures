#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/escalonador.h"
#include "procedimento.h"
#include "fila.h"
#include "paciente.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Abrir o arquivo de entrada
    const char* arquivoEntrada = argv[1];
    FILE* arquivo = fopen(arquivoEntrada, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    //printf("[DEBUG] Arquivo '%s' aberto com sucesso\n", arquivoEntrada);

    // Carregar os procedimentos
    Procedimento procedimento[6];
    carregar_arquitetura(arquivo, procedimento);

     // Carregar os pacientes
    Paciente* pacientes = NULL;
    int num_pacientes = 0;
    carregar_pacientes(arquivo, &pacientes, &num_pacientes);
    //printf("[DEBUG] %d pacientes carregados\n", num_pacientes);

    // Inicializar fila principal
    Fila fila_principal;
    inicializaFila(&fila_principal, num_pacientes);
    //printf("[DEBUG] Fila principal inicializada com capacidade para %d elementos\n", num_pacientes);

    // Inicialização do heap com capacidade inicial de 5 eventos
    MinHeap* escalonador = inicializaHeap(num_pacientes);

    // Inicialização do heap de saída
    MinHeap* saida = inicializaHeap(num_pacientes);

    // Inserindo eventos da entrada dos pacientes
    for (int i = 0; i < num_pacientes; i++) {
        double tempo_espera = 0.0; // Exemplo de cálculo inicial
        double tempo_chegada = calcularTempoChegada(&pacientes[i]);
        atualizar_estado_paciente(&pacientes[i]); // Atualizar o estado para fila de triagem
        enfileira(&fila_principal, pacientes[i], tempo_espera, tempo_chegada);
        //printf("[DEBUG] Paciente ID %d enfileirado com tempo de chegada %.2f e estado atualizado para %d\n", pacientes[i].id, tempo_chegada, pacientes->estado);
        insereEvento(escalonador, &procedimento[0], &fila_principal, &pacientes[i], tempo_chegada);
    }

    //printf("[DEBUG] Estado do heap após inserções:\n");
    //exibeHeap(escalonador);

    // Processamento dos eventos
    //printf("\n[DEBUG] Retirando eventos ordenados:\n");
    // Processamento dos eventos
    while (escalonador->tamanho > 0) {
        Evento proximo = retiraProximoEvento(escalonador);
        //printf("[DEBUG] Evento retirado -> ID: %d, Tempo: %.02f, Tempo de espera: %.02f, Estado: %d\n", proximo.paciente->id, proximo.inicio, proximo.paciente->tempo_espera_total, proximo.paciente->estado);

        double liberado = 0.0;
        switch (proximo.paciente->estado) {
            case 1: // Triagem
                liberado = tempo_minimo_termino(&procedimento[0]);
                if (liberado > proximo.inicio) {
                    double espera = liberado - proximo.inicio;
                    proximo.paciente->tempo_espera_total += espera;
                    proximo.fim = liberado + procedimento[0].tempo_medio;
                } else {
                    proximo.fim = proximo.inicio + procedimento[0].tempo_medio;
                }
                proximo.paciente->tempo_chegada = proximo.fim;
                insereEvento(escalonador, &procedimento[0], &fila_principal, proximo.paciente, proximo.fim);
                atualiza_tempo_termino(&procedimento[0], proximo.fim);
                proximo.paciente->estado = 3;
                break;

            case 3: // Atendimento
                liberado = tempo_minimo_termino(&procedimento[1]);
                if (liberado > proximo.inicio) {
                    double espera = liberado - proximo.inicio;
                    proximo.paciente->tempo_espera_total += espera;
                    proximo.fim = liberado + procedimento[1].tempo_medio;
                } else {
                    proximo.fim = proximo.inicio + procedimento[1].tempo_medio;
                }
                proximo.paciente->tempo_chegada = proximo.fim;
                insereEvento(escalonador, &procedimento[1], &fila_principal, proximo.paciente, proximo.fim);
                atualiza_tempo_termino(&procedimento[1], proximo.fim);
                if (proximo.paciente->alta == 1) {
                    proximo.paciente->estado = 13; // Alta imediata
                } else {
                    proximo.paciente->estado = 5; // Próximo estado: Medidas Hospitalares
                }
                break;

            case 5: // Medidas Hospitalares
                liberado = tempo_minimo_termino(&procedimento[2]);
                if (liberado > proximo.inicio) {
                    double espera = liberado - proximo.inicio;
                    proximo.paciente->tempo_espera_total += espera;
                    proximo.fim = liberado + (procedimento[2].tempo_medio * proximo.paciente->medidas_hospitalares);
                } else {
                    proximo.fim = proximo.inicio + (procedimento[2].tempo_medio * proximo.paciente->medidas_hospitalares);
                }
                proximo.paciente->tempo_chegada = proximo.fim;
                insereEvento(escalonador, &procedimento[2], &fila_principal, proximo.paciente, proximo.fim);
                atualiza_tempo_termino(&procedimento[2], proximo.fim);
                proximo.paciente->estado = 7;
                break;

            case 7: // Testes de Laboratório
                liberado = tempo_minimo_termino(&procedimento[3]);
                if (liberado > proximo.inicio) {
                    double espera = liberado - proximo.inicio;
                    proximo.paciente->tempo_espera_total += espera;
                    proximo.fim = liberado + (procedimento[3].tempo_medio * proximo.paciente->testes_laboratorio);
                } else {
                    proximo.fim = proximo.inicio + (procedimento[3].tempo_medio * proximo.paciente->testes_laboratorio);
                }
                proximo.paciente->tempo_chegada = proximo.fim;
                insereEvento(escalonador, &procedimento[3], &fila_principal, proximo.paciente, proximo.fim);
                atualiza_tempo_termino(&procedimento[3], proximo.fim);
                proximo.paciente->estado = 9;
                break;

            case 9: // Exames de Imagem
                liberado = tempo_minimo_termino(&procedimento[4]);
                if (liberado > proximo.inicio) {
                    double espera = liberado - proximo.inicio;
                    proximo.paciente->tempo_espera_total += espera;
                    proximo.fim = liberado + (procedimento[4].tempo_medio * proximo.paciente->exames_imagem);
                } else {
                    proximo.fim = proximo.inicio + (procedimento[4].tempo_medio * proximo.paciente->exames_imagem);
                }
                proximo.paciente->tempo_chegada = proximo.fim;
                insereEvento(escalonador, &procedimento[4], &fila_principal, proximo.paciente, proximo.fim);
                atualiza_tempo_termino(&procedimento[4], proximo.fim);
                proximo.paciente->estado = 11;
                break;

            case 11: // Instrumentos e Medicamentos
                liberado = tempo_minimo_termino(&procedimento[5]);
                if (liberado > proximo.inicio) {
                    double espera = liberado - proximo.inicio;
                    proximo.paciente->tempo_espera_total += espera;
                    proximo.fim = liberado + (procedimento[5].tempo_medio * proximo.paciente->instrumentos_medicamentos);
                } else {
                    proximo.fim = proximo.inicio + (procedimento[5].tempo_medio * proximo.paciente->instrumentos_medicamentos);
                }
                proximo.paciente->tempo_chegada = proximo.fim;
                insereEvento(escalonador, &procedimento[5], &fila_principal, proximo.paciente, proximo.fim);
                atualiza_tempo_termino(&procedimento[5], proximo.fim);
                proximo.paciente->estado = 13;
                break;

            case 13: // Alta Hospitalar
                insereEventoID(saida, &procedimento[5], &fila_principal, proximo.paciente, proximo.inicio, proximo.paciente->id);
                break;

            default:
                break;
        }
    }

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    exibeHeap(saida);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tempo de execução: %f segundos\n", cpu_time_used);

    // Finalizando os heaps
    finalizaHeap(escalonador);
    finalizaHeap(saida);

    // Liberar memória dos procedimentos
    for (int i = 0; i < 6; i++) {
        liberar_procedimento(&procedimento[i]);
    }
    //printf("[DEBUG] Memória de procedimentos liberada\n");

    // Liberar memória dos pacientes
    free(pacientes);
    //printf("[DEBUG] Memória de pacientes liberada\n");

    // Finalizar filas
    finalizaFila(&fila_principal);

    // Fechar arquivo
    fclose(arquivo);
    //printf("[DEBUG] Arquivo fechado\n");

    //printf("[DEBUG] Execução concluída com sucesso\n");
    return EXIT_SUCCESS;
}
