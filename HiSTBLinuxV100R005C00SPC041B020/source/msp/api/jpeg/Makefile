#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
#jpeg version
#===============================================================================
JPEG_VER := 9a

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all:
	$(AT)cd src && make all
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)cd $(HI_STATIC_LIB_DIR) && ln -sf libjpeg$(JPEG_VER).a libjpeg.a
	$(AT)cd $(HI_SHARED_LIB_DIR) && ln -sf libjpeg$(JPEG_VER).so libjpeg.so
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)cd $(HI_STATIC_LIB64_DIR) && ln -sf libjpeg$(JPEG_VER).a libjpeg.a
	$(AT)cd $(HI_SHARED_LIB64_DIR) && ln -sf libjpeg$(JPEG_VER).so libjpeg.so
endif

clean:
	$(AT)cd src && make clean
	$(AT)rm -rf $(HI_STATIC_LIB_DIR)/libjpeg*.a $(HI_SHARED_LIB_DIR)/libjpeg*.so
	$(AT)rm -rf $(HI_STATIC_LIB64_DIR)/libjpeg*.a $(HI_SHARED_LIB64_DIR)/libjpeg*.so
