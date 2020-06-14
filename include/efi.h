#pragma once
#define EFI_SUCCESS 0
#define EFI_ERROR(status) (((INTN) status) < 0)
#define TRUE ((BOOLEAN) 1)
#define FALSE ((BOOLEAN) 0)
#define NULL ((void *) 0)

#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

typedef char UINT8;
typedef unsigned short CHAR16;
typedef unsigned short UINT16;
typedef short INT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef unsigned long long UINTN;
typedef long long INTN;
typedef char BOOLEAN;
typedef unsigned long long EFI_STATUS;
typedef void *EFI_HANDLE;
typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

struct EFI_GUID {
	UINT32 Data1;
	UINT16 Data2;
	UINT16 Data3;
	UINT8 Data4[8];
};

struct EFI_MEMORY_DESCRIPTOR {
  UINT32 Type;
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  EFI_VIRTUAL_ADDRESS VirtualStart;
  UINT64 NumberOfPages;
  UINT64 Attribute;
};

typedef enum {
  AllHandles,
  ByRegisterNotify,
  ByProtocol,
} EFI_LOCATE_SEARCH_TYPE;

struct EFI_SYSTEM_TABLE {
  char _dummy[24];
  CHAR16 *FirmwareVendor;
  char _dummy2[28];
  struct EFI_SIMPLE_TEXT_OUTPUT_PROTCOL {
    void *_dummy;
    EFI_STATUS (*OutputString)(
      struct EFI_SIMPLE_TEXT_OUTPUT_PROTCOL *This,
      CHAR16 *String);
    void *_dummy2[4];
    EFI_STATUS (*ClearScreen)(
      struct EFI_SIMPLE_TEXT_OUTPUT_PROTCOL *This);
  } *ConOut;
  void *_dummy3[3]; // EFI_RUNTIME_SERVICES
  struct EFI_BOOT_SERVICES {
    char _dummy[24];
    void *_dummy2[2]; // Task Priority Tasks
    // Memory Services
    void *_dummy3[2];
    EFI_STATUS (*GetMemoryMap)(
        UINTN *MemoryMapSize,
        struct EFI_MEMORY_DESCRIPTOR *MemoryMap,
        UINTN *MapKey,
        UINTN *DescriptorSize,
        UINT32 *DescriptorVersion);
    void *_dummy4[2];
    // Memory Services - To here
    void *_dummy5[6]; // Event & Timer Services
    // Protocol Handler Services
    void *_dummy6[3];
    EFI_STATUS (*HandleProtocol)(
        EFI_HANDLE Handle,
        struct EFI_GUID *Protocol,
        void **Interface);
    void *_dummy7[5];
    // Protocol Handler Services - To here
    // Image Services
    void *_dummy8[4];
    EFI_STATUS (*ExitBootServices)(
        EFI_HANDLE ImageHandle,
        UINTN MapKey);
    // Image Services - To here
    void *_dummy9[3]; // Miscellaneous Services
    void *_dummy10[2]; // DriverSupport Services
    void *_dummy11[3]; // Open and Close Protcol Services
    // LIbrary Services
    void *_dummy12[1];
    // LocateHandleBuffer here
    EFI_STATUS (*LocateHandleBuffer)(
        EFI_LOCATE_SEARCH_TYPE SearchType,
        struct EFI_GUID *Protocol,
        void *SearchKey,
        UINTN *NoHandles,
        EFI_HANDLE **Buffer);
    EFI_STATUS (*LocateProtocol)(
        struct EFI_GUID *Protocol,
        void *Registration,
        void **Interface);
    void *_dummy13[2];
    // LIbrary Services - To here
    void *_dummy14; // 32-bit CRC Services
    // Miscellaneous Services
    void *_dummy15;
    void (*SetMem)(
        void *Buffer,
        UINTN Size,
        UINT8 Value);
    void *_dummy16;
    // Miscellaneous Services - To here
  } *BootServices;
  UINTN NumberOfTableEntries;
  struct EFI_CONFIGURATION_TABLE {
    struct EFI_GUID VendorGuid;
    void *VendorTable;
  } *ConfigurationTable;
};

typedef enum {
  EfiConsoleControlScreenText,
  EfiConsoleControlScreenGraphics,
  EfiConsoleControlScreenMaxValue
} EFI_CONSOLE_CONTROL_SCREEN_MODE;

struct EFI_CONSOLE_CONTROL_PROTOCOL {
  EFI_STATUS (*GetMode)(
      struct EFI_CONSOLE_CONTROL_PROTOCOL *This,
      EFI_CONSOLE_CONTROL_SCREEN_MODE *Mode,
      BOOLEAN *GopUgaExists,
      BOOLEAN *StdInLocked);
  EFI_STATUS (*SetMode)(
      struct EFI_CONSOLE_CONTROL_PROTOCOL *This,
      EFI_CONSOLE_CONTROL_SCREEN_MODE Mode);
};

struct EFI_FILE_PROTOCOL {
  char _dummy[8];
  EFI_STATUS (*Open)(
      struct EFI_FILE_PROTOCOL *This,
      struct EFI_FILE_PROTOCOL **NewHandle,
      CHAR16 *FileName,
      UINT64 OpenMode,
      UINT64 Attributes);
  EFI_STATUS (*Close)(
      struct EFI_FILE_PROTOCOL *This);
  void *_dummy2; // Delete
  EFI_STATUS (*Read)(
      struct EFI_FILE_PROTOCOL *This,
      UINTN *BufferSize,
      void *Buffer);
  EFI_STATUS (*Write)(
      struct EFI_FILE_PROTOCOL *This,
      UINTN *BufferSize,
      void *Buffer);
  void *_dummy3[2];
  EFI_STATUS (*GetInfo)(
      struct EFI_FILE_PROTOCOL *This,
      struct EFI_GUID *InformationType,
      UINTN *BufferSize,
      void *Buffer);
};

typedef enum {
  PixelRedGreenBlueReserved8BitPerColor,
  PixelBlueGreenRedReserved8BitPerColor,
  PixelBitMask,
  PixelBltOnly,
  PixelFormatMax,
} EFI_GRAPHICS_PIXEL_FORMAT;

struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
  void *_dummy[3];
  struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
    UINT32 MaxMode;
    UINT32 Mode;
    struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
      UINT32 Version;
      UINT32 HorizontalResolution;
      UINT32 VerticalResolution;
      EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
      UINT32 dummy[4]; // PixelInformation
      UINT32 PixelsPerScanLine;
    } *Info;
    UINTN SizeOfInfo;
    EFI_PHYSICAL_ADDRESS FrameBufferBase;
    UINTN FrameBufferSize;
  } *Mode;
};

struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
  char _dummy[8];
  EFI_STATUS (*OpenVolume)(
      struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
      struct EFI_FILE_PROTOCOL **Root);
};

struct EFI_TIME {
  UINT16 Year;
  UINT8 Month;
  UINT8 Day;
  UINT8 Hour;
  UINT8 Minute;
  UINT8 Second;
  UINT8 Pad1;
  UINT32 Nanosecond;
  INT16 TimeZone;
  UINT8 Daylight;
  UINT8 Pad2;
};

struct EFI_FILE_INFO {
  UINT64 Size;
  UINT64 FileSize;
  UINT64 PhysicalSize;
  struct EFI_TIME CreateTime;
  struct EFI_TIME LastAccessTime;
  struct EFI_TIME ModificationTime;
  UINT64 Attribute;
  CHAR16 FileName[64];
};

extern struct EFI_SYSTEM_TABLE *SystemTable;
extern struct EFI_CONSOLE_CONTROL_PROTOCOL *CCP;
extern struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
extern struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;

extern struct EFI_GUID ccp_guid;
extern struct EFI_GUID gop_guid;
extern struct EFI_GUID sfsp_guid;
extern struct EFI_GUID fi_guid;

void efi_init(struct EFI_SYSTEM_TABLE *st);
void *find_acpi_table();
