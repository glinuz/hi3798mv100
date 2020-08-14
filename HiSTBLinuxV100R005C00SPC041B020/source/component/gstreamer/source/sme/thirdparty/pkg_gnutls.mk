LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pkg_gnutls

#EXT_TYPE_GZ := tar.gz
EXT_TYPE_XZ := tar.xz
PKG_GNUTLS_NAME := gnutls-3.2.9


LOCAL_INTERMEDIATE_TARGETS := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE) \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE)_prepare \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE)_configure \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE)_make \
	$(LOCAL_MODULE)-clean

PRIVATE_PKG_GNUTLS_NAME_PREPARE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE)_prepare
PRIVATE_PKG_GNUTLS_NAME_CONFIGURE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE)_configure
PRIVATE_PKG_GNUTLS_NAME_MAKE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE)_make
PRIVATE_PKG_GNUTLS_NAME_INSTALL := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE)_install

PRIVATE_PKG_GNUTLS_DEPS := pkg_gmp-deps pkg_nettle-deps

$(LOCAL_MODULE)-deps: $(PRIVATE_PKG_GNUTLS_DEPS) $(LOCAL_MODULE)
$(LOCAL_MODULE):$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE)

$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/.$(LOCAL_MODULE):$(PRIVATE_PKG_GNUTLS_NAME_INSTALL)

$(PRIVATE_PKG_GNUTLS_NAME_INSTALL):$(PRIVATE_PKG_GNUTLS_NAME_MAKE)
	$(NS)echo "begin INSTALL $(PKG_GNUTLS_NAME)!"
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgnutls.so $(TARGET_OUT_LIB_DIR)/libgnutls.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgnutls.so.28  $(TARGET_OUT_LIB_DIR)/libgnutls.so.28
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgnutls.so.28.29.1 $(TARGET_OUT_LIB_DIR)/libgnutls.so.28.29.1
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgnutls.so.28.29.1
endif
	$(NS)echo "end INSTALL $(PKG_GNUTLS_NAME)!"

$(PRIVATE_PKG_GNUTLS_NAME_MAKE):$(PRIVATE_PKG_GNUTLS_NAME_CONFIGURE)
	$(NS)echo "begin make $(PKG_GNUTLS_NAME)!"
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME) $(MAKE_PARALLEL)
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME) install $(MAKE_PARALLEL)
	$(NS)echo "end make $(PKG_GNUTLS_NAME)!"

$(PRIVATE_PKG_GNUTLS_NAME_CONFIGURE):$(PRIVATE_PKG_GNUTLS_NAME_PREPARE)
	$(NS)echo "begin configure $(PKG_GNUTLS_NAME)..."
	chmod 777 $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)/configure
	cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME);ac_cv_path_install="/usr/bin/install -cp" ./configure --prefix="${THIRDPARTY_OBJ_DIR}" \
		--with-included-libtasn1 \
		--host=$(CFG_HOST_TYPE) \
		CFLAGS="$(OPENSOURCE_CFLAGS) -I$(THIRDPARTY_OBJ_DIR)/include --sysroot=$(CFG_COMPILE_SYSROOT)" \
		CXXFLAGS="$(OPENSOURCE_CXXFLAGS) -I$(THIRDPARTY_OBJ_DIR)/include --sysroot=$(CFG_COMPILE_SYSROOT)" \
		LDFLAGS="$(OPENSOURCE_LDFLAGS) -L$(THIRDPARTY_OBJ_DIR)/lib" \
		--disable-static --enable-shared --disable-doc \
		--disable-tests --disable-crywrap --without-p11-kit \
		--enable-local-libopts --without-idn \
		GMP_CFLAGS="-I$(THIRDPARTY_OBJ_DIR)/include" \
		GMP_LIBS="-L$(THIRDPARTY_OBJ_DIR)/lib -lgmp" \
		NETTLE_CFLAGS="-I$(THIRDPARTY_OBJ_DIR)/include/nettle" \
		NETTLE_LIBS="-L$(THIRDPARTY_OBJ_DIR)/lib -lnettle" \
		HOGWEED_CFLAGS="-I$(THIRDPARTY_OBJ_DIR)/include/nettle" \
		HOGWEED_LIBS="-L$(THIRDPARTY_OBJ_DIR)/lib -lhogweed" \
		CXX=$(TARGET_CXX) CC=$(TARGET_CC)
	-$(NS)cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME);sed -i 's//usr/bin/install -c//usr/bin/install -cp/g' `grep '/usr/bin/install -c' -rl Makefile`
	$(NS)touch $@
	$(NS)echo "end configure $(PKG_GNUTLS_NAME)..."

$(PRIVATE_PKG_GNUTLS_NAME_PREPARE):
	$(NS)echo "begin prepare $(PKG_GNUTLS_NAME)..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)
	$(NC)mkdir -p $(SME_THIRDPARTY_MERGE_DIR)
	tar -xf $(SME_THIRDPARTY_ORG_DIR)/$(PKG_GNUTLS_NAME).$(EXT_TYPE_XZ) -C $(SME_THIRDPARTY_MERGE_DIR)
	$(NC)cp -af $(SME_THIRDPARTY_PATCH_DIR)/$(PKG_GNUTLS_NAME) $(SME_THIRDPARTY_MERGE_DIR)/
	$(NS)touch $@
	$(NS)echo "end prepare $(PKG_GNUTLS_NAME)!"

$(LOCAL_MODULE)-clean:
	$(NS)echo "begin make clean $(PKG_GNUTLS_NAME) ..."
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgnutls.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgnutls.so.28
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgnutls.so.28.29.1
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME) uninstall $(MAKE_PARALLEL)
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME) clean $(MAKE_PARALLEL)
	$(NS)echo "end make clean $(PKG_GNUTLS_NAME)"

$(LOCAL_MODULE)-cfg-clean:
	$(NS)echo "begin make clean $(PKG_GNUTLS_NAME)-cfg ..."
	$(NC)rm -rf $(PRIVATE_PKG_GNUTLS_NAME_CONFIGURE)
	$(NS)echo "end make clean $(PKG_GNUTLS_NAME)-cfg ..."

$(LOCAL_MODULE)-prepare-clean:$(LOCAL_MODULE)-clean
	$(NS)echo "begin make clean $(PKG_GNUTLS_NAME)-prepare ..."
	$(NC)rm -rf $(PRIVATE_PKG_GNUTLS_NAME_CONFIGURE)
	$(NC)rm -rf $(PRIVATE_PKG_GNUTLS_NAME_PREPARE)
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GNUTLS_NAME)
	$(NS)echo "end make clean $(PKG_GNUTLS_NAME)-prepare ..."

include $(BUILD_OPENSOURCE_PKG)
