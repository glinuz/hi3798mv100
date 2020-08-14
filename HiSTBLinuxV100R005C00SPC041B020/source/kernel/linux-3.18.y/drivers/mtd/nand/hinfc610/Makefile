ccflags-y += -Idrivers/mtd/nand

obj-$(CONFIG_MTD_NAND_HINFC610) += hinfc610.o hinfc610_os.o hinfc610_gen.o hinfc610_dbg_inf.o \
	hinfc610_ecc_info.o hinfc610_rw_latch.o \
	hinfc610_read_retry_hynix_bg_cdie.o \
	hinfc610_read_retry_hynix_bg_bdie.o \
	hinfc610_read_retry_hynix_cg_adie.o \
	hinfc610_read_retry_micron.o  \
	hinfc610_read_retry_samsung.o \
	hinfc610_read_retry_toshiba_v2012.o \
	hinfc610_read_retry_toshiba_v2013.o \
	hinfc610_read_retry.o \
	hinfc610_sync.o \
	hinfc610_sync_onfi_23.o \
	hinfc610_sync_toggle.o \
	hinfc620_gen.o


obj-$(CONFIG_ARCH_HI3798MX) += hinfc610_rw_latch_hi3798mv100.o
obj-$(CONFIG_ARCH_S40)      += hinfc610_rw_latch_s40.o
obj-$(CONFIG_ARCH_HIFONE)   += hinfc610_rw_latch_hifone.o

obj-$(CONFIG_HINFC610_CACHE_ENABLE) += hinfc610_cache.o
obj-$(CONFIG_HINFC610_DBG_NAND_DEBUG) += hinfc610_dbg.o
obj-$(CONFIG_HINFC610_DBG_NAND_DUMP) += hinfc610_dbg_dump.o
obj-$(CONFIG_HINFC610_DBG_NAND_ERASE_COUNT) += hinfc610_dbg_erase_count.o
obj-$(CONFIG_HINFC610_DBG_NAND_READ_COUNT) += hinfc610_dbg_read_count.o
obj-$(CONFIG_HINFC610_DBG_NAND_ECC_COUNT) += hinfc610_dbg_ecc_count.o
obj-$(CONFIG_HINFC610_DBG_NAND_READ_RETRY) += hinfc610_dbg_read_retry.o
obj-$(CONFIG_HINFC610_DBG_NAND_CACHE) += hinfc610_dbg_cache.o

