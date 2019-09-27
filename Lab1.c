#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// MEMES
// gcc _File_
//./a.out
// git add Lab1.c
// git commit -m "thing"
// git push 

int get_a_line();


void main() {

    char *line = NULL;
    size_t n = 128; 
    int i = 0;

    while(1){
        if(get_a_line(&line, &n, stdin)==-1){
            perror("Error Does not end");
            exit(0);
        }
        if (strlen(line)>1){
            system(line);
        }
        else {
            exit(0);
        }
    }
   // printf("%s", get_a_line());
    /*
    while (1){
        line = get_a_line();
        if (length(line)>1) {
            my_system(line);
        }
    }
    */
}

int get_a_line(char** pointerL, size_t *n, FILE *in){
    char line [128];
    fgets(line, 128, in);
    unsigned int pointerLength;
    pointerLength=strlen(line); 
    char * pointer;
    pointer=strchr(line, '\n');
    if (pointer==NULL){
        *pointer = '\0';
    }
    if ((pointerLength+1)<256){
        pointer=realloc(*pointerL, 256);
        if (pointer==NULL){
            return (-1);
        }
        *pointerL=pointer;
        *n=128;
    }
    strcpy(*pointerL, line);
    return 0;
}

void my_system(){

}
