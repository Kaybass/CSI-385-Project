#include "utilities.h"

extern int read_sector(int sector_number, char* buffer);
extern int write_sector(int sector_number, char* buffer);
extern unsigned int get_fat_entry(int fat_entry_number, char* fat);
extern void set_fat_entry(int fat_entry_number, int value, char* fat);

ubyte* readFatTable(int fatNumber)
{
    ubyte* fat = malloc(FAT_TABLE_SIZE);

    for (int i = 0; i < NUM_FAT_SECTORS; i++)
    {
        read_sector(i + 1, &fat[i * BYTES_PER_SECTOR]);
    }

    return fat;
}
