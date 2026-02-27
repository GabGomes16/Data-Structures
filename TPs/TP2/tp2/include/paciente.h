#ifndef PACIENTE_H
#define PACIENTE_H
#include <stdio.h>
#include <stdlib.h>

// Enumeração para representar o estado do paciente
typedef enum {
    NAO_CHEGOU,
    FILA_TRIAGEM,
    SENDO_TRIADO,
    FILA_ATENDIMENTO,
    SENDO_ATENDIDO,
    FILA_MEDIDAS_HOSPITALARES,
    REALIZANDO_MEDIDAS_HOSPITALARES,
    FILA_TESTES_LABORATORIO,
    REALIZANDO_TESTES_LABORATORIO,
    FILA_EXAMES_IMAGEM,
    REALIZANDO_EXAMES_IMAGEM,
    FILA_INSTRUMENTOS_MEDICAMENTOS,
    APLICANDO_INSTRUMENTOS_MEDICAMENTOS,
    ALTA_HOSPITALAR
} EstadoPaciente;

// Estrutura de um paciente
typedef struct {
    int id; // Identificador do paciente
    int alta; // Indica se o paciente já recebeu alta (0 = não, 1 = sim)
    int ano; // Data de chegada do paciente
    int mes; // Data de chegada do paciente
    int dia; // Data de chegada do paciente
    int hora; // Hora de chegada do paciente
    int grau_urgencia; // Grau de urgência do paciente (de 0 a 2)
    int medidas_hospitalares; //Numwro de medidas hospitalares
    int testes_laboratorio; //Numero de testes de laboratorio
    int exames_imagem; //Numero de exames de imagem
    int instrumentos_medicamentos; //Numero de instrumentos e medicamentos
    double tempo_espera_total; // Para cálculo do tempo médio de espera
    double tempo_chegada;      // Para priorização inicial
    EstadoPaciente estado; // Representa o estado do paciente (de 1 a 14)
} Paciente;

// Função para carregar pacientes do arquivo
void carregar_pacientes(FILE* arquivo, Paciente** pacientes, int* num_pacientes);

// Inicializa um paciente com os dados fornecidos
void inicializar_paciente(Paciente* paciente, int id, int alta, int ano, int mes, int dia, int hora,
                          int grau_urgencia, int medidas_hospitalares, int testes_laboratorio,
                          int exames_imagem, int instrumentos_medicamentos);

// Função para calcular o tempo de chegada de um paciente
double calcularTempoChegada(const Paciente* paciente);

// Atualiza o estado do paciente
void atualizar_estado_paciente(Paciente* paciente);

// Verifica se o paciente está em um estado específico
int paciente_em_estado(const Paciente* paciente);

#endif