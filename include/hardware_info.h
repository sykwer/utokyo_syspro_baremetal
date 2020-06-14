#pragma once
#include <efi.h>

// Assume this type of pixel format
struct Pixel {
  unsigned char b;
  unsigned char g;
  unsigned char r;
  unsigned char _reserved;
};

struct FrameBuffer {
  struct Pixel *base;
  unsigned int width;
  unsigned int height;
  // PIxelsPerScanLine * VerticalResolution * PIxelElementSize
  unsigned int total_size;
};

struct HardwareInfo {
  struct FrameBuffer fb;
  void* rsdp;
};

extern struct HardwareInfo hardware_info;

void get_hardware_info(void);
EFI_GRAPHICS_PIXEL_FORMAT get_pixel_format(void);
