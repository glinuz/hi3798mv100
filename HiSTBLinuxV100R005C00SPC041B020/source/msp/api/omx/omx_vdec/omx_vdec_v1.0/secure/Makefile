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

CFLAGS  +=  -I$(HI_TEE_OS_DIR)/libteec/sec_mmz/inc
CFLAGS  +=  -I$(TEE_API_INCLUDE)

CFLAGS  +=  -DHI_OMX_TEE_SUPPORT

ifeq ($(CFG_ANDROID_TVOS),y)
LOCAL_CFLAGS += -DHI_TVOS_SUPPORT
endif

LOCAL_SRC_DIR := ../normal

SRCS := $(LOCAL_SRC_DIR)/omx_event_queue.c $(LOCAL_SRC_DIR)/omx_vdec.c $(LOCAL_SRC_DIR)/omx_drv_ctx.c

LIB_NAME := libOMX.hisi.video.decoder.secure

include $(SDK_DIR)/hi_rules.mak