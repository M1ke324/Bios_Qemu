.PHONY: all clean run cleanLog run-debug logfiles

all: bios.bin 

bios.bin: entry.asm 
	nasm -f bin entry.asm -o bios.bin

clean: cleanLog
	rm -rf bios.bin

cleanLog:
	rm -rf logfiles/*.txt

run: bios.bin logfiles
	qemu-system-x86_64 -serial stdio -vga std -d in_asm,cpu_reset,exec,unimp,int -no-reboot -no-shutdown -D logfiles/logfile$$(date "+%y-%m-%d-%H:%M:%S").txt -bios bios.bin

debug: bios.bin logfiles
	qemu-system-x86_64 -serial stdio -vga std -d in_asm,cpu_reset,exec,unimp,int -no-reboot -no-shutdown -D logfiles/logfile$$(date "+%y-%m-%d-%H:%M:%S").txt -bios bios.bin -S -s

logfiles:
	mkdir -p logfiles