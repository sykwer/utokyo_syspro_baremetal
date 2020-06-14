#include <efi.h>
#include <hardware_info.h>

struct HardwareInfo hardware_info;

void get_hardware_info() {
  hardware_info.fb.base = (struct Pixel *)GOP->Mode->FrameBufferBase;
  hardware_info.fb.width = GOP->Mode->Info->HorizontalResolution;
  hardware_info.fb.height = GOP->Mode->Info->VerticalResolution;
  hardware_info.fb.total_size = GOP->Mode->FrameBufferSize;

  hardware_info.rsdp = find_acpi_table();
}

EFI_GRAPHICS_PIXEL_FORMAT get_pixel_format() {
  return GOP->Mode->Info->PixelFormat;
}
