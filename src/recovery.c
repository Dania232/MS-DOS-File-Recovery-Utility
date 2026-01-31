#include <stdio.h>
#include <string.h>
#include "../include/recovery.h"
#include "../include/disk.h"
#include "../include/fat16.h"


int find_deleted_entry_coords(
    unsigned long root_dir_LBA,
    unsigned long data_start_LBA,
    const char finded_name[8],
    const char finded_ext[3],
    unsigned long *out_sector_LBA,
    int *out_sector_index,
    Root_dir_elem *out_elem)
{
    Root_dir_elem root_dir_elemsPerSec[16];
    unsigned long cur_readed_sec_LBA;
    int i;

    for (cur_readed_sec_LBA = root_dir_LBA; cur_readed_sec_LBA < data_start_LBA; cur_readed_sec_LBA++)
    {
        read_hard_disk_sec((unsigned char *)(root_dir_elemsPerSec), LBA_to_CHS(cur_readed_sec_LBA));
        for (i = 0; i < 16; i++)
        {
            Root_dir_elem *current = &(root_dir_elemsPerSec[i]);
            if (current->name[0] == 0xE5)
            {
                if (memcmp(current->name + 1, finded_name + 1, 7) == 0 &&
                    memcmp(current->name_e, finded_ext, 3) == 0)
                {
                    *out_sector_LBA = cur_readed_sec_LBA;
                    *out_sector_index = i;
                    *out_elem = *current;
                    return 1; 
                }
            }
        }
    }
    return 0;
}

void restore_unfragmented_file(
    unsigned long fat_LBA,
    unsigned long dir_sector_LBA,
    int dir_entry_index,
    Root_dir_elem entry,
    char new_first_char,
    BootRecord *br)
{
    unsigned char sector_buf[SECTOR_SIZE];
    long file_size = entry.size;
    unsigned int start_cluster = entry.n_clast;
    unsigned int current_cluster;
    unsigned long clusters_needed;
    unsigned long cluster_size_bytes;
    int i;
    Root_dir_elem *dir_table;
    CHS_adr dir_chs;
    printf("\nRestoring filename...\n");
    dir_chs = LBA_to_CHS(dir_sector_LBA);
    if (read_hard_disk_sec(sector_buf, dir_chs) != DISK_READED_SUCCESSFULLY) {
        printf("Error reading directory sector!\n");
        return;
    }
    
    dir_table = (Root_dir_elem*)sector_buf;
    dir_table[dir_entry_index].name[0] = new_first_char; 
    
    if (write_hard_disk_sec(sector_buf, dir_chs) != DISK_WRITTEN_SUCCESSFULLY) {
        printf("Error writing directory sector!\n");
        return;
    }
    printf("Filename restored: %c%6.6s.%3.3s\n", new_first_char, entry.name+1, entry.name_e);


    cluster_size_bytes = (unsigned long)br->sectSize * br->clustSize;
    clusters_needed = (file_size + cluster_size_bytes - 1) / cluster_size_bytes;
    
    printf("Restoring FAT chain: Start=%u, Clusters=%lu\n", start_cluster, clusters_needed);

    

    current_cluster = start_cluster;

    for (i = 0; i < clusters_needed; i++)
    {
        short val = read_FAT_elem(fat_LBA, current_cluster);
        if (val != 0 && val != -1) { 
            printf("Warning: Cluster %u is already occupied (val=%04X)! Stopping FAT restore.\n", current_cluster, val);
            break;
        }

        if (i == clusters_needed - 1) {
            write_FAT_elem(fat_LBA, current_cluster, 0xFFFF);
        } else {
            write_FAT_elem(fat_LBA, current_cluster, current_cluster + 1);
        }
        
        current_cluster++; 
    }
    printf("Done.\n");
}
