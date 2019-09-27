#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// MEMES
// git add Lab1.c
// git commit -m "thing"
// git push 

char* get_a_line();


void main() {

    char *line = NULL;
    size_t n = 128; 
    int i = 0;

    while(1){
        printf("%s","Input: ");
        if(getline(&line, &n, stdin)==-1){
            perror("Error");
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

//char* get_a_line(char** pointerL){
 //   fgets(line, 128, stdin);
  //  return line;
//}

void my_system(){

}
