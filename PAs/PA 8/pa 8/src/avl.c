#include "avl.h"
#include <stdlib.h>
#include <stdio.h>

// Função para obter a altura de um nó
int getHeight(Node* node) {
    return node ? node->height : 0;
}

// Função para calcular o fator de balanceamento de um nó
int getBalanceFactor(Node* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// Função para criar um novo nó
Node* createNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// Função para obter o maior entre dois valores
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Rotação à direita
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T = x->right;

    x->right = y;
    y->left = T;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Rotação à esquerda
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T = y->left;

    y->left = x;
    x->right = T;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Função para balancear o nó
Node* balanceNode(Node* node) {
    int balance = getBalanceFactor(node);

    // Caso de desbalanceamento à esquerda
    if (balance > 1 && getBalanceFactor(node->left) >= 0)
        return rotateRight(node);

    // Caso de desbalanceamento à esquerda-direita
    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Caso de desbalanceamento à direita
    if (balance < -1 && getBalanceFactor(node->right) <= 0)
        return rotateLeft(node);

    // Caso de desbalanceamento à direita-esquerda
    if (balance < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Função para inserir uma chave na árvore AVL
Node* insert(Node* node, int key) {
    if (!node)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // Chaves duplicadas não são permitidas

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    return balanceNode(node);
}

// Função para encontrar o menor valor em uma árvore
Node* findMin(Node* node) {
    while (node->left)
        node = node->left;
    return node;
}

// Função para remover uma chave da árvore AVL
Node* removeNode(Node* node, int key) {
    if (!node)
        return NULL;

    if (key < node->key)
        node->left = removeNode(node->left, key);
    else if (key > node->key)
        node->right = removeNode(node->right, key);
    else {
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            free(node);
            return temp;
        } else {
            Node* temp = findMin(node->right);
            node->key = temp->key;
            node->right = removeNode(node->right, temp->key);
        }
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    return balanceNode(node);
}

// Função para exibir o caminhamento pré-fixo
void preOrder(Node* root) {
    if (root) {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

// Função para exibir o caminhamento in-fixo
void inOrder(Node* root) {
    if (root) {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

// Função para exibir o caminhamento pós-fixo
void postOrder(Node* root) {
    if (root) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->key);
    }
}

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}