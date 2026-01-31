#ifndef RECOVERY_H
#define RECOVERY_H

#include "types.h"

int find_deleted_entry_coords(
    unsigned long root_dir_LBA,
    unsigned long data_start_LBA,
    const char finded_name[8],
    const char finded_ext[3],
    unsigned long *out_sector_LBA,
    int *out_sector_index,
    Root_dir_elem *out_elem);
void restore_unfragmented_file(
    unsigned long fat_LBA,
    unsigned long dir_sector_LBA,
    int dir_entry_index,
    Root_dir_elem entry,
    char new_first_char,
    BootRecord *br);
    
#endif

