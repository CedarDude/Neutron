#include <Uefi.h>
#include <neulib.h>
#include <NLIB.h>
#include <rshell.h>
#include <fpi.h>
#include "mainui/mainui.h"
#include "mainui/config_parser.h"
#include "utils.h"

EFI_SYSTEM_TABLE *gST = NULL;

EFI_STATUS efi_waitkey(EFI_SYSTEM_TABLE *SystemTable)
{
    UINTN Index;
    EFI_INPUT_KEY Key;

    SystemTable->BootServices->WaitForEvent(
        1,
        &SystemTable->ConIn->WaitForKey,
        &Index);

    SystemTable->ConIn->ReadKeyStroke(
        SystemTable->ConIn,
        &Key);
    
    if (Key.UnicodeChar == 0x13) {
        initshl(SystemTable);
    }

    return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    (void)ImageHandle;
    gST = SystemTable;
    
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    
    MenuConfig Config;
    EFI_STATUS Status = ParseLoaderConfig(SystemTable, L"\\EFI\\BOOT\\LOADER.CFG", &Config);
    
    if (EFI_ERROR(Status) || Config.count == 0) {
        printf(L"cannot load config.\r\n");
        efi_waitkey(SystemTable);
        return Status;
    }

    INT32 Selected = DisplayMenu(SystemTable, &Config);
    
    if (Selected == -1) {
        SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
        initshl(SystemTable);
    } else if (Selected >= 0 && Selected < (INT32)Config.count) {
        SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
        printfc(EFICOLOR_YELLOW, L"[BOOT PLACEHOLDER] Selected: %s\r\n", Config.entries[Selected].name);
        printf(L"Boot execution disabled in this build.\r\n");
        printf(L"Press any key to exit...\r\n");
        efi_waitkey(SystemTable);
    }

    return EFI_SUCCESS;
}
