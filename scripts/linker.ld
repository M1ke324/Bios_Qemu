OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(entry);

MEMORY
{
    rom (rx) : ORIGIN = 0xFFFE0000 , LENGTH = 128K
}

SECTIONS
{
  .text :
  {
    . = ORIGIN(rom);
    *(.text)
    *(.text.*)
  } > rom

  .rodata :
  {
    *(.rodata)
    *(.rodata.*)
  } > rom

  .data :
  {
    *(.data)
    *(.data.*)
  } > rom

  .bss :
  {
    *(.bss)
    *(.bss.*)
    *(COMMON)
  } > rom

  .16 :
  {
    . = (ORIGIN(rom) + LENGTH(rom) - 0x10000);
    *(.rodata16)
    . = ALIGN(2);
    *(.text16)
  } > rom

  .entry :
  {
    . = (ORIGIN(rom) + LENGTH(rom) - 16);
    *(.entry);
    . = (ORIGIN(rom) + LENGTH(rom) -1);
    BYTE(0x00);
  } > rom

  /DISCARD/ :
  {
    *(.note.gnu.property)
    *(.comment)
    *(.eh_frame)
  }
}
