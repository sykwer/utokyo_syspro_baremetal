#include "hardware.h"
#include "segmentation.h"

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  init_segmentation();
  // To here - Put this part at the top of start() function

  // Delete me. I'm a sample code.
  for (unsigned int i = 0; i < hardware_info.fb.height; i++) {
    for (unsigned int j = 0; j < hardware_info.fb.width; j++) {
      struct Pixel *pixel = hardware_info.fb.base + hardware_info.fb.width * i + j;
      // † AYAME †
      pixel->r = 111;
      pixel->g = 51;
      pixel->b = 129;
    }
  }
  // To here - sample code

  // Do not delete it!
  while (1);
}
