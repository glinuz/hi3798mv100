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
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a hi3796mv200),)
VDEC_VERSION := vdec_v2.0
else
VDEC_VERSION := vdec_v1.0
endif

obj-y += $(VDEC_VERSION)/

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(VDEC_VERSION)/ all

clean:
	$(AT)make -C $(VDEC_VERSION)/ clean

install: all
	$(AT)make -C $(VDEC_VERSION)/ install

uninstall:
	$(AT)make -C $(VDEC_VERSION)/ uninstall

