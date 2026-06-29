#include <Uefi.h>
#include <UefiLib.h>

#include <stdarg.h>

/* 
RAZZLE NEUTRON PRINTF
copyright jad 2026
*/
extern EFI_SYSTEM_TABLE *gST;

void printf(const CHAR16 *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    CHAR16 buf[2] = {0, 0};
    CHAR16 num_buf[32];
    
    while (*fmt) {
        if (*fmt == L'%') {
            fmt++;
            if (*fmt == L'\0') break;
            
            if (*fmt == L's') {
                CHAR16 *s = va_arg(args, CHAR16*);
                if (s) gST->ConOut->OutputString(gST->ConOut, s);
            } 
            else if (*fmt == L'd' || *fmt == L'u' || *fmt == L'x') {
                UINTN val = va_arg(args, UINTN);
                UINTN base = (*fmt == L'x') ? 16 : 10;
                int i = 30;
                num_buf[31] = L'\0';
                
                if (val == 0) {
                    num_buf[i--] = L'0';
                } else {
                    if (*fmt == L'd' && (INTN)val < 0) {
                        gST->ConOut->OutputString(gST->ConOut, L"-");
                        val = -(INTN)val;
                    }
                    while (val > 0) {
                        UINTN digit = val % base;
                        num_buf[i--] = (digit < 10) ? (L'0' + digit) : (L'a' + digit - 10);
                        val /= base;
                    }
                }
                gST->ConOut->OutputString(gST->ConOut, &num_buf[i + 1]);
            } 
            else if (*fmt == L'c') {
                UINTN attr = va_arg(args, UINTN);
                gST->ConOut->SetAttribute(gST->ConOut, attr);
            }
            else if (*fmt == L'%') {
                buf[0] = L'%';
                gST->ConOut->OutputString(gST->ConOut, buf);
            }
        } else {
            buf[0] = *fmt;
            gST->ConOut->OutputString(gST->ConOut, buf);
        }
        fmt++;
    }
    va_end(args);
}