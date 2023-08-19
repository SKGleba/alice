/* spad32k mode */
.cfg : {
		cfg_prog_uses_end = 0x00008000;
		cfg_sp_core0_addr = 0x00008000;
		cfg_sp_core1_addr = 0x00007000;
		cfg_sp_core2_addr = 0x00006000;
		cfg_sp_core3_addr = 0x00005000;
		cfg_max_prog_size = 0x00004000;
		cfg_prog_load_off = 0x00000000;
    }

/* dram mode
.cfg : {
		cfg_prog_uses_end = 0x00040000;
		cfg_sp_core0_addr = 0x00040000;
		cfg_sp_core1_addr = 0x0003c000;
		cfg_sp_core2_addr = 0x00038000;
		cfg_sp_core3_addr = 0x00034000;
		cfg_max_prog_size = 0x00030000;
		cfg_prog_load_off = 0x00000000;
    }
*/
