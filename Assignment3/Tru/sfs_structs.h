#define MAXFILENAME 15

typedef struct directoryEntry       //data type for directories
{
  char filename[MAXFILENAME + 1];
  unsigned int inode;
} directoryEntry;

typedef struct inodeEntry           //data type for inodes
{
  unsigned int mode;
  unsigned int linkCount;
  unsigned int size;
  unsigned int directPointers[12];
  unsigned int indirectPointer;
} inodeEntry;

typedef struct fileDescriptorEntry  //data type for file descriptors
{
  unsigned int inode;
  unsigned int rwPointer;
} fileDescriptorEntry;

int createFreeList();
void setFree(unsigned int index);
void setAlloc(unsigned int index);
int findFree();
int createSuperblock();
int createRootDir();
int createInodeTable();

