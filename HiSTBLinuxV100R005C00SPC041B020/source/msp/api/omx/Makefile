#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
objects:= omx_core omx_vdec omx_venc

ifeq ($(CFG_HI_AUDIO_OMX_SUPPORT),y)
objects += omx_audio
endif

objects_clean := $(addsuffix _clean,$(objects))

#===============================================================================
.PHONY: all clean
#===============================================================================
all: $(objects)

clean: $(objects_clean)

#===============================================================================
.PHONY: $(objects) $(objects_clean)
#===============================================================================
$(objects):
	$(AT)make -C $@

$(objects_clean):
	$(AT)make -C $(patsubst %_clean,%,$@) clean
