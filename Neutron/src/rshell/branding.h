#ifndef BRANDING_H
#define BRANDING_H

#include <Uefi.h>

extern int build;
extern int milestone;
extern CHAR16 license[];
extern int year_written;

void print_branding_info(void);

#endif