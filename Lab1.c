#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> //for fork
#include <unistd.h> //for fork and vfork
#include <sys/wait.h> //for waitpid
#include <time.h> //for timing

// MEMES
// gcc _File_
//./a.out
// git add Lab1.c
// git commit -m "thing"
// git push 

int get_a_line();
int my_system();

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
            my_system(line);
        }
        else {
            exit(0);
        }
    }
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

int my_system(char * line){

    pid_t pid=fork(); 
    
    char * args [128];
    
    char* token ;

    int tokenIndex=0;

    int status; 

    token=strtok(line," \n\t");
   
    while(token !=NULL){
        args[tokenIndex++] = token;
        token = strtok(NULL," \n\t");
    }
   
    args[tokenIndex]='\0';

    if (pid==-1){
        perror("Error with Fork");
        exit(1);
    }
    
    if (pid==0){
        //sleep(1);
        execvp(args[0], args);
    }

    else {
        waitpid(pid, &status, 0);
        printf("\nChildDone\n");
    }

    return 1;
}
