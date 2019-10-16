#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

void * reader(void *) ;  
void *writer (void *) ;  
sem_t wsem,mutex ;  
int readcount=0 ;  
int c = 0; 
main()  
{    
  printf("%s","clear");  
  sem_init(&wsem,0,1) ;  
  sem_init(&mutex,0,1) ;  
  pthread_t r,w,r1,w1 ;  
  pthread_create(&r,NULL,reader,(void *)c);  
  pthread_create(&w1,NULL,writer,(void *)c);  
  pthread_create(&r1,NULL,reader,(void *)c);  
  pthread_create(&w,NULL,writer,(void *)c);  
  pthread_join(r,NULL);  
  pthread_join(w1,NULL);  
  pthread_join(r1,NULL);  
  pthread_join(w,NULL) ;  
  printf("%s", "main terminated\n");  
}  
void * reader(void * arg)  
{  
  int d=(int)arg ;  
  printf("%s %d", "\nreader %d is created", d);  
  d=d+10;
     sleep(1);  
  sem_wait(&mutex) ;  
     readcount++;  
     if(readcount==1)  
         sem_wait(&wsem) ;  
  sem_post(&mutex) ;  
/*Critcal Section */  
  printf("%s %d", "\n\nreader %d is reading\n ",c);  
  sleep(1) ;  
  printf("%s %d", "\n\nreader %d is reading\n ",c);  
/* critical section completd */  
  sem_wait(&mutex) ;  
     readcount-- ;  
     if(readcount==0)  
         sem_post(&wsem) ;  
  sem_post(&mutex) ;  
}  
void * writer(void * arg)  
{  
  int d=(int)arg ;  
  printf("%s %d", "\nwriter %d is created",d); 
  arg=(int)arg+10; 
     sleep(1); 
  sem_wait(&wsem) ;  
  printf("%s %d", "\nwriter %d is writing\n" ,d) ;  
  sleep(1);  
  printf("%s %d", "\nwriter%d finished writing\n" ,d);  
  sem_post(&wsem) ;  
}
