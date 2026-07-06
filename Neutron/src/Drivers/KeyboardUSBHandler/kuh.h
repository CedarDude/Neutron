#ifndef KUH_H
#define KUH_H

#include <Uefi.h>
#include <stdarg.h>
#include <neulib.h>
#include "branding.h"

extern EFI_SYSTEM_TABLE *gST;

void efi_detect(EFI_SYSTEM_TABLE *SystemTable);
void keyboard_input(CHAR16 *Buffer, UINTN MaxLength);
void initshl(void);
int scanf(const CHAR16 *buf, const CHAR16 *fmt, ...);
void sleep(int seconds);

EFI_STATUS efi_clock_1(BOOLEAN EXITLOOP);
EFI_STATUS efi_clock_3(UINTN X, UINTN Y);

#endif