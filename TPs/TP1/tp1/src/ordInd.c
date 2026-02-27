#include "../include/ordInd.h"

// Função para realloc seguro
// Descrição: Tenta realocar a memória e, se falhar, libera a memória anterior e encerra o programa
// Entrada: ptr (ponteiro original), new_size (novo tamanho da memória)
// Saída: ponteiro para a memória realocada, ou encerra o programa em caso de falha
void *safe_realloc(void *ptr, size_t new_size) {
    void *new_ptr = realloc(ptr, new_size);
    if (!new_ptr) {
        fprintf(stderr, "Erro: falha ao realocar memória.\n");
        free(ptr); // Libera a memória antiga em caso de erro.
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

// Função para criar a estrutura OrdInd
// Descrição: Cria e inicializa a estrutura OrdInd
// Entrada: nenhuma
// Saída: ponteiro para a estrutura OrdInd
OrdInd_ptr Cria() {
    OrdInd_ptr poi = (OrdInd_ptr)malloc(sizeof(OrdInd));
    poi->registros = NULL;
    poi->num_registros = 0;
    poi->max_registros = 0;
    poi->atributos = NULL;
    poi->num_atributos = 0;
    poi->indices = NULL;
    return poi;
}

// Função para destruir a estrutura OrdInd
// Descrição: Libera a memória alocada para a estrutura e seus componentes
// Entrada: ponteiro para a estrutura OrdInd
// Saída: 0 em caso de sucesso, -1 em caso de erro
int Destroi(OrdInd_ptr poi) {
    if (!poi) return -1;

    if (poi->registros) {
        free(poi->registros); // Libera a memória dos registros
        poi->registros = NULL;
    }

    if (poi->atributos) {
        for (int i = 0; i < poi->num_atributos; i++) {
            if (poi->atributos[i]) {
                free(poi->atributos[i]); // Libera cada atributo
                poi->atributos[i] = NULL;
            }
        }
        free(poi->atributos); // Libera o array de atributos
        poi->atributos = NULL;
    }

    if (poi->indices) {
        for (int i = 0; i < poi->num_atributos; i++) {
            if (poi->indices[i]) {
                free(poi->indices[i]);  // Libera a memória de cada índice
                poi->indices[i] = NULL;
            }
        }
        free(poi->indices);  // Libera o array de ponteiros para índices
        poi->indices = NULL;
    }

    free(poi); // Libera a estrutura principal
    poi = NULL;

    return 0;
}

// Função para carregar um arquivo e preencher a estrutura OrdInd
// Descrição: Lê o arquivo de entrada e preenche os atributos e registros na estrutura OrdInd
// Entrada: poi (ponteiro para a estrutura OrdInd), nomeentrada (nome do arquivo)
// Saída: 0 em caso de sucesso, -1 em caso de erro
int CarregaArquivo(OrdInd_ptr poi, char *nomeentrada) {
    FILE *file = fopen(nomeentrada, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", nomeentrada);
        return -1;
    }

    char linha[1024];

    if (fgets(linha, sizeof(linha), file)) {
        sscanf(linha, "%d", &poi->num_atributos); // Lê o número de atributos
        poi->atributos = (char **)malloc(poi->num_atributos * sizeof(char *)); // Aloca memória para atributos
        if (!poi->atributos) {
            fprintf(stderr, "Erro ao alocar memória para atributos.\n");
            return -1;
        }
    } else {
        fprintf(stderr, "Erro ao ler o número de atributos.\n");
        fclose(file);
        return -1;
    }

    for (int i = 0; i < poi->num_atributos; i++) {
        if (fgets(linha, sizeof(linha), file)) {
            poi->atributos[i] = strdup(linha); // Aloca e copia o atributo
            poi->atributos[i][strcspn(poi->atributos[i], "\n")] = '\0'; // Remove o '\n'
        } else {
            fprintf(stderr, "Erro ao ler atributo %d.\n", i);
            fclose(file);
            return -1;
        }
    }

    if (fgets(linha, sizeof(linha), file)) {
        sscanf(linha, "%d", &poi->num_registros); // Lê o número de registros
        poi->registros = (Registro *)malloc(poi->num_registros * sizeof(Registro));// Aloca memória para registros
        if (!poi->registros) {
            fprintf(stderr, "Erro ao alocar memória para registros.\n");
            return -1;
        }
        poi->max_registros = poi->num_registros;
    } else {
        fprintf(stderr, "Erro ao ler o número de registros.\n");
        fclose(file);
        return -1;
    }

    // Lê os registros
    int registro_atual = 0;
    while (fgets(linha, sizeof(linha), file)) {
        if (registro_atual >= poi->num_registros) break;

        Registro *reg = &poi->registros[registro_atual];
        int campos_lidos = sscanf(linha, "%49[^,],%19[^,],%99[^,],%199[^\n]",
                                  reg->nome, reg->cpf, reg->endereco, reg->outros); // Lê os campos do registro
        if (campos_lidos != 4) {
            fprintf(stderr, "Linha inválida (campos lidos: %d): %s\n", campos_lidos, linha);
            continue; // Ignora a linha inválida
        }

        registro_atual++;
    }

    fclose(file);
    return 0;
}


int NumAtributos(OrdInd_ptr poi) {
    return poi ? poi->num_atributos : -1;
}

int NomeAtributo(OrdInd_ptr poi, int pos, char *nome) {
    if (!poi || pos < 0 || pos >= poi->num_atributos) return -1;
    strcpy(nome, poi->atributos[pos]);
    return 1;
}

// Função para criar um índice para um atributo específico
// Descrição: Cria um índice para um atributo específico, inicializando os ponteiros e alocando memória
// Entrada: poi (ponteiro para a estrutura OrdInd), atribid (ID do atributo)
// Saída: 0 em caso de sucesso, -1 em caso de erro
int CriaIndice(OrdInd_ptr poi, int atribid) {
    if (!poi || atribid < 0 || atribid >= poi->num_atributos) return -1;

    // Inicializa o array de índices, se necessário
    if (!poi->indices) {
        poi->indices = (IndiceIndireto **)malloc(poi->num_atributos * sizeof(IndiceIndireto *)); // Aloca memória para índices
        if (!poi->indices) {
            fprintf(stderr, "[DEBUG] Falha ao alocar memória para índices.\n");
            return -1;
        }
        for (int i = 0; i < poi->num_atributos; i++) {
            poi->indices[i] = NULL; // Inicializa os ponteiros
        }
    }

    // Libera a memória pré-existente para o índice, se necessário
    if (poi->indices[atribid]) {
        free(poi->indices[atribid]);
        poi->indices[atribid] = NULL; // Evita dangling pointers
    }

    // Aloca memória para o novo índice
    poi->indices[atribid] = (IndiceIndireto *)malloc(poi->num_registros * sizeof(IndiceIndireto)); // Aloca memória para o novo índice
    if (!poi->indices[atribid]) {
        fprintf(stderr, "[DEBUG] Falha ao alocar memória para índice %d.\n", atribid);
        return -1;
    }

    // Inicializa os índices
    for (int i = 0; i < poi->num_registros; i++) {
        poi->indices[atribid][i].index = i;
        poi->indices[atribid][i].registro = &poi->registros[i];
    }

    return 0;
}

// Função para ordenar um índice
// Descrição: Ordena o índice de um atributo usando o algoritmo especificado
// Entrada: poi (ponteiro para a estrutura OrdInd), atribid (ID do atributo), current_algo (algoritmo de ordenação)
// Saída: 0 em caso de sucesso, -1 em caso de erro
int OrdenaIndice(OrdInd_ptr poi, int atribid, int current_algo) {
    if (!poi || atribid < 0 || atribid >= poi->num_atributos) {
        fprintf(stderr, "Erro: Índice inválido ou estrutura não inicializada.\n");
        return -1;
    }

    int (*cmp)(const void *, const void *); // Função de comparação
    
     // Seleciona a função de comparação com base no nome do atributo
    if (!strcmp(poi->atributos[atribid], "name,s")) {
        cmp = cmp_nome;
    } else if (!strcmp(poi->atributos[atribid], "id,s")) {
        cmp = cmp_cpf;
    } else if (!strcmp(poi->atributos[atribid], "address,s")) {
        cmp = cmp_endereco;
    } else {
        fprintf(stderr, "Erro: Atributo desconhecido para ordenação: %s\n", poi->atributos[atribid]);
        return -1;
    }

    /// Seleciona e executa o algoritmo de ordenação
    if (current_algo == 0) {
        quicksort(poi->indices[atribid], 0, poi->num_registros - 1, cmp);
    } else if (current_algo == 1) {
        bubblesort(poi->indices[atribid], poi->num_registros, cmp);
    } else if (current_algo == 2) {
        insertionsort(poi->indices[atribid], poi->num_registros, cmp);
    } else {
        fprintf(stderr, "Erro: Algoritmo de ordenação inválido (%d).\n", current_algo);
        return -1;
    }

    return 0;
}

// Função para imprimir os registros ordenados
// Descrição: Imprime os registros ordenados com base no índice criado para o atributo
// Entrada: poi (ponteiro para a estrutura OrdInd), atribid (ID do atributo)
// Saída: 0 em caso de sucesso, -1 em caso de erro
int ImprimeOrdenadoIndice(OrdInd_ptr poi, int atribid) {
    if (!poi || atribid < 0 || atribid >= poi->num_atributos) {
        fprintf(stderr, "Erro: Índice inválido ou estrutura não inicializada.\n");
        return -1;
    }
    
    // Imprime os registros ordenados
    for (int i = 0; i < poi->num_registros; i++) {
        IndiceIndireto *ind = &poi->indices[atribid][i];
        Registro *reg = ind->registro;
        printf("%s,%s,%s,%s\n", reg->nome, reg->cpf, reg->endereco, reg->outros);
    }

    return 0;
}

