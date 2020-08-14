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
CFLAGS := -I$(SDK_DIR)/source/component/loader/api/include
CFLAGS += -I$(SDK_DIR)/source/component/loader/api/src

ifeq ($(CFG_HI_APPLOADER_SUPPORT),y)
CFLAGS += -DHI_LOADER_APPLOADER
endif

ifeq ($(CFG_HI_ADVCA_TYPE), NAGRA)
NAGRA_LIBS := -lhi_nagra
endif

SAMPLE_IMAGES := sample_loader

DEPEND_LIBS := -lhi_common -lhi_msp -ljpeg -lhiloader $(NAGRA_LIBS)

include $(SAMPLE_DIR)/hi_sample_rules.mak
