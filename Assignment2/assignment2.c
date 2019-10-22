#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

static sem_t rw_mutex,mutex ; 
int readCount=0; 
void *reader(void * ) ;  
void *writer (void * ) ; 
static int glob = 0;  


void * reader(void * arg) {

    int readerIter = 60;

    while (readerIter>0){


        sem_wait(&mutex);

        readCount++;

        if(readCount ==1 ){
            sem_wait(&rw_mutex);
        }

        sem_post(&mutex);

        int random = (rand() %100 + 1)*1000;
        usleep(random);
        
        sem_wait(&mutex);

        readCount--;
        if (readCount == 0){
            sem_post(&rw_mutex);
        }

        sem_post(&mutex);


        readerIter--;
    }

}
void *writer (void * arg) {

    int writerIter=30;
    int loc ;

    while (writerIter>0){
    
        sem_wait(&rw_mutex);

        int random = (rand() %100 + 1)*1000;
        usleep(random);
        
        loc = glob;
        loc = loc+10;
        glob = loc;

        sem_post(&rw_mutex);

        writerIter--; 

    }
}



int main()  {    
  
    int readCount=0;
    int writeCount=0;


    int loops = 100000;
    int rThreadCount=500;
    int wThreadCount=10;
    int i;


    pthread_t rThreads[rThreadCount+1], wThreads[wThreadCount+1];

    if (sem_init(&rw_mutex, 0, 1) == -1) {
        printf("Error, init semaphore\n");
        exit(1);
    }
    if (sem_init(&mutex, 0, 1) == -1) {
        printf("Error, init semaphore\n");
        exit(1);
    }

    for (i=0; i<rThreadCount; i++){
        if(pthread_create(&rThreads[i], NULL, reader, &loops)){
              printf("Error, creating threads\n");
              exit(1);
        }
        
    }

    for (i=0; i<wThreadCount; i++){
        if(pthread_create(&wThreads[i], NULL, writer, &loops)){
              printf("Error, creating threads\n");
              exit(1);
        }
    }

    for (i=0; i<rThreadCount; i++){
        if(pthread_join(rThreads[i], NULL)){
              printf("Error, creating threads\n");
              exit(1);
        }
    }

    for (i=0; i<wThreadCount; i++){
        if(pthread_join(wThreads[i], NULL)){
              printf("Error, creating threads\n");
              exit(1);
        }
    }
    
  printf("glob value %d \n", glob);
  exit(0);

}
