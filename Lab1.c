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






int get_a_line();
int my_system();
int history (char *list[], int currChar);
int chDirect(char * args []);

char * historyList[100];
int lastChar = 0;
int recentPID = 0;


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
     
     for (int i=0; i<100; i++){
         historyList[i]=NULL;
     }

    while(1){
        char *line = NULL;
        size_t n; 

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
    memset (args,'\0',128);
    
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
    lastChar=(lastChar+1);
    printf("%s%s","This is ", args[0]);

    int fd;
    

    char *fifoPath = "/tmp/home/richard.mansdoerfer/Desktop/Lab1.c";
    mkfifo(fifoPath, 0777);


    //int fd[2];
    //pipe(fd);
    
    fflush(stdout);

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
             printf("%s","Input Error");
        }

        
        else {
             printf("%s","Command does not exist \n"); 
        }
        
        fflush(stdout); 

        close(fd);

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
            //pid_t pid2=fork();
           // if (pid2==0){
                my_system(pipeArgs[0]);
            //}
            //else {
              //  waitpid(pid2, &status , 0);
            //}
        
        }
    
               
    }

    return 0;
}

int history ( char *list[], int currChar){

    int i=currChar+1;

    //printf("%s %d %d", "This is ", i, currChar);

    
    int records=1;

    //printf("%s",list[1]);

   while(i != currChar){
        if ((list[i])){ 
           printf("%s %d %s \n", "Record Number : ", records, list[i]); 
           records++;
        }
        printf("%d", i);
        i=(i+1)%100;
    }
    
    printf("%s", "Done");
 
    return 1;
}

int chDirect(char *test []) { 


    if (test[1]==NULL){
        if(chdir("..")==0){
            printf("%s\n", "Directory is ..");
        }
    }

    if(chdir(test[1])==0){
        printf("%s%s\n", "Directory is ", test[1]);
    }
    else {
        printf("%s%s\n", "Error the following directory does not exist: ", test[1]);
    }
    
    // /tmp/home/richard.mansdoerfer/Desktop/ECSE 427
    
    return 0;
}

