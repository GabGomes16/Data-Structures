#include <stdio.h>
#include <stdlib.h>

int actual_calc(int a, int b) {
    if (b == 0) {
        printf("Não é posivel dividir por 0.\n");
        return -1;
    }
    int c = a / b;
    printf("Result: %d / %d = %d\n", a, b, c);
    return 0;
}

int calc() {
    int a = 13;
    int b = 0;
    if (actual_calc(a, b) == -1) {
        printf("Falha de calculo.\n");
    }
    return 0;
}

int main() {
    calc();
    return 0;
}
