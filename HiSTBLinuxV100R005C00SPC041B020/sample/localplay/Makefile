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

CFLAGS += -DCHARSET_SUPPORT

SAMPLE_IMAGES := sample_localplay

LOCAL_OBJS := play_m3u9.o $(DRM_OBJS)

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS) -lhi_charsetMgr -lsubdec -lplayer -lhiplayer_utils

include $(SAMPLE_DIR)/hi_sample_rules.mak