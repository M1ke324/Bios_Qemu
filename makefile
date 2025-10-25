.PHONY: clean run cleanLog debug 

bios.bin: build/bios.elf
	objcopy -O binary build/bios.elf bios.bin

build/bios.elf:	entry.s build
	gcc entry.s  \
		-fno-pie \
		-no-pie  \
		-nostdlib \
		-m32 	 \
		-T scripts/linker.ld \
		-o build/bios.elf 

build:
	mkdir -p build

logfiles:
	mkdir -p logfiles

# PHONY

clean: cleanLog
	rm -rf bios.bin build

cleanLog:
	rm -rf logfiles/*.txt

run: bios.bin logfiles
	qemu-system-x86_64 -vga std -d in_asm,cpu_reset,exec,unimp,int -no-reboot -no-shutdown -D logfiles/logfile$$(date "+%y-%m-%d-%H:%M:%S").txt -bios bios.bin

debug: bios.bin logfiles
	qemu-system-x86_64 -vga std -d in_asm,cpu_reset,exec,unimp,int -no-reboot -no-shutdown -D logfiles/logfile$$(date "+%y-%m-%d-%H:%M:%S").txt -bios bios.bin -S -s
