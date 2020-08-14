#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
OPENSSL := openssl-1.0.2j

OPENSSL_DIR := $(HI_OUT_DIR)/obj/source/component/openssl
INSTALL_DIR := $(OPENSSL_DIR)/release

OPENSSL_DIR64 := $(HI_OUT_DIR)/obj64/source/component/openssl
INSTALL_DIR64 := $(OPENSSL_DIR64)/release

TARGETS :=

ifeq ($(HI_USER_SPACE_LIB),y)
TARGETS += ssl
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
TARGETS += sll64
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean ssl sll64 prepare

all: $(TARGETS)
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)cp -rf $(INSTALL_DIR)/include/openssl $(HI_INCLUDE_DIR)/
else ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)cp -rf $(INSTALL_DIR64)/include/openssl $(HI_INCLUDE_DIR)/
endif
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_STATIC_LIB_DIR) || mkdir -p $(HI_STATIC_LIB_DIR)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)cp -ralf $(INSTALL_DIR)/lib/lib*.a $(HI_STATIC_LIB_DIR)/
	$(AT)cp -ralf $(INSTALL_DIR)/lib/lib*.so* $(HI_SHARED_LIB_DIR)/
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_STATIC_LIB64_DIR) || mkdir -p $(HI_STATIC_LIB64_DIR)
	$(AT)test -d $(HI_SHARED_LIB64_DIR) || mkdir -p $(HI_SHARED_LIB64_DIR)
	$(AT)cp -ralf $(INSTALL_DIR64)/lib/lib*.a $(HI_STATIC_LIB64_DIR)/
	$(AT)cp -ralf $(INSTALL_DIR64)/lib/lib*.so* $(HI_SHARED_LIB64_DIR)/
endif

clean:
	$(AT)rm -rf $(INSTALL_DIR) $(INSTALL_DIR64) $(OPENSSL_DIR)/$(OPENSSL) $(OPENSSL_DIR64)/$(OPENSSL)
	$(AT)rm -rf $(HI_STATIC_LIB_DIR)/libcrypto.a $(HI_STATIC_LIB_DIR)/libssl.a
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/libcrypto*.so* $(HI_SHARED_LIB_DIR)/libssl*.so*
	$(AT)rm -rf $(HI_STATIC_LIB64_DIR)/libcrypto.a $(HI_STATIC_LIB64_DIR)/libssl.a
	$(AT)rm -rf $(HI_SHARED_LIB64_DIR)/libcrypto*.so* $(HI_SHARED_LIB64_DIR)/libssl*.so*
	$(AT)rm -rf $(HI_INCLUDE_DIR)/openssl

ssl: $(OPENSSL_DIR)/$(OPENSSL)/tar
	$(AT)cd $(OPENSSL_DIR)/$(OPENSSL) && make -j1 && make install -j1

sll64: $(OPENSSL_DIR64)/$(OPENSSL)/tar
	$(AT)cd $(OPENSSL_DIR64)/$(OPENSSL) && make -j1 && make install -j1

$(OPENSSL_DIR)/$(OPENSSL)/tar:
	$(AT)test -d $(INSTALL_DIR) || mkdir -p $(INSTALL_DIR)
	$(AT)test -d $(OPENSSL_DIR)/$(OPENSSL) || tar xf $(OPEN_SOURCE_DIR)/$(OPENSSL).tar.gz -C $(OPENSSL_DIR)
	$(AT)cd $(OPENSSL_DIR)/$(OPENSSL); CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc AR=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ar RANLIB=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ranlib CROSS_COMPILE= \
        ./Configure linux-armv4 threads shared no-zlib --prefix=$(INSTALL_DIR)
	$(AT)touch $@

$(OPENSSL_DIR64)/$(OPENSSL)/tar:
	$(AT)test -d $(INSTALL_DIR64) || mkdir -p $(INSTALL_DIR64)
	$(AT)test -d $(OPENSSL_DIR64)/$(OPENSSL) || tar xf $(OPEN_SOURCE_DIR)/$(OPENSSL).tar.gz -C $(OPENSSL_DIR64)
	$(AT)cd $(OPENSSL_DIR64)/$(OPENSSL); CC=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc AR=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ar RANLIB=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ranlib CROSS_COMPILE= \
        ./Configure linux-aarch64 threads shared no-zlib --prefix=$(INSTALL_DIR64)
	$(AT)touch $@
