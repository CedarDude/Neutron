#ifndef NEULIB_H
#define NEULIB_H

#include <Uefi.h>
#include <stdarg.h>

extern EFI_SYSTEM_TABLE *gST;

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

void printf(const CHAR16 *fmt, ...);
void printfc(UINTN Color, const CHAR16 *fmt, ...);

#endif