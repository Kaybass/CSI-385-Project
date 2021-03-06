/*
This really isn't a utilities file in practice as none of
the programs in the project can run without these functions
and constants lol
*/


#ifndef MASH_utilities
#define MASH_utilities

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../utils/fatSupport.h"

#define DIR_CHAR "/"

//shared memory key for programs
#define MASH_MEM_KEY 80242069

#define ENTRIES_PER_SECTOR 16

#define FLC_TO_PC_OFFSET 31

//File Attributes bitmask constants
#define FAT_READ_ONLY    0x01
#define FAT_HIDDEN       0x02
#define FAT_SYSTEM       0x04
#define FAT_VOLUMELABEL  0x08
#define FAT_SUBDIRECTORY 0x10
#define FAT_ARCHIVE      0x20

extern FILE * FILE_SYSTEM_ID;

//Types, Ethan's code uses the byte types

typedef unsigned char ubyte;
typedef char byte;

typedef struct _fileinfo
{
    char  Filename[9]; //starts at offset 0
    char  Type[4];
    char  Attributes;
    //char  Reserved[2];
    char  CreationTime[2]; //offset 14
    char  CreationDate[2];
    char  LastAccessDate[2];
    // ignored 2 bytes
    char  LastWriteTime[2]; //offset 22
    char  LastWriteDate[2];
    short FirstLogicalCluster;
    int   FileSize;
} FileInfo;

//file info for things that aren't ls
typedef struct _fileinfoweactuallycareabout{

    char Filename[9]; //offset 0
    char Type[4];     //offset 8

    char Attributes;  //offset 11

    short FirstLogicalCluster;//offset 26
} ShortFileInfo;

typedef struct _sharedstuff{
    char dir[100];
    short FLC;
    char filename[30];
} SharedStuff;


//Functions

ubyte* readFatTable(int fatTableSize,int numFatSectors,int bytesPerSector);

short searchForFolder(short currentFLC, char * target);
short searchHarderForFolder(short currentFLC, char ** dirs, int index, int depth);

short searchForFile(short currentFLC, char * target);
short searchHarderForFile(short currentFLC, char ** dirs, int index, int depth);

char ** splitDirectoryString(char * directoryName, int *entryc);
char ** splitFilenameExtension(char * file, int *shouldbetwo);

int* lookupSectors(int FLC, int * length, ubyte* image);

int getRootSize();

#endif
