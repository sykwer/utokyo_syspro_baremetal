#pragma once
#include <efi.h>

void putc(CHAR16 c);
void puts(CHAR16 *str);
void puth(unsigned long long value, unsigned char width);
UINT8 compare(const void *first, const void *second, UINT64 bytes);
UINT8 compare_guid(struct EFI_GUID *guid1, struct EFI_GUID *guid2);
void assert(EFI_STATUS status, CHAR16 *msg);
