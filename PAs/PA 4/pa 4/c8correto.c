#include <stdlib.h>
#include <stdint.h>

int main() {
    char* destination = calloc(27, sizeof(char));
    char* source = malloc(26 * sizeof(char));

    for (uint8_t i = 0; i < 26; i++) {
        *(destination + i) = *(source + i);
    }

    free(destination);
    free(source);

    return 0;
}
