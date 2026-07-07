#include <Uefi.h>
#include <neulib.h>
#include <NLIB.h>
#include <kuh.h>
#include <UefiLib.h>
#include <branding.h>
#include <UefiBootServicesTableLib.h>
#include <Protocol/PciIo.h>
#include <fpi.h>
#include <Pci.h>

/*
Razzle (c) 2026 - Neutron EFI Application Project
- Open Source                    LICENSE GPLv3
- Created by:                    Jad
- Used:
        EDK II                                                      -           https://github.com
        Orca.net Printf Implementation - Heavely Modified           -           https://github.com
        The Rest Is From Me (Jad)
*/

static const EFI_GUID LocalPciIoProtocolGuid = { 
    0x4CF5B200, 0x68B8, 0x4CA5, { 0x9E, 0xEC, 0xB2, 0x3E, 0x3F, 0x50, 0x02, 0x9A } 
};

static BOOLEAN StrEqual(const CHAR16 *A, const CHAR16 *B)
{
    while (*A && *B) {
        if (*A != *B)
            return FALSE;
        A++;
        B++;
    }
    return *A == *B;
}

static void PrintShellPrompt(void)
{
    printfc(EFICOLOR_YELLOW, L"EFI/BOOT/> ");
}

void ClearScreen(void)
{
    gST->ConOut->ClearScreen(gST->ConOut);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
}

/**
 scan
**/
EFI_STATUS GetGpuIdentifiers(UINT16 *VendorId, UINT16 *DeviceId)
{
    EFI_STATUS          Status;
    UINTN               HandleCount;
    EFI_HANDLE          *HandleBuffer;
    UINTN               Index;
    EFI_PCI_IO_PROTOCOL *PciIo;
    PCI_DEVICE_INDEPENDENT_REGION PciHeader;

    if (VendorId == NULL || DeviceId == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    // 1. Locate all hardware handles carrying the PCI I/O protocol
    Status = gST->BootServices->LocateHandleBuffer (
                                  ByProtocol,
                                  (EFI_GUID *)&LocalPciIoProtocolGuid,
                                  NULL,
                                  &HandleCount,
                                  &HandleBuffer
                                  );
    if (EFI_ERROR (Status)) {
        return Status;
    }

    // loops through every device
    for (Index = 0; Index < HandleCount; Index++) {
        Status = gST->BootServices->OpenProtocol (
                                      HandleBuffer[Index],
                                      (EFI_GUID *)&LocalPciIoProtocolGuid,
                                      (VOID **)&PciIo,
                                      NULL,
                                      NULL,
                                      EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                      );
        if (EFI_ERROR (Status)) {
            continue;
        }

        // read std thing
        // too lazy to say standard...
        Status = PciIo->Pci.Read (
                              PciIo,
                              EfiPciIoWidthUint8,
                              0, 
                              sizeof (PCI_DEVICE_INDEPENDENT_REGION),
                              &PciHeader
                              );

        if (!EFI_ERROR (Status)) {
            // 4. Target Class Code 0x03 (display stuff)
            if (PciHeader.ClassCode == 0x03) {
                *VendorId = PciHeader.VendorId;
                *DeviceId = PciHeader.DeviceId;
                
                gST->BootServices->FreePool (HandleBuffer);
                return EFI_SUCCESS;
            }
        }
    }

    gST->BootServices->FreePool (HandleBuffer);
    return EFI_NOT_FOUND;
}

/**
  Resolves known pci things to their vendor names
**/
const CHAR16* GetVendorName(UINT16 VendorId)
{
    switch (VendorId) {
        case 0x10DE: return L"NVIDIA GPU";
        case 0x1002: return L"Advanced Micro Devices, Inc. (AMD)";
        case 0x8086: return L"Intel Corporation";
        default:     return L"DEFAULT_VENDOR";
    }
}

void GetGPUInfo(void)
{
    UINT16 VendorId = 0;
    UINT16 DeviceId = 0;
    EFI_STATUS Status;

    Status = GetGpuIdentifiers(&VendorId, &DeviceId);

    if (EFI_ERROR(Status)) {
        printfc(EFICOLOR_RED, L"GPU Information: Display Controller not found on PCI bus.\r\n");
        return;
    }

    // Print out the extracted hex IDs along with the resolved vendor string
    printfc(EFICOLOR_LIGHTCYAN, L"GPU Information: %s (VID: 0x%04X, DID: 0x%04X)\r\n", 
            GetVendorName(VendorId), VendorId, DeviceId);
}

void initshl(EFI_SYSTEM_TABLE *SystemTable)
{
    ClearScreen();
    print_branding_info();
    printf(L"Type 'help' for commands, 'exit' to leave shell.\r\nRemember, the UEFI give you QWERTY, as it not my fault, blame them.\r\n");

    CHAR16 buffer[256];
    while (TRUE) {
        PrintShellPrompt();
        keyboard_input(buffer, sizeof(buffer) / sizeof(buffer[0]));

        if (buffer[0] == L'\0') {
            continue;
        }

        if (StrEqual(buffer, L"help")) {
            printf(L"Available commands:\r\n");
            printf(L"  help     - Show this help text\r\n");
            printf(L"  branding - Print branding information\r\n");
            printf(L"  clear    - Clear the screen\r\n");
            printf(L"  cls      - Clear the screen\r\n");
            printf(L"  gpuinfo  - Identify the primary graphics hardware\r\n");
            printf(L"  exit     - Exit the shell\r\n\r\n");
            continue;
        }

        if (StrEqual(buffer, L"branding")) {
            print_branding_info();
            continue;
        }

        if (StrEqual(buffer, L"clear") || StrEqual(buffer, L"cls")) {
            gST->ConOut->ClearScreen(gST->ConOut);
            continue;
        }

        if (StrEqual(buffer, L"exit")) {
            printf(L"Exiting recovery shell...\r\n");
            break;
        }

        // DIR AREA

        if (StrEqual(buffer, L"dir") || StrEqual(buffer, L"ls"))
        {
            static CHAR16 directory_data[2048];
        
            EFI_STATUS Status;
        
            Status = Dir(
                SystemTable,
                L"\\EFI\\BOOT",
                directory_data,
                2048
            );
        
            if (!EFI_ERROR(Status))
            {
                printf(L"%s", directory_data);
            }
            else
            {
                printf(L"Directory error: %r\r\n", Status);
            }
        }

        if (StrEqual(buffer, L"gpuinfo")) {
            GetGPUInfo();

            continue;
        }
        
        printfc(EFICOLOR_RED, L"Unknown command: %s\r\n", buffer);
    }
}
