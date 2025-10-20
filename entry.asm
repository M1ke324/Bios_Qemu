[bits 16]

ORG 0xF0000

align 4

; ======== GDT =======

temp_gdt: 
    DQ 0 ; NULL first descriptor 

; Descrittore segmento codice (Indice 1, Selettore 0x08)
code_seg: 
  DW 0xFFFF                   ; Limite (0-15)
  DW 0x0000                   ; Base (0-15)
  DB 0x00                     ; Base (16-23)
  DB 0b10011010               ; Access Byte: Present, Ring 0, Code, Executable, Readable
  DB 0b11001111               ; Flags (4K granularity, 32-bit) + Limite (16-19)
  DB 0x00                     ; Base (24-31)

; Descrittore segmento dati (Indice 2, Selettore 0x10)
data_seg:
  DW 0xFFFF
  DW 0x0000
  DB 0x00
  DB 0b10010010               ; Access Byte: Present, Ring 0, Data, Writable
  DB 0b11001111
  DB 0x00
temp_gdt_end:

; ==== GDT DESCRIPTOR ====

temp_gdt_descriptor:
  DW temp_gdt_end - temp_gdt - 1 ; limit
  DD temp_gdt ; base

; ==== IDT DESCRIPTOR ====

idt_descriptor:
  DW 0 
  DD 0

; ====  PROTECTED MODE JUMP ====

protected_mode_jump:
  DD protected_mode  ; Offset a 32 bit (l'indirizzo fisico reale)
  DW CODE_SEGMENT


CODE_SEGMENT EQU code_seg - temp_gdt
DATA_SEGMENT EQU data_seg - temp_gdt
PE_BIT EQU 0x1

; ======== ENTRY POINT 16 BIT ========

START: 
  
  CLI ; disable interrupt
  MOV AX, CS
  MOV DS, AX ; setup data segment

  
  LIDT [idt_descriptor] ; load empty IDT

  ; calculate GDT base address and load it
  MOV EAX, (0xF0000*4 + temp_gdt) 
  MOV [temp_gdt_descriptor + 2], EAX
  DB 0x66;
  LGDT [temp_gdt_descriptor]

  ; enter protected code
  MOV EBX, CR0 
  OR EBX, PE_BIT 
  MOV CR0, EBX

  ; far jump to clear prefetch queue
  DB 0x66; 
  jmp far [protected_mode_jump]

[bits 32]

; ======== ENTRY POINT PROTECTED MODE 32 BIT ========
protected_mode:
  ; Setup segment registers and stack
  MOV AX, DATA_SEGMENT
  MOV DS, AX
  MOV ES, AX
  MOV FS, AX
  MOV GS, AX
  MOV ESP, 0x90000
  MOV SS, AX


; test write memory
MOV DWORD [0x1000], 0x12345678

;test write to video memory, vga not working in qemu
MOV WORD [0xB8000], 0x0F48  ; Attribute 0x0F, Character 'H' (0x48)
MOV WORD [0xB8002], 0x0F49  ; Attribute 0x0F, Character 'I' (0x49)
;HLT
JMP $ ;Infinite loop, hang it here.

; ======== FILLER TO 0xFFF0 ========
TIMES 0xFFF0 - ($ - $$) DB 0


; ======== REAL ENTRY POINT FOR BIOS ========
[bits 16]
JMP START


; ======== FILLER TO 64KB ========
TIMES 0x10000 - ($-$$) DB 0
