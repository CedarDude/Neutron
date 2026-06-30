#include <Uefi.h>
#include <neulib.h>
#include <NLIB.h>

/*
Razzle (c) Neutron EFI
======
Copyright : 2026 JAD
LICENSE   : GPLv3
Milestone : 1
===== END INFO =====
*/

EFI_SYSTEM_TABLE *gST = NULL;

STATIC CONST CHAR16 *MonthNames[] = {
    L"",
    L"January", L"February", L"March", L"April",
    L"May", L"June", L"July", L"August",
    L"September", L"October", L"November", L"December"
};

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

    return EFI_SUCCESS;
}

static VOID print2(UINTN n)
{
    if (n < 10)
        printf(L"0");

    printf(L"%d", n);
}
EFI_STATUS efi_clock(UINTN X, UINTN Y)
{
    EFI_TIME Time;
    EFI_STATUS Status;
    UINTN Index;

    while (1)
    {
        // Check if a key has been pressed without blocking da clock
        Status = gST->BootServices->CheckEvent(gST->ConIn->WaitForKey);
        if (Status == EFI_SUCCESS) {
            // A key is waiting! fuck it and exit!
            EFI_INPUT_KEY Key;
            gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
            break; 
        }

        Status = gST->RuntimeServices->GetTime(&Time, NULL);
        if (EFI_ERROR(Status))
            return Status;

        gST->ConOut->SetCursorPosition(gST->ConOut, X, Y);

        printfc(EFICOLOR_LIGHTGREEN, L"Current Date: ");
        printf(L"%s ", MonthNames[Time.Month]);
        print2(Time.Day);
        printf(L", %d  ", Time.Year);

        print2(Time.Hour);
        printf(L":");
        print2(Time.Minute);
        printf(L":");
        print2(Time.Second);
        printf(L"      ");

        gST->BootServices->Stall(1000000);
    }

    return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    (void)ImageHandle;
    gST = SystemTable;
    
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    
    printf(L"Hello, World!\r\n");
    printf(L"Neutron EFI App Had Loaded!\r\n");

    printfc(
        EFICOLOR_LIGHTGREEN,
        L"Color Demo: %d, %x, %s\r\n",
        123,
        0xABC,
        L"Success");

    printf(L"\r\n");

    printf(L" _   _            _                    \r\n");
    printf(L"| \\ | |          | |                   \r\n");
    printf(L"|  \\| | ___ _   _| |_ _ __ ___  _ __  \r\n");
    printf(L"| . ` |/ _ \\ | | | __| '__/ _ \\| '_ \\ \r\n");
    printf(L"| |\\  |  __/ |_| | |_| | | (_) | | | |\r\n");
    printf(L"|_| \\_|\\___|\\__,_|\\__|_|  \\___/|_| |_|\r\n");

    printf(L"\r\n");

    UINTN ClockX = gST->ConOut->Mode->CursorColumn;
    UINTN ClockY = gST->ConOut->Mode->CursorRow;
 
    
    printf(L"\r\n"); 


    printfc(EFICOLOR_LIGHTBLUE, L"TIME ==========================================\r\n");
    CHAR16 cpu_buffer[49];
    get_cpu_name(cpu_buffer);
    printf(L"Collecting Hardware Information...\r\n");
	printf(L"CPU Name: %s\r\n", cpu_buffer);
	UINT64 ram_bytes = get_total_ram();
	printf(L"Total RAM: %d MB\r\n", ram_bytes / (1024 * 1024));
    printf(L"Press any key to exit...\r\n");
	
    efi_clock(ClockX, ClockY);

    // wait for the user to close this disgusting useless shitty efi app.
	// that waste just your computer potential.
	// idk may be useful
	// but idk if it useful.
    efi_waitkey(SystemTable);

    return EFI_SUCCESS;
}