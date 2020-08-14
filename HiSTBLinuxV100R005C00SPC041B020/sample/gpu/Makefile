#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all:
ifeq ($(CFG_HI_EGL_TYPE),fb)
	$(AT)make -C coverflow
	$(AT)make -C coverflow_vertical
	$(AT)make -C cylinder
endif

clean:
ifeq ($(CFG_HI_EGL_TYPE),fb)
	$(AT)make -C coverflow clean
	$(AT)make -C coverflow_vertical clean
	$(AT)make -C cylinder clean
endif

