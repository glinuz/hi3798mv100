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
SAMPLE_IMAGES := sample_audio_cast

ifeq ($(CFG_HI_AENC_SUPPORT),y)
SAMPLE_IMAGES += sample_aenc_cast
endif

COMMON_OBJS := $(addprefix ../common/,$(COMMON_SRCS:%.c=%.o))

DEPEND_LIBS := $(HI_LIBS)

include $(SAMPLE_DIR)/hi_sample_rules.mak
