#include <Uefi.h>
#include <neulib.h>
#include "branding.h"

int build = 1;
int milestone = 2;
CHAR16 license[] = L"GPLv3";
int year_written = 2026;

void print_branding_info(void)
{
    printf(L"Build: %d\r\n", build);
    printf(L"Milestone: %d\r\n", milestone);
    printf(L"License: %s\r\n", license);
    printf(L"Year Written: %d\r\n", year_written);
}
