.PHONY: clean run cleanLog debug 

bios.bin: build/bios.elf
	objcopy -O binary build/bios.elf bios.bin

build/bios.elf:	entry.s build
	gcc entry.s  \
		-fno-pie \ 				#Tell compiler to not create a position independent executable
		-no-pie  \ 				#Tell linker to not create a position independent executable
		-nostdlib \				#Do not use standard library
		-m32 	 \				#Generate 32 bit elf (As do not receive this directive, it will read the .code directive in the assembly file)
		-T scripts/linker.ld \	#Use custom linker script to place code entry code at 0xFFFFFFF0
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
