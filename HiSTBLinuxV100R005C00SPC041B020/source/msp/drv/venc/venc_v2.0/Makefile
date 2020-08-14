#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif
#===============================================================================
# local variables
#===============================================================================
ifeq ($(CFG_HI_64BIT_SYSTEM),y)
ASM_DST_DIR := 64bit
else
ASM_DST_DIR := 32bit
endif

EXTRA_CFLAGS += $(filter-out -Wformat-security,$(CFG_HI_KMOD_CFLAGS))
ifeq ($(CFG_HI_CHIP_TYPE),hi3716cv200)
EXTRA_CFLAGS += -D__VENC_3716CV200_CONFIG__
EXTRA_CFLAGS += -D__VENC_SUPPORT_JPGE__
endif

ifeq ($(CFG_HI_CHIP_TYPE),hi3716mv400)
EXTRA_CFLAGS += -D__VENC_3716CV200_CONFIG__
EXTRA_CFLAGS += -D__VENC_SUPPORT_JPGE__
endif

ifeq ($(CFG_HI_CHIP_TYPE),hi3719cv100)
EXTRA_CFLAGS += -D__VENC_3716CV200_CONFIG__
EXTRA_CFLAGS += -D__VENC_SUPPORT_JPGE__
endif

ifeq ($(CFG_HI_CHIP_TYPE),hi3718cv100)
EXTRA_CFLAGS += -D__VENC_3716CV200_CONFIG__
EXTRA_CFLAGS += -D__VENC_SUPPORT_JPGE__
endif

ifeq ($(CFG_HI_CHIP_TYPE),hi3718mv100)
EXTRA_CFLAGS += -D__VENC_3716CV200_CONFIG__  -D__VENC_3719MV100_CONFIG__
endif

ifeq ($(CFG_HI_CHIP_TYPE),hi3719mv100)
EXTRA_CFLAGS += -D__VENC_3716CV200_CONFIG__  -D__VENC_3719MV100_CONFIG__
endif

ifeq ($(CFG_HI_CHIP_TYPE),hi3751v100)
EXTRA_CFLAGS += -D__VENC_S5V100_CONFIG__
endif

ifeq ($(CFG_HI_CHIP_TYPE),hi3751v100b)
EXTRA_CFLAGS += -D__VENC_S5V100_CONFIG__
endif

ifeq  ($(CFG_HI_CHIP_TYPE),hi3798mv100)
EXTRA_CFLAGS += -D__VENC_98M_CONFIG__
EXTRA_CFLAGS += -D__VENC_SUPPORT_JPGE__
endif

ifeq  ($(CFG_HI_CHIP_TYPE),hi3796mv100)
EXTRA_CFLAGS += -D__VENC_98M_CONFIG__
EXTRA_CFLAGS += -D__VENC_SUPPORT_JPGE__
endif

ifeq  ($(CFG_HI_CHIP_TYPE),hi3716mv420)
EXTRA_CFLAGS += -D__VENC_98M_CONFIG__
EXTRA_CFLAGS += -D__VENC_SUPPORT_JPGE__
endif

ifeq  ($(CFG_HI_CHIP_TYPE),hi3798cv200)
EXTRA_CFLAGS += -D__VENC_98CV200_CONFIG__
endif

ifeq  ($(CFG_HI_CHIP_TYPE),hi3798mv200)
EXTRA_CFLAGS += -D__VENC_98MV200_CONFIG__
EXTRA_CFLAGS += -D__VENC_SUPPORT_JPGE__
endif

ifeq  ($(CFG_HI_CHIP_TYPE),hi3798mv200_a)
EXTRA_CFLAGS += -D__VENC_98MV200_CONFIG__
EXTRA_CFLAGS += -D__VENC_SUPPORT_JPGE__
EXTRA_CFLAGS += -D__VENC_98MV200_a_CONFIG__
endif

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/api/venc                   \
                -I$(MSP_DIR)/drv/venc                   \
                -I$(MSP_DIR)/drv/jpge/include           \
                -I$(MSP_DIR)/api/jpge/include

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

MOD_NAME := hi_venc

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y   :=  drv_venc_osal.o      \
                    drv_venc_intf.o      \
                    drv_venc.o           \
                    drv_venc_efl.o       \
                    drv_omxvenc.o        \
                    drv_omxvenc_efl.o    \
                    $(ASM_DST_DIR)/drv_venc_ratecontrol.o\
                    drv_venc_buf_mng.o   \
                    drv_venc_queue_mng.o \
                    drv_venc_proc.o      \
                    $(ASM_DST_DIR)/hal_venc.o
                    
#===============================================================================#       
asflags-y := -I$(COMMON_DRV_INCLUDE)
ifeq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE, $(CFG_HI_CFLAGS)), -DHI_ADVCA_FUNCTION_RELEASE)
asflags-y += -DCONFIG_REMOVE_STRING
endif
#===============================================================================

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall asm asm_clean

all:
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=$(CURDIR) modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	$(AT)cp -f $(CURDIR)/$(MOD_NAME).ko $(HI_MODULE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/$(MOD_NAME).ko	

