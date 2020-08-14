#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all:
	$(AT)make -C api all
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)make -C app all
endif

clean:
	$(AT)make -C api clean
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
	$(AT)make -C app clean
endif

