#include <Uefi.h>
#include <neulib.h>
#include <NLIB.h>
#include <kuh.h>
#include <UefiLib.h>
#include <branding.h>

/*
Razzle (c) 2026 - Neutron EFI Application Project
- Open Source                    LICENSE GPLv3
- Created by:                    Jad
- Used:
        EDK II                                                      -           https://github.com/tianocore/edk2
        Orca.net Printf Implementation - Heavely Modified           -           https://github.com/orca-pet3910YT/C-kernel-OS
        The Rest Is From Me (Jad)
*/


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

void initshl(void)
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

        printfc(EFICOLOR_RED, L"Unknown command: %s\r\n", buffer);
    }
}