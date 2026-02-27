#include "../include/voos.h"
#include "../include/arvore.h"

// Converte uma string de data e hora no formato "YYYY-MM-DDThh:mm:ss" para time_t
// Entrada: str - string contendo a data e hora
// Saída: time_t correspondente à data e hora fornecida
time_t parseDateTime(const char *str) {
    struct tm t = {0};
    sscanf(str, "%4d-%2d-%2dT%2d:%2d:%2d", 
           &t.tm_year, &t.tm_mon, &t.tm_mday,
           &t.tm_hour, &t.tm_min, &t.tm_sec);

    t.tm_year -= 1900; // Ajusta ano
    t.tm_mon -= 1;     // Ajusta mês (0-11)
    return mktime(&t);
}

// Remove parênteses de uma string
// Entrada: entrada - string original
// Saída: saida - string sem parênteses
void removerParenteses(const char *entrada, char *saida) {
    int j = 0;
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (entrada[i] != '(' && entrada[i] != ')') {
            saida[j++] = entrada[i];
        }
    }
    saida[j] = '\0'; // Adiciona o terminador de string
}

// Conta o número de componentes em uma expressão lógica
// Entrada: expressao - string contendo a expressão lógica
// Saída: número de componentes na expressão
int contarComponentes(const char *expressao) {
    int count = 1;
    const char *ptr = expressao;

    while (*ptr) {
        if ((ptr[0] == '&' && ptr[1] == '&') || 
            (ptr[0] == '|' && ptr[1] == '|') || 
            (ptr[0] == '!')) {
            count++;
            if (ptr[0] != '!') ptr++;  // Pular operador lógico duplo
        }
        ptr++;
    }

    return count;
}

// Converte uma string de 3 caracteres em um inteiro
// Entrada: str - string de 3 caracteres
// Saída: inteiro correspondente à string
int converterParaInt(char str[]) {
    return (str[0] * 10000) + (str[1] * 100) + str[2];
}

// Lê os dados de entrada de um arquivo e preenche os arrays de voos e consultas
// Entrada: arquivo - ponteiro para o arquivo de entrada
//          voos - array de voos
//          n - número de voos
//          consultas - array de consultas
//          c - número de consultas
void lerEntrada(FILE *arquivo, Voo voos[], int *n, Consulta consultas[], int *c) {
    // Lê o número de voos
    fscanf(arquivo, "%d", n);

    for (int i = 0; i < *n; i++) {
        char partida[25], chegada[25];
        fscanf(arquivo, "%s %s %lf %d %s %s %d",
               voos[i].org,
               voos[i].dst,
               &voos[i].prc,
               &voos[i].sea,
               partida,
               chegada,
               &voos[i].sto);

        voos[i].id = i + 1; // ID do voo

        // Converter os códigos de aeroporto para inteiros
        voos[i].org_val = converterParaInt(voos[i].org);
        voos[i].dst_val = converterParaInt(voos[i].dst);

        // Converter data-hora para time_t
        voos[i].dep = parseDateTime(partida);    

        voos[i].arr = parseDateTime(chegada);

        // Calcular duração
        voos[i].dur = (int)difftime(voos[i].arr, voos[i].dep);
    }

    // Lê o número de consultas
    fscanf(arquivo, "%d", c);
    fgetc(arquivo); // Limpar o caractere de nova linha

    for (int i = 0; i < *c; i++) {
        fscanf(arquivo, "%d %s %s", 
               &consultas[i].max_voos,
               consultas[i].criterio_ordenacao,
               consultas[i].expressao_base);
        
        // Remover parênteses da expressão base e armazenar em expressao_logica
        removerParenteses(consultas[i].expressao_base, consultas[i].expressao_logica);
        
        // Contar componentes na expressão lógica
        consultas[i].num_componentes = contarComponentes(consultas[i].expressao_logica);
    }
}

// Compara dois voos com base na origem
// Entrada: a, b - voos a serem comparados
// Saída: -1 se a < b, 1 se a > b, 0 se a == b
int compararOrigem(Voo a, Voo b) {
    if (a.org_val < b.org_val) return -1;
    if (a.org_val > b.org_val) return 1;
    return (a.id < b.id) ? -1 : 1;
}

// Compara dois voos com base na origem
// Entrada: a, b - voos a serem comparados
// Saída: -1 se a < b, 1 se a > b, 0 se a == b
int compararDestino(Voo a, Voo b) {
    if (a.dst_val < b.dst_val) return -1;
    if (a.dst_val > b.dst_val) return 1;
    return (a.id < b.id) ? -1 : 1;
}

// Compara dois voos com base na origem
// Entrada: a, b - voos a serem comparados
// Saída: -1 se a < b, 1 se a > b, 0 se a == b
int compararPreco(Voo a, Voo b) {
    if (a.prc < b.prc) return -1;
    if (a.prc > b.prc) return 1;
    return (a.id < b.id) ? -1 : 1;
}

// Compara dois voos com base na origem
// Entrada: a, b - voos a serem comparados
// Saída: -1 se a < b, 1 se a > b, 0 se a == b
int compararAssentos(Voo a, Voo b) {
    if (a.sea < b.sea) return -1;
    if (a.sea > b.sea) return 1;
    return (a.id < b.id) ? -1 : 1;
}

// Compara dois voos com base na origem
// Entrada: a, b - voos a serem comparados
// Saída: -1 se a < b, 1 se a > b, 0 se a == b
int compararParadas(Voo a, Voo b) {
    if (a.sto < b.sto) return -1;
    if (a.sto > b.sto) return 1;
    return (a.id < b.id) ? -1 : 1;
}

// Compara dois voos com base na origem
// Entrada: a, b - voos a serem comparados
// Saída: -1 se a < b, 1 se a > b, 0 se a == b
int compararDuracao(Voo a, Voo b) {
    if (a.dur < b.dur) return -1;
    if (a.dur > b.dur) return 1;
    return (a.id < b.id) ? -1 : 1;
}

// Processa as consultas e aplica os filtros e ordenações
// Entrada: voos - array de voos
//          n - número de voos
//          consultas - array de consultas
//          c - número de consultas
void processarConsultas(Voo voos[], int n, Consulta consultas[], int c) {
    for (int i = 0; i < c; i++) {
        Consulta consulta = consultas[i];

        // Criar árvores de índice
        AVLNo *arvoreOrigem = NULL, *arvoreDestino = NULL, *arvorePreco = NULL;
        AVLNo *arvoreAssentos = NULL, *arvoreDuracao = NULL, *arvoreParadas = NULL;

        for (int j = 0; j < n; j++) {
            arvoreOrigem = inserirAVL(arvoreOrigem, voos[j], compararOrigem);
            arvoreDestino = inserirAVL(arvoreDestino, voos[j], compararDestino);
            arvorePreco = inserirAVL(arvorePreco, voos[j], compararPreco);
            arvoreAssentos = inserirAVL(arvoreAssentos, voos[j], compararAssentos);
            arvoreParadas = inserirAVL(arvoreParadas, voos[j], compararParadas);
            arvoreDuracao = inserirAVL(arvoreDuracao, voos[j], compararDuracao);
        }

        //*************************PONTOS EXTRAS*************************
        // Permitir alterações na lista de voos durante a execução
        //char operacao[10];
        //while (scanf("%s", operacao) != EOF) {
        //    if (strcmp(operacao, "ADD") == 0) {
        //        Voo novoVoo;
        //        lerVoo(&novoVoo); // Função para ler um voo da entrada
        //        arvoreOrigem = inserirAVL(arvoreOrigem, novoVoo, compararOrigem);
        //        arvoreDestino = inserirAVL(arvoreDestino, novoVoo, compararDestino);
        //        arvorePreco = inserirAVL(arvorePreco, novoVoo, compararPreco);
        //        arvoreAssentos = inserirAVL(arvoreAssentos, novoVoo, compararAssentos);
        //        arvoreParadas = inserirAVL(arvoreParadas, novoVoo, compararParadas);
        //        arvoreDuracao = inserirAVL(arvoreDuracao, novoVoo, compararDuracao);
        //    } else if (strcmp(operacao, "REMOVE") == 0) {
        //        char id[20];
        //        scanf("%s", id);
        //        arvoreOrigem = removerAVL(arvoreOrigem, id, compararOrigem);
        //        arvoreDestino = removerAVL(arvoreDestino, id, compararDestino);
        //        arvorePreco = removerAVL(arvorePreco, id, compararPreco);
        //        arvoreAssentos = removerAVL(arvoreAssentos, id, compararAssentos);
        //        arvoreParadas = removerAVL(arvoreParadas, id, compararParadas);
        //        arvoreDuracao = removerAVL(arvoreDuracao, id, compararDuracao);
        //    } else {
        //        break;
        //    }
        //}
        //*************************PONTOS EXTRAS*************************

        // Criar array dinâmico para armazenar listas de resultados
        ListaVoos** listaDeLista = (ListaVoos**)malloc(consulta.num_componentes * sizeof(ListaVoos*));
        if (!listaDeLista) {
            fprintf(stderr, "Erro ao alocar listaDeLista\n");
            exit(EXIT_FAILURE);
        }

        char componentes[consulta.num_componentes][50];
        char operadores[consulta.num_componentes - 1][3];
        separarComponentes(consulta.expressao_logica, componentes, operadores, consulta.num_componentes);

        for (int j = 0; j < consulta.num_componentes; j++) {
            char variavel[4], operacao[4], valor[256];
            extrairComponentes(componentes[j], variavel, operacao, valor);

            // Criar lista vazia
            listaDeLista[j] = (ListaVoos*)malloc(sizeof(ListaVoos));
            listaDeLista[j]->inicio = NULL;

            ParametrosCriterio param;
            strcpy(param.variavel, variavel);
            strcpy(param.operacao, operacao);
            strcpy(param.valor, valor);

            // Aplicar filtro e armazenar na lista correspondente
            if (strcmp(variavel, "org") == 0) {
                arvoreOrigem = removerSeNaoAtende(arvoreOrigem, criterioWrapper, &param);
                avlParaLista(arvoreOrigem, listaDeLista[j]);
            } else if (strcmp(variavel, "dst") == 0) {
                arvoreDestino = removerSeNaoAtende(arvoreDestino, criterioWrapper, &param);
                avlParaLista(arvoreDestino, listaDeLista[j]);
            } else if (strcmp(variavel, "prc") == 0) {
                arvorePreco = removerSeNaoAtende(arvorePreco, criterioWrapper, &param);
                avlParaLista(arvorePreco, listaDeLista[j]);
            } else if (strcmp(variavel, "sea") == 0) {
                arvoreAssentos = removerSeNaoAtende(arvoreAssentos, criterioWrapper, &param);
                avlParaLista(arvoreAssentos, listaDeLista[j]);
            } else if (strcmp(variavel, "dur") == 0) {
                arvoreDuracao = removerSeNaoAtende(arvoreDuracao, criterioWrapper, &param);
                avlParaLista(arvoreDuracao, listaDeLista[j]);
            } else if (strcmp(variavel, "sto") == 0) {
                arvoreParadas = removerSeNaoAtende(arvoreParadas, criterioWrapper, &param);
                avlParaLista(arvoreParadas, listaDeLista[j]);
            }
        }

        // Processar operações lógicas
        if (consulta.num_componentes - 1 > 0) {
            ListaVoos* resultado = NULL;
            for (int j = 0; j < consulta.num_componentes - 1; j++) {
                ListaVoos** lista = (ListaVoos**)malloc(2 * sizeof(ListaVoos*));
                if (!lista) {
                    fprintf(stderr, "Erro ao alocar memória para lista\n");
                    exit(EXIT_FAILURE);
                }

                if (j == 0) {
                    lista[0] = listaDeLista[0];
                    lista[1] = listaDeLista[1];
                } else {
                    lista[0] = resultado;  // Usa o resultado anterior
                    lista[1] = listaDeLista[j + 1];
                }

                // Processa a interseção ou outra operação lógica necessária
                ListaVoos* novoResultado = NULL;
                if (strcmp(operadores[j], "&&") == 0){
                    intersecaoListas(lista, 2, &novoResultado);
                } else if (strcmp(operadores[j], "||") == 0) {
                    uniaoListas(lista, 2, &novoResultado);
                } else {
                    diferencaListas(lista, 2, &novoResultado);
                }
                

                // Libera a memória antiga do resultado se necessário
                if (resultado) {
                    liberarLista(resultado);
                }

                resultado = novoResultado;  // Atualiza o resultado para a próxima iteração

                free(lista);
            }
            ordenarListaVoos(resultado, consulta.criterio_ordenacao);
            imprimirLista(resultado, consulta);
            
            //*************************PONTOS EXTRAS*************************
            // Verificar número de voos e sugerir refinamento
            //int numResultados = contarVoos(resultado);
            //if (numResultados > 25) {
            //    printf("Sua consulta retornou %d voos. Considere adicionar mais filtros, como:\n", numResultados);
            //    printf(" - Limitar faixa de preço (ex: prc < 500)\n");
            //    printf(" - Reduzir a duração do voo (ex: dur < 3)\n");
            //    printf(" - Diminuir o número de escalas (ex: sto == 0)\n");
            //}
            //*************************PONTOS EXTRAS*************************

            liberarLista(resultado);
            
        } else {
            ordenarListaVoos(listaDeLista[0], consulta.criterio_ordenacao);
            imprimirLista(listaDeLista[0], consulta);
        }   

        // Liberar memória
        for (int j = 0; j < consulta.num_componentes; j++) {
            liberarLista(listaDeLista[j]);
        }
        free(listaDeLista);

        liberarAVL(arvoreOrigem);
        liberarAVL(arvoreDestino);
        liberarAVL(arvorePreco);
        liberarAVL(arvoreAssentos);
        liberarAVL(arvoreDuracao);
        liberarAVL(arvoreParadas);
    }
}

// Separa os componentes de uma expressão lógica em variáveis, operações e valores
// Entrada: expressao_logica - string contendo a expressão lógica
//          componentes - array para armazenar os componentes
//          operadores - array para armazenar os operadores
//          num_componentes - número de componentes na expressão
void separarComponentes(const char *expressao_logica, char componentes[][50], char operadores[][3], int num_componentes) {
    const char *ptr = expressao_logica;
    int k = 0; // Índice dos componentes
    int op_index = 0; // Índice dos operadores lógicos

    while (*ptr != '\0' && k < num_componentes) {
        // Pula espaços em branco
        while (*ptr == ' ') {
            ptr++;
        }

        // Captura um componente (variável, operador, valor)
        int i = 0;
        while (*ptr != '&' && *ptr != '|' && *ptr != '!' && *ptr != '\0') {
            componentes[k][i++] = *ptr++;
        }
        componentes[k][i] = '\0'; // Finaliza string
        k++;

        // Pula espaços em branco novamente
        while (*ptr == ' ') {
            ptr++;
        }

        // Captura operadores lógicos ('&&', '||', '!')
        if (*ptr == '&' || *ptr == '|' || *ptr == '!') {
            if (*ptr == '&' && *(ptr + 1) == '&') {
                strcpy(operadores[op_index++], "&&");
                ptr += 2;
            } else if (*ptr == '|' && *(ptr + 1) == '|') {
                strcpy(operadores[op_index++], "||");
                ptr += 2;
            } else if (*ptr == '!') {
                strcpy(operadores[op_index++], "!");
                ptr++;
            }
        }
    }
}

// Extrai os componentes de uma expressão (variável, operação, valor)
// Entrada: expressao - string contendo a expressão
//          variavel - string para armazenar a variável
//          operacao - string para armazenar a operação
//          valor - string para armazenar o valor
void extrairComponentes(const char *expressao, char *variavel, char *operacao, char *valor) {
    int i = 0, j = 0;

    // Extrai a variável (3 primeiros caracteres)
    while (j < 3 && expressao[i] != '\0') {
        variavel[j++] = expressao[i++];
    }
    variavel[j] = '\0'; // Finaliza string

    // Extrai a operação (2 caracteres, se for ==, <= ou >=)
    j = 0;
    if (expressao[i] == '=' || expressao[i] == '<' || expressao[i] == '>') {
        operacao[j++] = expressao[i++];
        if (expressao[i] == '=') { // Verifica segundo caractere
            operacao[j++] = expressao[i++];
        }
    }
    operacao[j] = '\0';

    // Extrai o valor restante
    strcpy(valor, &expressao[i]);
}

int criterio(Voo voo, const char* variavel, const char* operacao, const char* valor) {
    if (strcmp(variavel, "org") == 0) {
        return strcmp(voo.org, valor) == 0;
    } else if (strcmp(variavel, "dst") == 0) {
        return strcmp(voo.dst, valor) == 0;
    } else if (strcmp(variavel, "prc") == 0) {
        double preco_voo = voo.prc;
        double preco_valor = atof(valor);
        if (strcmp(operacao, "==") == 0) return preco_voo == preco_valor;
        if (strcmp(operacao, "<=") == 0) return preco_voo <= preco_valor;
        if (strcmp(operacao, ">=") == 0) return preco_voo >= preco_valor;
    } else if (strcmp(variavel, "sea") == 0) {
        int assentos_voo = voo.sea;
        int assentos_valor = atoi(valor);
        if (strcmp(operacao, "==") == 0) return assentos_voo == assentos_valor;
        if (strcmp(operacao, "<=") == 0) return assentos_voo <= assentos_valor;
        if (strcmp(operacao, ">=") == 0) return assentos_voo >= assentos_valor;
    } else if (strcmp(variavel, "dur") == 0) {
        int duracao_voo = voo.dur;
        int duracao_valor = atoi(valor);
        if (strcmp(operacao, "==") == 0) return duracao_voo == duracao_valor;
        if (strcmp(operacao, "<=") == 0) return duracao_voo <= duracao_valor;
        if (strcmp(operacao, ">=") == 0) return duracao_voo >= duracao_valor;
    } else if (strcmp(variavel, "sto") == 0) {
        int paradas_voo = voo.sto;
        int paradas_valor = atoi(valor);
        if (strcmp(operacao, "==") == 0) return paradas_voo == paradas_valor;
        if (strcmp(operacao, "<=") == 0) return paradas_voo <= paradas_valor;
        if (strcmp(operacao, ">=") == 0) return paradas_voo >= paradas_valor;
    }
    
    return 0; // Caso a variável não seja reconhecida
}

    int criterioWrapper(Voo voo, void *parametros) {
        ParametrosCriterio *param = (ParametrosCriterio *)parametros;
        return criterio(voo, param->variavel, param->operacao, param->valor);
    }