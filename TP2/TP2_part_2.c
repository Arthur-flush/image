#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

typedef enum { false, true } bool;

typedef struct couple_t {
    uint occ;
    char ch;
} couple;

typedef struct node_t {
    couple key;
    struct node_t *left, *right;
} node;

typedef struct code_t {
    char c; //char
    char* code; // code word
    uint n; // lenght of code word
} code;

//TP2.2
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

void sortTreeHeap(node **TreeHeap, uint n, uint offset) {
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = offset; i < n-1; i++) {
            if (TreeHeap[i]->key.occ > TreeHeap[i+1]->key.occ) {
                sorted = false;
                node* tmp = TreeHeap[i];
                TreeHeap[i] = TreeHeap[i+1];
                TreeHeap[i+1] = tmp;
            }
        }
    }
}

void printTreePre(node *root) {
    printf("{(%c, %u), ", root->key.ch, root->key.occ);
    if (root->left != NULL) 
        printf("(%c, %u), ", root->left->key.ch, root->left->key.occ);
    else
        printf("NULL, ");
    
    if (root->right != NULL)
        printf("(%c, %u)}\n", root->right->key.ch, root->right->key.occ);
    else
        printf("NULL}\n");
    
    if (root->left != NULL) 
        printTreePre(root->left);
    
    if (root->right != NULL)
        printTreePre(root->right);
}

node* create_node(couple key, node* left, node* right) {
    node *n = malloc(sizeof(node));
    n->key = key;
    n->left = left;
    n->right = right;
    return n;
}

int getCode(couple key, node* root, char code[], uint* n) {
    if ((root->key.ch == key.ch) && (root->key.occ == key.occ)) { // do we care about both ch AND occ or just ch ? here we care about both
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

void printTree(node *node, int h)
{
    if(node->right != NULL)
        printTree(node->right, h+1);
    
    int i;
    for(i = 0; i < h; i++)
    {
        printf("\t");
    }
    
    if(node->key.ch != '\n')
        printf("%c:%d\n", node->key.ch, node->key.occ);
    else
        printf("\\n:%d\n", node->key.occ);
    
    if(node->left != NULL)
        printTree(node->left, h+1);
}

node* buildHuffmanTree(couple *list, uint n) {
    node** TreeHeap = malloc(sizeof(node*) * n);
    for (int i = 0; i < n; i++)
        TreeHeap[i] = create_node(list[i], NULL, NULL);
    

    uint at = 0;
    while (at < n-1) {
        node* last1 = TreeHeap[at];
        node* last2 = TreeHeap[at+1];

        couple key;
        key.ch = '\0';
        key.occ = last1->key.occ + last2->key.occ;
        
        node* NewNode = create_node(key, last1, last2);

        at++;
        TreeHeap[at] = NewNode;
        sortTreeHeap(TreeHeap, n, at);
    }
    node* huffmanTree = TreeHeap[n-1];
    free(TreeHeap);
    return huffmanTree;
}


void free_tree(node* root) {
    if (root->left != NULL) 
        free_tree(root->left);

    if (root->right != NULL) 
        free_tree(root->right);
    
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

code* builCodeTable(couple *list, uint n, node* root) {
    code* CodeTable = malloc(sizeof(code) * n);
    for (int i = 0; i < n; i++) {
        CodeTable[i].c = list[i].ch;
        
        uint n = 0;
        char c[64];

        if (!getCode(list[i], root, c, &n)) {
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

    

    node* tree = buildHuffmanTree(couples, size);

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
    printf("Longueur de la compression : %u bits\n", l2);
    printf("Longueur du message initial : %ux7 = %u bits\n\n", l1, l1*7u);

    printf("Taux de compression : %.3f\n", (float)l2 / (float)(l1 * 7));

    free_tree(tree);
    free(couples);
    free(data);
    free(encoded_data);
    for (int i = 0; i < size; i++)
        free(CodeTable[i].code);
    free(CodeTable);
}