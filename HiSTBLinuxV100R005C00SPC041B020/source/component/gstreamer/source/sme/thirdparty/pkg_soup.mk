LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pkg_soup

#EXT_TYPE_GZ := tar.gz
EXT_TYPE_XZ := tar.xz
PKG_SOUP_NAME := libsoup-2.46.0


LOCAL_INTERMEDIATE_TARGETS := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE) \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE)_prepare \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE)_configure \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE)_make \
	$(LOCAL_MODULE)-clean

PRIVATE_PKG_SOUP_NAME_PREPARE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE)_prepare
PRIVATE_PKG_SOUP_NAME_CONFIGURE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE)_configure
PRIVATE_PKG_SOUP_NAME_MAKE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE)_make
PRIVATE_PKG_SOUP_NAME_INSTALL := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE)_install
ifeq ($(ENV_SME_VERSION_TYPE), rls)
$(PRIVATE_PKG_SOUP_NAME_CONFIGURE): PRIVATE_PKG_SOUP_DBG_FLG := --disable-debug
else
$(PRIVATE_PKG_SOUP_NAME_CONFIGURE): PRIVATE_PKG_SOUP_DBG_FLG := --enable-debug
endif

PRIVATE_PKG_SOUP_DEPS := pkg_glib-deps pkg_xml2-deps pkg_sqlite3-deps

$(LOCAL_MODULE)-deps: $(PRIVATE_PKG_SOUP_DEPS) $(LOCAL_MODULE)
$(LOCAL_MODULE):$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE)

$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/.$(LOCAL_MODULE):$(PRIVATE_PKG_SOUP_NAME_INSTALL)

$(PRIVATE_PKG_SOUP_NAME_INSTALL):$(PRIVATE_PKG_SOUP_NAME_MAKE)
	$(NS)echo "begin INSTALL $(PKG_SOUP_NAME)!"
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libsoup-2.4.so $(TARGET_OUT_LIB_DIR)/libsoup-2.4.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libsoup-2.4.so.1 $(TARGET_OUT_LIB_DIR)/libsoup-2.4.so.1
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libsoup-2.4.so.1.7.0 $(TARGET_OUT_LIB_DIR)/libsoup-2.4.so.1.7.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libsoup-2.4.so.1.7.0
endif
	$(NS)echo "end INSTALL $(PKG_SOUP_NAME)!"

$(PRIVATE_PKG_SOUP_NAME_MAKE):$(PRIVATE_PKG_SOUP_NAME_CONFIGURE)
	$(NS)echo "begin make $(PKG_SOUP_NAME)!"
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME) $(MAKE_PARALLEL)
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME) install $(MAKE_PARALLEL)
	$(NS)echo "end make $(PKG_SOUP_NAME)!"

$(PRIVATE_PKG_SOUP_NAME_CONFIGURE):$(PRIVATE_PKG_SOUP_NAME_PREPARE)
	$(NS)echo "begin configure $(PKG_SOUP_NAME)..."
	chmod 777 $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)/configure
	cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME);ac_cv_path_install="/usr/bin/install -cp" ./configure --prefix="${THIRDPARTY_OBJ_DIR}" \
		--host=$(CFG_HOST_TYPE) \
		CFLAGS="$(OPENSOURCE_CFLAGS) --sysroot=$(CFG_COMPILE_SYSROOT)" \
		CPPFLAGS="$(OPENSOURCE_CXXFLAGS)" \
		LDFLAGS="$(OPENSOURCE_LDFLAGS)" \
		--disable-static --enable-shared $(PRIVATE_PKG_SOUP_DBG_FLG) \
		PKG_CONFIG_PATH=$(THIRDPARTY_PKG_CONFIG_DIR) \
		CXX=$(TARGET_CXX) CC=$(TARGET_CC)
	-$(NS)cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME);sed -i 's/libsoup po tests examples docs/libsoup tests examples/g' `grep 'libsoup po tests examples docs' -rl Makefile`
	$(NS)touch $@
	$(NS)echo "end configure $(PKG_SOUP_NAME)..."

$(PRIVATE_PKG_SOUP_NAME_PREPARE):
	$(NS)echo "begin prepare $(PKG_SOUP_NAME)..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)
	$(NC)mkdir -p $(SME_THIRDPARTY_MERGE_DIR)
	tar -xf $(SME_THIRDPARTY_ORG_DIR)/$(PKG_SOUP_NAME).$(EXT_TYPE_XZ) -C $(SME_THIRDPARTY_MERGE_DIR)
	$(NC)cp -af $(SME_THIRDPARTY_PATCH_DIR)/$(PKG_SOUP_NAME) $(SME_THIRDPARTY_MERGE_DIR)/
	$(NS)touch $@
	$(NS)echo "end prepare $(PKG_SOUP_NAME)!"

$(LOCAL_MODULE)-clean:
	$(NS)echo "begin make clean $(PKG_SOUP_NAME) ..."
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libsoup-2.4.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libsoup-2.4.so.1
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libsoup-2.4.so.1.7.0
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME) uninstall $(MAKE_PARALLEL)
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME) clean $(MAKE_PARALLEL)
	$(NS)echo "end make clean $(PKG_SOUP_NAME)"

$(LOCAL_MODULE)-cfg-clean:
	$(NS)echo "begin make clean $(PKG_SOUP_NAME)-cfg ..."
	$(NC)rm -rf $(PRIVATE_PKG_SOUP_NAME_CONFIGURE)
	$(NS)echo "end make clean $(PKG_SOUP_NAME)-cfg ..."

$(LOCAL_MODULE)-prepare-clean:$(LOCAL_MODULE)-clean
	$(NS)echo "begin make clean $(PKG_SOUP_NAME)-prepare ..."
	$(NC)rm -rf $(PRIVATE_$(PKG_SOUP_NAME)_CONFIGURE)
	$(NC)rm -rf $(PRIVATE_PKG_SOUP_NAME_PREPARE)
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_SOUP_NAME)
	$(NS)echo "end make clean $(PKG_SOUP_NAME)-prepare ..."

include $(BUILD_OPENSOURCE_PKG)
