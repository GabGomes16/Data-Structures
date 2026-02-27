#include "../include/ordInd.h"

int main(int argc, char *argv[]) {
    char aux[1024];

    // Verifica se o número correto de argumentos foi fornecido
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <nome do arquivo>\n", argv[0]);
        return -1;
    }

    // Recupera o nome do arquivo a partir do argumento de linha de comando
    // Entrada: argc, argv
    // Saída: nome_arquivo
    char *nome_arquivo = argv[1];

    OrdInd_ptr poi = Cria();  // Cria a estrutura de dados para armazenar as informações

    // Tenta carregar o arquivo de dados para a estrutura OrdInd
    // Entrada: poi, nome_arquivo
    // Saída: 0 (sucesso) ou -1 (erro)
    if (CarregaArquivo(poi, nome_arquivo) != 0) {
        fprintf(stderr, "Erro ao carregar arquivo: %s\n", nome_arquivo);
        return -1;
    }

    // Exibe o cabeçalho original com o número de atributos e seus nomes
    // Entrada: poi
    // Saída: cabeçalho impresso na tela
    printf("%d\n", poi->num_atributos);
    for (int i = 0; i < poi->num_atributos; i++) {
        printf("%s\n", poi->atributos[i]);
    }
    printf("%d\n", poi->num_registros);

    // Para cada algoritmo de ordenação (QuickSort, BubbleSort, InsertionSort)
    // Entrada: algoritmos
    // Saída: dados ordenados para cada algoritmo
    int algoritmos = 3;  // QuickSort, BubbleSort e InsertionSort
    for (int algo = 0; algo < algoritmos; algo++) {
        for (int i = 0; i < poi->num_atributos; i++) {
            // Obtém o nome do atributo para comparações
            // Entrada: poi, i, aux
            // Saída: aux (nome do atributo)
            if (NomeAtributo(poi, i, aux) > 0) {
                // Verifica se o atributo é "name,s", "id,s" ou "address,s"
                // Entrada: aux
                // Saída: nome do atributo comparado
                if (!strcmp(aux, "name,s") || !strcmp(aux, "id,s") || !strcmp(aux, "address,s")) {
                    // Cria o índice para ordenação
                    // Entrada: poi, i
                    // Saída: índice criado
                    CriaIndice(poi, i);

                    // Ordena os registros de acordo com o índice e algoritmo
                    // Entrada: poi, i, algo
                    // Saída: registros ordenados
                    OrdenaIndice(poi, i, algo);  // Passa o número do algoritmo
                    ImprimeOrdenadoIndice(poi, i);  // Exibe os registros ordenados

                    // Exibe novamente o cabeçalho para manter o padrão, caso não seja a última iteração
                    if (!(i == 2 && algo == 2)) {
                        // Entrada: poi
                        // Saída: cabeçalho reexibido
                        printf("%d\n", poi->num_atributos);
                        for (int j = 0; j < poi->num_atributos; j++) {
                            printf("%s\n", poi->atributos[j]);
                        }
                        printf("%d\n", poi->num_registros);
                    }
                }
            }
        }
    }

    // Destroi a estrutura de dados e libera a memória
    // Entrada: poi
    // Saída: estrutura destruída
    Destroi(poi);

    return 0;
}