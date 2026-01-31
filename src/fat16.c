#include <stdio.h>
#include "../include/fat16.h"
#include "../include/disk.h"


short read_FAT_elem(int FAT_table_LBA, int ind)
{
    static unsigned char sec_buf[SECTOR_SIZE];
    unsigned long fat_offset = (unsigned long)ind * 2; // *2 для FAT16
    unsigned long fat_lba = FAT_table_LBA + (fat_offset / SECTOR_SIZE);
    int offset_in_sector = (int)(fat_offset % SECTOR_SIZE);
    CHS_adr chs = LBA_to_CHS(fat_lba);

    if (read_hard_disk_sec(sec_buf, chs) != DISK_READED_SUCCESSFULLY) {
        printf("FAT read error at LBA %lu\n", fat_lba);
        return -1;
    }
    return *((short *)(sec_buf + offset_in_sector));
}

int write_FAT_elem(int FAT_table_LBA, int ind, unsigned short val)
{
    unsigned char sec_buf[SECTOR_SIZE];
    unsigned long fat_offset = (unsigned long)ind * 2;
    unsigned long fat_lba = FAT_table_LBA + (fat_offset / SECTOR_SIZE);
    int offset_in_sector = (int)(fat_offset % SECTOR_SIZE);
    CHS_adr chs = LBA_to_CHS(fat_lba);

    if (read_hard_disk_sec(sec_buf, chs) != DISK_READED_SUCCESSFULLY) {
        printf("FAT read error before write at LBA %lu\n", fat_lba);
        return -1;
    }

    *((unsigned short *)(sec_buf + offset_in_sector)) = val;

    if (write_hard_disk_sec(sec_buf, chs) != DISK_WRITTEN_SUCCESSFULLY) {
         printf("FAT write error at LBA %lu\n", fat_lba);
         return -1;
    }
    return 0;
}

CHS_adr get_FAT_adr(Partition *active_partition, BootRecord *bootRecord) {
    unsigned long LBA = active_partition->starting_sec + bootRecord->resSect;
    return LBA_to_CHS(LBA);
}

CHS_adr get_root_dir_adr(Partition *active_partition, BootRecord *bootRecord) {
    unsigned long LBA = active_partition->starting_sec + bootRecord->resSect +
                        bootRecord->fatCnt * bootRecord->fatSize;
    return LBA_to_CHS(LBA);
}

CHS_adr get_data_start_adr(Partition *active_partition, BootRecord *bootRecord) {
    unsigned long first_partition_LBA = active_partition->starting_sec;
    unsigned long FAT_start = first_partition_LBA + bootRecord->resSect;
    unsigned long Root_start = FAT_start + bootRecord->fatCnt * bootRecord->fatSize;
    unsigned long RootDirSectors = (bootRecord->rootSize * 32 + bootRecord->sectSize - 1) / bootRecord->sectSize;
    unsigned long data_start = Root_start + RootDirSectors;
    return LBA_to_CHS(data_start);
}