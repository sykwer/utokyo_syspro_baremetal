unsigned long long text_section_top = 0x106000000;

void app3() {
  // char *str = "Hello from app3\r\n";

  // TODO: Say hello from here by issuing `puts` syscall
  // syscall_puts(str);

  volatile int i = 100000000;
  while (i--);

  asm volatile ("jmp *%0" :: "m"(text_section_top));
}
