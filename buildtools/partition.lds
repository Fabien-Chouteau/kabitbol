MEMORY
{
  ram (rwx) : org = 0x10000, l = 16M
}

ENTRY(__init)
FORCE_COMMON_ALLOCATION

SECTIONS
{
  . = 0x10000;
  .text : { *(.text) }
  .rela.text : { *(.rela.text) }
  .data : { *(.data) }
  .rodata : { *(.rodata) }
  .bss : { *(.bss) *(COMMON) }
}
