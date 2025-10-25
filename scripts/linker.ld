OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(entry);

MEMORY
{
    rom (rx) : ORIGIN = 0xFFFF0000, LENGTH = 64K
}

SECTIONS
{
    
    .rodata :
    {
        . = ORIGIN(rom);
        *(.rodata)
    } > rom AT> rom
    
    .text :
    {
        . = ALIGN(2);
        *(.text16)
        . = ALIGN(4);
        *(.text)
    } > rom AT> rom 

    .entry  :
    {
        . = (ORIGIN(rom) + 0xFFF0);
        *(.entry);
        . = (ORIGIN(rom) + 0xFFFF);
        BYTE(0x00);
    } > rom AT> rom = 0

    /DISCARD/ :
    {
         *(.note.gnu.property)
         *(.comment)
         *(.eh_frame)
         *(.bss)
         *(.data)
         *(.COMMON)
    }
}