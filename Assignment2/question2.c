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

static sem_t resoureAccess, readCountAccess, serviceQueue ;   // Semaphore values 

static long maxWriting = 0;         // Initial value for writer threads, includes statistic counts and writer thread counts 
static long minWriting = 100000000;
static long avgWriting = 0;
static int avgWritingCount = 0; 
static int writeInput;

static long maxReading = 0;         // Initial value for reader threads, includes statistic counts and reader thread counts 
static long minReading = 100000000;
static long avgReading = 0; 
static int avgReadingCount = 0;
static int readInput;

void * reader(void * arg) {         // Reader thread process 

    int readerIter = readInput;     // Takes inputted reader iteration count from command argument 

    while (readerIter>0){           // While loop through reader interation count 

        sem_wait(&serviceQueue);

        clock_t timerStartR = clock();      // Start clock for reader thread 

        sem_wait(&readCountAccess);                   // Lock shared file for reader  

        readCount++;                        

        if(readCount ==1 ){                 // Lock shared file for reader+writer if count is 1 
            sem_wait(&resoureAccess);
        }

        sem_post(&serviceQueue);                   // Unlock shared file for writer 
        sem_post(&readCountAccess);                   // Unlock shared file for writer 

        clock_t fullTimeR = clock()-timerStartR;        // End clock, calculate difference 
        long microsecR = fullTimeR*1000000/CLOCKS_PER_SEC;  //Calculate process time in microseconds
       
        if (microsecR>maxReading){          // Change maxReading if valid 
            maxReading=microsecR;
        }
       
        if (microsecR<minReading){          // Change minReading if valid 
            minReading=microsecR;
        }
       
        avgReading=avgReading+microsecR;    // Update total reading time 
        avgReadingCount++;                  // Update total reads 

        int random = (rand() %100 + 1)*1000; // Random sleep time between 1-100 milliseconds
        usleep(random);             
        
        sem_wait(&readCountAccess);                   // Lock shared file for reader

        readCount--;
        if (readCount == 0){                // Unlock shared file for reader/writer if count is 0 
            sem_post(&resoureAccess);
        }

        sem_post(&readCountAccess);                   // Unlock shared file for reader 

        readerIter--;
    }
}

void *writer (void * arg) {         // Writer thread process 

    int writerIter=writeInput;       // Takes inputted writer iteration count from command argument 
    int loc ;

    while (writerIter>0){
        
        sem_wait(&serviceQueue);

        clock_t timerStartW = clock();  // Take initial clock value before writer thread 
        sem_wait(&resoureAccess);            // Lock rw_mutex shared file 

        sem_post(&serviceQueue);

        int random = (rand() %100 + 1)*1000;    // Random sleep value 
        usleep(random);
        
        loc = glob;                     // Increment global value through additional variable 
        loc = loc+10;
        glob = loc;

        sem_post(&resoureAccess);            // Unlock rw_mutex shared file 
        
        clock_t fullTimeW = clock()-timerStartW; // Ends clock, calculates difference between start and end 
        long microsecW = fullTimeW*1000000/CLOCKS_PER_SEC; // Adjusts clock time for microsec 
        
        if (microsecW>maxWriting){      // Updates writer max clock time if necessary 
            maxWriting=microsecW;
        }
        
        if (microsecW<minWriting){      // Updates writer min clock time if necessary 
            minWriting=microsecW;
        }
        
        avgWriting=avgWriting+microsecW;    // Update total write time 
        avgWritingCount++;                  // Update total writes 

        writerIter--; 

    }
}

int main(int argCount, char * argv[])  {    // Takes command arguments, creates thread processes, displays thread statistics 

    if (argCount<3){                            // Checks if too few inputs are given 
        printf("\nNot enough arguments\n\n");
        exit(1);
    }
  
    if (argCount>3){                            // Checks if too many inputs are given
        printf("\nToo many arguments\n\n");
        exit(1);
    }

    if (sscanf (argv[1], "%i", &writeInput) != 1) {     // Checks if writer input is valid int, assigns int to writeInput
        fprintf(stderr, "\n Error - Write Input is not an integer\n\n");
        exit(1);
    }

    if (sscanf (argv[2], "%i", &readInput) != 1) {      // Checks if reader input is valid int, assigns int to readerInput
        fprintf(stderr, "\n Error - Read Input is not an integer\n\n");
        exit(1);
    }

    int nullInput = 0;
    int rThreadCount=500;       // Number of reader threads
    int wThreadCount=10;        // Number of writer threads
    int i;                      // Variable to increment threads 

    pthread_t rThreads[rThreadCount+1], wThreads[wThreadCount+1]; // Creates p_thread of writer and reader counts 

    if (sem_init(&serviceQueue, 0, 1) == -1) {  // Initializes service queue semaphore
        printf("Error, init semaphore\n");
        exit(1);
    }
    if (sem_init(&resoureAccess, 0, 1) == -1) {     // Initializes resource access semaphore
        printf("Error, init semaphore\n");
        exit(1);
    }
    if (sem_init(&readCountAccess, 0, 1) == -1) {     // Initializes resource access semaphore
        printf("Error, init semaphore\n");
        exit(1);
    }

    for (i=0; i<rThreadCount; i++){         // Creates reader threads 
        if(pthread_create(&rThreads[i], NULL, reader, &nullInput)){
              printf("Error, creating threads\n");
              exit(1);
        }
        
    }

    for (i=0; i<wThreadCount; i++){         // Creates writer threads 
        if(pthread_create(&wThreads[i], NULL, writer, &nullInput)){
              printf("Error, creating threads\n");
              exit(1);
        }
    }

    for (i=0; i<rThreadCount; i++){         // Joins reader threads 
        if(pthread_join(rThreads[i], NULL)){
              printf("Error, joining threads\n");
              exit(1);
        }
    }

    for (i=0; i<wThreadCount; i++){         // Joins writer threads 
        if(pthread_join(wThreads[i], NULL)){
              printf("Error, joining threads\n");
              exit(1);
        }
    }
    
  printf("Global value :  %d \n\n", glob);  // Prints global variable 

  printf("Reading minimum in microseconds : %ld \n", minReading);       // Reading thread statistic printouts 
  printf("Reading maximum in microseconds : %ld \n", maxReading);
  printf("Reading average in microseconds : %ld \n", avgReading/avgReadingCount); // Calculates average read count from total read time / count 
  printf("Reading count is : %d \n\n", avgReadingCount);

  printf("Writing minimum in microseconds : %ld \n", minWriting);       //Writing thread statistic printouts 
  printf("Writing maximum in microseconds : %ld \n", maxWriting);
  printf("Writing average in microseconds : %ld \n", avgWriting/avgWritingCount); // Calculates average write count from total write time / count 
  printf("Writing count is : %d \n\n", avgWritingCount);

  exit(0);

}
