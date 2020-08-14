#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================
SAMPLE_IMAGES := sample_flash \
    sample_flash_erase \
    sample_flash_read \
    sample_flash_write \
    sample_flash_write_yaffs \
    sample_flash_write_yaffs2

DEPEND_LIBS := -lhi_common

include $(SAMPLE_DIR)/hi_sample_rules.mak
