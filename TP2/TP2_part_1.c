#include <stdio.h>
#include <stdlib.h>


typedef unsigned int uint;

typedef struct node_t {
    int key;
    struct node_t *left, *right;
} node;



// TP2.1
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


void printTreePre(node *root) {
    printf("{%d, ", root->key);
    if (root->left != NULL) 
        printf("%d, ", root->left->key);
    else
        printf("NULL, ");
    
    if (root->right != NULL)
        printf("%d}\n", root->right->key);
    else
        printf("NULL}\n");
    
    if (root->left != NULL) 
        printTreePre(root->left);
    
    if (root->right != NULL)
        printTreePre(root->right);
}

void printTreePost(node *root) {
    if (root->left != NULL) 
        printTreePost(root->left);
    
    if (root->right != NULL)
        printTreePost(root->right);

    printf("{%d, ", root->key);
    if (root->left != NULL) 
        printf("%d, ", root->left->key);
    else
        printf("NULL, ");
    
    if (root->right != NULL)
        printf("%d}\n", root->right->key);
    else
        printf("NULL}\n");
    
    
}

int getCode(int key, node* root, char code[], uint* n) {
    if (root->key == key) {
        return 1;
    }
    
    if (root->left != NULL) {
        code[*n] = '0';
        (*n)++;
        if (getCode(key, root->left, code, n))
            return 1;
        (*n)--;
    }

    if (root->right != NULL) {
        code[*n] = '1';
        (*n)++;
        if (getCode(key, root->right, code, n))
            return 1;
        (*n)--;
    }
    return 0;
}

int main() {
    // testing was accidentally deleted
}