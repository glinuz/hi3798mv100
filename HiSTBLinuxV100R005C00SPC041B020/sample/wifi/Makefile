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
ifeq ($(CFG_HI_WIFI_MODE_AP),y)
	$(AT)make -C ap
endif
ifeq ($(CFG_HI_WIFI_MODE_STA),y)
	$(AT)make -C sta
endif

clean:
	$(AT)make -C ap clean
	$(AT)make -C sta clean


