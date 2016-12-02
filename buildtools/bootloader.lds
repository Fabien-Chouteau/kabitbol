
MEMORY
{
  flash (rx) : org = 0x0, l = 16M
}

SECTIONS
{
  . = 0x0;
  .text : { *(.start) } >flash
  .text : { *(.text) } >flash
  .data : { *(.data) } >flash
  .bss : { *(.bss) } >flash
	.rodata : { *(.rodata) } >flash
}
