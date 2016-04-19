#ifndef MASH_utilities
#define MASH_utilities

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef unsigned char ubyte;
typedef char byte;

typedef struct _fileinfo
{
    char* name;
    char* type;
    int filesize;
    int FLC;
} FileInfo;

ubyte* readFatTable(int fatTableSize,int numFatSectors,int bytesPerSector);

#endif
