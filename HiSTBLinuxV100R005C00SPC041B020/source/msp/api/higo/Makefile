#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
HIGO_DIR := $(shell pwd)

#===============================================================================
#real higo dir
#===============================================================================
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
HIGO_GHFP_DIR := $(HIGO_DIR)/lib/loader/$(CFG_HI_ARM_TOOLCHAINS_NAME)
HIGO_GHFP_DIR64 := $(HIGO_DIR)/lib/loader/$(CFG_HI_AARCH64_TOOLCHAINS_NAME)
else
HIGO_GHFP_DIR := $(HIGO_DIR)/lib/$(CFG_HI_ARM_TOOLCHAINS_NAME)
HIGO_GHFP_DIR64 := $(HIGO_DIR)/lib/$(CFG_HI_AARCH64_TOOLCHAINS_NAME)
endif

HEADER_FILES := $(HIGO_DIR)/include/hi_go_winc.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go_text.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go_surface.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go_gdev.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go_errno.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go_encoder.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go_decoder.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go_cursor.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go_comm.h
HEADER_FILES += $(HIGO_DIR)/include/hi_go_bliter.h
HEADER_FILES += $(HIGO_DIR)/include/hi_effect.h

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all:
ifeq ($(CFG_HI_LOADER_APPLOADER),y)
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_STATIC_LIB_DIR) || mkdir -p $(HI_STATIC_LIB_DIR)
    ifeq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE, $(CFG_HI_CFLAGS)),)
	$(AT)cp -rf $(HIGO_GHFP_DIR)/nca_lib/*.a        $(HI_STATIC_LIB_DIR)/
    else
	$(AT)cp -rf $(HIGO_GHFP_DIR)/ca_lib/*.a         $(HI_STATIC_LIB_DIR)/
    endif
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_STATIC_LIB64_DIR) || mkdir -p $(HI_STATIC_LIB64_DIR)
    ifeq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE, $(CFG_HI_CFLAGS)),)
	$(AT)cp -rf $(HIGO_GHFP_DIR64)/nca_lib/*.a      $(HI_STATIC_LIB64_DIR)/
    else
	$(AT)cp -rf $(HIGO_GHFP_DIR64)/ca_lib/*.a       $(HI_STATIC_LIB64_DIR)/
    endif
endif
else
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
	$(AT)cp -rf $(HEADER_FILES) $(HI_INCLUDE_DIR)/
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_STATIC_LIB_DIR) || mkdir -p $(HI_STATIC_LIB_DIR)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)test -d $(HI_EXTERN_LIB_DIR) || mkdir -p $(HI_EXTERN_LIB_DIR)
	$(AT)cp -rf $(HIGO_GHFP_DIR)/nca_lib/*.so       $(HI_SHARED_LIB_DIR)/
	$(AT)cp -rf $(HIGO_GHFP_DIR)/higo-adp           $(HI_EXTERN_LIB_DIR)/
    ifeq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE, $(CFG_HI_CFLAGS)),)
	$(AT)cp -rf $(HIGO_GHFP_DIR)/nca_lib/*.a        $(HI_STATIC_LIB_DIR)/
    else
	$(AT)cp -rf $(HIGO_GHFP_DIR)/ca_lib/*.a         $(HI_STATIC_LIB_DIR)/
    endif
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_STATIC_LIB64_DIR) || mkdir -p $(HI_STATIC_LIB64_DIR)
	$(AT)test -d $(HI_SHARED_LIB64_DIR) || mkdir -p $(HI_SHARED_LIB64_DIR)
	$(AT)test -d $(HI_EXTERN_LIB64_DIR) || mkdir -p $(HI_EXTERN_LIB64_DIR)
	$(AT)cp -rf $(HIGO_GHFP_DIR64)/nca_lib/*.so     $(HI_SHARED_LIB64_DIR)/
	$(AT)cp -rf $(HIGO_GHFP_DIR64)/higo-adp         $(HI_EXTERN_LIB64_DIR)/
    ifeq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE, $(CFG_HI_CFLAGS)),)
	$(AT)cp -rf $(HIGO_GHFP_DIR64)/nca_lib/*.a      $(HI_STATIC_LIB64_DIR)/
    else
	$(AT)cp -rf $(HIGO_GHFP_DIR64)/ca_lib/*.a       $(HI_STATIC_LIB64_DIR)/
    endif
endif
endif

clean:
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/libhigo*.so
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/libeffect.so
	$(AT)rm -rf $(HI_EXTERN_LIB_DIR)/higo-adp
	$(AT)rm -rf $(HI_STATIC_LIB_DIR)/libhigo*.a
	$(AT)rm -rf $(HI_STATIC_LIB_DIR)/libeffect.a
	$(AT)rm -rf $(foreach file, $(HEADER_FILES), $(HI_INCLUDE_DIR)/$(notdir $(file)))
