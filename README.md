# Bios_Qemu

This project assembles a BIOS-like binary

## Requirements

This project is intended to be run with QEMU. It requires:

- nasm
- make
- qemu-system-x86_64 (or another suitable QEMU binary) to run the image

Install examples:
- Debian/Ubuntu: `sudo apt install nasm make qemu-system-x86`
- macOS (Homebrew): `brew install nasm qemu`

## Makefile overview

The included Makefile automates assembling, linking and running the BIOS image. Common targets:

- `bios.bin` (default)
    Assemble source files with nasm

- `run`  
    Launches the built image in QEMU for testing.

- `debug`  
    Starts QEMU with debugging options. Qemu opens a port on :1234 for gdb debugging and wait

- `clean` 
    Remove bios.bin and logfiles.

- `cleanLog`
    Remove logfiles

## Notes

- Edit QEMU options in the Makefile if you need a different machine, CPU or amount of memory.
