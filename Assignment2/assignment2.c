#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


void *reader(void * ) ;         // Initiates reader and writer methods
void *writer (void * ) ; 

int readCount=0;                // Initiates readCount (reader) and global count 
static int glob = 0;  


static sem_t rw_mutex,mutex ;   // Semaphore values 

static long maxWriting = 0;
static long minWriting = 100000000;
static long maxReading = 0;
static long minReading = 100000000;
static long avgReading = 0; 
static long avgWriting = 0; 
static int avgReadingCount = 0;
static int avgWritingCount = 0; 

void * reader(void * arg) {

    int readerIter = 60;

    while (readerIter>0){

        clock_t timerStartR = clock();

        sem_wait(&mutex);

        readCount++;

        if(readCount ==1 ){
            sem_wait(&rw_mutex);
        }

        sem_post(&mutex);

        clock_t fullTimeR = clock()-timerStartR;
        long microsecR = fullTimeR*1000000/CLOCKS_PER_SEC;
        if (microsecR>maxReading){
            maxReading=microsecR;
        }
        if (microsecR<minReading){
            minReading=microsecR;
        }
        avgReading=avgReading+microsecR;
        avgReadingCount++;


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
    
        clock_t timerStartW = clock();
        sem_wait(&rw_mutex);


        int random = (rand() %100 + 1)*1000;
        usleep(random);
        
        loc = glob;
        loc = loc+10;
        glob = loc;

        sem_post(&rw_mutex);
        
        clock_t fullTimeW = clock()-timerStartW;
        long microsecW = fullTimeW*1000000/CLOCKS_PER_SEC;
        if (microsecW>maxWriting){
            maxWriting=microsecW;
        }
        if (microsecW<minWriting){
            minWriting=microsecW;
        }
        avgWriting=avgWriting+microsecW;
        avgWritingCount++;


        writerIter--; 

    }
}

int main()  {    
  
    int readCount=0;
    int writeCount=0;


    int loops = 100000;
    int rThreadCount=500;
    int wThreadCount=5;
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
    
  printf("Global value :  %d \n\n", glob);

  printf("Reading minimum in microseconds : %ld \n", minReading);
  printf("Reading maximum in microseconds : %ld \n", maxReading);
  printf("Reading average in microseconds : %ld \n", avgReading/avgReadingCount);
  printf("Reading count is : %d \n\n", avgReadingCount);



  printf("Writing minimum in microseconds : %ld \n", minWriting);
  printf("Writing maximum in microseconds : %ld \n", maxWriting);
  printf("Writing average in microseconds : %ld \n", avgWriting/avgWritingCount);
  printf("Writing count is : %d \n\n", avgWritingCount);

  exit(0);

}
