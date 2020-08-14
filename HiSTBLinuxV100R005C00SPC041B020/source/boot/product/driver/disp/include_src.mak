SRC     := $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/drv_disp_adp2unf.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/drv/drv_display.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/alg/drv_disp_alg_rgb2yuv.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/com/drv_disp_osal.c)

SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/common/drv_disp_da.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/common/drv_disp_intf.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/common/drv_disp_hw.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/common/drv_disp_hal.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/common/drv_disp_rwzb.c)

SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/hal_specific_config.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_software_selfdefine.c)

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100 hi3716mv410 hi3716mv420),)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_driver.c)
endif


ifeq ($(DRIVER_PATH), vdp_v4_0)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_hal_chn.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/hd_date_driver.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_hal_vid.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/sd_date_driver.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_hal_comm.c)

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200),)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/hal_specific_rwzb.c)
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a),)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/component/drv_com_logo.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/hal_specific/vdp_hal_intf.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/hal_specific/vdp_software_selfdefine.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/hal_specific/hal_specific_config.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/hal_specific/hal_specific_rwzb.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_hal_mmu.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_drv_wbc.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_hal_wbc.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_drv_coef.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_drv_vid.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_drv_pq_zme.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_drv_pq_csc.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_drv_comm.c)
SRC     += $(wildcard $(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/vdp_func_pq_csc.c)
CFLAGS  += -I$(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/hal_specific
endif

endif
