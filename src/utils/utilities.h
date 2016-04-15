#ifndef MASH_utilities
#define MASH_utilities

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


ubyte* readFatTable(int fatTableSize,int numFatSectors,int bytesPerSector);

#endif
