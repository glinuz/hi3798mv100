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
.PHONY: all clean install uninstall

all:
	$(AT)make -C play all
	$(AT)make -C mix all
ifeq ($(CFG_HI_ADEC_AUDSPECTRUM_SUPPORT),y)
	$(AT)make -C mp3_spectrum all
endif

clean:
	$(AT)make -C play clean
	$(AT)make -C mix clean
ifeq ($(CFG_HI_ADEC_AUDSPECTRUM_SUPPORT),y)
	$(AT)make -C mp3_spectrum clean
endif

install:
	$(AT)make -C play install
	$(AT)make -C mix install
ifeq ($(CFG_HI_ADEC_AUDSPECTRUM_SUPPORT),y)
	$(AT)make -C mp3_spectrum install
endif

uninstall:
	$(AT)make -C play uninstall
	$(AT)make -C mix uninstall
ifeq ($(CFG_HI_ADEC_AUDSPECTRUM_SUPPORT),y)
	$(AT)make -C mp3_spectrum uninstall
endif
