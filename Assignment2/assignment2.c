#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

sem_t rw_mutex,mutex ; 
void * reader(void * ) ;  
void *writer (void * ) ;  

void * reader(void * arg) {

}
void *writer (void * arg) {
    
}



int main(int argc, char *argv[])  
{    
    int readCount=0;
    int writeCount=0;
}
