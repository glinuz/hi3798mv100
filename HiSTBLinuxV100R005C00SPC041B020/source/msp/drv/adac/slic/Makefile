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
EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

SLIC_DIR = $(SDK_DIR)/source/msp/drv/adac/slic

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(SLIC_DIR)                           \
                -I$(SLIC_DIR)/includes                  \
                -I$(SLIC_DIR)/hal                       \
                -I$(SLIC_DIR)/profiles                  \
                -I$(SLIC_DIR)/vp890_api

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

MOD_NAME := hi_slic

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y   :=  ./profiles/profiles8.o  \
                    ./hal/vp_hal.o          \
                    ./hal/sys_service.o     \
                    ./common/vp_api_init.o  \
                    ./common/vp_api_control.o \
                    ./common/vp_api_query.o   \
                    ./common/vp_api_common.o  \
                    ./common/vp_api_cslac_seq.o  \
                    ./common/vp_api_config.o     \
                    ./common/vp_debug.o       \
                    ./common/vp_api_test.o    \
                    ./vp890_api/vp890_config.o   \
                    ./vp890_api/vp890_init.o     \
                    ./vp890_api/vp890_control.o  \
                    ./vp890_api/vp890_common.o   \
                    ./vp890_api/vp890_apiCal.o   \
                    ./vp890_api/vp890_query.o    \
                    ./vp890_api/vp890_seq.o      \
                    ./vp890_api/vp890_fxo_control.o \
                    ./vp890_api/vp890_fxs_control.o \
                    ./vp890_api/vp890_lp_control.o  \
                    ./slac.o                        \
                    ./slac89116.o

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

