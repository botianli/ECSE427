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
void history();
char * historyList[100];
int lastChar = 0;
char *tempChar;

void main() {

    while(1){

        char *line = NULL;
        size_t n = 128; 
        int i = 0;

        if(get_a_line(&line, &n, stdin)==-1){
            perror("Error Does not end");
            exit(0);
        }
        if (strlen(line)>sizeof(char)){
            my_system(line);
        }
        else {
            printf("HELLO");
            history();
            exit(0);
        }
    }
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

    pid_t pid=fork(); 

    tempChar = historyList[lastChar%100];
    historyList[lastChar%100]=args[0];
    lastChar=lastChar+1;

    if (pid==-1){
        perror("Error with Fork");
        exit(1);
    }
    
    if (pid==0){
        if (strcmp(args[0],"history")==0){
            history();
        }
        else{

        if(execvp(args[0], &args[0])!=-1){
        }
        else {
            lastChar=lastChar-1;
            historyList[lastChar%100]=tempChar;
        }
        }
        
    }

    else {
        waitpid(pid, &status, 0);
    }

    return 1;
}

void history (){
    printf("\n\n%s", "History of the System Has : ");
    int temp = lastChar;
    printf("%d %s", temp, "entries");
    temp=temp%100;
    while(temp<100){
        if (historyList[temp]!=NULL){
            printf("\n%s", historyList[temp]);
        }
        temp++;
    }
    temp=0;
    while (temp<(lastChar%100)){
        if (historyList[temp]!=NULL){
            printf("\n%s", historyList[temp]);
        }
        temp++;
    }
}
