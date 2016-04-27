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
short searchForFolder(short currentFLC, char * target){

    int entryCount;

    unsigned char sector;

    char ** dirs;
    int     depth;

    if(target[0] == '/'){
        splitDirectoryString(dirs,depth);


    }
    else{
        splitDirectoryString(dirs,depth);
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

int* lookupSectors(int FLC, ubyte* image)
{
    // This is disgusting fix this later rofl
    int* sectors = (int*)malloc(10 * sizeof(int));
    int currEntry = FLC;
    int i = 0;

    sectors[0] = currEntry;

    while (currEntry < 0xff8)
    {
        currEntry = get_fat_entry(currEntry, (byte*)image);
        sectors[i] = currEntry + 31;
        i++;
    }

    return sectors;
}
