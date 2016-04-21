#ifndef MASH_utilities
#define MASH_utilities

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef unsigned char ubyte;
typedef char byte;

typedef struct _fileinfo
{
    char  Filename[9]; //starts at offset 0
    char  type[4];
    char  Attributes;
    //char  Reserved[2];
    char  CreationTime[2]; //offset 14
    char  CreationDate[2];
    char  LastAccessDate[2];
    // ignored 2 bytes
    char  LastWriteTime[2]; //offset 22
    char  LastWriteDate[2];
    char  FirstLogicalCluster[2];
    int   FileSize;
} FileInfo;

ubyte* readFatTable(int fatTableSize,int numFatSectors,int bytesPerSector);

#endif
