#ifndef MAINUI_H
#define MAINUI_H

#include <Uefi.h>
#include "config_parser.h"

/**
 * Initialize the main UI system
 * @param SystemTable - EFI System Table
 * @return EFI_STATUS
 */
EFI_STATUS InitUI(EFI_SYSTEM_TABLE *SystemTable);

/**
 * Display the menu and handle user interaction
 * @param SystemTable - EFI System Table
 * @param Config - Menu configuration
 * @return Index of selected menu entry, or -1 if cancelled
 */
INT32 DisplayMenu(EFI_SYSTEM_TABLE *SystemTable, MenuConfig *Config);

/**
 * Render the menu display
 * @param SystemTable - EFI System Table
 * @param Config - Menu configuration
 * @param SelectedIndex - Currently selected index
 * @param ScrollOffset - Scroll offset for display
 */
VOID RenderMenu(EFI_SYSTEM_TABLE *SystemTable, 
                MenuConfig *Config, 
                UINTN SelectedIndex,
                UINTN ScrollOffset);

/**
 * Cleanup UI resources
 */
VOID CleanupUI(VOID);

#endif // MAINUI_H
