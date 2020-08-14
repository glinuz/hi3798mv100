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
	$(AT)make -C server
	$(AT)make -C client
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)cp -rf key $(HI_OUT_DIR)/obj/sample/openssl/
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)cp -rf key $(HI_OUT_DIR)/obj64/sample/openssl/
endif

clean:
	$(AT)make -C server clean
	$(AT)make -C client clean
