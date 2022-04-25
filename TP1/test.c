#include <stdio.h>
#include <stdlib.h> 

struct couple_t {
    char c; 
    unsigned int occ ; 
} ; typedef struct couple_t couple ; 


unsigned int stringLenght(char *string) {
    unsigned int i = 0 ; 
    while (string[i] != '\0')
        i ++ ;     
    return i ; 
} 

unsigned int charOccurenceInString(char c, char*string){
    int i ; 
    unsigned int occur = 0 ; 
    for (i=0 ; i < stringLenght(string) ; i++) {
        if (string[i] == c) {
            occur ++ ; 
        }
    }
    return occur ; 
} 

char *getStringFromFile(char *filename) {
    FILE* f = fopen(filename, "r");
    fseek(f, 0, SEEK_END); 
    long size = ftell(f);
    fseek(f, 0, SEEK_SET); 
    char* fcontent = malloc(size);
    fread(fcontent, 1, size, f);
    //fcontent[size] = '\0';
    fclose(f);
    return fcontent;
} 

unsigned int nbDifferentChar(char *string) { 
    unsigned int nbdiff = 0 ; 
    int i , j  ; 
    unsigned int lentmp = stringLenght(string) ;  
    char tmp[256] = {0}; 
    for (i=0 ; i< lentmp ; i++)  {
        unsigned int test = 0 ; 
        char val = string[i] ; 
        for(j=0 ; (j<256) && (test==0); j++) {
            if(tmp[j] == val) {
                test = 1 ;
            }
        }
        if(test == 0) {
            tmp[nbdiff] = val ; 
            nbdiff ++ ;
            }
    }
    tmp[nbdiff] = '\0' ; 
    i = 0;
    while(i < 256 && tmp[i] != '\0') {
          printf("%c", tmp[i]);
          i++;
    } 
    return nbdiff ; 
} 
        

int main (){
    char *fichier = getStringFromFile("elizabeth.txt") ; 
    unsigned int bendo = nbDifferentChar(fichier) ; 
    printf("\nnb diff char = %u\n", bendo) ; 
    free(fichier) ; 
    return 0 ; 
} 