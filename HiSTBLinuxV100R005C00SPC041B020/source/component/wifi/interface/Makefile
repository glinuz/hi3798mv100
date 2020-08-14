#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall

all:
ifeq ($(CFG_HI_WIFI_MODE_AP),y)
	$(AT)make -C src/ap
endif
ifeq ($(CFG_HI_WIFI_MODE_STA),y)
	$(AT)make -C src/sta
endif

clean:
	$(AT)make -C src/ap clean
	$(AT)make -C src/sta clean

install:
ifeq ($(CFG_HI_WIFI_MODE_AP),y)
	$(AT)make -C src/ap install
endif
ifeq ($(CFG_HI_WIFI_MODE_STA),y)
	$(AT)make -C src/sta install
endif

uninstall:
ifeq ($(CFG_HI_WIFI_MODE_AP),y)
	$(AT)make -C src/ap uninstall
endif
ifeq ($(CFG_HI_WIFI_MODE_STA),y)
	$(AT)make -C src/sta uninstall
endif




