#COMPILER
CC = gcc
CFLAG = -fno-pie -no-pie -nostdlib -m32 -std=c89 -ffreestanding -fno-builtin -Wall -Wextra -O2 -g3 -fno-stack-protector

#LINKER
LSCRIPTS = scripts/linker.ld 
LFLAG = -T $(LSCRIPTS) # -Wl,-Map=bios.map # to debug teh linker scripts

#BIN
BINTOOL = objcopy
BINFLAG = -O binary

#DIR
BUILDDIR = build
LOGDIR = logfiles

#MACHINE
QEMU = qemu-system-x86_64
QEMUFLAG = -vga std -no-reboot -no-shutdown -bios bios.bin
QEMUDEBUG = -d in_asm,cpu_reset,exec,unimp,int,vpu -trace enable=vga* -D logfiles/logfile$$(date "+%y-%m-%d-%H:%M:%S").txt -S -s  #
	#debug version require to digit "c" to start

#FILE
IN = entry/entry.s utilities/utilities.* vga/vga.*
ELF = $(BUILDDIR)/bios.elf
BIN = bios.bin

# PHONY
.PHONY: all clean run cleanLog debug 

all: $(BIN)

$(ELF): $(IN) $(BUILDDIR) $(LSCRIPTS)
	$(CC) $(IN) $(CFLAG) $(LFLAG) -o $@

$(BIN): $(ELF)
	$(BINTOOL) $(BINFLAG) $(ELF) $@

$(BUILDDIR):
	mkdir -p $@

$(LOGDIR):
	mkdir -p $@

clean: cleanLog
	rm -rf $(BIN) $(BUILDDIR)

cleanLog:
	rm -rf $(LOGDIR)

run: $(BIN)
	$(QEMU) $(QEMUFLAG)

debug: $(BIN) $(LOGDIR)
	$(QEMU) $(QEMUFLAG) $(QEMUDEBUG)
