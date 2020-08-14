#===============================================================================
#   export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
HA_CODEC_DIR := $(COMPONENT_DIR)/ha_codec
#===============================================================================
CFLAGS += \
	-I$(HA_CODEC_DIR)/include \
	-I$(CURDIR)/base \
	-I$(CURDIR)/omx_ha/include \
	-I$(CURDIR)/omx_ha/ha_adec \
	-I$(CURDIR)/omx_ha/ha_aenc \
	-I$(CURDIR)/osal \
	-I$(CURDIR)/../../include
#===============================================================================
# source files
SRCS := \
	base/omx_audio_base.c \
	omx_ha/ha_adec/ha_adec.c \
	omx_ha/ha_aenc/ha_aenc.c \
	osal/OMX_CONF_StubbedComponent.c \
	osal/omx_osal_linux.c
#===============================================================================
LIB_NAME := libOMX.hisi.audio.utils
#===============================================================================
FILTER := -Werror
include $(SDK_DIR)/hi_rules.mak
#===============================================================================
