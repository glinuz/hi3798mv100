#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
CFLAGS  +=  -I$(MSP_DIR)/api/omx/include    \
            -I$(MSP_DIR)/drv/venc

SRCS := omx_allocator.c omx_venc_queue.c omx_venc.c omx_venc_drv.c omx_venc_buf_mng.c

LIB_NAME := libOMX.hisi.video.encoder

include $(SDK_DIR)/hi_rules.mak