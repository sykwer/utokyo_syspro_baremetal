#pragma once

// Root System Description Pointer
struct RSDP {
  char signature[8];
  unsigned char check_sum;
  char oem_id[6];
  unsigned char revision;
  unsigned int rsdt_address;
  unsigned int length;
  unsigned long long xsdt_address;
  unsigned char extended_checksum;
  char reserved[3];
} __attribute__((packed));

// System Description Table Header
struct SDTH {
  char signature[4];
  unsigned int length;
  unsigned char revision;
  unsigned char checksum;
  char oem_id[6];
  char oem_table_id[8];
  unsigned int oem_revision;
  unsigned int creator_id;
  unsigned int creator_revision;
} __attribute__((packed));

// Extended System Descriptor Table
// other_tables is an array with variable length, so not included in struct definition
struct XSDT {
  struct SDTH sdth;
  // unsinged long long other_tables[]
} __attribute__((packed));

// Fixed ACPI Description Table
struct FADT {
  struct SDTH sdth; // 36byte
  char _dummy1[40]; // 40byte
  unsigned int PM_TMR_BLK; // 4byte
  char _dummy2[32]; // 32bytre
  unsigned int flags;
} __attribute__((packed));

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
  unsigned int total_size;
};

struct HardwareInfo {
  struct FrameBuffer fb;
  struct RSDP *rsdp;
};

extern struct HardwareInfo hardware_info;
