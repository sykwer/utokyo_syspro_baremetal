#pragma once
#include <efi.h>

void load_file(struct EFI_FILE_PROTOCOL *src, void *dst, unsigned long long size);
unsigned long long file_size(struct EFI_FILE_PROTOCOL *file);
