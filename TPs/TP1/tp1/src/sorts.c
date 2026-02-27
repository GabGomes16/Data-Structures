#include "../include/ordInd.h"

// Função de ordenação QuickSort
// Descrição: Ordena o array de Índices Indiretos usando o algoritmo QuickSort.
// Entrada: arr (array de índices), low (índice inicial), high (índice final), cmp (função de comparação)
// Saída: Ordenação do array de índices conforme o critério especificado pela função de comparação
void quicksort(IndiceIndireto *arr, int low, int high, int (*cmp)(const void *, const void *)) {
    if (low < high) {
        int pivot = low;
        int i = low, j = high;
        IndiceIndireto temp;
        
        // Realiza a troca para posicionar o pivô corretamente
        while (i < j) {
            while (i <= high && cmp(&arr[i], &arr[pivot]) <= 0) i++; // Move i até encontrar valor maior que o pivô
            while (cmp(&arr[j], &arr[pivot]) > 0) j--; // Move j até encontrar valor menor que o pivô
            if (i < j) { // Troca os elementos nos índices i e j
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        
        // Coloca o pivô na posição correta
        temp = arr[pivot];
        arr[pivot] = arr[j];
        arr[j] = temp;
        
        // Recursão para ordenar as duas metades do array
        quicksort(arr, low, j - 1, cmp);
        quicksort(arr, j + 1, high, cmp);
    }
}

// Função de ordenação BubbleSort
// Descrição: Ordena o array de Índices Indiretos usando o algoritmo BubbleSort.
// Entrada: arr (array de índices), n (tamanho do array), cmp (função de comparação)
// Saída: Ordenação do array de índices conforme o critério especificado pela função de comparação
void bubblesort(IndiceIndireto *arr, int n, int (*cmp)(const void *, const void *)) {
    // Percorre o array, comparando e trocando elementos adjacentes
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) { 
            if (cmp(&arr[j], &arr[j + 1]) > 0) { // Se o elemento atual for maior que o próximo
                // Troca os elementos
                IndiceIndireto temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Função de ordenação InsertionSort
// Descrição: Ordena o array de Índices Indiretos usando o algoritmo InsertionSort.
// Entrada: arr (array de índices), n (tamanho do array), cmp (função de comparação)
// Saída: Ordenação do array de índices conforme o critério especificado pela função de comparação
void insertionsort(IndiceIndireto *arr, int n, int (*cmp)(const void *, const void *)) {
    // Itera sobre o array e insere os elementos na posição correta
    for (int i = 1; i < n; i++) {
        IndiceIndireto key = arr[i]; // Elemento a ser inserido
        int j = i - 1;
        
        // Move os elementos maiores que o chave uma posição à frente
        while (j >= 0 && cmp(&arr[j], &key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        
        // Insere o chave na posição correta
        arr[j + 1] = key;
    }
}

// Função de comparação para nome
// Descrição: Compara dois registros pelo nome
// Entrada: a e b (do tipo IndiceIndireto)
// Saída: valor negativo se a < b, 0 se a == b, valor positivo se a > b
int cmp_nome(const void *a, const void *b) {
    return strcmp(((IndiceIndireto *)a)->registro->nome, ((IndiceIndireto *)b)->registro->nome);
}

// Função de comparação para CPF
// Descrição: Compara dois registros pelo CPF
// Entrada: a e b (do tipo IndiceIndireto)
// Saída: valor negativo se a < b, 0 se a == b, valor positivo se a > b
int cmp_cpf(const void *a, const void *b) {
    return strcmp(((IndiceIndireto *)a)->registro->cpf, ((IndiceIndireto *)b)->registro->cpf);
}

// Função de comparação para endereço
// Descrição: Compara dois registros pelo endereço
// Entrada: a e b (do tipo IndiceIndireto)
// Saída: valor negativo se a < b, 0 se a == b, valor positivo se a > b
int cmp_endereco(const void *a, const void *b) {
    return strcmp(((IndiceIndireto *)a)->registro->endereco, ((IndiceIndireto *)b)->registro->endereco);
}