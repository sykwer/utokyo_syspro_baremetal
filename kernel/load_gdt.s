# void load_gdt(unsigned long long addr);
.global load_gdt
load_gdt:
  lgdt (%rdi)

  movw $0x10, %ax #index=2, TableIndicator=0, RPL=00(Highest)
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  movw %ax, %ss

  sub $16, %rsp
  movq $0x08, 8(%rsp) # index=1, TableIndicator=0, RPL=00(Highest)
  movabsq $next, %rax
  mov %rax, (%rsp)
  lretq

next:
  ret
