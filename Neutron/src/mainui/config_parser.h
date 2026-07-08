#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <Uefi.h>

#define MAX_MENU_ENTRIES 16
#define MAX_ENTRY_NAME 64
#define MAX_ENTRY_ABOUT 128
#define MAX_ENTRY_VERSION 32
#define MAX_ENTRY_TYPE 32

typedef struct {
    CHAR16 name[MAX_ENTRY_NAME];
    CHAR16 about[MAX_ENTRY_ABOUT];
    CHAR16 version[MAX_ENTRY_VERSION];
    CHAR16 type[MAX_ENTRY_TYPE];
} MenuEntry;

typedef struct {
    MenuEntry entries[MAX_MENU_ENTRIES];
    UINTN count;
} MenuConfig;

/**
 * Parse loader.cfg file and extract menu entries
 * @param SystemTable - EFI System Table
 * @param ConfigPath - Path to loader.cfg file
 * @param Config - Pointer to MenuConfig structure to fill
 * @return EFI_STATUS
 */
EFI_STATUS ParseLoaderConfig(EFI_SYSTEM_TABLE *SystemTable, 
                             CHAR16 *ConfigPath, 
                             MenuConfig *Config);

/**
 * Free resources allocated for menu config
 */
VOID FreeMenuConfig(MenuConfig *Config);

#endif // CONFIG_PARSER_H
