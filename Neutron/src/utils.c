#include <Uefi.h>
#include <neulib.h>
#include <NLIB.h>
#include <rshell.h>
#include <fpi.h>
#include "utils.h"

static CONST CHAR16 *MonthNames[] = {
    L"",
    L"January", L"February", L"March", L"April",
    L"May", L"June", L"July", L"August",
    L"September", L"October", L"November", L"December"
};

static VOID Print2Digits(UINTN n)
{
    if (n < 10)
        printf(L"0");
    printf(L"%d", n);
}

static BOOLEAN IsCtrlSKey(const EFI_INPUT_KEY *Key)
{
    return (Key->UnicodeChar == 0x13);
}

extern EFI_SYSTEM_TABLE *gST;

EFI_STATUS DisplayClock(EFI_SYSTEM_TABLE *SystemTable, UINTN X, UINTN Y)
{
    EFI_TIME Time;
    EFI_STATUS Status;

    while (1)
    {
        // Check if a key has been pressed without blocking the clock
        Status = SystemTable->BootServices->CheckEvent(SystemTable->ConIn->WaitForKey);
        if (Status == EFI_SUCCESS) {
            // A key is waiting, exit!
            EFI_INPUT_KEY Key;
            SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
            if (IsCtrlSKey(&Key)) {
                initshl(SystemTable);
            }
            break; 
        }

        Status = SystemTable->RuntimeServices->GetTime(&Time, NULL);
        if (EFI_ERROR(Status))
            return Status;

        SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, X, Y);

        printfc(EFICOLOR_LIGHTGREEN, L"Current Date: ");
        printf(L"%s ", MonthNames[Time.Month]);
        Print2Digits(Time.Day);
        printf(L", %d  ", Time.Year);

        Print2Digits(Time.Hour);
        printf(L":");
        Print2Digits(Time.Minute);
        printf(L":");
        Print2Digits(Time.Second);
        printf(L"      ");

        SystemTable->BootServices->Stall(1000000);
    }

    return EFI_SUCCESS;
}

VOID DisplayNeutronBanner(EFI_SYSTEM_TABLE *SystemTable)
{
    (void)SystemTable;
    printf(L"Neutron Loader\r\n");
    printf(L"=======\r\n");
    printf(L"\r\n");
    printf(L" _   _            _                    \r\n");
    printf(L"| \\ | |          | |                   \r\n");
    printf(L"|  \\| | ___ _   _| |_ _ __ ___  _ __  \r\n");
    printf(L"| . ` |/ _ \\ | | | __| '__/ _ \\| '_ \\ \r\n");
    printf(L"| |\\  |  __/ |_| | |_| | | (_) | | | |\r\n");
    printf(L"|_| \\_|\\___|\\__,_|\\__|_|  \\___/|_| |_|\r\n");
    printf(L"\r\n");
}

VOID DisplayHardwareInfo(EFI_SYSTEM_TABLE *SystemTable)
{
    (void)SystemTable;
    CHAR16 cpu_buffer[49];
    get_cpu_name(cpu_buffer);
    printf(L"Collecting Hardware Information...\r\n");
    printf(L"CPU Name: %s\r\n", cpu_buffer);
    
    UINT64 ram_bytes = get_total_ram();
    printf(L"Total RAM: %d MB\r\n", ram_bytes / (1024 * 1024));
}

VOID DisplayFileContents(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *FilePath)
{
    VOID *cool_buffer;
    UINTN cool_size;
    EFI_STATUS FileStatus;
    
    FileStatus = UseFile(SystemTable, FilePath, &cool_buffer, &cool_size);
    
    if (!EFI_ERROR(FileStatus))
    {
        printf(L"File contents:\r\n");
    
        UINT8 *raw = (UINT8 *)cool_buffer;
        UINTN offset = 0;
    
        if (cool_size >= 2 && raw[0] == (UINT8)0xFE && raw[1] == (UINT8)0xFF)
        {
            offset = 2;
        }
    
        while (offset + 1 < cool_size)
        {
            CHAR16 character;
            character = ((CHAR16)raw[offset] << 8) |
                        (CHAR16)raw[offset + 1];
            printf(L"%c", character);
            offset += 2;
        }
    
        printf(L"\r\n");
        FreeFileMem(cool_buffer);
    }
    else
    {
        printf(L"File error: %r\r\n", FileStatus);
    }
}
