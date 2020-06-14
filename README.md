# utokyo_syspro_baremetal
Baremetal environment for "System programming lab" class in Dept. of Information Science, The University of Tokyo.


## Dependencies
The bootloader is implemented referencing UEFI Specification Version 2.8 ([PDF](https://uefi.org/sites/default/files/resources/UEFI_Spec_2_8_final.pdf)).
To support UEFI boot in qemu emulation, OVMF is automatically installed by `make` command. The following tools need to
be installed by users to build the bootloader, the kernel and the apps.

- x86_64-w64-mingw32-gcc : Cross-compiler for building UEFI bootloader
- qemu-system-x86_64 : Emulator
- unzip : Tool for unzipping OVMF.zip

```
$ sudo apt update
$ sudo apt upgrade
$ sudo apt install -y mingw-w64 qemu-system-x86 unzip git make gcc
```

The bootloader can be built on any environment if the cross-compiler 'mingw-64' is installed. The kernel and the apps
have to be built on x86_64 Linux (in order to use `-T` option of `ld` command). I do not assume some specific
version of `gcc`, but the `gcc` version on writer's test environment is shown below.

```
$ gcc --version

gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
Copyright (C) 2017 Free Software Foundation, Inc.
```


## Quick start
After installing `x86_64-w64-mingw32-gcc` and `qemu-system-x86_64`, do the following commands.

```
$ git clone https://github.com/sykwer/utokyo_syspro_baremetal
$ cd utokyo_syspro_baremetal/kernel
$ make qemu
```

The kernel will paint all the part of the window in ayame color.

![alt Physical memory map](https://raw.githubusercontent.com/sykwer/utokyo_syspro_baremetal/master/images/ayame.png)


## Boot from USB
Booting from USB is supported on UEFI-boot suppored systems. The writer tested USB boot on the following machines.

- HP ZBook 15v G5
- MacBookPro 2014
- MacBookPro 2019

To boot from USB, follow the instructions below.

- Format Partition1 of USB drive in FAT32
- Run 'make' command in 'kernel' directory
- Copy all the files under 'fs/' directory to USB
- Boot from USB on your real machine


## Physical memory map
The built images of the kernel and the apps are loaded by the bootloader to the physical addresses dipicted in the figure below.
- kernel : origin=0x100000000, length=16MiB
- app1   : origin=0x104000000, length=16MiB
- app2   : origin=0x105000000, length=16MiB
- app3   : origin=0x106000000, length=16MiB

![alt Physical memory map](https://raw.githubusercontent.com/sykwer/utokyo_syspro_baremetal/master/images/memory_map.png)
