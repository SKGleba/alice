#ifndef __STOR_H__
#define __STOR_H__

#include "sdif.h"
#include "types.h"
#include "utils.h"
#include "defs.h"
#include "bob.h"

#ifndef SDIF_UNUSE
int stor_read_sd(uint32_t sector_off, void* dst, uint32_t sector_count);
int stor_write_sd(uint32_t sector_off, void* dst, uint32_t sector_count);
int stor_read_emmc(uint32_t sector_off, void* dst, uint32_t sector_count);
int stor_write_emmc(uint32_t sector_off, void* dst, uint32_t sector_count);
int stor_init_sd(bool init_ctrl);
int stor_init_emmc(bool init_ctrl, bool set_keys);
int stor_export_ctx(int dev_id, unk2_sdif_gigactx* dst_gctx, unk_sdif_ctx_init* dst_sctx);
int stor_import_ctx(int dev_id, unk2_sdif_gigactx* src_g, unk_sdif_ctx_init* src_s);
#else
#define stor_read_sd(sector, dst, count) bob_sendSimpleCmd(BOB_A2B_READ_SD, sector, (uint32_t)dst, count)
#define stor_write_sd(sector, dst, count) bob_sendSimpleCmd(BOB_A2B_WRITE_SD, sector, (uint32_t)dst, count)
#define stor_read_emmc(sector, dst, count) bob_sendSimpleCmd(BOB_A2B_READ_EMMC, sector, (uint32_t)dst, count)
#define stor_write_emmc(sector, dst, count) bob_sendSimpleCmd(BOB_A2B_WRITE_EMMC, sector, (uint32_t)dst, count)
#define stor_init_sd(init_ctrl) bob_sendSimpleCmd(BOB_A2B_INIT_STORAGE, !SDIF_DEV_SD, init_ctrl, 0)
#define stor_init_emmc(init_ctrl, set_keys) bob_sendSimpleCmd(BOB_A2B_INIT_STORAGE, !SDIF_DEV_EMMC, init_ctrl, set_keys)
#define stor_export_ctx(a, b) stub()
#define stor_import_ctx(a, b) stub()
#endif

#endif