#include <efi.h>
#include <util.h>

void putc(CHAR16 c) {
  CHAR16 str[2] = L" ";
  str[0] = c;
  SystemTable->ConOut->OutputString(SystemTable->ConOut, str);
}

void puts(CHAR16 *str) {
  SystemTable->ConOut->OutputString(SystemTable->ConOut, str);
}

void puth(unsigned long long value, unsigned char width) {
  CHAR16 buffer[width+1];
  for (int i = (int)width - 1; i >= 0; i--) {
    CHAR16 offset = value & 0x0f;
    buffer[i] = offset < 0xa ? L'0' + offset : L'A' + (offset - 0xa);
    value >>= 4;
  }
  buffer[width] = L'\0';
  puts(buffer);
}

UINT8 compare(const void *first, const void *second, UINT64 bytes) {
  const UINT8 *f = first, *s = second;
  for (UINT64 i = 0; i < bytes; i++) {
    if (f[i] != s[i]) return 0;
  }
  return 1;
}

UINT8 compare_guid(struct EFI_GUID *guid1, struct EFI_GUID *guid2) {
  if (guid1->Data1 != guid2->Data1 || guid1->Data2 != guid2->Data2 || guid1->Data3 != guid2->Data3) {
    return 0;
  }
  for (int i = 0; i < 8; i++) {
    if (guid1->Data4[i] != guid2->Data4[i]) return 0;
  }
  return 1;
}

void assert(EFI_STATUS status, CHAR16 *msg) {
  if (status != EFI_SUCCESS) {
    puts(msg);
    puts(L": error code = ");
    puth(status, 16);
    puts(L"\r\n");
    while (1);
  }
}
