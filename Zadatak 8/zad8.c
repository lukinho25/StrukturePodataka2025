#include <stdio.h>
#include <stdlib.h>

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
    else if (value > root->value)
        root->right = Insert(root->right, value);

    return root;
}

treePos FindMin(treePos root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

treePos Delete(treePos root, int value) {
    if (root == NULL)
        return NULL;

    if (value < root->value)
        root->left = Delete(root->left, value);
    else if (value > root->value)
        root->right = Delete(root->right, value);
    else {
        if (root->left != NULL && root->right != NULL) {
            treePos temp = FindMin(root->right);
            root->value = temp->value;
            root->right = Delete(root->right, temp->value);
        } else {
            treePos temp = root;
            if (root->left == NULL)
                root = root->right;
            else
                root = root->left;
            free(temp);
        }
    }
    return root;
}

void Inorder(treePos root) {
    if (root == NULL) return;
    Inorder(root->left);
    printf("%d ", root->value);
    Inorder(root->right);
}

void Preorder(treePos root) {
    if (root == NULL) return;
    printf("%d ", root->value);
    Preorder(root->left);
    Preorder(root->right);
}

void Postorder(treePos root) {
    if (root == NULL) return;
    Postorder(root->left);
    Postorder(root->right);
    printf("%d ", root->value);
}

#define MAX 100

void LevelOrder(treePos root) {
    if (root == NULL) return;

    treePos queue[MAX];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        treePos current = queue[front++];
        printf("%d ", current->value);

        if (current->left)
            queue[rear++] = current->left;
        if (current->right)
            queue[rear++] = current->right;
    }
}

void FreeTree(treePos root) {
    if (root == NULL) return;
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

int main(void) {
    treePos root = NULL;
    int x;

    while (1) {
        scanf("%d", &x);
        if (x == -1) break;
        root = Insert(root, x);
    }

    Inorder(root);
    printf("\n");
    Preorder(root);
    printf("\n");
    Postorder(root);
    printf("\n");
    LevelOrder(root);

    FreeTree(root);
    return 0;
}
