#===============================================================================
#   export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
HA_CODEC_DIR := $(COMPONENT_DIR)/ha_codec

OMX_DIR := $(MSP_DIR)/api/omx
OMX_AUDIO_DIR := $(OMX_DIR)/omx_audio
OMX_AUDIO_COMMON_DIR := $(OMX_AUDIO_DIR)/common
#===============================================================================
CFLAGS += \
	-I$(HA_CODEC_DIR)/include \
	-I$(OMX_AUDIO_COMMON_DIR)/base \
	-I$(OMX_AUDIO_COMMON_DIR)/omx_ha/ha_adec \
	-I$(OMX_AUDIO_COMMON_DIR)/omx_ha/ha_aenc \
	-I$(OMX_AUDIO_COMMON_DIR)/osal \
	-I$(OMX_DIR)/include
#===============================================================================
# source files
SRCS := src/omx_aac_aenc.c
#===============================================================================
LIB_NAME := libOMX.hisi.audio.aacenc
#===============================================================================
FILTER := -Werror
include $(SDK_DIR)/hi_rules.mak
#===============================================================================
