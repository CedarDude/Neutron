/*
Razzle (c) Neutron EFI NLIB Library.
Copyright Jad 2026 (c)
Licensed Under GPLv3
*/

#include <Uefi.h>
#include <UefiLib.h>
extern EFI_SYSTEM_TABLE *gST;


void get_cpu_name(CHAR16 *out_buffer16) {
    UINT32 registers[4];
    CHAR8 local_buffer8[48];
    UINT32 *ptr = (UINT32 *)local_buffer8;

    for (UINT32 i = 0x80000002; i <= 0x80000004; i++) {
        __asm__ volatile("cpuid"
                         : "=a"(registers[0]), "=b"(registers[1]), "=c"(registers[2]), "=d"(registers[3])
                         : "a"(i));
        
        ptr[0] = registers[0];
        ptr[1] = registers[1];
        ptr[2] = registers[2];
        ptr[3] = registers[3];
        ptr += 4;
    }

    for (int i = 0; i < 48; i++) {
        out_buffer16[i] = (CHAR16)local_buffer8[i];
    }
    
    out_buffer16[48] = L'\0'; 
}


UINT64 get_total_ram(void) {
    UINTN memory_map_size = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    UINTN map_key;
    UINTN descriptor_size;
    UINT32 descriptor_version;
    EFI_STATUS status;

    status = gST->BootServices->GetMemoryMap(&memory_map_size, NULL, &map_key, &descriptor_size, &descriptor_version);
    memory_map_size += 2 * descriptor_size;
    status = gST->BootServices->AllocatePool(EfiLoaderData, memory_map_size, (VOID **)&memory_map);
    status = gST->BootServices->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
    
    if (EFI_ERROR(status)) {
        if (memory_map) gST->BootServices->FreePool(memory_map);
        return 0;
    }

    UINT64 total_bytes = 0;
    UINTN num_entries = memory_map_size / descriptor_size;
    EFI_MEMORY_DESCRIPTOR *desc = memory_map;

    for (UINTN i = 0; i < num_entries; i++) {
        if (desc->Type == EfiConventionalMemory || 
            desc->Type == EfiLoaderCode || 
            desc->Type == EfiLoaderData) {
            total_bytes += (desc->NumberOfPages * 4096);
        }
        desc = (EFI_MEMORY_DESCRIPTOR *)((UINT8 *)desc + descriptor_size);
    }

    gST->BootServices->FreePool(memory_map);
    return total_bytes;
}
