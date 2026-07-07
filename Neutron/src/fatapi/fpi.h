#ifndef NEUTRON_FAT32_H
#define NEUTRON_FAT32_H

#include <Uefi.h>
#include <SimpleFileSystem.h>
#include <Guid/FileInfo.h>

/*
    Neutron Fat32 UEFI API Layer

    This header exposes a small set of helpers for working with files from a
    UEFI application. The routines use the EFI simple filesystem protocol to
    open a volume, access files, and read directory entries.
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

    These helpers are intended for simple boot-time or UEFI environment file
    access. All paths are passed as UCS-2/UTF-16 CHAR16 strings and are opened
    relative to the filesystem volume.

    Functions:

    UseFile()
        Convenience wrapper for reading a file into memory.

    ReadFile()
        Open a file, determine its size, allocate a buffer, and read it.

    DeleteFile()
        Declared for future use. The current implementation does not provide
        a matching body.

    FreeFileMem()
        Release memory allocated by ReadFile() or UseFile().

    WriteIntoFile()
        Declared for future use. The current implementation does not provide
        a matching body.

    Dir()
        List directory entries into a caller-supplied buffer.
*/

/**
  Read a file from the current UEFI filesystem volume into a newly allocated
  buffer.

  @param[in]  SystemTable  Pointer to the UEFI system table.
  @param[in]  Position     File path to open, expressed as a CHAR16 string.
  @param[out] Buffer       On success, points to a heap buffer containing the file data.
  @param[out] Size         On success, receives the number of bytes read.
  @retval EFI_SUCCESS      The file was read successfully.
  @retval EFI_ERROR(...)   The open/read/allocate operation failed.
*/
EFI_STATUS UseFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    VOID **Buffer,
    UINTN *Size
);


/**
  Read a file from the current UEFI filesystem volume into a newly allocated
  buffer.

  @param[in]  SystemTable  Pointer to the UEFI system table.
  @param[in]  Position     File path to open, expressed as a CHAR16 string.
  @param[out] Buffer       On success, points to a heap buffer containing the file data.
  @param[out] Size         On success, receives the number of bytes read.
  @retval EFI_SUCCESS      The file was read successfully.
  @retval EFI_ERROR(...)   The open/read/allocate operation failed.
*/
EFI_STATUS ReadFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    VOID **Buffer,
    UINTN *Size
);


/**
  Delete a file from the current UEFI filesystem volume.

  This entry point is declared for the public API but is not implemented in
  the current source file.
*/
EFI_STATUS DeleteFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position
);


/**
  Release memory previously allocated for file contents.

  @param[in] Pointer  Heap pointer returned by ReadFile() or UseFile().
*/
VOID FreeFileMem(
    VOID *Pointer
);


/**
  Write data into a file at a specific offset.

  This entry point is declared for the public API but is not implemented in
  the current source file.
*/
EFI_STATUS WriteIntoFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    VOID *WriteData,
    UINTN DataSize,
    UINTN Localization
);


/**
  List the contents of a directory into a caller-provided buffer.

  @param[in]  SystemTable  Pointer to the UEFI system table.
  @param[in]  Position     Directory path to open, expressed as a CHAR16 string.
  @param[out] DirData      Buffer that receives a CR/LF separated list of names.
  @param[in]  DirDataSize  Size of DirData in CHAR16 units.
  @retval EFI_SUCCESS      The directory listing was generated successfully.
  @retval EFI_ERROR(...)   The open/list operation failed.
*/
EFI_STATUS Dir(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    CHAR16 *DirData,
    UINTN DirDataSize
);

/*
    Global UEFI System Table pointer
    tired guys
*/

extern EFI_SYSTEM_TABLE *gST;


#endif