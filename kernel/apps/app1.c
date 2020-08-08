unsigned long long text_section_top = 0x104000000;

void app1() {
  // char *str = "Hello from app1\r\n";

  // TODO: Say hello from here by issuing `puts` syscall
  // syscall_puts(str);

  volatile int i = 100000000;
  while (i--);

  asm volatile ("jmp *%0" :: "m"(text_section_top));
}
