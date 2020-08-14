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
CURL := curl-7.49.1

CURL_DIR := $(HI_OUT_DIR)/obj/source/component/curl
INSTALL_DIR = $(CURL_DIR)/release

CURL_DIR64 := $(HI_OUT_DIR)/obj64/source/component/curl
INSTALL_DIR64 := $(CURL_DIR64)/release

TARGETS :=

ifeq ($(HI_USER_SPACE_LIB),y)
TARGETS += curl
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
TARGETS += curl64
endif

CFLAGS  += $(subst -Werror,,$(CFG_HI_CFLAGS))

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean curl curl64

all: $(TARGETS)
	$(AT)test -d $(HI_INCLUDE_DIR)    || mkdir -p $(HI_INCLUDE_DIR)
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)cp -rf $(INSTALL_DIR)/include/* $(HI_INCLUDE_DIR)/
else ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)cp -rf $(INSTALL_DIR64)/include/* $(HI_INCLUDE_DIR)/
endif
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_STATIC_LIB_DIR) || mkdir -p $(HI_STATIC_LIB_DIR)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)cp -ralf $(INSTALL_DIR)/lib/libcurl.a     $(HI_STATIC_LIB_DIR)/
	$(AT)cp -ralf $(INSTALL_DIR)/lib/libcurl.so*    $(HI_SHARED_LIB_DIR)/
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_STATIC_LIB64_DIR) || mkdir -p $(HI_STATIC_LIB64_DIR)
	$(AT)test -d $(HI_SHARED_LIB64_DIR) || mkdir -p $(HI_SHARED_LIB64_DIR)
	$(AT)cp -ralf $(INSTALL_DIR64)/lib/libcurl.a     $(HI_STATIC_LIB64_DIR)/
	$(AT)cp -ralf $(INSTALL_DIR64)/lib/libcurl.so*    $(HI_SHARED_LIB64_DIR)/
endif

clean:
	$(AT)rm -rf $(INSTALL_DIR) $(INSTALL_DIR64) $(CURL_DIR) $(CURL_DIR64)
	$(AT)rm -rf $(HI_STATIC_LIB_DIR)/libcurl.a $(HI_SHARED_LIB_DIR)/libcurl.so*
	$(AT)rm -rf $(HI_STATIC_LIB64_DIR)/libcurl.a $(HI_SHARED_LIB64_DIR)/libcurl.so*

curl: $(CURL_DIR)/$(CURL)/tar
	$(AT)cd $(CURL_DIR)/$(CURL) && make && make install

curl64: $(CURL_DIR64)/$(CURL)/tar
	$(AT)cd $(CURL_DIR64)/$(CURL) && make && make install

$(CURL_DIR)/$(CURL)/tar:
	$(AT)test -d $(INSTALL_DIR) || mkdir -p $(INSTALL_DIR)
	$(AT)test -d $(CURL_DIR)/$(CURL) || (tar xf $(OPEN_SOURCE_DIR)/$(CURL).tar.gz -C $(CURL_DIR); \
	cd $(CURL_DIR)/$(CURL); 	\
	sh ./configure CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc CPP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-cpp AR=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ar AS=$(CFG_HI_ARM_TOOLCHAINS_NAME)-as LD=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ld NM=$(CFG_HI_ARM_TOOLCHAINS_NAME)-nm STRIP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-strip OBJDUMP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-objdump RANLIB=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ranlib CFLAGS="$(CFLAGS)"  --target=$(CFG_HI_ARM_TOOLCHAINS_NAME) --host=$(CFG_HI_ARM_TOOLCHAINS_NAME) --prefix=$(INSTALL_DIR) --disable-rtsp --disable-smtp --disable-pop3 --disable-imap --disable-gopher --disable-file --disable-dict --disable-telnet;)
	$(AT)touch $@

$(CURL_DIR64)/$(CURL)/tar :
	$(AT)test -d $(INSTALL_DIR64) || mkdir -p $(INSTALL_DIR64)
	$(AT)test -d $(CURL_DIR64)/$(CURL) || (tar xf $(OPEN_SOURCE_DIR)/$(CURL).tar.gz -C $(CURL_DIR64); \
	cd $(CURL_DIR64)/$(CURL); 	\
	sh ./configure CC=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc CPP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-cpp AR=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ar AS=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-as LD=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ld NM=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-nm STRIP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-strip OBJDUMP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-objdump RANLIB=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ranlib CFLAGS="$(CFLAGS)" --target=$(CFG_HI_AARCH64_TOOLCHAINS_NAME) --host=$(CFG_HI_AARCH64_TOOLCHAINS_NAME) --prefix=$(INSTALL_DIR64) --disable-rtsp --disable-smtp --disable-pop3 --disable-imap --disable-gopher --disable-file --disable-dict --disable-telnet;)
	$(AT)touch $@
