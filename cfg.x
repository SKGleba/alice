/* spad32k mode */
.cfg : {
		cfg_prog_uses_end = _as_ARM_BOOT_end;
		cfg_sp_pcore_size = 0x00001000;
		cfg_sp_core0_addr = cfg_prog_uses_end;
		cfg_sp_core1_addr = cfg_sp_core0_addr - cfg_sp_pcore_size;
		cfg_sp_core2_addr = cfg_sp_core1_addr - cfg_sp_pcore_size;
		cfg_sp_core3_addr = cfg_sp_core2_addr - cfg_sp_pcore_size;
		cfg_prog_load_end = cfg_sp_core3_addr - cfg_sp_pcore_size;
		cfg_prog_load_off = _as_ARM_BOOT;
		cfg_phys_load_off = _as_SPAD32K;
		cfg_max_prog_size = cfg_prog_load_end - cfg_prog_load_off;
    }

/* dram mode
.cfg : {
		cfg_prog_uses_end = 0x00040000;
		cfg_sp_pcore_size = 0x00004000;
		cfg_sp_core0_addr = cfg_prog_uses_end;
		cfg_sp_core1_addr = cfg_sp_core0_addr - cfg_sp_pcore_size;
		cfg_sp_core2_addr = cfg_sp_core1_addr - cfg_sp_pcore_size;
		cfg_sp_core3_addr = cfg_sp_core2_addr - cfg_sp_pcore_size;
		cfg_max_prog_size = cfg_sp_core3_addr - cfg_sp_pcore_size;
		cfg_prog_load_off = _as_ARM_BOOT;
		cfg_phys_load_off = _as_MAIN_DRAM;
    }
*/
