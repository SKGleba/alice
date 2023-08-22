OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

SECTIONS
{
	INCLUDE cfg.x /* configuration data */

	. = cfg_prog_load_off;

	.text : { 
		*(.text.vectors) /* exception vectors & cfg ptrs */
		*(.text*) 
	}

	.rodata ALIGN(4) : SUBALIGN(4) { *(.rodata*) }

	.data ALIGN(4) : SUBALIGN(4) {
		*(.data*)
		. = ALIGN(4);
	}

	.bss ALIGN(4) (NOLOAD) : SUBALIGN(4) {
		prog_bss_addr = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		prog_bss_end = .;
	}
}
