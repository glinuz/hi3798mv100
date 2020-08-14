#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif
#===============================================================================
HEADER_FILES := $(wildcard $(CURDIR)/include/*.h)
#===============================================================================
ifeq ($(CFG_HI_HACODEC_MP3DECODE_SUPPORT),y)
HA_LIB += libHA.AUDIO.MP2.decode.so
HA_LIB += libHA.AUDIO.MP3.decode.so
endif
ifeq ($(CFG_HI_HACODEC_AACDECODE_SUPPORT),y)
HA_LIB += libHA.AUDIO.AAC.decode.so
endif
ifeq ($(CFG_HI_HACODEC_AACENCODE_SUPPORT),y)
HA_LIB += libHA.AUDIO.AAC.encode.so
endif
ifeq ($(CFG_HI_HACODEC_AC3PASSTHROUGH_SUPPORT),y)
HA_LIB += libHA.AUDIO.AC3PASSTHROUGH.decode.so
endif
ifeq ($(CFG_HI_HACODEC_AMRNBCODEC_SUPPORT),y)
HA_LIB += libHA.AUDIO.AMRNB.codec.so
endif
ifeq ($(CFG_HI_HACODEC_AMRWBCODEC_SUPPORT),y)
HA_LIB += libHA.AUDIO.AMRWB.codec.so
endif
ifeq ($(CFG_HI_HACODEC_BLURAYLPCMDECODE_SUPPORT),y)
HA_LIB += libHA.AUDIO.BLURAYLPCM.decode.so
endif
ifeq ($(CFG_HI_HACODEC_COOKDECODE_SUPPORT),y)
HA_LIB += libHA.AUDIO.COOK.decode.so
endif
ifeq ($(CFG_HI_HACODEC_DRADECODE_SUPPORT),y)
HA_LIB += libHA.AUDIO.DRA.decode.so
endif
ifeq ($(CFG_HI_HACODEC_DTSPASSTHROUGH_SUPPORT),y)
HA_LIB += libHA.AUDIO.DTSPASSTHROUGH.decode.so
endif
ifeq ($(CFG_HI_HACODEC_G711CODEC_SUPPORT),y)
HA_LIB += libHA.AUDIO.G711.codec.so
endif
ifeq ($(CFG_HI_HACODEC_G722CODEC_SUPPORT),y)
HA_LIB += libHA.AUDIO.G722.codec.so
endif
ifeq ($(CFG_HI_HACODEC_PCMDECODE_SUPPORT),y)
HA_LIB += libHA.AUDIO.PCM.decode.so
endif
ifeq ($(CFG_HI_HACODEC_TRUEHDPASSTHROUGH_SUPPORT),y)
HA_LIB += libHA.AUDIO.TRUEHDPASSTHROUGH.decode.so
endif
ifeq ($(CFG_HI_HACODEC_WMADECODE_SUPPORT),y)
HA_LIB += libHA.AUDIO.WMA.decode.so
endif
#===============================================================================
ifeq ($(HI_USER_SPACE_LIB),y)
HA_LIB_INSTALL := $(foreach file, $(HA_LIB), $(wildcard $(CURDIR)/lib/$(CFG_HI_ARM_TOOLCHAINS_NAME)/$(file)))
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
HA_LIB64_INSTALL := $(foreach file, $(HA_LIB), $(wildcard $(CURDIR)/lib/$(CFG_HI_AARCH64_TOOLCHAINS_NAME)/$(file)))
endif
#===============================================================================
.PHONY: all clean
#===============================================================================
all:
ifneq ($(HEADER_FILES),)
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
	$(AT)cp -f $(HEADER_FILES) $(HI_INCLUDE_DIR)/
endif
ifneq ($(strip $(HA_LIB_INSTALL)),)
	$(AT)test -d $(HI_EXTERN_LIB_DIR) || mkdir -p $(HI_EXTERN_LIB_DIR)
	$(AT)cp -f $(HA_LIB_INSTALL) $(HI_EXTERN_LIB_DIR)/
endif
ifneq ($(strip $(HA_LIB64_INSTALL)),)
	$(AT)test -d $(HI_EXTERN_LIB64_DIR) || mkdir -p $(HI_EXTERN_LIB64_DIR)
	$(AT)cp -f $(HA_LIB64_INSTALL) $(HI_EXTERN_LIB64_DIR)/
endif

clean:
ifneq ($(HEADER_FILES),)
	$(AT)rm -rf $(foreach file, $(HEADER_FILES), $(HI_INCLUDE_DIR)/$(notdir $(file)))
endif
ifneq ($(strip $(HA_LIB_INSTALL)),)
	$(AT)rm -rf $(foreach file, $(HA_LIB_INSTALL), $(HI_EXTERN_LIB_DIR)/$(notdir $(file)))
endif
ifneq ($(strip $(HA_LIB64_INSTALL)),)
	$(AT)rm -rf $(foreach file, $(HA_LIB64_INSTALL), $(HI_EXTERN_LIB64_DIR)/$(notdir $(file)))
endif
#===============================================================================
