#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../utils/utilities.h"

#define BYTES_TO_READ_IN_BOOT_SECTOR 62

FILE* FILE_SYSTEM_ID;
int BYTES_PER_SECTOR;

extern int read_sector(int sector_number, char* buffer);

int main(int argc, char *argv[])// Argument like
{
   unsigned char* boot;

   int mostSignificantBits;
   int leastSignificantBits;
   int bytesPerSector;
   int sectorsPerCluster;
   int reservedSectors;
   int numberFats;
   int maxRootEntries;
   int TSC;
   int sectorsPerFat;
   int sectorsPerTrack;
   int numberHeads;
   int one,two,three,four;
   unsigned char bootSignature;
   unsigned int volumeId;
   unsigned char volumeLabel[12], fileSystemType[9];

   shmid = shmget(MASH_MEM_KEY, sizeof(SharedStuff),0666);

   if(shmid < 0){

       //We couldn't create the segment
       perror("Oh my god shared memory didn't work, god save us.");
       exit(EXIT_FAILURE);
   }

   stuff = (SharedStuff *) shmat(shmid,NULL,0);

   FILE_SYSTEM_ID = stuff->file;

   // Set it to this only to read the boot sector
   BYTES_PER_SECTOR = BYTES_TO_READ_IN_BOOT_SECTOR;

   // Then reset it per the value in the boot sector

   boot = (unsigned char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));

   if (read_sector(0, boot) == -1)
      printf("Something has gone wrong -- could not read the boot sector\n");


   // 12 (not 11) because little endian
   mostSignificantBits  = ( ( (int) boot[12] ) << 8 ) & 0x0000ff00;
   leastSignificantBits =   ( (int) boot[11] )        & 0x000000ff;
   bytesPerSector = mostSignificantBits | leastSignificantBits;

   sectorsPerCluster = (unsigned int) boot[13];

   mostSignificantBits  = ( ( (int) boot[15] ) << 8 ) & 0x0000ff00;
   leastSignificantBits =   ( (int) boot[14] )        & 0x000000ff;
   reservedSectors = mostSignificantBits | leastSignificantBits;

   numberFats = (unsigned int) boot[16];


   mostSignificantBits  = ( ( (int) boot[18] ) << 8 ) & 0x0000ff00;
   leastSignificantBits =   ( (int) boot[17] )        & 0x000000ff;
   maxRootEntries = mostSignificantBits | leastSignificantBits;

   mostSignificantBits  = ( ( (int) boot[20] ) << 8 ) & 0x0000ff00;
   leastSignificantBits =   ( (int) boot[19] )        & 0x000000ff;
   TSC = mostSignificantBits | leastSignificantBits;

   mostSignificantBits  = ( ( (int) boot[23] ) << 8 ) & 0x0000ff00;
   leastSignificantBits =   ( (int) boot[22] )        & 0x000000ff;
   sectorsPerFat = mostSignificantBits | leastSignificantBits;

   mostSignificantBits  = ( ( (int) boot[25] ) << 8 ) & 0x0000ff00;
   leastSignificantBits =   ( (int) boot[24] )        & 0x000000ff;
   sectorsPerTrack = mostSignificantBits | leastSignificantBits;

   mostSignificantBits  = ( ( (int) boot[27] ) << 8 ) & 0x0000ff00;
   leastSignificantBits =   ( (int) boot[26] )        & 0x000000ff;
   numberHeads = mostSignificantBits | leastSignificantBits;

   bootSignature = boot[38];

   one =   ( ( (int) boot[42] ) << 24 ) & 0xff000000;
   two =   ( ( (int) boot[41] ) << 16 ) & 0x00ff0000;
   three = ( ( (int) boot[40] ) << 8  ) & 0x0000ff00;
   four =  (   (int) boot[39] )         & 0x000000ff;

   volumeId = one | two | three | four;

   for(int i = 0; i < 11; i++){
       volumeLabel[i] = boot[43+i];
   }
   volumeLabel[12] = '\0';

   for(int i = 0; i < 8; i++){
       fileSystemType[i] = boot[54+i];
   }
   fileSystemType[9] = '\0';

   printf("Bytes per sector = %d\n"
            "Sectors per cluster = %d\n"
            "Number of FATs = %d\n"
            "Number of reserved sectors = %d\n"
            "Number of root entries = %d\n"
            "Total sector count = %d\n"
            "Sectors per FAT = %d\n"
            "Sectors per track = %d\n"
            "Number of heads = %d\n"
            "Boot signature (in hex) = 0x%x\n"
            "Volume ID (in hex) = 0x%8x\n"
            "Volume label = %s\n"
            "File system type = %s\n",
             bytesPerSector,
             sectorsPerCluster,
             numberFats,
             reservedSectors,
             maxRootEntries,
             TSC,
             sectorsPerFat,
             sectorsPerTrack,
             numberHeads,
             ((unsigned int)bootSignature),
             volumeId,
             volumeLabel,
             fileSystemType);

   free(boot);

   return 0;
}
