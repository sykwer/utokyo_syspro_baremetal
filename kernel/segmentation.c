#include "segmentation.h"

const unsigned long long GDT[] = {
  0x0000000000000000, /* NULL descriptor */
  0x00af9a000000ffff, /* base=0, limit=4GB, mode=code(r-x),kernel */
  0x00cf93000000ffff, /* base=0, limit=4GB, mode=data(rw-),kernel */
};

/*
struct GDTR {
  unsigned short limit;
  unsigned long long base;
} __attribute__((packed));
*/

void load_gdt(unsigned long long addr);

void init_segmentation() {
  /*
  struct GDTR gdtr;
  gdtr.limit = sizeof(GDT);
  gdtr.base = (unsigned long long)GDT;
  load_gdt((unsigned long long)&gdtr);
  */
  unsigned long long gdtr[2];
  gdtr[0] = ((unsigned long long)GDT << 16) | (sizeof(GDT) - 1);
  gdtr[1] = ((unsigned long long)GDT >> 48);
  load_gdt((unsigned long long)&gdtr);
}
