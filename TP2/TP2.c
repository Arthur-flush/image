#include <stdio.h>
#include <stdlib.h>


typedef unsigned int uint;

typedef struct node_t {
    int key;
    struct node_t *left, *right;
} node;


node* create_node(int key, node* left, node* right) {
    node *n = malloc(sizeof(node));
    n->key = key;
    n->left = left;
    n->right = right;
    return n;
}

void free_tree(node* root) {
    if (root->left != NULL) 
        free_tree(root->left);

    if (root->right != NULL) 
        free_tree(root->right);
    
    free(root);
}

void _printTree(node* root, int tab) {
    if (root == NULL) {
        
        return;
    }

    for (int i = 0; i < tab; i++) 
        printf("|  ");
    
    
    printf("%d\n", root->key);

    _printTree(root->left, tab+1);
    _printTree(root->right, tab+1);
}

void printTree(node* root) {
    _printTree(root, 0);
}



uint nbLeafs(node* root) {
    if ((root->left == NULL) && (root->right) == NULL)
        return 1;

    int sum = 0;
    if (root->left != NULL)
        sum += nbLeafs(root->left);
    if (root->right != NULL)
        sum += nbLeafs(root->right);

    return sum;
}

uint _depth(node *root, uint d) {
    uint new_d = d;
    uint d1 = 0;
    uint d2 = 0;

    if (root->right != NULL)
        d1 = _depth(root->right, d+1) ;
    if (root->left != NULL)
        d2 = _depth(root->left, d+1);


    new_d = d1 > new_d ? d1 : new_d;
    new_d = d2 > new_d ? d2 : new_d;

    return new_d;
}

uint depth(node *root) {
    return _depth(root, 0);
}

char* _getCode(int key, node* root, int* size) {
    if (root->key == key) {
        char* c = malloc(0);
        return c;
    }

    if (root->left != NULL) {
        char* c = _getCode(key, root->left, size);
        if (c != NULL) {
            (*size) += 1;
            c = realloc(c, sizeof(char) * *size);
            c[(*size)-1] = '0';
            return c;
        }
    }

    if (root->right != NULL) {
        char* c = _getCode(key, root->right, size);
        if (c != NULL) {
            (*size) += 1;
            c = realloc(c, sizeof(char) * *size);
            c[(*size)-1] = '1';
            return c;
        }
    }

    return NULL;


}

int getCode(int key, node* root, char **code, int* n) {
    int size;
    char *c = _getCode(key, root, &size);
    c = realloc(c, sizeof(char) * size + 1);
    c[size] = 0;
    *code = malloc(sizeof(char) * size + 1);
    //fucked
    *n = size;
    if (n > 0) 
        return 1;
    else
        return 0;
}


int main() {
    node* tree = create_node(0, 
        create_node(1, 
            create_node(2, 
                create_node(3, 
                    NULL, 
                    NULL), 
                NULL), 
            create_node(4, 
                NULL, 
                NULL)), 
        create_node(5, 
            create_node(6, 
                NULL, 
                NULL), 
            NULL));


    printTree(tree);

    printf("nbLeafs: %u\n", nbLeafs(tree));
    printf("depth: %d\n", depth(tree));

    int node_n = 3;
    int size = 0;
    char* code;
    
    if (getCode(node_n, tree, &code, &size)) {
        printf("found node %d\n", node_n);
        printf("code: %s\n", code);
    }
    else
        printf("didnt find node %d\n", node_n);
    free_tree(tree);
}