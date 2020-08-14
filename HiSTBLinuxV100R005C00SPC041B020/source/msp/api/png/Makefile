#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
#png version
#===============================================================================
PNG_VER := 1.6.16

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all:
	$(AT)make -C libpng-$(PNG_VER) all
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)cd $(HI_STATIC_LIB_DIR) && ln -sf libpng$(PNG_VER).a libpng.a
	$(AT)cd $(HI_SHARED_LIB_DIR) && ln -sf libpng$(PNG_VER).so libpng.so
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)cd $(HI_STATIC_LIB64_DIR) && ln -sf libpng$(PNG_VER).a libpng.a
	$(AT)cd $(HI_SHARED_LIB64_DIR) && ln -sf libpng$(PNG_VER).so libpng.so
endif

clean:
	$(AT)make -C libpng-$(PNG_VER) clean
	$(AT)rm -rf $(HI_STATIC_LIB_DIR)/libpng*.a $(HI_SHARED_LIB_DIR)/libpng*.so
	$(AT)rm -rf $(HI_STATIC_LIB64_DIR)/libpng*.a $(HI_SHARED_LIB64_DIR)/libpng*.so
