#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

int main() {
    int n;
    scanf("%d", &n);

    Node* root = NULL;

    for (int i = 0; i < n; i++) {
        char op;
        int key;
        scanf(" %c %d", &op, &key);

        if (op == 'i') {
            root = insert(root, key);
        } else if (op == 'r') {
            root = removeNode(root, key);
        }
    }

    preOrder(root);
    printf("\n");
    inOrder(root);
    printf("\n");
    postOrder(root);
    printf("\n");

    // Libera a memória da árvore
    freeTree(root);

    return 0;
}
