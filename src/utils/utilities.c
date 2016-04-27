#include "utilities.h"

extern int read_sector(int sector_number, char* buffer);
extern int write_sector(int sector_number, char* buffer);
extern unsigned int get_fat_entry(int fat_entry_number, char* fat);
extern void set_fat_entry(int fat_entry_number, int value, char* fat);

ubyte* readFatTable(int fatTableSize,int numFatSectors,int bytesPerSector)
{
    ubyte* fat = malloc(fatTableSize);

    for (int i = 0; i < numFatSectors; i++)
    {
        read_sector(i + 1, &fat[i * bytesPerSector]);
    }

    return fat;
}

/*
 * Things assumed by this function:
 * - It's being fed a real directory i.e input checking was done
 * - I'm a bad prgrammer
 * - Like really really bad
 *
*/
short searchForFileEntry(short currentFLC, char * target){


    int entryCount;

    char ** dirs;
    int     depth;

    dirs = splitDirectoryString(target,depth);

    //not root
    if(target[0] != '/' && currentFLC != 0){

        //directory above
        if(target[0] == '.' && target[1] == '.'){

            //Jesus christ I don't know what we do here
        }
        //current directory
        else{


        }
    }
    //root
    else{
        int status = 1, h, l, z = 0;

        while(status && z < 13){

            ShortFileInfo entries[16];
            char *sector = (byte*)malloc(512 * sizeof(ubyte));
            read_sector(19 + z, sector);

            for(int i = 0; i < ENTRIES_PER_SECTOR; i++){
                for(int j = 0; j < 8; j++){
                    entries[i].Filename[j] = sector[j + i * 32];
                }
                entries[i].Filename[8] = '\0';

                for(int j = 8; j < 11; j++){
                    entries[i].Type[j] = sector[j + i * 32];
                }
                entries[i].Type[3] = '\0';

                entries[i].Attributes = sector[11 + i * 32];

                h = ( ( (int) sector[27 + i * 32] ) << 8 ) & 0x0000ff00;
                l =   ( (int) sector[26 + i * 32] )        & 0x000000ff;
                entries[i].FirstLogicalCluster = h | l;
            }

            for(int i = 0; i < ENTRIES_PER_SECTOR; i++){

                if(strcmp(dirs[0],entries[i].Filename) == 0 &&
                    entries[i].Attributes & FAT_SUBDIRECTORY == 0){

                    return entries[i].FirstLogicalCluster;
                }
                if (entries[i].Filename[0] == 0){
                    status = 0;
                }
            }

            z++;

            free(sector);

        }
    }
}

char ** splitDirectoryString(char * directoryName, int entryc){

    char *s = strdup(directoryName);
    size_t tokensAllocated = 1;
    size_t tokensUsed = 0;

    char **tokens = calloc(tokensAllocated, sizeof(char*));
    char *token, *rest = s;

    while((token = strsep(&rest, DIR_CHAR)) != NULL){

        if(tokensUsed == tokensAllocated){

            tokensAllocated *= 2;
            tokens = realloc(tokens, tokensAllocated * sizeof(char*));
        }

        tokens[tokensUsed++] = strdup(token);
    }
    if(tokensUsed == 0){
        free(tokens);
        tokens = NULL;
    }
    else{
        tokens = realloc(tokens, tokensUsed * sizeof(char*));
    }

    entryc = tokensUsed;
    free(s);

    return tokens;
}
