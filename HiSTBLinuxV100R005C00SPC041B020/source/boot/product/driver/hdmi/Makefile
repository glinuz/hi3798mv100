#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR     := $(shell pwd)/../../../../..
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
ifeq ($(CFG_HI_HDMI_SUPPORT_1_4),y)
HDMI_VER := hdmi_1_4
else ifeq ($(CFG_HI_HDMI_SUPPORT_2_0),y)
HDMI_VER := hdmi_2_0
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: prepare all clean install uninstall

prepare:
ifeq ($(CFG_HI_HDMI_SUPPORT_2_0),y)
	make -C $(HDMI_VER) prepare
endif

all: prepare
	make -C $(HDMI_VER) all

clean:
	make -C $(HDMI_VER) clean

install: all
	make -C $(HDMI_VER) install

uninstall:
	make -C $(HDMI_VER) uninstall

