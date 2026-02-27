#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 50
char buf[BUF_SIZE];

int sum_to_n(int num) {
    int i, sum = 0;
    for (i = 1; i <= num; i++) {
        sum += i;
    }
    return sum;
}

void printSum() {
    char line[10];
    printf("Enter a number:\n");

    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input.\n");
        return;
    }
    line[strcspn(line, "\n")] = '\0';
    int num = atoi(line);
    if (num <= 0) {
        printf("Invalid input. Please enter a positive integer.\n");
        return;
    }
    snprintf(buf, BUF_SIZE, "sum=%d", sum_to_n(num));
    printf("%s\n", buf);
}

int main(void) {
    printSum();
    return 0;
}
