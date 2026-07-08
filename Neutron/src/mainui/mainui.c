#include <Uefi.h>
#include <Library/UefiLib.h>
#include <rshell.h>
#include "mainui.h"

typedef struct {
    EFI_SYSTEM_TABLE *SystemTable;
    UINTN SelectedIndex;
    BOOLEAN IsInitialized;
} UIState;

static UIState gUIState = {NULL, 0, FALSE};

EFI_STATUS InitUI(EFI_SYSTEM_TABLE *SystemTable)
{
    if (SystemTable == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    
    gUIState.SystemTable = SystemTable;
    gUIState.SelectedIndex = 0;
    gUIState.IsInitialized = TRUE;
    
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
    }
    
    return 80;
}

static UINTN GetConsoleHeight(EFI_SYSTEM_TABLE *SystemTable)
{
    if (SystemTable == NULL || SystemTable->ConOut == NULL) {
        return 25;
    }
    
    UINTN Columns = 80;
    UINTN Rows = 25;
    EFI_STATUS Status = SystemTable->ConOut->QueryMode(SystemTable->ConOut, 0, &Columns, &Rows);
    if (!EFI_ERROR(Status)) {
        return Rows;
    }
    
    return 25;
}

VOID DrawBox(EFI_SYSTEM_TABLE *SystemTable, UINTN X, UINTN Y, UINTN Width, UINTN Height)
{
    if (Width < 2 || Height < 2) {
        return;
    }
    
    UINTN BottomY = Y + Height - 1;
    UINTN RightX = X + Width - 1;
    
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, X, Y);
    printf(L"+");
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, RightX, Y);
    printf(L"+");
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, X, BottomY);
    printf(L"+");
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, RightX, BottomY);
    printf(L"+");
    
    for (UINTN i = X + 1; i < RightX; i++) {
        SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, i, Y);
        printf(L"-");
        SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, i, BottomY);
        printf(L"-");
    }
    
    for (UINTN i = Y + 1; i < BottomY; i++) {
        SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, X, i);
        printf(L"|");
        SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, RightX, i);
        printf(L"|");
    }
}

VOID RenderMenu(EFI_SYSTEM_TABLE *SystemTable, 
                MenuConfig *Config, 
                UINTN SelectedIndex,
                UINTN ScrollOffset)
{
    (void)ScrollOffset;
    
    if (!Config || Config->count == 0) {
        printfc(EFICOLOR_LIGHTRED, L"No menu entries found!\r\n");
        return;
    }
    
    UINTN ConsoleWidth = GetConsoleWidth(SystemTable);
    UINTN ConsoleHeight = GetConsoleHeight(SystemTable);
    
    UINTN BoxWidth = ConsoleWidth > 70 ? 70 : (ConsoleWidth > 40 ? ConsoleWidth - 2 : 38);
    if (BoxWidth > 70) BoxWidth = 70;
    if (BoxWidth < 20) BoxWidth = 20;
    
    UINTN MenuHeight = Config->count + 2;
    if (MenuHeight < 6) MenuHeight = 6;
    if (MenuHeight + 7 > ConsoleHeight) {
        MenuHeight = ConsoleHeight - 7;
    }
    if (MenuHeight < 6) MenuHeight = 6;
    
    UINTN InfoHeight = 5;
    if (InfoHeight < 3) InfoHeight = 3;
    
    UINTN TotalHeight = MenuHeight + InfoHeight;
    if (TotalHeight + 2 > ConsoleHeight) {
        TotalHeight = ConsoleHeight - 2;
        InfoHeight = TotalHeight < MenuHeight ? 3 : (TotalHeight - MenuHeight);
        if (InfoHeight > 5) InfoHeight = 5;
        if (InfoHeight < 3) InfoHeight = 3;
        MenuHeight = TotalHeight - InfoHeight;
        if (MenuHeight < 6) MenuHeight = 6;
    }
    
    UINTN BoxX = (ConsoleWidth - BoxWidth) / 2;
    UINTN BoxY = (ConsoleHeight - TotalHeight) / 2;
    if (BoxY < 1) BoxY = 1;
    if (BoxY + TotalHeight > ConsoleHeight - 1) {
        BoxY = ConsoleHeight - 1 - TotalHeight;
    }
    
    UINTN MenuY = BoxY;
    UINTN InfoY = MenuY + MenuHeight;
    
    UINTN RightX = BoxX + BoxWidth - 1;
    
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, 0, 0);
    printfc(EFICOLOR_LIGHTBLUE, L"Please Choose Your Operating System To Load, By using ");
    printfc(EFICOLOR_WHITE, L"<");
    printfc(EFICOLOR_LIGHTBLUE, L" or ");
    printfc(EFICOLOR_WHITE, L">");
    printfc(EFICOLOR_LIGHTBLUE, L". If You Have Boot Problems Here, Please Press ");
    printfc(EFICOLOR_WHITE, L"CTRL + S");
    printfc(EFICOLOR_LIGHTBLUE, L" To Open The Recovery Shell.\r\n");
    
    DrawBox(SystemTable, BoxX, MenuY, BoxWidth, MenuHeight);
    
    UINTN MenuStartY = MenuY + 1;
    UINTN MaxEntries = MenuHeight - 2;
    if (MaxEntries == 0) MaxEntries = 1;
    if (SelectedIndex >= MaxEntries) {
        ScrollOffset = SelectedIndex - MaxEntries + 1;
    }
    
    for (UINTN i = 0; i < Config->count; i++) {
        if (i < ScrollOffset) {
            continue;
        }
        
        UINTN DisplayIndex = i - ScrollOffset;
        if (DisplayIndex >= MaxEntries) {
            break;
        }
        
        UINTN RowY = MenuStartY + DisplayIndex;
        if (RowY >= MenuY + MenuHeight - 1) {
            break;
        }
        
        SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, BoxX + 2, RowY);
        
        if (i == SelectedIndex) {
            printfc(EFICOLOR_LIGHTGREEN, L"%s", Config->entries[i].name);
            if (RightX - 6 >= BoxX + 2) {
                SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, RightX - 6, RowY);
                printfc(EFICOLOR_YELLOW, L"|<<<");
            }
        } else {
            printf(L"%s", Config->entries[i].name);
            SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, RightX - 1, RowY);
            printf(L"|");
        }
        
        SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, BoxX + 1, RowY);
        printf(L"* ");
    }
    
    DrawBox(SystemTable, BoxX, InfoY, BoxWidth, InfoHeight);
    
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, BoxX + 2, InfoY + 1);
    printfc(EFICOLOR_LIGHTBLUE, L"Name: ");
    if (SelectedIndex < Config->count) {
        printfc(EFICOLOR_WHITE, L"%s", Config->entries[SelectedIndex].name);
    }
    
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, BoxX + 2, InfoY + 2);
    printfc(EFICOLOR_LIGHTBLUE, L"About: ");
    if (SelectedIndex < Config->count) {
        printfc(EFICOLOR_WHITE, L"%s", Config->entries[SelectedIndex].about);
    }
    
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, BoxX + 2, InfoY + 3);
    printfc(EFICOLOR_LIGHTBLUE, L"Version: ");
    if (SelectedIndex < Config->count) {
        printfc(EFICOLOR_WHITE, L"%s", Config->entries[SelectedIndex].version);
    }
    
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, BoxX + 2, InfoY + 4);
    printfc(EFICOLOR_LIGHTBLUE, L"Type: ");
    if (SelectedIndex < Config->count) {
        printfc(EFICOLOR_WHITE, L"%s", Config->entries[SelectedIndex].type);
    }
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFICOLOR_LIGHTGRAY);
}

static BOOLEAN IsCtrlSKey(const EFI_INPUT_KEY *Key)
{
    return (Key->UnicodeChar == 0x13);
}

INT32 DisplayMenu(EFI_SYSTEM_TABLE *SystemTable, MenuConfig *Config)
{
    if (!Config || Config->count == 0) {
        return -1;
    }
    
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
    UINTN SelectedIndex = 0;
    UINTN ScrollOffset = 0;
    BOOLEAN Exit = FALSE;
    
    InitUI(SystemTable);
    
    while (!Exit) {
        RenderMenu(SystemTable, Config, SelectedIndex, ScrollOffset);
        
        UINTN Index;
        Status = SystemTable->BootServices->WaitForEvent(
            1,
            &SystemTable->ConIn->WaitForKey,
            &Index);
        
        if (EFI_ERROR(Status)) {
            continue;
        }
        
        Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
        if (EFI_ERROR(Status)) {
            continue;
        }
        
        if (IsCtrlSKey(&Key)) {
            return -1;
        }
        
        if (Key.ScanCode == SCAN_UP) {
            if (SelectedIndex > 0) {
                SelectedIndex--;
                UINTN MaxEntries = GetConsoleHeight(SystemTable) - 7;
                if (MaxEntries == 0) {
                    MaxEntries = 1;
                }
                if (SelectedIndex < ScrollOffset) {
                    ScrollOffset = SelectedIndex;
                }
            }
        } else if (Key.ScanCode == SCAN_DOWN) {
            if (SelectedIndex < Config->count - 1) {
                SelectedIndex++;
                UINTN MaxEntries = GetConsoleHeight(SystemTable) - 7;
                if (MaxEntries == 0) {
                    MaxEntries = 1;
                }
                if (SelectedIndex >= ScrollOffset + MaxEntries) {
                    ScrollOffset = SelectedIndex - MaxEntries + 1;
                }
            }
        } else if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
            Exit = TRUE;
        }
    }
    
    return (INT32)SelectedIndex;
}

VOID CleanupUI(VOID)
{
    gUIState.IsInitialized = FALSE;
}
