#include <efi.h>
#include <file.h>
#include <hardware_info.h>
#include <memory.h>
#include <util.h>

#define MB 1048576 // 1024 * 1024
#define KERNEL_NAME_LEN  6
static const CHAR16 AppleFirmwareVendor[6] = L"Apple";
static CHAR16 KERNEL_NAME[KERNEL_NAME_LEN] = L"kernel";
static CHAR16 APP1_NAME[] = L"app1";
static CHAR16 APP2_NAME[] = L"app2";
static CHAR16 APP3_NAME[] = L"app3";
static const unsigned long long KERNEL_START = 0x100000000;
static const unsigned long long APP1_START = 0x104000000;
static const unsigned long long APP2_START = 0x105000000;
static const unsigned long long APP3_START = 0x106000000;

void check_pixel_format() {
  // TODO: Support other pixel formats
  if (get_pixel_format() == PixelBlueGreenRedReserved8BitPerColor) {
    puts(L"Pixel format `PixelRedGreenBlueReversed8BitPerColor is supported\r\n`");
  } else {
    puts(L"Unsupported pixel format: ");
    puth(get_pixel_format(), 8);
    while(1);
  }
}

void apple_support() {
  EFI_STATUS status;

  // Thanks to the rEFIt 0.14 project for having figured this out long ago.
  // ConsoleControlProtocol is needed to switch Mac boot graphics to text mode.
  if (compare(SystemTable->FirmwareVendor, AppleFirmwareVendor, 6)) {
    status = SystemTable->BootServices->LocateProtocol(&ccp_guid, NULL, (void **)&CCP);
    if (EFI_ERROR(status)) CCP = NULL;
    if (CCP != NULL) {
      EFI_CONSOLE_CONTROL_SCREEN_MODE mode;
      CCP->GetMode(CCP, &mode, NULL, NULL);
      if (mode != EfiConsoleControlScreenText) {
        CCP->SetMode(CCP, EfiConsoleControlScreenText);
      }
    }
  }
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, struct EFI_SYSTEM_TABLE *st) {
  EFI_STATUS status;

  efi_init(st);
  get_hardware_info();
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  check_pixel_format();
  apple_support();

  puts(L"Loading kernel image...\r\n");

  // Assuming UEFI firmware recognizes multiple volumes, find the volume where kernel is located
  struct EFI_FILE_PROTOCOL *kernel_file = NULL;
  struct EFI_FILE_PROTOCOL *app1_file = NULL;
  struct EFI_FILE_PROTOCOL *app2_file = NULL;
  struct EFI_FILE_PROTOCOL *app3_file = NULL;
  {
    UINTN handle_cnt;
    EFI_HANDLE *handle_buffer;
    status = SystemTable->BootServices->LocateHandleBuffer(ByProtocol, &sfsp_guid, NULL, &handle_cnt, &handle_buffer);
    assert(status, L"BootServices#LocateHandleBuffer error");

    struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
    for (UINTN i = 0; i < handle_cnt; i++) {
      status = SystemTable->BootServices->HandleProtocol(handle_buffer[i], &sfsp_guid, (void **)&fs);
      assert(status, L"BootServices#HandleProtocol error");

      struct EFI_FILE_PROTOCOL *root;
      status = fs->OpenVolume(fs, &root);
      assert(status, L"OpenVolume error");

      char kernel_found = 0;
      while (1) {
        UINTN buffer_size = 256; // Arbitrary size
        unsigned char buffer[buffer_size];
        status = root->Read(root, &buffer_size, (void *)buffer);
        assert(status, L"EFI_FILE_PROTOCOL#Read error");
        if (buffer_size == 0) break;

        struct EFI_FILE_INFO *file_info = (struct EFI_FILE_INFO *)buffer;
        if (compare(file_info->FileName, KERNEL_NAME, KERNEL_NAME_LEN)) kernel_found = 1;
      }

      if (kernel_found) {
        status = root->Open(root, &kernel_file, KERNEL_NAME, EFI_FILE_MODE_READ, 0);
        assert(status, L"EFI_FILE_PROTOCOL#Read error");

        status = root->Open(root, &app1_file, APP1_NAME, EFI_FILE_MODE_READ, 0);
        assert(status, L"EFI_FILE_PROTOCOL#Read error");

        status = root->Open(root, &app2_file, APP2_NAME, EFI_FILE_MODE_READ, 0);
        assert(status, L"EFI_FILE_PROTOCOL#Read error");

        status = root->Open(root, &app3_file, APP3_NAME, EFI_FILE_MODE_READ, 0);
        assert(status, L"EFI_FILE_PROTOCOL#Read error");
      }

      root->Close(root);
    }
  }

  if (!kernel_file) {
    puts(L"kernel file not found\r\n");
    while (1);
  }

  struct header {
    void *bss_start;
    UINTN bss_size;
  } head;
  unsigned long long head_size = sizeof(head);

  {
    unsigned long long kernel_size = file_size(kernel_file);
    load_file(kernel_file, (void *)&head, head_size);
    kernel_size -= head_size;
    load_file(kernel_file, (void *)KERNEL_START, kernel_size);
  }

  {
    unsigned long long app1_size = file_size(app1_file);
    load_file(app1_file, (void *)&head, head_size);
    app1_size -= head_size;
    load_file(app1_file, (void *)APP1_START, app1_size);
  }

  {
    unsigned long long app2_size = file_size(app2_file);
    load_file(app2_file, (void *)&head, head_size);
    app2_size -= head_size;
    load_file(app2_file, (void *)APP2_START, app2_size);
  }

  {
    unsigned long long app3_size = file_size(app3_file);
    load_file(app3_file, (void *)&head, head_size);
    app3_size -= head_size;
    load_file(app3_file, (void *)APP3_START, app3_size);
  }

  kernel_file->Close(kernel_file);
  app1_file->Close(app1_file);
  app2_file->Close(app2_file);
  app3_file->Close(app3_file);
  SystemTable->BootServices->SetMem(head.bss_start, head.bss_size, 0);

  unsigned long long arg1 = (unsigned long long)SystemTable;
  unsigned long long arg2 = (unsigned long long)&hardware_info;
  unsigned long long stack_base = KERNEL_START + (16 * MB);

  // On success, this loader owns all available memory in the system.
  puts(L"Exiting boot service...\r\n");
  exit_boot_services(ImageHandle);

  asm ("mov %0, %%rdi\n"
       "mov %1, %%rsi\n"
       "mov %2, %%rsp\n"
       "jmp *%3\n"
       ::"m"(arg1), "m"(arg2), "m"(stack_base), "m"(KERNEL_START));

  puts(L"Not reached here\r\n");
  while (1);
}
