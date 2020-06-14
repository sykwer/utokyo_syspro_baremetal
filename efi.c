#include <efi.h>
#include <util.h>

struct EFI_SYSTEM_TABLE *SystemTable;
struct EFI_CONSOLE_CONTROL_PROTOCOL *CCP;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;

struct EFI_GUID ccp_guid = { 0xf42f7782, 0x12e, 0x4c12, {0x99, 0x56, 0x49, 0xf9, 0x43, 0x4, 0xf7, 0x21} };
struct EFI_GUID gop_guid = {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};
struct EFI_GUID sfsp_guid = {0x0964e5b22,0x6459,0x11d2, {0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}};

// EFI_FILE_INFO_ID
struct EFI_GUID fi_guid = {0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

// EFI_ACPI_TABLE_GUID
struct EFI_GUID acpi_table_guid = {0x8868e871,0xe4f1,0x11d3, {0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}};

void efi_init(struct EFI_SYSTEM_TABLE *st) {
  SystemTable = st;
  SystemTable->BootServices->LocateProtocol(&gop_guid, NULL, (void **)&GOP);
  SystemTable->BootServices->LocateProtocol(&sfsp_guid, NULL, (void **)&SFSP);
}

void *find_acpi_table() {
  for (UINTN i = 0; i < SystemTable->NumberOfTableEntries; i++) {
    struct EFI_GUID *guid = &SystemTable->ConfigurationTable[i].VendorGuid;
    if (compare_guid(guid, &acpi_table_guid)) return SystemTable->ConfigurationTable[i].VendorTable;
  }
  return NULL;
}
