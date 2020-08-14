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
SAMPLE_IMAGES := sample_omxaenc_aac

CFLAGS  += -I$(SDK_DIR)/source/msp/api/omx/include
CFLAGS  += -I$(SDK_DIR)/source/msp/api/omx/omx_audio/common/base
CFLAGS  += -I$(SDK_DIR)/source/msp/api/omx/omx_audio/common/osal

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS) -lOMX_Core

DEPEND_LIBS += -lOMX.hisi.audio.utils

include $(SAMPLE_DIR)/hi_sample_rules.mak
