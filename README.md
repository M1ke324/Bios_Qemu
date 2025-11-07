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
    Launches the built image in QEMU.

- `debug`  
    Starts QEMU with debugging options. Qemu opens a port on :1234 for gdb debugging and wait. It aslo print a log of the code execute by the machine and the read/write operations on the vga registers.

- `clean` 
    Remove bios.bin, logfiles and build files.

- `cleanLog`
    Remove logfiles

---

  ## Debug vga
  
- [x] Alla fine dell'inizializzaione i registri contengono effetivamente i valori elencati nell'array  
- [x] La memoria video è coerente con la modalità testo. Dump:  
      ```
        20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07
        20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07
        20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07
        20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07
        20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07
        20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07
        20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07 20 07
      ```  
- [x] Il plane 2 contiente effettivamente i font. Dump:  
     ```00000860: 00000000  .
        00000861: 00000000  .
        00000862: 00111100  <
        00000863: 01100110  f
        00000864: 11000010  .
        00000865: 11000000  .
        00000866: 11000000  .
        00000867: 11000000  .
        00000868: 11000000  .
        00000869: 11000010  .
        0000086a: 01100110  f
        0000086b: 00111100  <
        0000086c: 00000000  .
        0000086d: 00000000  .
        0000086e: 00000000  .
        0000086f: 00000000  .
        00000870: 00000000  .
        00000871: 00000000  .
        00000872: 00000000  .
        00000873: 00000000  .
        00000874: 00000000  .
        00000875: 00000000  .
        00000876: 00000000  .
        00000877: 00000000  .
        00000878: 00000000  .
        00000879: 00000000  .
        0000087a: 00000000  .
        0000087b: 00000000  .
        0000087c: 00000000  .
        0000087d: 00000000  .
        0000087e: 00000000  .
        0000087f: 00000000  .
        ```
