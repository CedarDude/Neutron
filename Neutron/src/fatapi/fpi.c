#include <Uefi.h>
#include <SimpleFileSystem.h>
#include <Guid/FileInfo.h>
#define GET_FUCKED 1

/*
    FAT32 helper implementation for UEFI applications.

    These routines wrap the EFI_SIMPLE_FILE_SYSTEM_PROTOCOL and
    EFI_FILE_PROTOCOL interfaces to allow simple file reads and directory
    listings from a boot environment.
*/

EFI_STATUS ReadFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    VOID **Buffer,
    UINTN *Size
)
{
    EFI_STATUS Status;

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    EFI_FILE_PROTOCOL *Root;
    EFI_FILE_PROTOCOL *File;

    EFI_FILE_INFO *Info;
    UINTN InfoSize = 0;


    Status = SystemTable->BootServices->LocateProtocol(
        &gEfiSimpleFileSystemProtocolGuid,
        NULL,
        (VOID **)&FileSystem
    );

    if (EFI_ERROR(Status))
        return Status;


    Status = FileSystem->OpenVolume(
        FileSystem,
        &Root
    );

    if (EFI_ERROR(Status))
        return Status;


    Status = Root->Open(
        Root,
        &File,
        Position,
        EFI_FILE_MODE_READ,
        0
    );

    if (EFI_ERROR(Status))
        return Status;


    File->GetInfo(
        File,
        &gEfiFileInfoGuid,
        &InfoSize,
        NULL
    );


    Status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        InfoSize,
        (VOID **)&Info
    );

    if (EFI_ERROR(Status))
    {
        File->Close(File);
        return EFI_OUT_OF_RESOURCES;
    }


    Status = File->GetInfo(
        File,
        &gEfiFileInfoGuid,
        &InfoSize,
        Info
    );

    if (EFI_ERROR(Status))
    {
        SystemTable->BootServices->FreePool(Info);
        File->Close(File);
        return Status;
    }


    *Size = Info->FileSize;


    Status = SystemTable->BootServices->AllocatePool(
        EfiLoaderData,
        *Size,
        Buffer
    );

    if (EFI_ERROR(Status))
    {
        SystemTable->BootServices->FreePool(Info);
        File->Close(File);
        return EFI_OUT_OF_RESOURCES;
    }


    Status = File->Read(
        File,
        Size,
        *Buffer
    );


    SystemTable->BootServices->FreePool(Info);

    File->Close(File);


    return Status;
}


EFI_STATUS UseFile(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    VOID **Buffer,
    UINTN *Size
)
{
    return ReadFile(
        SystemTable,
        Position,
        Buffer,
        Size
    );
}

// I AM GONNA SLAP MY MONITOR
// I HATE HOW RANDOMLY YOUHAVE A MSSING SHIT!

VOID FreeFileMem(
    EFI_SYSTEM_TABLE *SystemTable,
    VOID *Pointer
)
{
    if (Pointer != NULL)
    {
        SystemTable->BootServices->FreePool(Pointer);
    }

    
}




/**
DIRECTORY DIR
DIR COMMAND START
**/

EFI_STATUS Dir(
    EFI_SYSTEM_TABLE *SystemTable,
    CHAR16 *Position,
    CHAR16 *DirData,
    UINTN DirDataSize
)
{
    EFI_STATUS Status;

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    EFI_FILE_PROTOCOL *Root;
    EFI_FILE_PROTOCOL *Directory;

    EFI_FILE_INFO *Info;

    UINTN InfoSize;
    UINTN Offset = 0;


    Status = SystemTable->BootServices->LocateProtocol(
        &gEfiSimpleFileSystemProtocolGuid,
        NULL,
        (VOID **)&FileSystem
    );

    if (EFI_ERROR(Status))
        return Status;


    Status = FileSystem->OpenVolume(
        FileSystem,
        &Root
    );

    if (EFI_ERROR(Status))
        return Status;


    Status = Root->Open(
        Root,
        &Directory,
        Position,
        EFI_FILE_MODE_READ,
        EFI_FILE_DIRECTORY
    );

    if (EFI_ERROR(Status))
        return Status;


    while (1)
    {
        InfoSize = 0;

        Directory->Read(
            Directory,
            &InfoSize,
            NULL
        );


        if (InfoSize == 0)
            break;


        Status = SystemTable->BootServices->AllocatePool(
            EfiLoaderData,
            InfoSize,
            (VOID **)&Info
        );

        if (EFI_ERROR(Status))
            break;
            // nooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo


        Status = Directory->Read(
            Directory,
            &InfoSize,
            Info
        );


        if (EFI_ERROR(Status))
        {
            SystemTable->BootServices->FreePool(Info);
            break;
        }


        UINTN NameSize = 0;

        while (Info->FileName[NameSize] != L'\0')
        {
            NameSize++;
        }

        if ((Offset + NameSize + 3) < DirDataSize)
        {
            for (UINTN i = 0; i < NameSize; i++)
            {
                DirData[Offset++] = Info->FileName[i];
            }
            // cooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooool
            DirData[Offset++] = L'\r';
            DirData[Offset++] = L'\n';
            DirData[Offset] = L'\0';
        }


        SystemTable->BootServices->FreePool(Info);
    }


    Directory->Close(Directory);

    return EFI_SUCCESS;
}