#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================


ifeq ($(CFG_HI_ADVCA_TYPE), VERIMATRIX)
SSD_DIR := vmx
else ifeq ($(CFG_HI_ADVCA_TYPE), VERIMATRIX_ULTRA)
SSD_DIR := vmx_ultra
else ifeq ($(CFG_HI_ADVCA_TYPE), DCAS)
SSD_DIR := dcas
else
SSD_DIR :=
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all:
ifneq ($(SSD_DIR),)
	$(AT)make -C $(SSD_DIR) ssd_install
endif

clean:
ifneq ($(SSD_DIR),)
	$(AT)make -C $(SSD_DIR) ssd_clean
endif
