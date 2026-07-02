#ifndef RSHELL_H
#define RSHELL_H


/*
Copyright (c) 2026 - Neutron EFI Application Project Jad
Licensed under GPLv3 -- Early EFI Stage (Milestone 2)
[M1 was the milestone i published]
[M2 would be having fat32 and a recovery shell]
[M3 -- ExitBootServices and load a EXRKRNL file]
*/
#include <Uefi.h>
#include <stdarg.h>
#include <neulib.h>
#include <NLIB.h>
#include <kuh.h>
#include <branding.h>

extern EFI_SYSTEM_TABLE *gST;

void initshl(void);

#endif