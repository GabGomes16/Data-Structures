#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int ARR_SIZE = 1000;

int main() {
    int *intArray = malloc(sizeof(int) * ARR_SIZE);
    for (int i = 0; i < ARR_SIZE; i++) {
        intArray[i] = i;
    }
    srand(time(NULL));
    int randNum = rand() % ARR_SIZE;

    printf("intArray[%d]: %d\n", randNum, intArray[randNum]);
    
    free(intArray);
    return 0;
}
