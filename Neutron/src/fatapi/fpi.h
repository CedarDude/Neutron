#ifndef NEUTRON_FAT32_H
#define NEUTRON_FAT32_H

#include <Uefi.h>
#include <SimpleFileSystem.h>
#include <Guid/FileInfo.h>

/*
    Neutron Fat32 UEFI API Layer
*/

/*
    Razzle (c) 2026 - Neutron EFI Application Project

    License:
        GPLv3 Open Source

    Created by:
        Jad

    Used:
        EDK II
            https://github.com/tianocore/edk2

        Orca.net Printf Implementation
        Heavily Modified
            https://github.com/orca-pet3910YT/C-kernel-OS

        The Rest Is From Me (Jad)
*/


/*
    Neutron FAT32 File API

    Functions:

    UseFile()
        Open and read a file.

    ReadFile()
        Read a file from FAT32 filesystem.

    DeleteFile()
        Delete a file.

    FreeFileMem()
        Free memory allocated by ReadFile().

    WriteIntoFile()
        Write data into a file at a specific offset.
*/


EFI_STATUS UseFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    VOID **Buffer,
    UINTN *Size
);


EFI_STATUS ReadFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    VOID **Buffer,
    UINTN *Size
);


EFI_STATUS DeleteFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position
);


VOID FreeFileMem(
    VOID *Pointer
);


EFI_STATUS WriteIntoFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    VOID *WriteData,
    UINTN DataSize,
    UINTN Localization
);


EFI_STATUS Dir(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    CHAR16 *DirData,
    UINTN DirDataSize
);

/*
    Global UEFI System Table pointer
    tired guys...
*/

extern EFI_SYSTEM_TABLE *gST;


#endif