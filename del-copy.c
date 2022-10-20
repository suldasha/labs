#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

void remove_copy(Node *root) {
    if (root == NULL)
        return;

    while (root->next != NULL)
        if (root->val == root->next->val) {
            Node *tmp = root->next;
            root->next = root->next->next;
            free(tmp);
        } else {
            root = root->next;
        }
}

int main() {
    int n;
    Node *root, *x;

    x = root = malloc(sizeof(Node));

    printf("n: ");
    scanf("%d", &n);

    if (n <= 0)
        return 0;

    printf("list: ");
    for (int i = 0; i < n; ++i) {
        x->next = malloc(sizeof(Node));
        x = x->next;
        scanf("%d", &x->val);
    }
    x->next = NULL;
    x = root;
    root = x->next;
    free(x);

    remove_copy(root);

    printf("new list:");
    for (x = root; x != NULL; x = x->next)
        printf(" %d", x->val);

    while (root != NULL) {
        x = root;
        root = x->next;
        free(x);
    }

    return 0;
}
