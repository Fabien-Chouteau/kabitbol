MEMORY
{
  ram (rwx) : org = 0x40000000, l = 16M
}

SECTIONS
{
  . = 0x40000000;
	.text : { *(.trapvector) } >ram
  .text : { *(.text) } >ram
  .rela.text : { *(.rela.text) } >ram
  .data : { *(.data) } >ram
  .rodata : { *(.rodata) } >ram
  .bss : { *(.bss) *(COMMON)} >ram

	. = ALIGN(4);
  . += 4096;
  _kstack = .;
  . = ALIGN(4);
  __brk = .;
}
