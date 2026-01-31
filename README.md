# FAT16 File Recovery Tool (Undelete)

![Type](https://img.shields.io/badge/Type-University_Coursework-yellow) ![Platform](https://img.shields.io/badge/Platform-MS--DOS-red) ![Language](https://img.shields.io/badge/Language-C-blue)

## 📖 About

A low-level system programming utility written in C that recovers deleted files by directly interacting with the disk hardware.

**Context:** Developed as university coursework to demonstrate understanding of file system architecture, direct memory access, and BIOS interrupts.

## ⚙️ Technical Highlights

Instead of using high-level file APIs (like `fopen`), this tool operates "close to the metal":

* **Direct Hardware Access:** Uses BIOS interrupt `0x13` to read/write raw disk sectors.
* **Manual Parsing:** Manually parses the **MBR** (Master Boot Record), **Partition Table**, and **FAT16** structures.
* **Geometry Conversion:** Implements manual LBA (Logical Block Addressing) to CHS (Cylinder-Head-Sector) translation.

## ⚠️ Scope & Limitations

To keep the scope focused on low-level logic, the utility has the following constraints:

* **File System:** Supports **FAT16** partitions only.
* **Location:** Scans and restores files in the **Root Directory** only.
* **Platform:** Runs strictly on **MS-DOS** (Real Mode) due to direct hardware access requirements.
* **Fragmentation:** Assumes the deleted file was stored contiguously (unfragmented).

## 🏗 Architecture & Code Structure

The project is modularized to separate hardware interaction, file system logic, and data structures.

### `include/types.h` (Data Structures)
Defines the byte-aligned structures using `#pragma pack(1)`. This ensures that C structures map perfectly to the binary data on the disk without compiler padding.
* `MBR`: Master Boot Record layout.
* `Partition`: Partition table entry.
* `BootRecord`: BIOS Parameter Block (BPB) and filesystem metadata.
* `Root_dir_elem`: 32-byte FAT16 directory entry.

### `src/disk.c` (Hardware Layer)
Acts as a driver layer. It isolates all direct BIOS interactions.
* Wraps `int 86x` calls to read/write sectors.
* Handles **CHS ↔ LBA conversion** math, allowing the rest of the program to work with logical sector numbers.

### `src/fat16.c` (File System Abstraction)
Provides a high-level API for interacting with FAT16 specific features.
* Calculates offsets for the FAT table, Root Directory, and Data Area based on the Boot Record.
* Reads and updates specific entries in the File Allocation Table (FAT).

### `src/recovery.c` (Core Logic)
Contains the specific "Undelete" business logic:
1.  Iterates through sectors of the Root Directory.
2.  Identifies deleted files by the `0xE5` byte marker.
3.  Reconstructs the FAT chain linearly to restore file integrity.

---
**Built with Turbo C++**
