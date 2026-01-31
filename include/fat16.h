#ifndef FAT_H
#define FAT_H
#include "types.h"

short read_FAT_elem(int FAT_table_LBA, int ind);
int write_FAT_elem(int FAT_table_LBA, int ind, unsigned short val);
CHS_adr get_FAT_adr(Partition *active_partition, BootRecord *bootRecord);
CHS_adr get_data_start_adr(Partition *active_partition, BootRecord *bootRecord);
CHS_adr get_root_dir_adr(Partition *active_partition, BootRecord *bootRecord);

#endif