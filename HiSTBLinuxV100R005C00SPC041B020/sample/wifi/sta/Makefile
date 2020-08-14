#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================
SAMPLE_IMAGES := sample_sta

LOCAL_OBJS := sta_ui.o

DEPEND_LIBS := -lpthread -lwlansta

include $(SAMPLE_DIR)/hi_sample_rules.mak
