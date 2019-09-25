#include <stdio.h>
#include <string.h>

void main() {

    char ansString []="Hello 1";
    printf(" Middle ");
    printf(ansString);
    char getAns [] = get_a_line();
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
    char* ansString = "Hello 2"; 
    return ansString;
}

void my_system(){

}
