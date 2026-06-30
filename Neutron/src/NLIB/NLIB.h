#ifndef NLIB_H
#define NLIB_H

#include <Uefi.h>

void get_cpu_name(CHAR16 *out_buffer16);
UINT64 get_total_ram(void);
#endif