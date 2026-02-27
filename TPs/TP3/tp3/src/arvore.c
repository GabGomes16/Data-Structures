#include "../include/arvore.h"
#include "../include/hash.h"

// Retorna a altura de um nó da árvore AVL. Se o nó for NULL, retorna 0.
// Entrada: no - ponteiro para o nó da árvore
// Saída: altura do nó
int altura(AVLNo *no) {
    return no ? no->altura : 0;
}

// Calcula o fator de balanceamento de um nó da árvore AVL.
// Entrada: no - ponteiro para o nó da árvore
// Saída: fator de balanceamento (altura da subárvore esquerda - altura da subárvore direita)
int fatorBalanceamento(AVLNo *no) {
    return no ? altura(no->esquerdo) - altura(no->direito) : 0;
}

// Realiza uma rotação simples à direita em um nó da árvore AVL.
// Entrada: y - ponteiro para o nó desbalanceado
// Saída: ponteiro para o novo nó raiz após a rotação
AVLNo* rotacaoDireita(AVLNo *y) {
    AVLNo *x = y->esquerdo;
    AVLNo *T2 = x->direito;

    // Realizar a rotação
    x->direito = y;
    y->esquerdo = T2;

    // Atualizar alturas
    y->altura = 1 + (altura(y->esquerdo) > altura(y->direito) ? altura(y->esquerdo) : altura(y->direito));
    x->altura = 1 + (altura(x->esquerdo) > altura(x->direito) ? altura(x->esquerdo) : altura(x->direito));

    return x;
}

// Realiza uma rotação simples à esquerda em um nó da árvore AVL.
// Entrada: x - ponteiro para o nó desbalanceado
// Saída: ponteiro para o novo nó raiz após a rotação
AVLNo* rotacaoEsquerda(AVLNo *x) {
    AVLNo *y = x->direito;
    AVLNo *T2 = y->esquerdo;

    // Realizar a rotação
    y->esquerdo = x;
    x->direito = T2;

    // Atualizar alturas
    x->altura = 1 + (altura(x->esquerdo) > altura(x->direito) ? altura(x->esquerdo) : altura(x->direito));
    y->altura = 1 + (altura(y->esquerdo) > altura(y->direito) ? altura(y->esquerdo) : altura(y->direito));

    return y;
}

// Insere um voo na árvore AVL, mantendo o balanceamento.
// Entrada: raiz - ponteiro para a raiz da árvore
//          voo - estrutura do tipo Voo a ser inserida
//          comparar - função de comparação para ordenação dos voos
// Saída: ponteiro para a nova raiz da árvore após a inserção
AVLNo* inserirAVL(AVLNo *raiz, Voo voo, Comparador comparar) {
    if (!raiz) {
        AVLNo *novo = malloc(sizeof(AVLNo));
        novo->voo = voo;
        novo->esquerdo = novo->direito = NULL;
        novo->altura = 1;
        return novo;
    }

    // Usar a função de comparação para decidir a inserção
    if (comparar(voo, raiz->voo) < 0) {
        raiz->esquerdo = inserirAVL(raiz->esquerdo, voo, comparar);
    } else if (comparar(voo, raiz->voo) > 0) {
        raiz->direito = inserirAVL(raiz->direito, voo, comparar);
    } else {
        if (voo.id < raiz->voo.id) {
            raiz->esquerdo = inserirAVL(raiz->esquerdo, voo, comparar);
        } else {
            raiz->direito = inserirAVL(raiz->direito, voo, comparar);
        }
    }

    // Atualizar a altura do nó atual
    raiz->altura = 1 + (altura(raiz->esquerdo) > altura(raiz->direito) ? altura(raiz->esquerdo) : altura(raiz->direito));

    // Balancear a árvore
    int balance = fatorBalanceamento(raiz);

    // Aplicar rotações conforme necessário
    if (balance > 1 && comparar(voo, raiz->esquerdo->voo) < 0)
        return rotacaoDireita(raiz);

    if (balance < -1 && comparar(voo, raiz->direito->voo) > 0)
        return rotacaoEsquerda(raiz);

    if (balance > 1 && comparar(voo, raiz->esquerdo->voo) > 0) {
        raiz->esquerdo = rotacaoEsquerda(raiz->esquerdo);
        return rotacaoDireita(raiz);
    }

    if (balance < -1 && comparar(voo, raiz->direito->voo) < 0) {
        raiz->direito = rotacaoDireita(raiz->direito);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Remove nós da árvore AVL que não atendem a um critério específico.
// Entrada: raiz - ponteiro para a raiz da árvore
//          criterio - função de critério para remoção
//          parametros - parâmetros adicionais para a função de critério
// Saída: ponteiro para a nova raiz da árvore após a remoção
AVLNo* removerSeNaoAtende(AVLNo* raiz, int (*criterio)(Voo, void *), void *parametros) {
    if (!raiz) return NULL;

    raiz->esquerdo = removerSeNaoAtende(raiz->esquerdo, criterio, parametros);
    raiz->direito = removerSeNaoAtende(raiz->direito, criterio, parametros);

    if (!criterio(raiz->voo, parametros)) {
        AVLNo* temp;
        if (!raiz->esquerdo) {
            temp = raiz->direito;
            free(raiz);
            return temp;
        } else if (!raiz->direito) {
            temp = raiz->esquerdo;
            free(raiz);
            return temp;
        }

        AVLNo* sucessor = raiz->direito;
        while (sucessor->esquerdo)
            sucessor = sucessor->esquerdo;

        raiz->voo = sucessor->voo;
        raiz->direito = removerSeNaoAtende(raiz->direito, criterio, parametros);
    }

    raiz->altura = 1 + (altura(raiz->esquerdo) > altura(raiz->direito) ? altura(raiz->esquerdo) : altura(raiz->direito));

    int balance = fatorBalanceamento(raiz);
    if (balance > 1 && fatorBalanceamento(raiz->esquerdo) >= 0) return rotacaoDireita(raiz);
    if (balance > 1 && fatorBalanceamento(raiz->esquerdo) < 0) {
        raiz->esquerdo = rotacaoEsquerda(raiz->esquerdo);
        return rotacaoDireita(raiz);
    }
    if (balance < -1 && fatorBalanceamento(raiz->direito) <= 0) return rotacaoEsquerda(raiz);
    if (balance < -1 && fatorBalanceamento(raiz->direito) > 0) {
        raiz->direito = rotacaoDireita(raiz->direito);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Percorre a árvore AVL em ordem (esquerda, raiz, direita).
// Entrada: raiz - ponteiro para a raiz da árvore
void emOrdem(AVLNo *raiz) {
    if (raiz) {
        emOrdem(raiz->esquerdo);
        //printf se precisar
        emOrdem(raiz->direito);
    }
}

// Imprime os voos da árvore AVL em ordem.
// Entrada: raiz - ponteiro para a raiz da árvore
void imprimirAVL(AVLNo *raiz) {
    if (raiz == NULL) return;
    imprimirAVL(raiz->esquerdo);
    printf("Voo %d | Origem: %s (%d) | Destino: %s (%d) | Preço: %.2f | Assentos: %d | Duração: %f | Paradas: %d\n",
           raiz->voo.id, raiz->voo.org, raiz->voo.org_val, raiz->voo.dst, raiz->voo.dst_val,
           raiz->voo.prc, raiz->voo.sea, raiz->voo.dur, raiz->voo.sto);
    imprimirAVL(raiz->direito);
}

// Libera a memória alocada para a árvore AVL.
// Entrada: raiz - ponteiro para a raiz da árvore
void liberarAVL(AVLNo *raiz) {
    if (raiz == NULL) return;
    liberarAVL(raiz->esquerdo);
    liberarAVL(raiz->direito);
    free(raiz);
}

// Insere um voo em uma lista encadeada.
// Entrada: lista - ponteiro para a lista
//          voo - estrutura do tipo Voo a ser inserida
void inserirLista(ListaVoos* lista, Voo voo) {
    if (!lista) {
        fprintf(stderr, "Erro: lista não inicializada\n");
        return;
    }
    NoLista* novo = (NoLista*)malloc(sizeof(NoLista));
    if (!novo) {
        fprintf(stderr, "Erro: falha ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
    novo->voo = voo;
    novo->prox = lista->inicio;
    lista->inicio = novo;
}

// Verifica se um voo está presente na lista.
// Entrada: lista - ponteiro para a lista
//          codigo - código do voo a ser buscado
// Saída: 1 se o voo estiver na lista, 0 caso contrário
int contemVoo(ListaVoos* lista, int codigo) {
    if (!lista) return 0; // Evita acessar memória inválida
    NoLista* atual = lista->inicio;
    while (atual) {
        if (atual->voo.id == codigo) return 1;
        atual = atual->prox;
    }
    return 0;
}

// Converte um timestamp (time_t) para uma string no formato ISO 8601 (YYYY-MM-DDTHH:MM:SS).
// Entrada: timestamp - valor do tipo time_t
//          buffer - string onde o resultado será armazenado
//          tamanho - tamanho do buffer
void formatarHorario(time_t timestamp, char* buffer, size_t tamanho) {
    struct tm* tm_info = localtime(&timestamp);
    strftime(buffer, tamanho, "%Y-%m-%dT%H:%M:%S", tm_info);
}

// Imprime a lista de voos com um limite máximo de impressões.
// Entrada: lista - ponteiro para a lista de voos
//          consulta - estrutura do tipo Consulta contendo o limite de impressões
void imprimirLista(ListaVoos* lista, Consulta consulta) {
    printf("%d %s %s\n", consulta.max_voos, consulta.criterio_ordenacao, consulta.expressao_base);
    if (!lista || !lista->inicio) {
        printf("Nenhum voo encontrado.\n");
        return;
    }

    NoLista* atual = lista->inicio;
    int contador = 0;

    while (atual && contador < consulta.max_voos) {
        char depStr[20], arrStr[20];
        formatarHorario(atual->voo.dep, depStr, sizeof(depStr));
        formatarHorario(atual->voo.arr, arrStr, sizeof(arrStr));
        printf("%s %s %g %d %s %s %d\n",
                atual->voo.org, atual->voo.dst, atual->voo.prc,
                atual->voo.sea, depStr, arrStr, atual->voo.sto);

        atual = atual->prox;
        contador++;
    }
}

// Converte uma árvore AVL em uma lista encadeada.
// Entrada: raiz - ponteiro para a raiz da árvore
//          lista - ponteiro para a lista onde os voos serão armazenados
void avlParaLista(AVLNo* raiz, ListaVoos* lista) {
    if (!raiz) return;
    if (!lista) {
        fprintf(stderr, "Erro: lista não inicializada em avlParaLista\n");
        return;
    }
    avlParaLista(raiz->esquerdo, lista);
    inserirLista(lista, raiz->voo);
    avlParaLista(raiz->direito, lista);
}

// Realiza a interseção de múltiplas listas de voos usando uma tabela hash.
// Entrada: listas - array de ponteiros para as listas
//          numListas - número de listas
//          resultado - ponteiro para a lista de interseção resultante
void intersecaoListas(ListaVoos** listas, int numListas, ListaVoos** resultado) {
    if (!listas || numListas <= 0 || !resultado) {
        fprintf(stderr, "Erro: listas ou resultado inválidos.\n");
        return;
    }

    HashTable hash;
    inicializarHash(&hash);

    // Inserir todos os voos de todas as listas na tabela hash
    for (int i = 0; i < numListas; i++) {
        if (!listas[i]) {
            fprintf(stderr, "Erro: listas[%d] é NULL.\n", i);
            continue;
        }

        NoLista* atual = listas[i]->inicio;
        while (atual) {
            inserirHash(&hash, atual->voo);
            atual = atual->prox;
        }
    }

    // Se já houver um resultado anterior, liberamos antes de criar um novo
    if (*resultado) {
        liberarLista(*resultado);
        *resultado = NULL;
    }

    // Criar a nova lista de interseção
    ListaVoos* novaLista = (ListaVoos*)malloc(sizeof(ListaVoos));
    if (!novaLista) {
        fprintf(stderr, "Erro ao alocar nova lista.\n");
        liberarHash(&hash);
        return;
    }
    novaLista->inicio = NULL;

    // Verificar quais voos estão presentes em todas as listas
    if (!listas[0]) {
        fprintf(stderr, "Erro: listas[0] é NULL.\n");
        free(novaLista);
        liberarHash(&hash);
        return;
    }

    NoLista* atual = listas[0]->inicio;
    while (atual) {
        if (buscarContagemHash(&hash, atual->voo) == numListas) {
            inserirLista(novaLista, atual->voo);
        }
        atual = atual->prox;
    }

    // Se a lista estiver vazia, liberar memória
    if (novaLista->inicio == NULL) {
        free(novaLista);
        novaLista = NULL;
    }

    *resultado = novaLista;
    liberarHash(&hash);
}

// Realiza a união de duas listas de voos usando uma tabela hash.
// Entrada: listas - array de ponteiros para as listas
//          numListas - número de listas (deve ser 2)
//          resultado - ponteiro para a lista de união resultante
void uniaoListas(ListaVoos** listas, int numListas, ListaVoos** resultado) {
    if (!listas || numListas != 2 || !resultado) return; // Garantindo que há exatamente duas listas

    HashTable hash;
    inicializarHash(&hash);

    // Se já houver um resultado anterior, liberamos antes de criar um novo
    if (*resultado) {
        liberarLista(*resultado);
        *resultado = NULL;
    }

    // Criar a nova lista de união
    ListaVoos* novaLista = (ListaVoos*)malloc(sizeof(ListaVoos));
    if (!novaLista) return;
    novaLista->inicio = NULL;

    // Percorrer ambas as listas e inserir os voos na tabela hash e na nova lista
    for (int i = 0; i < 2; i++) {
        NoLista* atual = listas[i]->inicio;
        while (atual) {
            if (!buscarContagemHash(&hash, atual->voo)) {
                inserirHash(&hash, atual->voo);
                inserirLista(novaLista, atual->voo);
            }
            atual = atual->prox;
        }
    }

    *resultado = novaLista;
    liberarHash(&hash);
}

// Realiza a diferença entre duas listas de voos usando uma tabela hash.
// Entrada: listas - array de ponteiros para as listas
//          numListas - número de listas (deve ser 2)
//          resultado - ponteiro para a lista de diferença resultante
void diferencaListas(ListaVoos** listas, int numListas, ListaVoos** resultado) {
    if (!listas || numListas != 2 || !resultado) return; // Garantindo que há exatamente duas listas

    HashTable hash;
    inicializarHash(&hash);

    // Se já houver um resultado anterior, liberamos antes de criar um novo
    if (*resultado) {
        liberarLista(*resultado);
        *resultado = NULL;
    }

    // Criar a nova lista de diferença
    ListaVoos* novaLista = (ListaVoos*)malloc(sizeof(ListaVoos));
    if (!novaLista) return;
    novaLista->inicio = NULL;

    // Inserir todos os voos da segunda lista na tabela hash
    NoLista* atual = listas[1]->inicio;
    while (atual) {
        inserirHash(&hash, atual->voo);
        atual = atual->prox;
    }

    // Adicionar à nova lista apenas os voos da primeira lista que não estão na segunda
    atual = listas[0]->inicio;
    while (atual) {
        if (!buscarContagemHash(&hash, atual->voo)) {
            inserirLista(novaLista, atual->voo);
        }
        atual = atual->prox;
    }

    *resultado = novaLista;
    liberarHash(&hash);
}

// Compara dois voos com base em uma ordem de critérios dinâmica.
// Entrada: a, b - estruturas do tipo Voo a serem comparadas
//          ordem - string contendo a ordem de critérios (ex: "pds" para preço, duração, paradas)
// Saída: -1 se a < b, 1 se a > b, 0 se a == b
int compararVoos(Voo a, Voo b, const char* ordem) {
    for (int i = 0; i < 3; i++) {
        switch (ordem[i]) {
            case 'p': // Comparação por preço
                if (a.prc != b.prc) return (a.prc > b.prc) ? 1 : -1;
                break;
            case 'd': // Comparação por duração
                if (a.dur != b.dur) return (a.dur > b.dur) ? 1 : -1;
                break;
            case 's': // Comparação por número de paradas
                if (a.sto != b.sto) return (a.sto > b.sto) ? 1 : -1;
                break;
        }
    }
    return 0; // São iguais em todos os critérios
}

// Divide uma lista encadeada ao meio.
// Entrada: origem - ponteiro para o início da lista
//          frente - ponteiro para armazenar a primeira metade da lista
//          tras - ponteiro para armazenar a segunda metade da lista
void dividirLista(NoLista* origem, NoLista** frente, NoLista** tras) {
    NoLista* rapido = origem->prox;
    NoLista* lento = origem;

    while (rapido) {
        rapido = rapido->prox;
        if (rapido) {
            lento = lento->prox;
            rapido = rapido->prox;
        }
    }

    *frente = origem;
    *tras = lento->prox;
    lento->prox = NULL;
}

// Mescla duas listas encadeadas ordenadas.
// Entrada: lista1, lista2 - ponteiros para as listas a serem mescladas
//          ordem - string contendo a ordem de critérios para comparação
// Saída: ponteiro para a lista mesclada
NoLista* mesclarListas(NoLista* lista1, NoLista* lista2, char* ordem) {
    if (!lista1) return lista2;
    if (!lista2) return lista1;

    NoLista* resultado = NULL;

    if (compararVoos(lista1->voo, lista2->voo, ordem) <= 0) {
        resultado = lista1;
        resultado->prox = mesclarListas(lista1->prox, lista2, ordem);
    } else {
        resultado = lista2;
        resultado->prox = mesclarListas(lista1, lista2->prox, ordem);
    }
    
    return resultado;
}

// Ordena uma lista encadeada usando o algoritmo Merge Sort.
// Entrada: cabecaRef - ponteiro para o início da lista
//          ordem - string contendo a ordem de critérios para comparação
void mergeSortLista(NoLista** cabecaRef, char* ordem) {
    if (!(*cabecaRef) || !(*cabecaRef)->prox) return;

    NoLista* frente;
    NoLista* tras;

    dividirLista(*cabecaRef, &frente, &tras);
    
    mergeSortLista(&frente, ordem);
    mergeSortLista(&tras, ordem);

    *cabecaRef = mesclarListas(frente, tras, ordem);
}

// Ordena a lista de voos com base em uma ordem de critérios.
// Entrada: lista - ponteiro para a lista de voos
//          ordem - string contendo a ordem de critérios (ex: "pds" para preço, duração, paradas)
void ordenarListaVoos(ListaVoos* lista, char* ordem) {
    if (!lista || !lista->inicio) return;
    mergeSortLista(&(lista->inicio), ordem);
}

// Libera a memória alocada para uma lista encadeada de voos.
// Entrada: lista - ponteiro para a lista a ser liberada
void liberarLista(ListaVoos* lista) {
    if (!lista) return;

    NoLista* atual = lista->inicio;
    while (atual) {
        NoLista* temp = atual;
        atual = atual->prox;
        free(temp);  // Libera cada nó da lista
    }

    free(lista);  // Libera a estrutura da lista
}