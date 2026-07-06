#include <Uefi.h>
#include <UefiLib.h>
#include <stdarg.h>

/*
RAZZLE NEUTRON PRINTF
Copyright JAD 2026
*/

extern EFI_SYSTEM_TABLE *gST;

/* ========= COLORS ========= */

#define EFICOLOR_BLACK         EFI_BLACK
#define EFICOLOR_BLUE          EFI_BLUE
#define EFICOLOR_GREEN         EFI_GREEN
#define EFICOLOR_CYAN          EFI_CYAN
#define EFICOLOR_RED           EFI_RED
#define EFICOLOR_MAGENTA       EFI_MAGENTA
#define EFICOLOR_BROWN         EFI_BROWN
#define EFICOLOR_LIGHTGRAY     EFI_LIGHTGRAY
#define EFICOLOR_DARKGRAY      EFI_DARKGRAY
#define EFICOLOR_LIGHTBLUE     EFI_LIGHTBLUE
#define EFICOLOR_LIGHTGREEN    EFI_LIGHTGREEN
#define EFICOLOR_LIGHTCYAN     EFI_LIGHTCYAN
#define EFICOLOR_LIGHTRED      EFI_LIGHTRED
#define EFICOLOR_LIGHTMAGENTA  EFI_LIGHTMAGENTA
#define EFICOLOR_YELLOW        EFI_YELLOW
#define EFICOLOR_WHITE         EFI_WHITE
#define EFICOLOR(FG, BG) ((FG) | ((BG) << 4))

static void PrintNumber(UINTN value, UINTN base)
{
    CHAR16 buffer[32];
    INTN i = 30;

    buffer[31] = L'\0';

    if (value == 0) {
        buffer[i--] = L'0';
    } else {
        while (value > 0) {
            UINTN digit = value % base;

            buffer[i--] =
                (digit < 10)
                ? (L'0' + digit)
                : (L'A' + digit - 10);

            value /= base;
        }
    }

    gST->ConOut->OutputString(gST->ConOut, &buffer[i + 1]);
}

static void vprintf_internal(const CHAR16 *fmt, va_list args)
{
    CHAR16 ch[2] = {0, 0};

    while (*fmt)
    {
        /* --- Handle Backspace Escape Sequence --- */
        if (*fmt == L'\b')
        {
            UINTN col = gST->ConOut->Mode->CursorColumn;
            UINTN row = gST->ConOut->Mode->CursorRow;
            
            // Only backspace if we aren't already at the start of the line
            if (col > 0)
            {
                gST->ConOut->SetCursorPosition(gST->ConOut, col - 1, row);
                gST->ConOut->OutputString(gST->ConOut, L" ");
                gST->ConOut->SetCursorPosition(gST->ConOut, col - 1, row);
            }
            fmt++;
            continue;
        }

        if (*fmt != L'%')
        {
            ch[0] = *fmt;
            gST->ConOut->OutputString(gST->ConOut, ch);
            fmt++;
            continue;
        }

        fmt++;

        switch (*fmt)
        {
            case L's':
            {
                CHAR16 *str = va_arg(args, CHAR16 *);

                if (str)
                    gST->ConOut->OutputString(gST->ConOut, str);
                else
                    gST->ConOut->OutputString(gST->ConOut, L"(null)");

                break;
            }

            case L'd':
            {
                INTN value = va_arg(args, INTN);

                if (value < 0)
                {
                    gST->ConOut->OutputString(gST->ConOut, L"-");
                    value = -value;
                }

                PrintNumber((UINTN)value, 10);
                break;
            }

            case L'u':
            {
                UINTN value = va_arg(args, UINTN);
                PrintNumber(value, 10);
                break;
            }

            case L'x':
            {
                UINTN value = va_arg(args, UINTN);
                PrintNumber(value, 16);
                break;
            }

            case L'c':
            {
                ch[0] = (CHAR16)va_arg(args, UINTN);
                
                // Also handle backspace if passed as a %c variable
                if (ch[0] == L'\b') 
                {
                    UINTN col = gST->ConOut->Mode->CursorColumn;
                    UINTN row = gST->ConOut->Mode->CursorRow;
                    if (col > 0) 
                    {
                        gST->ConOut->SetCursorPosition(gST->ConOut, col - 1, row);
                        gST->ConOut->OutputString(gST->ConOut, L" ");
                        gST->ConOut->SetCursorPosition(gST->ConOut, col - 1, row);
                    }
                } 
                else 
                {
                    gST->ConOut->OutputString(gST->ConOut, ch);
                }
                break;
            }

            case L'o':
            {
                UINTN color = va_arg(args, UINTN);
                gST->ConOut->SetAttribute(gST->ConOut, color);
                break;
            }

            case L'%':
            {
                ch[0] = L'%';
                gST->ConOut->OutputString(gST->ConOut, ch);
                break;
            }

            default:
            {
                ch[0] = L'%';
                gST->ConOut->OutputString(gST->ConOut, ch);

                ch[0] = *fmt;
                gST->ConOut->OutputString(gST->ConOut, ch);
                break;
            }
        }

        fmt++;
    }
}

void printf(const CHAR16 *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vprintf_internal(fmt, args);
    va_end(args);
}

/* printf with colors ! */

void printfc(UINTN Color, const CHAR16 *fmt, ...)
{
    UINTN OldColor = gST->ConOut->Mode->Attribute;

    gST->ConOut->SetAttribute(gST->ConOut, Color);

    va_list args;

    va_start(args, fmt);
    vprintf_internal(fmt, args);
    va_end(args);

    gST->ConOut->SetAttribute(gST->ConOut, OldColor);
}