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

static VOID GetConsoleMetrics(EFI_SYSTEM_TABLE *SystemTable, UINTN *OutWidth, UINTN *OutHeight)
{
    INT32 CurrentMode = 0;
    UINTN Cols = 80;
    UINTN Rows = 25;
    
    if (SystemTable != NULL && SystemTable->ConOut != NULL && SystemTable->ConOut->Mode != NULL) {
        CurrentMode = SystemTable->ConOut->Mode->Mode;
        EFI_STATUS Status = SystemTable->ConOut->QueryMode(SystemTable->ConOut, (UINTN)CurrentMode, &Cols, &Rows);
        if (EFI_ERROR(Status)) {
            Status = SystemTable->ConOut->QueryMode(SystemTable->ConOut, 0, &Cols, &Rows);
            if (EFI_ERROR(Status)) {
                Cols = 80;
                Rows = 25;
            }
        }
    }
    
    *OutWidth = Cols;
    *OutHeight = Rows;
}

static UINTN StringLength(const CHAR16 *Str)
{
    UINTN Len = 0;
    while (Str[Len] != L'\0') Len++;
    return Len;
}

static VOID WriteCentered(EFI_SYSTEM_TABLE *SystemTable, UINTN Row, const CHAR16 *Text)
{
    UINTN Width = 0;
    UINTN Height = 0;
    GetConsoleMetrics(SystemTable, &Width, &Height);
    
    UINTN TextLen = StringLength(Text);
    UINTN Col = (Width - TextLen) / 2;
    if (Col > Width) {
        Col = 0;
    }
    
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, Col, Row);
    printf(L"%s", Text);
}

static VOID ClearRow(EFI_SYSTEM_TABLE *SystemTable, UINTN Row)
{
    UINTN Width = 0;
    UINTN Height = 0;
    GetConsoleMetrics(SystemTable, &Width, &Height);
    
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, 0, Row);
    for (UINTN i = 0; i < Width; i++) {
        printf(L" ");
    }
}
// THIS TOOK A LOT OF TIME FOR THIS.... AGHHH
// IT WORTH EVERY SECOND!
VOID RenderMenu(EFI_SYSTEM_TABLE *SystemTable, 
                MenuConfig *Config, 
                UINTN SelectedIndex,
                UINTN ScrollOffset)
{
    (void)ScrollOffset;
    
    if (!Config || Config->count == 0) {
        printf(L"No menu entries found!\r\n");
        return;
    }
    
    UINTN Width = 0;
    UINTN Height = 0;
    GetConsoleMetrics(SystemTable, &Width, &Height);
    
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFICOLOR_LIGHTBLUE);
    ClearRow(SystemTable, 0);
    WriteCentered(SystemTable, 0, L"Neutron Boot Manager");
    
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFICOLOR_LIGHTGRAY);
    ClearRow(SystemTable, 2);
    WriteCentered(SystemTable, 2, L"Please choose the operating system to start:");
    
    UINTN CenterY = Height / 2;
    if (CenterY < 6) CenterY = 6;
    if (CenterY > Height - 10) CenterY = Height - 10;
    
    INTN StartIdx = (INTN)SelectedIndex;
    if (StartIdx > (INTN)(SelectedIndex - 2)) StartIdx = (INTN)(SelectedIndex - 2);
    if (StartIdx < 0) StartIdx = 0;
    
    INTN EndIdx = (INTN)SelectedIndex + 3;
    if (EndIdx > (INTN)Config->count) EndIdx = (INTN)Config->count;
    
    UINTN VisualIndex = 0;
    for (INTN i = StartIdx; i < EndIdx; i++) {
        UINTN Row = CenterY - 2 + VisualIndex;
        if (Row >= Height - 6) break;
        
        ClearRow(SystemTable, Row);
        
        if ((UINTN)i == SelectedIndex) {
            UINTN ArrowLen = 4;
            UINTN NameLen = StringLength(Config->entries[i].name);
            UINTN TotalLen = ArrowLen + NameLen;
            UINTN Col = (Width - TotalLen) / 2;
            if (Col > Width) Col = 0;
            
            SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, Col, Row);
            printfc(EFICOLOR_LIGHTGREEN, L">> ");
            printfc(EFICOLOR_YELLOW, L"%s", Config->entries[i].name);
            printfc(EFICOLOR_YELLOW, L" <<");
        } else {
            UINTN NameLen = StringLength(Config->entries[i].name);
            UINTN Col = (Width - NameLen) / 2;
            if (Col > Width) Col = 0;
            
            SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, Col, Row);
            printf(L"%s", Config->entries[i].name);
        }
        
        VisualIndex++;
    }
    
    if (SelectedIndex < Config->count) {
        UINTN InfoRow = Height - 4;
        if (InfoRow < CenterY + 3) InfoRow = CenterY + 3;
        if (InfoRow + 1 >= Height) InfoRow = Height - 2;
        
        SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFICOLOR_DARKGRAY);
        ClearRow(SystemTable, InfoRow);
        WriteCentered(SystemTable, InfoRow, L"");
        
        printfc(EFICOLOR_DARKGRAY, L"Name: ");
        printf(L"%s  ", Config->entries[SelectedIndex].name);
        // if you triple fault at me again i will find you and beat you until you are dead
        printfc(EFICOLOR_DARKGRAY, L"About: ");
        printf(L"%s", Config->entries[SelectedIndex].about);
        
        ClearRow(SystemTable, InfoRow + 1);
        WriteCentered(SystemTable, InfoRow + 1, L"");
        
        printfc(EFICOLOR_DARKGRAY, L"Version: ");
        printf(L"%s  ", Config->entries[SelectedIndex].version);
        printfc(EFICOLOR_DARKGRAY, L"Type: ");
        printf(L"%s", Config->entries[SelectedIndex].type);
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
    BOOLEAN Exit = FALSE;
    
    InitUI(SystemTable);
    
    while (!Exit) {
        RenderMenu(SystemTable, Config, SelectedIndex, 0);
        
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
            }
        } else if (Key.ScanCode == SCAN_DOWN) {
            if (SelectedIndex < Config->count - 1) {
                SelectedIndex++;
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
// kelin dat ting
