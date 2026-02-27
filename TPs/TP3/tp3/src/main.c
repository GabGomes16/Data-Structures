#include <stdio.h>
#include <stdlib.h>
#include "../include/voos.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.txt>\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    Voo voos[MAX_VOOS];
    int n;
    Consulta consultas[MAX_CONSULTAS];
    int c;

    // Lê a entrada do arquivo
    lerEntrada(arquivo, voos, &n, consultas, &c);

    // Fecha o arquivo após leitura
    fclose(arquivo);

    // Processa as consultas
    processarConsultas(voos, n, consultas, c);

    return 0;
}