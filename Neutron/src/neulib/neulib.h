#ifndef NEULIB_H
#define NEULIB_H

#include <Uefi.h>
#include <stdarg.h>
extern EFI_SYSTEM_TABLE *gST;

void printf(const CHAR16 *fmt, ...);

#endif