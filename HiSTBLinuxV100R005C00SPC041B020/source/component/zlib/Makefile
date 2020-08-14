#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
ZLIB := zlib-1.2.8

ZLIB_DIR := $(HI_OUT_DIR)/obj/source/component/zlib
INSTALL_DIR := $(ZLIB_DIR)/release

ZLIB_DIR64 := $(HI_OUT_DIR)/obj64/source/component/zlib
INSTALL_DIR64 := $(ZLIB_DIR64)/release

TARGETS :=

ifeq ($(HI_USER_SPACE_LIB),y)
TARGETS += zlib
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
TARGETS += zlib64
endif

CFLAGS += $(CFG_HI_CFLAGS)
ifneq ($(findstring -fPIC, $(CFG_HI_CFLAGS)), -fPIC)
CFLAGS += -fPIC
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean zlib zlib64 prepare

all: $(TARGETS)
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
	$(AT)cp -rf include/*.h $(HI_INCLUDE_DIR)/
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_STATIC_LIB_DIR) || mkdir -p $(HI_STATIC_LIB_DIR)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)cp -ralf $(INSTALL_DIR)/lib/libz.a $(HI_STATIC_LIB_DIR)/
	$(AT)cp -ralf $(INSTALL_DIR)/lib/libz.so* $(HI_SHARED_LIB_DIR)/
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_STATIC_LIB64_DIR) || mkdir -p $(HI_STATIC_LIB64_DIR)
	$(AT)test -d $(HI_SHARED_LIB64_DIR) || mkdir -p $(HI_SHARED_LIB64_DIR)
	$(AT)cp -ralf $(INSTALL_DIR64)/lib/libz.a $(HI_STATIC_LIB64_DIR)/
	$(AT)cp -ralf $(INSTALL_DIR64)/lib/libz.so* $(HI_SHARED_LIB64_DIR)/
endif

clean:
	$(AT)rm -rf $(INSTALL_DIR) $(INSTALL_DIR64) $(ZLIB_DIR)/$(ZLIB) $(ZLIB_DIR64)/$(ZLIB)
	$(AT)rm -rf $(HI_STATIC_LIB_DIR)/libz.a $(HI_SHARED_LIB_DIR)/libz.so*
	$(AT)rm -rf $(HI_STATIC_LIB64_DIR)/libz.a $(HI_SHARED_LIB64_DIR)/libz.so*

zlib: $(ZLIB_DIR)/$(ZLIB)/tar
	$(AT)cd $(ZLIB_DIR)/$(ZLIB) && make install

zlib64: $(ZLIB_DIR64)/$(ZLIB)/tar
	$(AT)cd $(ZLIB_DIR64)/$(ZLIB) && make install

$(ZLIB_DIR)/$(ZLIB)/tar:
	$(AT)test -d $(INSTALL_DIR) || mkdir -p $(INSTALL_DIR)
	$(AT)test -d $(ZLIB_DIR)/$(ZLIB) || (tar xf $(OPEN_SOURCE_DIR)/$(ZLIB).tar.gz -C $(ZLIB_DIR); \
        cd $(ZLIB_DIR)/$(ZLIB); CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc AR=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ar RANLIB=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ranlib CFLAGS="$(CFLAGS)" sh ./configure --prefix=$(INSTALL_DIR);)
	$(AT)touch $@

$(ZLIB_DIR64)/$(ZLIB)/tar:
	$(AT)test -d $(INSTALL_DIR64) || mkdir -p $(INSTALL_DIR64)
	$(AT)test -d $(ZLIB_DIR64)/$(ZLIB) || (tar xf $(OPEN_SOURCE_DIR)/$(ZLIB).tar.gz -C $(ZLIB_DIR64); \
        cd $(ZLIB_DIR64)/$(ZLIB); CC=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc AR=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ar RANLIB=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ranlib CFLAGS="$(CFLAGS)" sh ./configure --prefix=$(INSTALL_DIR64);)
	$(AT)touch $@
