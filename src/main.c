#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h> 

#include "../include/types.h"
#include "../include/disk.h"
#include "../include/fat16.h"
#include "../include/recovery.h"

int main(int argc, char *argv[])
{
    MBR master_boot_record;
    Partition *active_partition;
    BootRecord bootRecord;

    CHS_adr FAT_adr, root_dir_adr, bootRecord_adr, MBR_adr;
    unsigned long FAT_LBA, data_start_LBA, root_dir_LBA;
    
    unsigned long found_sec_LBA;
    int found_sec_idx;
    Root_dir_elem found_elem;

    char *input = argv[1];
    char finded_name[8];
    char finded_ext[3];
    int i, j;
    char restore_char;

    if (argc < 2) {
        printf("Usage: program <FILENAME.EXT>\n");
        return 1;
    }

    memset(finded_name, ' ', 8);
    memset(finded_ext, ' ', 3);
    
    restore_char = toupper(input[0]); 
    
    i = 0; j = 0;
    while (input[i] && input[i] != '.' && j < 8) {
        finded_name[j++] = toupper(input[i]);
        i++;
    }
    if (input[i] == '.') i++;
    j = 0;
    while (input[i] && j < 3) {
        finded_ext[j++] = toupper(input[i]);
        i++;
    }

    MBR_adr.cyl = 0; MBR_adr.head = 0; MBR_adr.sec = 1;
    if (read_hard_disk_sec((unsigned char *)&master_boot_record, MBR_adr) != DISK_READED_SUCCESSFULLY) {
        printf("MBR read error\n"); return -1;
    }

    active_partition = &master_boot_record.partitions[0]; 
    bootRecord_adr = LBA_to_CHS(active_partition->starting_sec);
    
    if (read_hard_disk_sec((unsigned char *)&bootRecord, bootRecord_adr) != DISK_READED_SUCCESSFULLY) {
        printf("BootRecord read error\n"); return -1;
    }

    FAT_adr = get_FAT_adr(active_partition, &bootRecord);
    root_dir_adr = get_root_dir_adr(active_partition, &bootRecord);
    
    FAT_LBA = CHS_to_LBA(FAT_adr);
    root_dir_LBA = CHS_to_LBA(root_dir_adr);
    data_start_LBA = CHS_to_LBA(get_data_start_adr(active_partition, &bootRecord));

    printf("Searching for deleted file...\n");

    if (find_deleted_entry_coords(root_dir_LBA, data_start_LBA, finded_name, finded_ext, 
                                  &found_sec_LBA, &found_sec_idx, &found_elem)) 
    {
        printf("FOUND deleted file!\n");
        printf("Start Cluster: %u, Size: %lu bytes\n", found_elem.n_clast, found_elem.size);
        
        restore_unfragmented_file(FAT_LBA, found_sec_LBA, found_sec_idx, found_elem, restore_char, &bootRecord);
    } 
    else 
    {
        printf("Deleted file not found.\n");
    }

    return 0;
}