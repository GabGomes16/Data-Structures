#ifndef AVL_TREE_H
#define AVL_TREE_H

// Estrutura do nó da árvore AVL
typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// Declarações das funções
int getHeight(Node* node);
int getBalanceFactor(Node* node);
Node* createNode(int key);
int max(int a, int b);
Node* rotateRight(Node* y);
Node* rotateLeft(Node* x);
Node* balanceNode(Node* node);
Node* insert(Node* node, int key);
Node* findMin(Node* node);
Node* removeNode(Node* node, int key);
void preOrder(Node* root);
void inOrder(Node* root);
void postOrder(Node* root);
void freeTree(Node* root);

#endif // AVL_TREE_H
