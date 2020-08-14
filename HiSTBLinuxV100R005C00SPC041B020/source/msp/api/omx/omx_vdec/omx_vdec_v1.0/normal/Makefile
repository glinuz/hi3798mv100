#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
CFLAGS  +=  -I$(MSP_DIR)/api/omx/include            \
            -I$(MSP_DIR)/drv/omxvdec/omxvdec_v1.0   \
            -I$(MSP_DIR)/drv/vfmw/vfmw_v4.0

ifeq ($(CFG_ANDROID_TVOS),y)
LOCAL_CFLAGS += -DHI_TVOS_SUPPORT
endif

SRCS := omx_event_queue.c omx_vdec.c omx_drv_ctx.c

LIB_NAME := libOMX.hisi.video.decoder

include $(SDK_DIR)/hi_rules.mak