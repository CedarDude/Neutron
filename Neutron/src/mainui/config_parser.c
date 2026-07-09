#include <Uefi.h>
#include <rshell.h>
#include <fpi.h>
#include "config_parser.h"

static BOOLEAN StringContains(const UINT8 *Buffer, UINTN Len, const char *Keyword)
{
    int KLen = 0;
    while (Keyword[KLen]) KLen++;
    
    if (Len < (UINTN)KLen) return FALSE;
    
    for (UINTN i = 0; i <= Len - KLen; i++) {
        BOOLEAN Match = TRUE;
        for (int j = 0; j < KLen; j++) {
            if ((CHAR8)Buffer[i + j] != Keyword[j]) {
                Match = FALSE;
                break;
            }
        }
        if (Match) return TRUE;
    }
    return FALSE;
}

static VOID ExtractQuotedValue(const UINT8 *Raw, UINTN LineStart, UINTN LineLen, CHAR16 *Dest, UINTN DestMax)
{
    UINTN FirstQuote = 0;
    UINTN SecondQuote = 0;
    
    for (UINTN i = 0; i < LineLen; i++) {
        if (Raw[LineStart + i] == '"') {
            if (FirstQuote == 0) {
                FirstQuote = i + 1;
            } else {
                SecondQuote = i;
                break;
            }
        }
    }
    
    if (FirstQuote > 0 && SecondQuote > FirstQuote) {
        UINTN CopyLen = SecondQuote - FirstQuote;
        if (CopyLen >= DestMax) {
            CopyLen = DestMax - 1;
        }
        for (UINTN i = 0; i < CopyLen; i++) {
            Dest[i] = (CHAR16)Raw[LineStart + FirstQuote + i];
        }
        Dest[CopyLen] = L'\0';
    } else {
        Dest[0] = L'\0';
    }
}

EFI_STATUS ParseLoaderConfig(EFI_SYSTEM_TABLE *SystemTable, 
                             CHAR16 *ConfigPath, 
                             MenuConfig *Config)
{
    EFI_STATUS Status;
    VOID *FileBuffer;
    UINTN FileSize;
    
    if (!Config) {
        return EFI_INVALID_PARAMETER;
    }
    
    Config->count = 0;
    
    for (UINTN i = 0; i < MAX_MENU_ENTRIES; i++) {
        Config->entries[i].name[0] = L'\0';
        Config->entries[i].about[0] = L'\0';
        Config->entries[i].version[0] = L'\0';
        Config->entries[i].type[0] = L'\0';
    }
    
    Status = UseFile(SystemTable, ConfigPath, &FileBuffer, &FileSize);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    UINT8 *Raw = (UINT8 *)FileBuffer;
    UINTN LineStart = 0;
    UINTN EntryIndex = 0;
    BOOLEAN InEntry = FALSE;
    
    for (UINTN i = 0; i <= FileSize; i++) {
        BOOLEAN IsLineEnd = (i == FileSize || Raw[i] == '\n');
        
        if (IsLineEnd) {
            UINTN LineLen = i - LineStart;
            if (LineLen > 0 && Raw[i - 1] == '\r') {
                LineLen--;
            }
            
            if (LineLen > 0) {
                if (!InEntry) {
                    if (StringContains(&Raw[LineStart], LineLen, "menuentry")) {
                        if (EntryIndex < MAX_MENU_ENTRIES) {
                            ExtractQuotedValue(Raw, LineStart, LineLen, Config->entries[EntryIndex].name, MAX_ENTRY_NAME);
                        }
                        InEntry = TRUE;
                    }
                } else {
                    if (StringContains(&Raw[LineStart], LineLen, "}")) {
                        InEntry = FALSE;
                        EntryIndex++;
                    } else if (EntryIndex < MAX_MENU_ENTRIES) {
                        if (StringContains(&Raw[LineStart], LineLen, "about=")) {
                            ExtractQuotedValue(Raw, LineStart, LineLen, Config->entries[EntryIndex].about, MAX_ENTRY_ABOUT);
                        } else if (StringContains(&Raw[LineStart], LineLen, "version=")) {
                            ExtractQuotedValue(Raw, LineStart, LineLen, Config->entries[EntryIndex].version, MAX_ENTRY_VERSION);
                        } else if (StringContains(&Raw[LineStart], LineLen, "type=")) {
                            ExtractQuotedValue(Raw, LineStart, LineLen, Config->entries[EntryIndex].type, MAX_ENTRY_TYPE);
                        }
                    }
                }
            }
            
            Config->count = EntryIndex;
            LineStart = i + 1;
        }
    }
    
    FreeFileMem(FileBuffer);
    return EFI_SUCCESS;
}

VOID FreeMenuConfig(MenuConfig *Config)
{
    (void)Config;
}
