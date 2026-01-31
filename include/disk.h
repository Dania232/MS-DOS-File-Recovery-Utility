#ifndef DISK_H
#define DISK_H

#include "types.h"

#define SECTOR_SIZE 512
#define SECTORS_PER_TRACK 63
#define HEADS_PER_CYL 16

#define DISK_READED_SUCCESSFULLY 0
#define DISC_READED_WITH_ERROR_FLAG -1
#define DISC_READED_WITH_ERROR_AH -2
#define DISK_WRITTEN_SUCCESSFULLY 0

unsigned long CHS_to_LBA(CHS_adr adr);
CHS_adr LBA_to_CHS(unsigned long lba);
int read_hard_disk_sec(unsigned char sec[SECTOR_SIZE], CHS_adr chs);
int write_hard_disk_sec(unsigned char sec[SECTOR_SIZE], CHS_adr chs);

#endif