/*
This really isn't a utilities file in practice as none of
the programs in the project can run without these functions
and constants lol
*/


#ifndef MASH_utilities
#define MASH_utilities

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MASH_MEM_KEY 6969696699

//File Attributes bitmask constants
#define FAT_READ_ONLY    0x01
#define FAT_HIDDEN       0x02
#define FAT_SYSTEM       0x04
#define FAT_VOLUMELABEL  0x08
#define FAT_SUBDIRECTORY 0x10
#define FAT_ARCHIVE      0x20

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
    int   FirstLogicalCluster;
    int   FileSize;
} FileInfo;

typedef struct _sharedstuff{
    char dir[100];
    short FLC;
    File *file;
} SharedStuff;

ubyte* readFatTable(int fatTableSize,int numFatSectors,int bytesPerSector);

#endif
