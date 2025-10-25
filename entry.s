
.section .rodata

# ================ GDT ================

temp_gdt: 
  .quad 0 # NULL first descriptor 

# Code segment descriptor (Index 1, Selector 0x08)
code_seg: 
  .word 0xFFFF                   # Limit (0-15)
  .word 0x0000                   # Base (0-15)
  .byte 0x00                     # Base (16-23)
  .byte 0b10011010               # Access Byte: Present, Ring 0, Code, Executable, Readable
  .byte 0b11001111               # Flags (4K granularity, 32-bit) + Limit (16-19)
  .byte 0x00                     # Base (24-31)

# Data segment descriptor (Index 2, Selector 0x10)
data_seg:
  .word 0xFFFF
  .word 0x0000
  .byte 0x00
  .byte 0b10010010               # Access Byte: Present, Ring 0, Data, Writable
  .byte 0b11001111
  .byte 0x00
temp_gdt_end:

# ================ GDT DESCRIPTOR ================

temp_gdt_descriptor:
  .word temp_gdt_end - temp_gdt - 1   # limit
  .long temp_gdt                      # base

# ==== IDT DESCRIPTOR ====

idt_descriptor:
  .word 0 
  .long 0

# ================ ENTRY POINT 16 BIT ================

.section .text16
.code16

START: 
  cli           # disable interrupt
  movw %cs, %ax
  movw %ax, %ds # setup data segment

  # Load IDT and GDT
  
  lidt (idt_descriptor)
  lgdtl (temp_gdt_descriptor)

  # enter protected code
  movl %cr0, %ebx 
  orw $0x1, %bx 
  movl %ebx, %cr0

  movl $protected_mode, %eax
  # far jump to clear prefetch queue
  ljmpl $0x08,$protected_mode

# ================ ENTRY POINT PROTECTED MODE 32 BIT ================

.section .text
.code32

protected_mode:
  # Setup segment registers and stack
  movw $0x10, %ax # Data segment selector
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  movl $0x90000, %esp
  movw %ax, %ss


  # test write memory
  movl $0x12345678, (0x1000)

  # test write to video memory, vga not working in qemu
  movw $0x0F48, (0xB8000)  # Attribute 0x0F, Character 'H' (0x48)
  movw $0x0F49, (0xB8002)  # Attribute 0x0F, Character 'I' (0x49)
  #HLT
  jmp . #Infinite loop, hang it here.

# ================ REAL ENTRY POINT FOR BIOS ================

.section .entry
.global entry
.code16

entry:
  jmp START
