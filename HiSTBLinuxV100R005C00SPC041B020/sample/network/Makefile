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
SAMPLE_IMAGES := sample_ipcfg

LOCAL_OBJS := hi_api_eth.o

DEPEND_LIBS := -lhi_common

include $(SAMPLE_DIR)/hi_sample_rules.mak
