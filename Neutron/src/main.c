#include <Uefi.h>

/* 
Razzle (c) Neutron EFI
======

Copyright : 2026 JAD
LICENSE   : GPLv3
Milestone : 1

===== END INFO =====
*/

EFI_STATUS efi_writetext(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *Text) {
    return SystemTable->ConOut->OutputString(SystemTable->ConOut, Text);
}

EFI_STATUS efi_waitkey(EFI_SYSTEM_TABLE *SystemTable) {
    EFI_INPUT_KEY Key;
    UINTN Index;

    SystemTable->BootServices->WaitForEvent(
        1,
        &SystemTable->ConIn->WaitForKey,
        &Index
    );

    SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

    return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    CHAR16 Text[] = L"Hello, World!\r\nNeutron EFI App Had Loaded!";

    efi_writetext(SystemTable, Text);
    efi_waitkey(SystemTable);

    return EFI_SUCCESS;
}