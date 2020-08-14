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
DEMUX_DRV_VER = demux_v2

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE) \
                -I$(COMMON_API_INCLUDE) \
                -I$(COMMON_DRV_INCLUDE) \
                -I$(MSP_UNF_INCLUDE) \
                -I$(MSP_API_INCLUDE) \
                -I$(MSP_DRV_INCLUDE) \
                -I$(MSP_DIR)/api/demux/$(DEMUX_DRV_VER) \
                -I$(MSP_DIR)/drv/demux/$(DEMUX_DRV_VER) \
                -I$(MSP_DIR)/drv/demux/$(DEMUX_DRV_VER)/descrambler

ifdef CFG_HI_TEE_SUPPORT
EXTRA_CFLAGS += -I$(TEE_DRV_INCLUDE) -DDMX_TEE_SUPPORT
endif

EXTRA_CFLAGS += -DCFG_HI_DEMUX_POOLBUF_SIZE=$(CFG_HI_DEMUX_POOLBUF_SIZE)

EXTRA_CFLAGS += -DDMX_DESCRAMBLER_SUPPORT

ifdef CFG_HI_ADVCA_FUNCTION
    ifeq ($(CFG_HI_ADVCA_FUNCTION),DEBUG)
        EXTRA_CFLAGS += -DHI_DEMUX_PROC_SUPPORT
    endif
else
    EXTRA_CFLAGS += -DHI_DEMUX_PROC_SUPPORT
endif

ifdef CFG_HI_SMMU_SUPPORT
EXTRA_CFLAGS += -DDMX_MMU_SUPPORT
endif

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

MOD_NAME := hi_demux

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y   :=  $(DEMUX_DRV_VER)/drv_demux_intf.o        \
                    $(DEMUX_DRV_VER)/drv_demux.o             \
                    $(DEMUX_DRV_VER)/drv_demux_func.o        \
                    $(DEMUX_DRV_VER)/hal_demux.o             \
                    $(DEMUX_DRV_VER)/drv_demux_osal.o        \
                    $(DEMUX_DRV_VER)/drv_demux_scd.o         \
                    $(DEMUX_DRV_VER)/drv_demux_bitstream.o   \
                    $(DEMUX_DRV_VER)/drv_demux_index.o       \
                    $(DEMUX_DRV_VER)/drv_demux_sw.o

$(MOD_NAME)-y   +=  $(DEMUX_DRV_VER)/descrambler/drv_descrambler.o       \
                    $(DEMUX_DRV_VER)/descrambler/drv_descrambler_func.o  \
                    $(DEMUX_DRV_VER)/descrambler/hal_descrambler.o

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

