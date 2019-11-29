#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "disk_emu.h"
#include "sfs_api.h"
#include "sfs_structs.h"

#define MAGIC_NUMBER 256
#define BLOCKSIZE 512 //M block size
#define MAXFILENAME 15
#define NUM_BLOCKS SUPERBLOCK_SIZE + FREELIST_SIZE + DIRECTORY_SIZE + INODE_TABLE_SIZE + BLOCKSIZE  //N blocks
#define MAX_FILES 99

#define SUPERBLOCK 0
#define SUPERBLOCK_SIZE 1
#define FREELIST 1
#define FREELIST_SIZE 1
#define DIRECTORY_LOCATION 2
#define DIRECTORY_SIZE 4
#define INODE_TABLE DIRECTORY_LOCATION + DIRECTORY_SIZE
#define INODE_TABLE_SIZE 13
#define START INODE_TABLE + INODE_TABLE_SIZE

#define FILENAME "my.sfs"
#define ALIGN(x)((x/BLOCKSIZE + 1) * BLOCKSIZE) //align x to next largest block size

int createFreeList(){    //creates free list
  unsigned int *buff = malloc(BLOCKSIZE);
  if (buff == NULL){
    return -1;
  }

  int i;
  for(i = 0; i < (BLOCKSIZE)/sizeof(unsigned int); i++){
    buff[i] = ~0;   //set all bits to 1
  }

  write_blocks(FREELIST, FREELIST_SIZE, buff);
  free(buff);
  return 0;
}

void setFree(unsigned int index){  //sets bit to free
  if(index > NUM_BLOCKS){
    fprintf(stderr, "Error, bad allocation attempt");
    return;
  }
  int byte = index / (8*sizeof(unsigned int));  //find exact location
  int bit = index % (8*sizeof(unsigned int));
  unsigned int *buff = malloc(BLOCKSIZE);

  if(buff == NULL){
    fprintf(stderr, "Error assigning free bit\n");
    return;
  }
  read_blocks(FREELIST, FREELIST_SIZE, buff);
  buff[byte] |= 1 << bit; //sets bit to 1(free)
  write_blocks(FREELIST, FREELIST_SIZE, buff);
  free(buff);
}

void setAlloc(unsigned int index){ //set index to allocated in FREELIST
  int byte = index / (8*sizeof(unsigned int));  //find byte to change
  int bit = index % (8*sizeof(unsigned int));   //find bit to change
  unsigned int *buff = malloc(BLOCKSIZE);

  if(buff == NULL){
    fprintf(stderr, "Error assigning allocated bit\n");
    return;
  }

  if(index >= BLOCKSIZE){
    fprintf(stderr, "Error, bad allocation attempt");
    return;
  }

  read_blocks(FREELIST, FREELIST_SIZE, buff);
  buff[byte] &= ~(1 << bit);    //set bit to 0 (allocated)
  write_blocks(FREELIST, FREELIST_SIZE, buff);
  free(buff);
}

int findFree(){ //find next free bit in bitmap

  unsigned int *buff = malloc(BLOCKSIZE);

  if(buff == NULL){
    fprintf(stderr, "Error finding free bit\n");
    return -1;
  }

  read_blocks(FREELIST, FREELIST_SIZE, buff);
  int i;
  for(i = 0; i < (BLOCKSIZE)/sizeof(unsigned int); i++){
    int find = ffs(buff[i]);
    if(find){ //check to see the block that is free is actually in file system
      if(find + i*8*sizeof(unsigned int) - 1 < BLOCKSIZE){
        return find + i*8*sizeof(unsigned int) - 1;       //return correct value
      }
    }
  }
  return -1;  //if invalid return -1 (failure)
}

int createSuperblock(){  //create superblock
  unsigned int *buff = malloc(BLOCKSIZE);

  if(buff == NULL){
    return -1;
  }
  //set up all important superblock data
  buff[0] = MAGIC_NUMBER;
  buff[1] = BLOCKSIZE;
  buff[2] = NUM_BLOCKS;
  buff[3] = FREELIST;
  buff[4] = DIRECTORY_LOCATION;
  buff[5] = DIRECTORY_SIZE;
  buff[6] = INODE_TABLE;
  buff[7] = INODE_TABLE_SIZE;
  buff[8] = START;

  write_blocks(SUPERBLOCK, SUPERBLOCK_SIZE, buff);
  free(buff);
  return 0;
}

int createRootDir(){ //create root directory
  directoryEntry *buff = malloc(ALIGN(MAX_FILES*sizeof(directoryEntry)));

  if(buff == NULL){
    return -1;
  }

  int i;
  for(i = 0; i < MAX_FILES; i++){
    //set up directory values
    buff[i] = (directoryEntry){.filename = "\0",.inode = 5000};
  }
  write_blocks(DIRECTORY_LOCATION, DIRECTORY_SIZE, buff);
  free(buff);
  return 0;
}

int createInodeTable(){  //create i-node table
  inodeEntry *buff = malloc(ALIGN((MAX_FILES+1)*sizeof(inodeEntry)));

  if(buff == NULL){
    return -1;
  }
  int i;
  //set up i-node table
  for(i = 0; i < MAX_FILES+1; i++){
    buff[i].mode = 0;                       //mode
    buff[i].linkCount = 0;                  //link count
    buff[i].size = 0;                       //size
    buff[i].indirectPointer = 5000;       //indirect pointers
    int j;
    for(j = 0; j < 12; j++){
      buff[i].directPointers[j] = 5000;         //direct pointers
    }
  }
  write_blocks(INODE_TABLE, INODE_TABLE_SIZE, buff);
  free(buff);
  return 0;
}
