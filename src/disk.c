#include <dos.h>
#include "../include/disk.h"


unsigned long CHS_to_LBA(CHS_adr adr)
{
    return (unsigned long)(adr.cyl * HEADS_PER_CYL + adr.head) * SECTORS_PER_TRACK + (adr.sec - 1);
}

CHS_adr LBA_to_CHS(unsigned long lba)
{
    CHS_adr chs;
    chs.cyl = lba / (HEADS_PER_CYL * SECTORS_PER_TRACK);
    chs.head = (lba / SECTORS_PER_TRACK) % HEADS_PER_CYL;
    chs.sec = (lba % SECTORS_PER_TRACK) + 1;
    return chs;
}


int read_hard_disk_sec(unsigned char sec[SECTOR_SIZE], CHS_adr chs)
{
    const unsigned char INT_DISK = 0x13;
    const unsigned char READ_DISK = 0x02;
    const unsigned char HARD_DISK_DRIVE = 0x80;
    const unsigned char NUM_SEC_TO_READ = 0x01;

    union REGS in, out;
    struct SREGS sr;

    unsigned char CH = (unsigned char)(chs.cyl & 0xFF);
    unsigned char CL = (unsigned char)((chs.sec & 0x3F) | ((chs.cyl >> 2) & 0xC0));
    unsigned char DH = chs.head;
    unsigned char DL = HARD_DISK_DRIVE;

    in.h.ah = READ_DISK;
    in.h.al = NUM_SEC_TO_READ;
    in.h.ch = CH;
    in.h.cl = CL;
    in.h.dh = DH;
    in.h.dl = DL;

    sr.es = FP_SEG(sec);
    in.x.bx = FP_OFF(sec);

    int86x(INT_DISK, &in, &out, &sr);

    if (out.x.cflag) return DISC_READED_WITH_ERROR_FLAG;
    else if (out.h.ah != 0) return DISC_READED_WITH_ERROR_AH;
    else return DISK_READED_SUCCESSFULLY;
}

int write_hard_disk_sec(unsigned char sec[SECTOR_SIZE], CHS_adr chs)
{
    const unsigned char INT_DISK = 0x13;
    const unsigned char WRITE_DISK = 0x03; // Функция записи
    const unsigned char HARD_DISK_DRIVE = 0x80;
    const unsigned char NUM_SEC_TO_READ = 0x01;

    union REGS in, out;
    struct SREGS sr;

    unsigned char CH = (unsigned char)(chs.cyl & 0xFF);
    unsigned char CL = (unsigned char)((chs.sec & 0x3F) | ((chs.cyl >> 2) & 0xC0));
    unsigned char DH = chs.head;
    unsigned char DL = HARD_DISK_DRIVE;

    in.h.ah = WRITE_DISK;
    in.h.al = NUM_SEC_TO_READ;
    in.h.ch = CH;
    in.h.cl = CL;
    in.h.dh = DH;
    in.h.dl = DL;

    sr.es = FP_SEG(sec);
    in.x.bx = FP_OFF(sec);

    int86x(INT_DISK, &in, &out, &sr);

    if (out.x.cflag) return -1;
    else if (out.h.ah != 0) return -2;
    else return DISK_WRITTEN_SUCCESSFULLY;
}