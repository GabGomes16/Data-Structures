#ifndef ARVORE_H
#define ARVORE_H
#include "voos.h"
#include <stdbool.h>

//Estrutura do nó da AVL
typedef struct AVLNo {
    Voo voo;                  // O voo armazenado no nó
    struct AVLNo *esquerdo;   // Subárvore esquerda
    struct AVLNo *direito;    // Subárvore direita
    int altura;               // Altura do nó para balanceamento
} AVLNo;

//Estrutura do nó da Lista
typedef struct NoLista {
    Voo voo;
    struct NoLista* prox;
} NoLista;

//Estrutura da Lista de Vôos
typedef struct {
    NoLista* inicio;
} ListaVoos;

typedef int (*Comparador)(Voo, Voo);

int altura(AVLNo *no);
int fatorBalanceamento(AVLNo *no);
AVLNo* rotacaoDireita(AVLNo *y);
AVLNo* rotacaoEsquerda(AVLNo *x);
AVLNo* inserirAVL(AVLNo *raiz, Voo voo, Comparador comparar);
AVLNo* removerSeNaoAtende(AVLNo* raiz, int (*criterio)(Voo, void *), void *parametros);
void emOrdem(AVLNo *raiz);
void imprimirAVL(AVLNo *raiz);
void liberarAVL(AVLNo *raiz);

void inserirLista(ListaVoos* lista, Voo voo);
int contemVoo(ListaVoos* lista, int codigo);
void formatarHorario(time_t timestamp, char* buffer, size_t tamanho);
void imprimirLista(ListaVoos* lista, Consulta Consulta);
void avlParaLista(AVLNo* raiz, ListaVoos* lista);
void intersecaoListas(ListaVoos** listas, int numListas, ListaVoos** resultado);
void uniaoListas(ListaVoos** listas, int numListas, ListaVoos** resultado);
void diferencaListas(ListaVoos** listas, int numListas, ListaVoos** resultado);
void liberarLista(ListaVoos* lista);

int compararVoos(Voo a, Voo b, const char* ordem);
void dividirLista(NoLista* origem, NoLista** frente, NoLista** tras);
NoLista* mesclarListas(NoLista* lista1, NoLista* lista2, char* ordem);
void mergeSortLista(NoLista** cabecaRef, char* ordem);
void ordenarListaVoos(ListaVoos* lista, char* ordem);

#endif  