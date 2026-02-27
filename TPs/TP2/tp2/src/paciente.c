#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "paciente.h"

// Inicializa um paciente com os dados fornecidos
void inicializar_paciente(Paciente* paciente, int id, int alta, int ano, int mes, int dia, int hora,
                          int grau_urgencia, int medidas_hospitalares, int testes_laboratorio,
                          int exames_imagem, int instrumentos_medicamentos) {
    paciente->id = id;
    paciente->alta = alta;
    paciente->ano = ano;
    paciente->mes = mes;
    paciente->dia = dia;
    paciente->hora = hora;
    paciente->grau_urgencia = grau_urgencia;
    paciente->medidas_hospitalares = medidas_hospitalares;
    paciente->testes_laboratorio = testes_laboratorio;
    paciente->exames_imagem = exames_imagem;
    paciente->instrumentos_medicamentos = instrumentos_medicamentos;
    paciente->estado = NAO_CHEGOU;  // Paciente começa no estado inicial

    //printf("[DEBUG] Paciente inicializado: ID = %d, Alta = %d, Data = %d/%d/%d, Hora = %d, Grau = %d, Medidas = %d, Testes = %d, Imagem = %d Medicamentos = %d\n",
    //       id, alta, dia, mes, ano, hora, grau_urgencia, medidas_hospitalares, testes_laboratorio, exames_imagem, instrumentos_medicamentos);
}

// Função para carregar pacientes do arquivo
void carregar_pacientes(FILE* arquivo, Paciente** pacientes, int* num_pacientes) {
    if (fscanf(arquivo, "%d", num_pacientes) != 1) {
        fprintf(stderr, "Erro ao ler o número de pacientes.\n");
        exit(EXIT_FAILURE);
    }

    *pacientes = malloc(sizeof(Paciente) * (*num_pacientes));
    if (!*pacientes) {
        perror("Erro ao alocar memória para pacientes");
        exit(EXIT_FAILURE);
    }

    //printf("[DEBUG] Número de pacientes: %d\n", *num_pacientes);

    for (int i = 0; i < *num_pacientes; i++) {
        int id, alta, ano, mes, dia, hora, grau_urgencia, medidas_hospitalares, testes_laboratorio, exames_imagem, instrumentos_medicamentos;
        if (fscanf(arquivo, "%d %d %d %d %d %d %d %d %d %d %d",
                   &id, &alta, &ano, &mes, &dia, &hora, &grau_urgencia, &medidas_hospitalares, &testes_laboratorio, &exames_imagem, &instrumentos_medicamentos) != 11) {
            fprintf(stderr, "Erro ao ler os dados do paciente %d.\n", i);
            exit(EXIT_FAILURE);
        }
        inicializar_paciente(&(*pacientes)[i], id, alta, ano, mes, dia, hora,
                             grau_urgencia, medidas_hospitalares, testes_laboratorio, exames_imagem, instrumentos_medicamentos);
    }
}

// Função para calcular o tempo de chegada de um paciente em horas, considerando anos bissextos
double calcularTempoChegada(const Paciente* paciente) {
    struct tm data = {0};

    // Preencher a estrutura 'tm' com os valores do paciente
    data.tm_year = paciente->ano - 1900;  // 'tm_year' é o ano desde 1900
    data.tm_mon = paciente->mes - 1;      // 'tm_mon' varia de 0 (janeiro) a 11 (dezembro)
    data.tm_mday = paciente->dia;         // Dia do mês
    data.tm_hour = paciente->hora;        // Hora do dia

    // Converter a data para o número de segundos desde a época (1970-01-01 00:00:00 UTC)
    time_t tempoEmSegundos = mktime(&data);

    if (tempoEmSegundos == -1) {
        // Caso ocorra algum erro na conversão
        return -1.0;
    }

    // Converter segundos para horas
    return tempoEmSegundos / 3600.0;
}

// Atualiza o estado do paciente
void atualizar_estado_paciente(Paciente* paciente) {
    paciente->estado += 1;
}

// Verifica se o paciente está em um estado específico
int paciente_em_estado(const Paciente* paciente) {
    return paciente->estado;
}
