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
ifeq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a hi3796mv200),) 
OMXVDEC_VERSION := omxvdec_v1.0
else
OMXVDEC_VERSION := omxvdec_v2.0
endif

obj-y += $(OMXVDEC_VERSION)/

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(OMXVDEC_VERSION)/ all

clean:
	$(AT)make -C $(OMXVDEC_VERSION)/ clean

install: all
	$(AT)make -C $(OMXVDEC_VERSION)/ install

uninstall:
	$(AT)make -C $(OMXVDEC_VERSION)/ uninstall

