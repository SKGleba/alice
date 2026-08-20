OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

SECTIONS
{
	INCLUDE psp2ref/misc/map.x /* adress space map */
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

	prog_act_size = (. - cfg_prog_load_off);
}

ASSERT(!(prog_act_size > cfg_max_prog_size), "cfg_max_prog_size");
