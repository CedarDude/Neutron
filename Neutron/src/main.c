#include <Uefi.h>
#include <neulib.h>
#include <NLIB.h>
#include <rshell.h>
#include <fpi.h>
#include <mainui.h>
#include <config_parser.h>
#include <utils.h>

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

static UINTN GetConsoleWidth(EFI_SYSTEM_TABLE *SystemTable)
{
    if (SystemTable == NULL || SystemTable->ConOut == NULL) {
        return 80;
    }
    
    UINTN Columns = 80;
    UINTN Rows = 25;
    EFI_STATUS Status = SystemTable->ConOut->QueryMode(SystemTable->ConOut, 0, &Columns, &Rows);
    if (!EFI_ERROR(Status)) {
        return Columns;
        // get vood coooooooooooool nembers to celcule da shi <- i should turn this into a quote

    }
    
    return 80;
}

static VOID PrintCentered(EFI_SYSTEM_TABLE *SystemTable, UINTN Row, const CHAR16 *Text, UINTN Color)
{
    UINTN ConsoleWidth = GetConsoleWidth(SystemTable);
    UINTN TextLen = 0;
    const CHAR16 *Temp = Text;
    while (*Temp) {
        TextLen++;
        Temp++;
    }
    
    UINTN Col = (ConsoleWidth - TextLen) / 2;
    if (Col > ConsoleWidth) {
        Col = 0;
    }
    
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, Col, Row);
    printfc(Color, L"%s", Text);
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
        SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFICOLOR_LIGHTGRAY);
        PrintCentered(SystemTable, 10, L"Early Build", EFICOLOR_RED);
        PrintCentered(SystemTable, 11, Config.entries[Selected].name, EFICOLOR_LIGHTGRAY);
        PrintCentered(SystemTable, 13, L"Boot execution disabled in this build.", EFICOLOR_LIGHTBLUE);
        PrintCentered(SystemTable, 14, L"Press any key to exit...", EFICOLOR_WHITE);
        efi_waitkey(SystemTable);
    }

    return EFI_SUCCESS;
}
