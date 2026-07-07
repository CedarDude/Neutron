#include <Uefi.h>
#include <stdarg.h>
#include <neulib.h>
#include <kuh.h>
#include <rshell.h>

/*
Razzle (c) 2026 - Neutron EFI Application Project
- Open Source                    LICENSE GPLv3
- Created by:                    Jad
- Used:
        EDK II                                                      -           https://github.com/tianocore/edk2
        Orca.net Printf Implementation - Heavely Modified           -           https://github.com/orca-pet3910YT/C-kernel-OS
        The Rest Is From Me (Jad)
*/

/* shity converter*/
int efi_atoi(const CHAR16 *str, const CHAR16 **endptr) {
    int res = 0;
    int sign = 1;
    
    while (*str == L' ' || *str == L'\t') str++;
    
    if (*str == L'-') {
        sign = -1;
        str++;
    } else if (*str == L'+') {
        str++;
    }
    
    while (*str >= L'0' && *str <= L'9') {
        res = res * 10 + (*str - L'0');
        str++;
    }
    
    if (endptr) *endptr = str;
    return sign * res;
}

int scanf(const CHAR16 *buf, const CHAR16 *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int fields_read = 0;

    while (*fmt && *buf) {
        /* Skip whitespace in format string so we don't fuck it*/
        if (*fmt == L' ') {
            while (*buf == L' ' || *buf == L'\t') buf++;
            fmt++;
            continue;
        }

        /* parse that fucker */
        if (*fmt == L'%') {
            fmt++; 
            
            if (*fmt == L'd') {
                int *val = va_arg(args, int *);
                const CHAR16 *next;
                *val = efi_atoi(buf, &next);
                if (buf == next) break; /* Parsing failed,
                if i was you, pc, i would triple fault this
                entire fucking efi app.
                 */
                buf = next;
                fields_read++;
            } 
            else if (*fmt == L's') { 
                CHAR16 *out_str = va_arg(args, CHAR16 *);
                while (*buf == L' ' || *buf == L'\t') buf++;
                while (*buf && *buf != L' ' && *buf != L't' && *buf != L'\r' && *buf != L'\n') {
                    *out_str++ = *buf++;
                }
                *out_str = L'\0';
                fields_read++;
            }
            fmt++;
        } else {
            /* Literal characters must match perfectly or it's fucked 
            yes you need that.... or get fucked up
            */
            if (*fmt != *buf) break;
            fmt++;
            buf++;
        }
    }

    va_end(args);
    return fields_read;
}

void sleep(int seconds)
{
    gST->BootServices->Stall(seconds * 1000000);
}

void efi_detect(EFI_SYSTEM_TABLE *SystemTable)
{
    (void)SystemTable;
}

static BOOLEAN gShellActive = FALSE;

static BOOLEAN IsCtrlSKey(const EFI_INPUT_KEY *Key)
{
    return (Key->UnicodeChar == 0x13);
}

void keyboard_input(CHAR16 *Buffer, UINTN MaxLength) {
    UINTN Index = 0;
    EFI_INPUT_KEY Key;
    EFI_STATUS Status;
    UINTN EventIndex;

    /* Zero out the buffer safely first so we don't fuck up this thing! */
    for (UINTN i = 0; i < MaxLength; i++) {
        Buffer[i] = 0;
    }
    while (Index < MaxLength - 1) {
        gST->BootServices->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
        Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
        if (EFI_ERROR(Status)) {
            continue; 
        }

        if (IsCtrlSKey(&Key)) {
            if (!gShellActive) {
                gShellActive = TRUE;
                gST->ConOut->ClearScreen(gST->ConOut);
                printfc(EFICOLOR_LIGHTRED, L"\r\nGoing into Recovery Shell....\r\n");
                initshl(gST);
                gShellActive = FALSE;
                return;
            }
            continue;
        }

        if (Key.UnicodeChar == L'\r' || Key.UnicodeChar == L'\n') {
            Buffer[Index] = L'\0';
            printf(L"\r\n");   
            break;
        }

        /* handle back space so we don't fuck this! */      
        if (Key.UnicodeChar == L'\b') {
            if (Index > 0) {
                Index--;
                Buffer[Index] = L'\0';
                
                printf(L"\b \b"); 
            }
            continue;
        }

        /* handle keys */
        if (Key.UnicodeChar >= 0x20 && Key.UnicodeChar <= 0x7E) {
            Buffer[Index] = Key.UnicodeChar;
            Index++;

            CHAR16 EchoBuffer[2] = { Key.UnicodeChar, 0 };
            gST->ConOut->OutputString(gST->ConOut, EchoBuffer);
        }

        
    }

}