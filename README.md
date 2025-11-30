# Bios_Qemu

This project assembles a BIOS-like binary, that:  
- Enter protected mode with memory in flat mode
- Initialize VGA in text mode 80x25
- Map PCI devices 

## Requirements

This project is intended to be run with QEMU. It requires:

- gcc
- make
- qemu-system-x86_64 to run the bios

## Makefile overview

The included Makefile automates assembling, linking and running the BIOS image. Common targets:

- `bios.bin` (default)  
    Assemble source files

- `run`  
    Launches the built image in QEMU.

- `debug`  
    Starts QEMU with debugging options. Qemu opens a port on :1234 for gdb debugging and wait. It aslo print a log of the code execute by the machine and the read/write operations on the vga and pci registers.

- `clean` 
    Remove bios.bin, logfiles and build files.

- `cleanLog`
    Remove logfiles