CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wextra
CFLAGS += -nostdinc -nostdlib -fno-builtin -fno-common
# https://forum.osdev.org/viewtopic.php?f=1&t=28307
CFLAGS += -fno-stack-check -fno-stack-protector -mno-stack-arg-probe
CFLAGS += -Wl,--subsystem,10

all: build/OVMF.fd fs/EFI/BOOT/BOOTX64.EFI

build/BOOTX64.EFI: build/bootloader.o build/efi.o build/file.o build/hardware_info.o build/memory.o build/util.o
	mkdir -p build
	$(CC) $(CFLAGS) -e efi_main -o $@ $^

build/%.o: %.c
	mkdir -p build
	$(CC) $(CFLAGS) -Iinclude -c -o $@ $<

build/OVMF.fd:
	mkdir -p build
	wget -O build/OVMF.zip http://downloads.sourceforge.net/project/edk2/OVMF/OVMF-X64-r15214.zip
	unzip build/OVMF.zip -d build

fs/EFI/BOOT/BOOTX64.EFI: build/BOOTX64.EFI
	mkdir -p fs/EFI/BOOT
	cp $< $@

clean:
	rm -Rf build/* fs/*

.PHONY: all clean
