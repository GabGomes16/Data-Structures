#ifndef VOOS_H
#define VOOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_VOOS 10000
#define MAX_CONSULTAS 1000

// Estrutura do voo
typedef struct {
    int id;
    char org[10];
    char dst[10];
    int org_val;  // Valor ASCII convertido de org
    int dst_val;  // Valor ASCII convertido de dst 
    double prc;
    int sea;
    time_t dep;
    char dep_char[20];
    time_t arr;
    char arr_char[20];
    int sto;
    double dur;
} Voo;

// Estrutura para armazenar uma consulta
typedef struct {
    int max_voos;
    char criterio_ordenacao[4];
    char expressao_base[256];
    char expressao_logica[256];
    int num_componentes;
} Consulta;

//Estrutura pra armazenar os parametros de cada comparação  relacional
typedef struct {
    char variavel[4];
    char operacao[4];
    char valor[256];
} ParametrosCriterio;

time_t parseDateTime(const char *str);
void removerParenteses(const char *entrada, char *saida);
int contarComponentes(const char *expressao);
int converterParaInt(char str[]);
void lerEntrada(FILE *arquivo, Voo voos[], int *n, Consulta consultas[], int *c);
int compararOrigem(Voo a, Voo b);
int compararDestino(Voo a, Voo b);
int compararPreco(Voo a, Voo b);
int compararAssentos(Voo a, Voo b);
int compararParadas(Voo a, Voo b);
int compararDuracao(Voo a, Voo b);
void processarConsultas(Voo voos[], int n, Consulta consultas[], int c);
void separarComponentes(const char *expressao_logica, char componentes[][50], char operadores[][3], int num_componentes);
void extrairComponentes(const char *expressao, char *variavel, char *operacao, char *valor);

int criterio(Voo voo, const char* variavel, const char* operacao, const char* valor);
int criterioWrapper(Voo voo, void *parametros);

#endif  // VOOS_H