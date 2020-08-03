CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common
LDFLAGS = -s -x

all: kernel apps/app1 apps/app2 apps/app3
	make -C ../
	make copy

kernel: kernel.o  segmentation.o hardware.o load_gdt.o font.o
	ld $(LDFLAGS) -T kernel.ld -o $@ $+

apps/app1: apps/app1.o
	ld $(LDFLAGS) -T apps/app1.ld -o $@ $+

apps/app2: apps/app2.o
	ld $(LDFLAGS) -T apps/app2.ld -o $@ $+

apps/app3: apps/app3.o
	ld $(LDFLAGS) -T apps/app3.ld -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

apps/%.o: apps/%.c
	gcc $(CFLAGS) -c -o $@ $<

%.o:%.s
	gcc $(CFLAGS) -c -o $@ $<

qemu: kernel apps/app1 apps/app2 apps/app3
	make all
	qemu-system-x86_64 -m 4G -bios ../build/OVMF.fd -hda fat:rw:../fs \
		-netdev user,id=u1,hostfwd=tcp::8080-:80 -device e1000,netdev=u1 \
		-object filter-dump,id=f1,netdev=u1,file=dump.pcap \
		-monitor unix:qemu-monitor-socket,server,nowait

copy:
	cp kernel ../fs/
	cp apps/app1 ../fs/
	cp apps/app2 ../fs/
	cp apps/app3 ../fs/

monitor:
	socat -,echo=0,icanon=0 unix-connect:qemu-monitor-socket

clean:
	rm -f *.o *apps/*.o apps/app1 apps/app2 apps/app3  kernel

.PHONY: all qemu copy monitor clean
