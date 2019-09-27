#include <stdio.h>
#include <string.h>
// MEMES
// git add Lab1.c
// git commit
// git push 

char* get_a_line();


void main() {

    char *line = NULL;


    while(1){
        line = get_a_line();
        if (strlen(line)>1){
            printf("%s", line);
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

char* get_a_line(){
    fgets(line, 128, stdin);
    return line;
}

void my_system(){

}
