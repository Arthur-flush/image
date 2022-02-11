#include <stdio.h>
#include <stdlib.h>


typedef unsigned int uint;

typedef enum { false, true } bool;

typedef struct node_t {
    int key;
    struct node_t *left, *right;
} node;

typedef struct couple_t {
    uint occ;
    char ch;
} couple;

typedef struct nodeH_t { // for huffman coding
    couple key;
    struct nodeH_t *left, *right;
} nodeH;

typedef struct code_t {
    char c; //char
    char* code; // code word
    uint n; // lenght of code word
} code;


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

void _printTree(node* root, int tab) { // custom tree printing function
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

couple* readfile(char* filename, uint* size) { // returns of shape (size, 2) where size is the number of different chars in file 
    // doesnt support unicode but c is terrible with unicode
    FILE* f = fopen(filename, "r");

    char c;
    couple* couples = malloc(0); // will get reallocated with every new char
    while ((c = fgetc(f)) != EOF) {
        bool new = true;
        for (int i = 0; i < (*size); i++) {
            if (couples[i].ch == c) {
                new = false;
                couples[i].occ++;
                break;
            }
        }
        if (new) {
            couples = realloc(couples, sizeof(couple) * ((*size)+1)); 
            if (couples == NULL) {
                perror("heap error with realloc");
                exit(1);
            }
            
            couples[*size].ch = c;
            couples[*size].occ = 1;
            (*size)++;
        }
    }
    fclose(f);
    return couples;
}



void sortOccurencesList(couple *list, uint n) { //bubble sort cause it's easy to implement and optimisation is not a problem
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < n-1; i++) {
            if (list[i].occ > list[i+1].occ) {
                sorted = false;
                couple tmp = list[i];
                list[i] = list[i+1];
                list[i+1] = tmp;
            }
        }
    }
}

void sortTreeHeap(nodeH **TreeHeap, uint n, uint offset) {
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = offset; i < n-1; i++) {
            if (TreeHeap[i]->key.occ > TreeHeap[i+1]->key.occ) {
                sorted = false;
                nodeH* tmp = TreeHeap[i];
                TreeHeap[i] = TreeHeap[i+1];
                TreeHeap[i+1] = tmp;
            }
        }
    }
}

void printTreePreHuffman(nodeH *root) {
    printf("{(%c, %d), ", root->key.ch, root->key.occ);
    if (root->left != NULL) 
        printf("(%c, %d), ", root->left->key.ch, root->left->key.occ);
    else
        printf("NULL, ");
    
    if (root->right != NULL)
        printf("(%c, %d)}\n", root->right->key.ch, root->right->key.occ);
    else
        printf("NULL}\n");
    
    if (root->left != NULL) 
        printTreePreHuffman(root->left);
    
    if (root->right != NULL)
        printTreePreHuffman(root->right);
}

nodeH* create_nodeHuffman(couple key, nodeH* left, nodeH* right) {
    nodeH *n = malloc(sizeof(nodeH));
    n->key = key;
    n->left = left;
    n->right = right;
    return n;
}

int getCodeHuffman(couple key, nodeH* root, char code[], uint* n) {
    if ((root->key.ch == key.ch) && (root->key.occ == key.occ)) { // do we care about both ch AND occ or just ch ? here we care about both
        return 1;
    }
    
    if (root->left != NULL) {
        code[*n] = '0';
        (*n)++;
        if (getCodeHuffman(key, root->left, code, n))
            return 1;
        (*n)--;
    }

    if (root->right != NULL) {
        code[*n] = '1';
        (*n)++;
        if (getCodeHuffman(key, root->right, code, n))
            return 1;
        (*n)--;
    }
    return 0;
}

nodeH* buildHuffmanTree(couple *list, uint n) {
    nodeH** TreeHeap = malloc(sizeof(nodeH*) * n);
    for (int i = 0; i < n; i++)
        TreeHeap[i] = create_nodeHuffman(list[i], NULL, NULL);
    

    uint at = 0;
    while (at < n-1) {
        nodeH* last1 = TreeHeap[at];
        nodeH* last2 = TreeHeap[at+1];

        couple key;
        key.ch = '\0';
        key.occ = last1->key.occ + last2->key.occ;
        
        nodeH* NewNode = create_nodeHuffman(key, last1, last2);

        at++;
        TreeHeap[at] = NewNode;
        sortTreeHeap(TreeHeap, n, at);
    }

    return TreeHeap[n-1];
}


void free_treeHuffman(nodeH* root) {
    if (root->left != NULL) 
        free_treeHuffman(root->left);

    if (root->right != NULL) 
        free_treeHuffman(root->right);
    
    free(root);
}

bool str_larger_than_binary(char* str1, char* str2, int n1, int n2) { // compares str1 > str2. n1 and n2 are the size of their respective string. needs str as string of binary numbers
    bool largest_string = n1 > n2 ? true : false; // assumes no leading zeros
    int smallest_n = n1 > n2 ? n1 : n2;

    for (int i = 0; i < smallest_n; i++) {
        if (str1[i] > str2[i]) {
            return true;
        }
        if (str2[i] > str1[i]) {
            return false;
        }
    }
    return largest_string;
}

code* builCodeTable(couple *list, uint n, nodeH* root) {
    code* CodeTable = malloc(sizeof(code) * n);
    for (int i = 0; i < n; i++) {
        CodeTable[i].c = list[i].ch;
        
        uint n = 0;
        char c[64];

        if (!getCodeHuffman(list[i], root, c, &n)) {
            perror("Cannot build the code table of a code that's not in the tree");
            exit(1);
        }

        char* C = malloc(n+1);
        for (int i = 0; i < n; i++)
            C[i] = c[i];
        C[n] = '\0';
        CodeTable[i].code = C;
        CodeTable[i].n = n;
    }

    //sort by code
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < n-1; i++) {
            if (str_larger_than_binary(CodeTable[i].code, CodeTable[i+1].code, CodeTable[i].n, CodeTable[i+1].n)) {
                sorted = false;
                code tmp = CodeTable[i];
                CodeTable[i] = CodeTable[i+1];
                CodeTable[i+1] = tmp;
            }
        }
    }

    return CodeTable;
}

char* compressString(char* str, code* codeList, uint n) {
    char *ret = malloc(0); //dynamically sized
    uint ret_size = 0;

    char c;
    for (int i = 0; (c = str[i]) != '\0'; i++) {
        int j;
        for (j = 0; (j < n) && (codeList[j].c != c); j++);
        uint size = codeList[j].n;
        ret_size += size;
        ret = realloc(ret, ret_size);
        for (uint k = 0; k < size; k++)
            ret[(ret_size - size) + k] = codeList[j].code[k];
    }

    ret = realloc(ret, ret_size+1);
    ret[ret_size] = '\0';
    return ret;
}

uint stringlen(char* str) {
    uint i;
    for (i = 0; str[i] != '\0'; i++);
    return i;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        perror("wrong argument number, should be 2");
        exit(1);
    }

    FILE* file = fopen(argv[1], "r");
    char* data = malloc(4096); // should be enough for testing and works for both text files 
    fseek(file, 0, SEEK_END); 
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);  
    if (!fread(data, 1, fsize, file)) {
        perror("invalid file");
        exit(1);
    }
    fclose(file);

    printf("Message a compresser :\n\n%s\n\n", data);

    uint size = 0;
    couple* couples = readfile(argv[1], &size);
    sortOccurencesList(couples, size);

    

    nodeH* tree = buildHuffmanTree(couples, size);

    code* CodeTable = builCodeTable(couples, size, tree);


    printf("Table des symboles :\n");
    for (int i = 0; i < size; i++) {
        printf("'%c' : ", CodeTable[i].c);
        for (int j = 0; j < CodeTable[i].n; j++) {
            putchar(CodeTable[i].code[j]);
        }
        putchar('\n');
    }
    putchar('\n');

    char* encoded_data = compressString(data, CodeTable, size);
    printf("Message compresse : \n\n%s\n", encoded_data);

    uint l1 = stringlen(data);
    uint l2 = stringlen(encoded_data);
    putchar('\n');
    printf("Longueur de la compression : %d bits\n", l2);
    printf("Longueur du message initial : %dx7 = %d bits\n\n", l1, l1*7);

    printf("Taux de compression : %.3f\n", (float)l2 / (float)(l1 * 7));

    free_treeHuffman(tree);
    free(couples);
    free(data);
    free(encoded_data);
}