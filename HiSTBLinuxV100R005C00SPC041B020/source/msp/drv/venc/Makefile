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
ifneq ($(findstring $(CFG_HI_CHIP_TYPE),  hi3798mv200 hi3798mv200_a hi3796mv200),)
VENC_VERSION := venc_v2.0
else
VENC_VERSION := venc_v1.0
endif

obj-y += $(VENC_VERSION)/

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(VENC_VERSION)/ all

clean:
	$(AT)make -C $(VENC_VERSION)/ clean

install: all
	$(AT)make -C $(VENC_VERSION)/ install

uninstall:
	$(AT)make -C $(VENC_VERSION)/ uninstall

