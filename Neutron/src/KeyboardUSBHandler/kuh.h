#ifndef KUH_H
#define KUH_H

#include <Uefi.h>
#include <stdarg.h>
#include <neulib.h>

extern EFI_SYSTEM_TABLE *gST;

void efi_detect(EFI_SYSTEM_TABLE *SystemTable);
void keyboard_input(CHAR16 *Buffer, UINTN MaxLength);
int scanf(const CHAR16 *buf, const CHAR16 *fmt, ...);
void sleep(int seconds);

#endif