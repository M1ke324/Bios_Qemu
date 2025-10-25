# Bios_Qemu

This project assembles a BIOS-like binary

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
    Launches the built image in QEMU for testing.

- `debug`  
    Starts QEMU with debugging options. Qemu opens a port on :1234 for gdb debugging and wait

- `clean` 
    Remove bios.bin, logfiles and build files.

- `cleanLog`
    Remove logfiles