#include <stdio.h>
#include <stdlib.h>

typedef struct Tree {
    int key;
    struct Tree *left, *right;
} Tree;

Tree *insert(Tree *t, int key) {
    if (!t) {
        t = malloc(sizeof *t);
        t->key = key;
        t->left = t->right = NULL;
    } else if (key < t->key) t->left = insert(t->left, key);
    else if (key > t->key) t->right = insert(t->right, key);
    return t;
}

void inorder(const Tree *t) {
    if (!t) return;
    inorder(t->left);
    printf("%d ", t->key);
    inorder(t->right);
}

int height(const Tree *t) {
    if (!t) return 0;
    int l = height(t->left), r = height(t->right);
    return 1 + (l > r ? l : r);
}

int contains(const Tree *t, int key) {
    while (t) {
        if (key == t->key) return 1;
        t = key < t->key ? t->left : t->right;
    }
    return 0;
}

void destroy(Tree *t) {
    if (!t) return;
    destroy(t->left);
    destroy(t->right);
    free(t);
}

int main(void) {
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 30, 65};
    Tree *root = NULL;
    for (int i = 0; i < 9; i++) root = insert(root, keys[i]);
    inorder(root);
    printf("\nheight=%d has40=%d has45=%d\n", height(root), contains(root, 40), contains(root, 45));
    destroy(root);
    return 0;
}
