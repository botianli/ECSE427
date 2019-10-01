#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> //for fork
#include <unistd.h> //for fork and vfork
#include <sys/wait.h> //for waitpid
#include <time.h> //for timing
#include <sys/stat.h>
#include <fcntl.h>


char * historyList[100];
int lastChar = 0;
int recentPID = 0;


int get_a_line();
int my_system();
int history (char *list[], int currChar);
int chDirect(char * args []);


void zHandler (int check){
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    
    signal(SIGTSTP, zHandler);

    printf(" : This system ignores that command! \n");
    fflush(stdout);
}


void intHandler2(int sigCheck){

    char c; 

    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    printf("Do you want to quit the program right now? Please input : [y/n] ");
    
    
    c =getchar();

    if (c=='y' || c == 'Y'){
        exit(0);
    }
    else {
        signal(SIGINT, intHandler2);
    }
    getchar();
}



void main() {
     
    fflush(stdout);
    fflush(stdin);

    
    

    while(1){
        char *line = NULL;
        size_t n; 
        fflush(stdout);

       //if(get_a_line(&line, &n, stdin)==-1){
        if (getline(&line, &n, stdin)==-1){
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
    return 1;
}

int my_system(char * lineArg){

    signal(SIGINT, intHandler2);
    signal(SIGTSTP, zHandler);

   // printf("%s%s", "At My system : ", lineArg);

    char * args [128];
    
    char * pipeArgs [128];

    char* token ;

    char * nextTok; 
    
    int tokenIndex=0;

    int pipedIndex=0;

    int status; 

    int pipedArg=0;

    char  buffer [256];
    memset (buffer,'\0',256);


    token=strtok(lineArg," \n\t");

    while(token !=NULL){
       
        if (pipedArg==1){
            pipeArgs[pipedIndex++]=token;
            //printf("%s", pipeArgs[pipedIndex-1]);
            token = strtok(NULL," \n\t");
            //token = strtok(NULL," \n\t");
            
        }
        else {
            args[tokenIndex++] = token;
            //printf("%s", args[tokenIndex-1]);
            if ((strcmp(args[tokenIndex-1],"|"))==0 ){
                   pipedArg=1;
            }
            token = strtok(NULL," \n\t");
        }
    }


    if (pipedArg==1){
        args[tokenIndex-1]='\0';
    }
    else {
        args[tokenIndex]='\0';
    }
    pipeArgs[pipedIndex]='\0';
    

    historyList[lastChar]=args[0];
    lastChar=(lastChar+1)%100;

    int fd;
    char answerArray [256];
    memset (answerArray,'\0',256);

    char *fifoPath = "/tmp/home/richard.mansdoerfer/Desktop/Lab1.c";
    mkfifo(fifoPath, 0777);


    //int fd[2];
    //pipe(fd);
    
    
    pid_t pid =fork();


    if (pid==-1){
        perror("Error with Fork");
        exit(1);
    }
    
    else if (pid==0){

       // close(fd[0]); -- Commands for piping procedure 
       // dup2(fd[1], STDOUT_FILENO);
        
        fd=open(fifoPath, O_WRONLY);
        
        dup2(fd, STDOUT_FILENO);

        if (strcmp(args[0],"history")==0){
                history(historyList, lastChar);
        }

        else if (strcmp(args[0],"chdir")==0){
                chDirect(args);
        }    
        
        else if(execvp(args[0], &args[0])!=-1){
             fprintf(stdout, "Input Error");
        }

        
        else {
             fprintf(stdout, "Command does not exist \n"); 
        }
        //fgets(answerArray,256, stdout);
        //write(fd,answerArray,strlen(answerArray));
       
        close(fd);

        fflush(stdout); 

        recentPID=getpid(); 
        
        kill(recentPID, SIGKILL);
          

    }

    else {

        // close(fd[1]);   -- Commands for piping procedure
        // read(fd[0], buffer, 256);

        fd=open(fifoPath,O_RDONLY);
        read(fd,buffer,256);

        printf("%s", buffer);

        close(fd);

        if (pipedArg==1){
            pid_t pid2=fork();
            if (pid2==0){
                my_system(pipeArgs[0]);
            }
            else {
                waitpid(pid2, &status , 0);
            }
        
        }
               
    }

    return 0;
}

int chDirect(char *test []) { 
    
    char direction [256];
    
    //while (test[i]!=0){
    //    printf("%s", test[i]);
        //direction[i-1]=&test[i];
        //i++;
   // }
    
    //char *newArr = strcat(buf, "chdir ");
    //fix = strcat(newArr, test[1]);
    

    if(chdir(test[1])==0){
        printf("%s%s\n", "Directory is ", test[1]);
    }
    else {
        printf("%s%s\n", "Error the following directory does not exist: ", test[1]);
        lastChar=lastChar-1;
        
    }
    
    // /tmp/home/richard.mansdoerfer/Desktop/ECSE 427
    
    return 0;
}

int history ( char *list[], int currChar){
    printf("%d %s", currChar, " current value" );

    int i=currChar+1;
    int records=1;

    while(i != currChar){
        if (list[i]){
            printf("%s %d %s \n", "Record Number : ", records, list[i]);
            records++;
        }
        i=(i+1)%100;
    }
    
    return 1;
}
