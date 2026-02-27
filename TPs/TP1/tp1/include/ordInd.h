#ifndef ORDIND_H
#define ORDIND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[50];
    char cpf[20];
    char endereco[100];
    char outros[200];
} Registro;

typedef struct {
    int index;
    Registro *registro;
} IndiceIndireto;
 
void quicksort(IndiceIndireto *arr, int low, int high, int (*cmp)(const void *, const void *));
void bubblesort(IndiceIndireto *arr, int n, int (*cmp)(const void *, const void *));
void insertionsort(IndiceIndireto *arr, int n, int (*cmp)(const void *, const void *));

// Declaração de funções de comparação
int cmp_nome(const void *a, const void *b);
int cmp_cpf(const void *a, const void *b);
int cmp_endereco(const void *a, const void *b);

typedef struct {
    char **atributos;
    int num_atributos;
    Registro *registros;
    int num_registros;
    int max_registros;
    IndiceIndireto **indices;
} OrdInd, *OrdInd_ptr;

// Funções do TAD OrdInd
OrdInd_ptr Cria();
int Destroi(OrdInd_ptr poi);
int CarregaArquivo(OrdInd_ptr poi, char *nomeentrada);
int NumAtributos(OrdInd_ptr poi);
int NomeAtributo(OrdInd_ptr poi, int pos, char *nome);  
int CriaIndice(OrdInd_ptr poi, int atribid);
int OrdenaIndice(OrdInd_ptr poi, int atribid, int current_algo);
int ImprimeOrdenadoIndice(OrdInd_ptr poi, int atribid);

#endif