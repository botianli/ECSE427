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

        //Reading it out 

        // printf("%s","At reader\n");

        // Reading is finished
        
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

        //   printf("%s","At writer \n");
        
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


    pthread_t t1, t2, t3, t4, t5;
  
    int s;
    int loops = 100000;

  if (sem_init(&rw_mutex, 0, 1) == -1) {
    printf("Error, init semaphore\n");
    exit(1);
  }
  if (sem_init(&mutex, 0, 1) == -1) {
    printf("Error, init semaphore\n");
    exit(1);
  }

  s = pthread_create(&t1, NULL, reader, &loops);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }

  s = pthread_create(&t2, NULL, reader, &loops);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }

  s = pthread_create(&t3, NULL, writer, &loops);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }

  s = pthread_create(&t4, NULL, writer, &loops);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }

   s = pthread_create(&t5, NULL, writer, &loops);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }

  s = pthread_join(t1, NULL);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }

  s = pthread_join(t2, NULL);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }

  s = pthread_join(t3, NULL);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }

  s = pthread_join(t4, NULL);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }
  s = pthread_join(t5, NULL);
  if (s != 0) {
    printf("Error, creating threads\n");
    exit(1);
  }

  printf("glob value %d \n", glob);
  exit(0);

}
