OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

SECTIONS
{
	INCLUDE ../output/alice_linker.x
	. = 0x40000000;
	.text   : { *(.text.rpcp) *(.text   .text.*   .gnu.linkonce.t.*) }
  	.rodata : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
  	.data   : { *(.data   .data.*   .gnu.linkonce.d.*) }
  	.bss    : { *(.bss    .bss.*    .gnu.linkonce.b.*) *(COMMON) }
}
