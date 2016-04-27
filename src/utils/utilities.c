#include "utilities.h"

extern int read_sector(int sector_number, char* buffer);
extern int write_sector(int sector_number, char* buffer);
extern unsigned int get_fat_entry(int fat_entry_number, char* fat);
extern void set_fat_entry(int fat_entry_number, int value, char* fat);

int BYTES_PER_SECTOR;

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
*/
short searchForFolder(short currentFLC, char * target){

    int entryCount;

    unsigned char * sector;

    char ** dirs;
    int     depth;

    BYTES_PER_SECTOR = 512;

    int index = 0;

    if(target[0] == '/'){
        splitDirectoryString(target,&depth);

        //the way string splitting works in our case /dir makes depth 2
        depth--;
        index++;

        sector = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));

        ShortFileInfo entries[16];

        /*  Massive bad boy points here, we know that
            every example image has a root folder that
            is only one sector long and we're taking
            advantage of that here.*/
        read_sector(19,sector);

        for(int i = 0; i < BYTES_PER_SECTOR / 32; i++){
            for(int j = 0; j < 8; j++){
                entries[i].Filename[j] = sector[j + i * 32];
            }
            entries[i].Filename[8] = '\0';
        }

        free(sector);

        for(int i = 0; i < 16; i++){

            if(strcmp(dirs[1],entries[i].Filename) == 0 &&
                (FAT_SUBDIRECTORY & entries[i].Attributes) != 0){

                if(depth != index){

                    return searchHarderForFolder(entries[i].FirstLogicalCluster,dirs,index,depth);
                }
                else{

                    return entries[i].FirstLogicalCluster;
                }
            }
        }

        return -1;

    }
    else{
        splitDirectoryString(target,&depth);

        
    }

}

short searchHarderForFolder(short currentFLC, char ** dirs, int index, int depth){

}

short searchForFile(short currentFLC, char * target){

}

short searchHarderForFile(short currentFLC, char ** dirs, int index, int depth){

}


char ** splitDirectoryString(char * directoryName, int *entryc){

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

    *entryc = tokensUsed;
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
