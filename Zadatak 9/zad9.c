#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tree* treePos;

struct tree {
    int value;
    treePos left;
    treePos right;
};

treePos Insert(treePos root, int value) {
    if (root == NULL) {
        root = (treePos)malloc(sizeof(struct tree));
        root->value = value;
        root->left = NULL;
        root->right = NULL;
        return root;
    }

    if (value < root->value)
        root->left = Insert(root->left, value);
    else
        root->right = Insert(root->right, value);

    return root;
}

int Replace(treePos root) {
    if (root == NULL)
        return 0;

    int old = root->value;
    root->value = Replace(root->left) + Replace(root->right);
    return root->value + old;
}

void Inorder(treePos root) {
    if (root == NULL) return;
    Inorder(root->left);
    printf("%d ", root->value);
    Inorder(root->right);
}

void InorderFile(treePos root, FILE* fp) {
    if (root == NULL) return;
    InorderFile(root->left, fp);
    fprintf(fp, "%d ", root->value);
    InorderFile(root->right, fp);
}

void FreeTree(treePos root) {
    if (root == NULL) return;
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

int main(void) {
    treePos root = NULL;
    int n = 10;

    srand((unsigned)time(NULL));

    for (int i = 0; i < n; i++) {
        int x = rand() % 81 + 10;
        root = Insert(root, x);
    }

    FILE* fp = fopen("stablo.txt", "w");
    InorderFile(root, fp);

    Replace(root);

    fprintf(fp, "\n");
    InorderFile(root, fp);
    fclose(fp);

    Inorder(root);

    FreeTree(root);
    return 0;
}
