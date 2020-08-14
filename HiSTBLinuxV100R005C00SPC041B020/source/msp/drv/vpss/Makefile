#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
VPSS_DIR := $(MSP_DIR)/drv/vpss

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)

ifeq ($(CFG_HI_TEE_SUPPORT),y)
EXTRA_CFLAGS += -I$(KERNEL_DIR)/$(CFG_HI_KERNEL_VERSION)/drivers/hisilicon/sec_mmz
endif

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

ifdef CFG_HI_VPSS_MAX_BUFFER_NUMB
EXTRA_CFLAGS += -DHI_VPSS_MAX_BUFFER_NUMB=$(CFG_HI_VPSS_MAX_BUFFER_NUMB)
else
EXTRA_CFLAGS += -DHI_VPSS_MAX_BUFFER_NUMB=10
endif

ifdef CFG_HI_SMMU_SUPPORT
EXTRA_CFLAGS += -DHI_VPSS_SMMU_SUPPORT
endif
MOD_NAME := hi_vpss

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

ifeq ($(CFG_HI_CHIP_TYPE), hi3796mv200)
EXTRA_CFLAGS += -DCHIP_TYPE_hi3798mv200
EXTRA_CFLAGS := $(filter-out %CHIP_TYPE_hi3796mv200, $(EXTRA_CFLAGS))
endif


ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a hi3796mv200),)
    EXTRA_CFLAGS += -I$(VPSS_DIR)/vpss_v4_0/                    \
                    -I$(VPSS_DIR)/vpss_v4_0/hal                 \
                    -I$(VPSS_DIR)/vpss_v4_0/vpss_in             \
                    -I$(VPSS_DIR)/vpss_v4_0/alg \
                    -I$(VPSS_DIR)/vpss_v4_0/vpss_in/hi3798cv200 \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798cv200     \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200     \
                    -I$(VPSS_DIR)/vpss_v4_0/vpss_in/hi3798mv200 \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/hal \
    		    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/common_inc \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/intf \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/base \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/alg \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/func \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/reg \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/debug \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/info \
                    -I$(VPSS_DIR)/vpss_v4_0/hal/hi3798mv200/frame
    DRIVER_PATH := $(CURDIR)/vpss_v4_0
else
    EXTRA_CFLAGS += -I$(VPSS_DIR)/vpss_v3_0/                    \
                    -I$(VPSS_DIR)/vpss_v3_0/hal                 \
                    -I$(VPSS_DIR)/vpss_v3_0/hal/hi3798mv100     \
                    -I$(VPSS_DIR)/vpss_v3_0/hal/hifoneb02       \
                    -I$(VPSS_DIR)/vpss_v3_0/hal/hi3716mv410     \
                    -I$(VPSS_DIR)/vpss_v3_0/vpss_in             \
                    -I$(VPSS_DIR)/vpss_v3_0/vpss_in/hi3798mv100 \
                    -I$(VPSS_DIR)/vpss_v3_0/vpss_in/hifoneb02   \
                    -I$(VPSS_DIR)/vpss_v3_0/vpss_in/hi3716mv410 \
                    -I$(VPSS_DIR)/vpss_v3_0/alg
    DRIVER_PATH := $(CURDIR)/vpss_v3_0
endif


ifeq ($(DRIVER_PATH), $(CURDIR)/vpss_v4_0)
$(MOD_NAME)-y   :=  vpss_v4_0/vpss_osal.o \
                    vpss_v4_0/vpss_info.o\
                    vpss_v4_0/vpss_fb.o \
                    vpss_v4_0/vpss_mcdei.o \
                    vpss_v4_0/vpss_stt_mcdei.o \
		    vpss_v4_0/vpss_wbc_mcdei.o \
                    vpss_v4_0/vpss_wbc.o \
                    vpss_v4_0/vpss_in/vpss_src.o \
                    vpss_v4_0/vpss_his.o \
                    vpss_v4_0/vpss_sttinf.o \
                    vpss_v4_0/vpss_in/vpss_img.o \
                    vpss_v4_0/alg/vpss_alg_ratio.o \
                    vpss_v4_0/alg/vpss_alg_rwzb.o \
                    vpss_v4_0/alg/vpss_alg_scd.o\
                    vpss_v4_0/vpss_rwzb.o \
                    vpss_v4_0/vpss_instance.o\
                    vpss_v4_0/vpss_ctrl.o \
                    vpss_v4_0/drv_vpss_intf_k.o \
                    vpss_v4_0/vpss_trans_fb.o
endif
#
ifeq ($(DRIVER_PATH), $(CURDIR)/vpss_v3_0)
$(MOD_NAME)-y   :=  vpss_v3_0/vpss_osal.o \
                    vpss_v3_0/vpss_info.o\
                    vpss_v3_0/vpss_fb.o \
                    vpss_v3_0/vpss_wbc.o \
                    vpss_v3_0/vpss_in/vpss_src.o \
                    vpss_v3_0/vpss_his.o \
                    vpss_v3_0/vpss_sttinf.o \
                    vpss_v3_0/vpss_in/vpss_img.o \
                    vpss_v3_0/alg/vpss_alg_ratio.o \
                    vpss_v3_0/alg/vpss_alg_rwzb.o \
                    vpss_v3_0/alg/vpss_alg_scd.o\
                    vpss_v3_0/vpss_rwzb.o \
                    vpss_v3_0/vpss_instance.o\
                    vpss_v3_0/vpss_ctrl.o \
                    vpss_v3_0/drv_vpss_intf_k.o \
                    vpss_v3_0/vpss_trans_fb.o
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200),)
$(MOD_NAME)-y   +=  \
            vpss_v4_0/vpss_in/hi3798cv200/vpss_in_3798cv200.o \
            vpss_v4_0/hal/hi3798cv200/vpss_reg_3798cv200.o \
            vpss_v4_0/hal/hi3798cv200/vpss_hal_3798cv200.o
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a hi3796mv200),)
$(MOD_NAME)-y   +=  \
                    vpss_v4_0/hal/hi3798mv200/alg/vpss_cbb_alg.o \
                    vpss_v4_0/hal/hi3798mv200/base/vpss_cbb_base.o \
                    vpss_v4_0/hal/hi3798mv200/debug/vpss_cbb_debug.o \
                    vpss_v4_0/hal/hi3798mv200/debug/vpss_cbb_assert.o \
                    vpss_v4_0/hal/hi3798mv200/frame/vpss_cbb_frame.o \
                    vpss_v4_0/hal/hi3798mv200/func/vpss_cbb_func.o \
                    vpss_v4_0/hal/hi3798mv200/info/vpss_cbb_info.o \
                    vpss_v4_0/hal/hi3798mv200/intf/vpss_cbb_intf.o \
                    vpss_v4_0/hal/hi3798mv200/util/vpss_cbb_util.o \
                    vpss_v4_0/hal/hi3798mv200/hal/vpss_cbb_hal.o \
                    vpss_v4_0/hal/hi3798mv200/reg/vpss_cbb_reg.o \
                    vpss_v4_0/vpss_in/hi3798mv200/vpss_in_3798mv200.o
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
$(MOD_NAME)-y   +=  \
            vpss_v3_0/hal/hi3798mv100/vpss_reg_3798m.o \
            vpss_v3_0/hal/hi3798mv100/vpss_hal_3798m.o \
            vpss_v3_0/vpss_in/hi3798mv100/vpss_in_3798mv100.o
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420),)
$(MOD_NAME)-y   +=  \
            vpss_v3_0/hal/hi3716mv410/vpss_reg_3716mv410.o \
            vpss_v3_0/hal/hi3716mv410/vpss_hal_3716mv410.o \
            vpss_v3_0/vpss_in/hi3716mv410/vpss_in_3716mv410.o
endif


REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=$(CURDIR) modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	$(AT)cp -f $(CURDIR)/$(MOD_NAME).ko $(HI_MODULE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/$(MOD_NAME).ko

