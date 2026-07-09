#ifndef UTILS_H
#define UTILS_H

#include <Uefi.h>
// I will try documenting these, as its get a bit confusing...


/**
 * Display a clock at the given position
 * @param SystemTable - EFI System Table
 * @param X - X coordinate
 * @param Y - Y coordinate
 * @return EFI_STATUS
 */
EFI_STATUS DisplayClock(EFI_SYSTEM_TABLE *SystemTable, UINTN X, UINTN Y);

/**
 * Display Neutron banner
 * @param SystemTable - EFI System Table
 */
VOID DisplayNeutronBanner(EFI_SYSTEM_TABLE *SystemTable);

/**
 * Display hardware information
 * @param SystemTable - EFI System Table
 */
VOID DisplayHardwareInfo(EFI_SYSTEM_TABLE *SystemTable);

/**
 * Display file contents
 * @param SystemTable - EFI System Table
 * @param FilePath - Path to file
 */
VOID DisplayFileContents(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *FilePath);

#endif // UTILS_H
