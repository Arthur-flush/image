#include <stdio.h>
#include <stdlib.h>
typedef unsigned int uint;

typedef struct couple_t {
    char c;
    uint occ;
} couple;

uint string_len(char *str) {
    uint count = 0;
    while(*str != '\0') {
        count++;
        str++;
    } 
    return count;
}

char* read_file(char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("File doesnt exits");
        exit(1);
    } 

    fseek(f, 0, SEEK_END); 
    long size = ftell(f);
    fseek(f, 0, SEEK_SET); 
    char* fcontent = malloc(size);
    fread(fcontent, 1, size, f);

    fclose(f);

    return fcontent;
}

uint charOccurenceInString(char c, char* str) {
    uint count = 0;
    while(*str != '\0') {
        if (*str == c)
            count++;
        str++;
    }
    return count;
}

uint nbDifferentChar(char *str) { //assuming ascii
    uint count = 0;
    char chars[256] = {0};
    while (*str != 0x00) {
        str++;
        chars[(uint)*str]++;
    }

    for (int i = 0; i < 256; i++) 
        if (chars[i] != 0)
            count++;
    
    return count;
}

void printCouple(couple c) {
    if (c.c == '\n')
        printf("(\n, %d)\n", c.occ);
    else
        printf("(%c, %d)\n", c.c, c.occ);
}

couple Couple(char c, uint n) {
    couple C;
    C.c = c;
    C.occ = n;
    return C;
}

couple* getOccurencesList(char *str, uint* return_size) {
    *return_size = nbDifferentChar(str);
    couple *couples = malloc(sizeof(couple) * *return_size);
    uint pos = 0;
    
    for (int i = 0; i < 256; i++) {
        uint occ = charOccurenceInString((char)i, str);
        if (occ > 0) {
            couple new_couple = Couple((char)i, occ);
            
            couples[pos] = new_couple;
            pos++;
        }
    }

    return couples;
}


int main(int argc, char* argv) {
    
    if (argc != 2) {
        perror("Error: wrong argument number\nusage: main <filename>");
        exit(1);
    }
    char* data = read_file(argv[1]);
    printf("len: %u\n", string_len(data));
    printf("count(o): %u\n", charOccurenceInString('o', data));
    printf("nb of unique chars: %d\n", nbDifferentChar(data));


    uint n = 0;
    couple *couples = getOccurencesList(data, &n);
    for (int i = 0; i < n; i++) {
        printCouple(couples[i]);
    }

    free(couples);
    free(data);
}