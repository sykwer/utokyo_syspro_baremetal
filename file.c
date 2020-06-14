#include <efi.h>
#include <file.h>
#include <util.h>

#define READ_UNIT 16384 // 16KiB

void load_file(struct EFI_FILE_PROTOCOL *src, void *dst, unsigned long long size) {
  while (size > READ_UNIT) {
    UINTN unit = READ_UNIT;
    EFI_STATUS status = src->Read(src, &unit, dst);
    assert(status, L"EFI_FILE_PROTOCOL#Read error");
    size -= unit;
    dst += unit;
  }

  while (size > 0) {
    unsigned long long sz = size;
    EFI_STATUS status = src->Read(src, &sz, dst);
    assert(status, L"EFI_FILE_PROTOCOL#Read error");
    size -= sz;
    dst += sz;
  }
}

unsigned long long file_size(struct EFI_FILE_PROTOCOL *file) {
  struct EFI_FILE_INFO file_info;
  UINTN buffer_size = sizeof(file_info);
  EFI_STATUS status = file->GetInfo(file, &fi_guid, &buffer_size, (void *)&file_info);
  assert(status, L"EFI_FILE_PROTOCOL#GetInfo error");
  return file_info.FileSize;
}
