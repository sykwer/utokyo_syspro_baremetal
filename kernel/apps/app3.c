void app3() {
  while (1) {
    // char *str = "Hello from app3\r\n";

    // TODO: Say hello from here by issuing `puts` syscall
    // syscall_puts(str);

    volatile int i = 100000000;
    while (i--);
  }
}
