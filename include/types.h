#ifndef TYPES_H
#define TYPES_H

#pragma pack(1)

typedef struct
{
    unsigned char boot_flag;
    unsigned char begin_head;
    unsigned char begin_sec_cyl;
    unsigned char begin_cyl;
    unsigned char system_code;
    unsigned char end_head;
    unsigned char end_sec_cyl;
    unsigned char end_cyl;
    unsigned long starting_sec;
    unsigned long num_sectors;
} Partition;

typedef struct
{
    unsigned char load_prog[0x1BE];
    Partition partitions[4];
    unsigned short signature;
} MBR;

typedef struct
{
    unsigned char JMPxxNEAR[3];
    unsigned char name_and_version[8];
    unsigned short sectSize;
    unsigned char clustSize;
    unsigned short resSect;
    unsigned char fatCnt;
    unsigned short rootSize;
    unsigned short totSect;
    unsigned char media;
    unsigned short fatSize;
    unsigned short trkSec;
    unsigned short headCnt;
    unsigned short hidnSec;
    unsigned char other[512 - 30];
} BootRecord;

typedef struct
{
    unsigned char name[8];
    unsigned char name_e[3];
    unsigned char atr;
    unsigned char rez[10];
    unsigned int time;
    unsigned int date;
    unsigned int n_clast;
    unsigned long size;
} Root_dir_elem;

typedef struct
{
    unsigned char head;
    unsigned short cyl;
    unsigned char sec;
} CHS_adr;

#pragma pack()

#endif
