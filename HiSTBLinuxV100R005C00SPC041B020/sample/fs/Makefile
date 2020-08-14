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
SAMPLE_IMAGES := sample_fs

LOCAL_OBJS := sample_fsdisk.o sample_fsmanager.o sample_fstst.o

include $(SAMPLE_DIR)/hi_sample_rules.mak
