#include <Uefi.h>
#include <neulib.h>

 /* 
Razzle (c) Neutron EFI
======
Copyright : 2026 JAD
LICENSE   : GPLv3
Milestone : 1
===== END INFO =====
*/ 

EFI_SYSTEM_TABLE *gST = NULL;

EFI_STATUS efi_waitkey(EFI_SYSTEM_TABLE *SystemTable) {
    UINTN Index;
    EFI_INPUT_KEY Key;

    SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &Index);
    SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

    return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    gST = SystemTable;

    printf(L"Hello, World!\r\nNeutron EFI App Had Loaded!\r\n");
    printf(L"%cColor Demo: %d, %x, %s\r\n", EFI_LIGHTGREEN, 123, 0xABC, L"Success");
	
	
	
	printf(L" _   _            _                   \r\n");
	printf(L" | \\ | |          | |                  \r\n");
	printf(L" |  \\| | ___ _   _| |_ _ __ ___  _ __  \r\n");
	printf(L" | . ` |/ _ \\ | | | __| '__/ _ \\| '_ \\ \r\n");
	printf(L" | |\\  |  __/ |_| | |_| | | (_) | | | |\r\n");
	printf(L" |_| \\_|\\___|\\__,_|\\__|_|  \\___/|_| |_|\r\n");
	
	
    efi_waitkey(SystemTable);

    return EFI_SUCCESS;
}
