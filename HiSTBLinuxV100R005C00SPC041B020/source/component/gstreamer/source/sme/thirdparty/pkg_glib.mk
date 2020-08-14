LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pkg_glib

#EXT_TYPE_GZ := tar.gz
EXT_TYPE_XZ := tar.xz
EXT_TYPE_TAR := tar
PKG_GLIB_NAME := glib-2.46.2


LOCAL_INTERMEDIATE_TARGETS := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE) \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)_prepare \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)_configure \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)_make \
	$(LOCAL_MODULE)-clean

PRIVATE_PKG_GLIB_NAME_PREPARE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)_prepare
PRIVATE_PKG_GLIB_NAME_CONFIGURE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)_configure
PRIVATE_PKG_GLIB_NAME_MAKE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)_make
PRIVATE_PKG_GLIB_NAME_INSTALL := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)_install
ifeq ($(ENV_SME_VERSION_TYPE), rls)
$(PRIVATE_PKG_GLIB_NAME_CONFIGURE): PRIVATE_PKG_GLIB_DBG_FLG := --disable-debug
else
$(PRIVATE_PKG_GLIB_NAME_CONFIGURE): PRIVATE_PKG_GLIB_DBG_FLG := --enable-debug
endif

PRIVATE_PKG_GLIB_DEPS := pkg_iconv pkg_gettext pkg_ffi

$(LOCAL_MODULE)-deps: $(PRIVATE_PKG_GLIB_DEPS) $(LOCAL_MODULE)

$(LOCAL_MODULE): $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE)

$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/.$(LOCAL_MODULE):$(PRIVATE_PKG_GLIB_NAME_INSTALL)

$(PRIVATE_PKG_GLIB_NAME_INSTALL):$(PRIVATE_PKG_GLIB_NAME_MAKE)
	$(NS)echo "begin INSTALL $(PKG_GLIB_NAME)!"
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgio-2.0.so $(TARGET_OUT_LIB_DIR)/libgio-2.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgio-2.0.so.0 $(TARGET_OUT_LIB_DIR)/libgio-2.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgio-2.0.so.0.4600.2 $(TARGET_OUT_LIB_DIR)/libgio-2.0.so.0.4600.2
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgio-2.0.so.0.4600.2
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libglib-2.0.so $(TARGET_OUT_LIB_DIR)/libglib-2.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libglib-2.0.so.0 $(TARGET_OUT_LIB_DIR)/libglib-2.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libglib-2.0.so.0.4600.2 $(TARGET_OUT_LIB_DIR)/libglib-2.0.so.0.4600.2
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libglib-2.0.so.0.4600.2
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgmodule-2.0.so $(TARGET_OUT_LIB_DIR)/libgmodule-2.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgmodule-2.0.so.0 $(TARGET_OUT_LIB_DIR)/libgmodule-2.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgmodule-2.0.so.0.4600.2 $(TARGET_OUT_LIB_DIR)/libgmodule-2.0.so.0.4600.2
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgmodule-2.0.so.0.4600.2
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgobject-2.0.so $(TARGET_OUT_LIB_DIR)/libgobject-2.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgobject-2.0.so.0 $(TARGET_OUT_LIB_DIR)/libgobject-2.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgobject-2.0.so.0.4600.2 $(TARGET_OUT_LIB_DIR)/libgobject-2.0.so.0.4600.2
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgobject-2.0.so.0.4600.2
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgthread-2.0.so $(TARGET_OUT_LIB_DIR)/libgthread-2.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgthread-2.0.so.0 $(TARGET_OUT_LIB_DIR)/libgthread-2.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgthread-2.0.so.0.4600.2 $(TARGET_OUT_LIB_DIR)/libgthread-2.0.so.0.4600.2
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgthread-2.0.so.0.4600.2
endif
	$(NS)echo "end INSTALL $(PKG_GLIB_NAME)!"

$(PRIVATE_PKG_GLIB_NAME_MAKE):$(PRIVATE_PKG_GLIB_NAME_CONFIGURE)
	$(NS)echo "begin make $(PKG_GLIB_NAME)!"
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME) $(MAKE_PARALLEL)
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME) install $(MAKE_PARALLEL)
	$(NS)echo "end make $(PKG_GLIB_NAME)!"

ifeq ($(CFG_PROJECT_NAME), linux_hi3798m_generic)
$(PRIVATE_PKG_GLIB_NAME_CONFIGURE): PRIVATE_PKG_GLIB_CACHE_FILE_NAME := linux.cache
else
$(PRIVATE_PKG_GLIB_NAME_CONFIGURE): PRIVATE_PKG_GLIB_CACHE_FILE_NAME := android.cache
endif

ifeq ($(CFG_ENABLE_NDK), y)
GLIB_LDFLAGS:=$(OPENSOURCE_LDFLAGS) -L$(CFG_COMPILE_SYSROOT)/usr/lib -L$(THIRDPARTY_OBJ_DIR)/lib
else
GLIB_LDFLAGS:=$(OPENSOURCE_LDFLAGS) -L$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR) -L$(THIRDPARTY_OBJ_DIR)/lib
endif

$(PRIVATE_PKG_GLIB_NAME_CONFIGURE):$(PRIVATE_PKG_GLIB_NAME_PREPARE)
	$(NS)echo "begin configure $(PKG_GLIB_NAME)..."
	cp -af $(SME_THIRDPARTY_PATCH_DIR)/$(PKG_GLIB_NAME)/$(PRIVATE_PKG_GLIB_CACHE_FILE_NAME) $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/
	chmod 777 $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/configure
	cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME);ac_cv_path_GLIB_GENMARSHAL=$(SME_DEPS_BIN_PATH)/glib-genmarshal ac_cv_path_install="/usr/bin/install -cp" ./configure --prefix="${THIRDPARTY_OBJ_DIR}" \
		--host=$(CFG_HOST_TYPE) \
		CFLAGS="$(OPENSOURCE_CFLAGS) --sysroot=$(CFG_COMPILE_SYSROOT) -I$(THIRDPARTY_OBJ_DIR)/include" \
		CPPFLAGS="$(OPENSOURCE_CXXFLAGS) --sysroot=$(CFG_COMPILE_SYSROOT) -I$(THIRDPARTY_OBJ_DIR)/include" \
		--disable-gtk-doc-html --disable-gtk-doc --disable-gtk-doc-pdf \
		--disable-man \
		--disable-static --enable-shared --cache-file=$(PRIVATE_PKG_GLIB_CACHE_FILE_NAME) \
		--enable-iconv=no --with-libiconv=gnu \
		--disable-always-build-tests  CXX=$(TARGET_CXX) CC=$(TARGET_CC) $(PRIVATE_PKG_GLIB_DBG_FLG) LIBS="-lz" \
		ZLIB_CFLAGS="$(CFG_ZLIB_CFLAGS)" ZLIB_LIBS="$(CFG_ZLIBS)" \
		LDFLAGS="$(GLIB_LDFLAGS)" \
		PKG_CONFIG_PATH=$(THIRDPARTY_PKG_CONFIG_DIR)
	-cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/docs;sed -i "s/install: install-recursive/install:/g" `grep "install: install-recursive" -rl Makefile`
	-cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)/po;sed -i "s/install: install-exec install-data/install:/g" `grep "install: install-exec install-data" -rl Makefile`
	$(NS)touch $@
	$(NS)echo "end configure $(PKG_GLIB_NAME)..."

$(PRIVATE_PKG_GLIB_NAME_PREPARE):
	$(NS)echo "begin prepare $(PKG_GLIB_NAME)..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)
	$(NC)mkdir -p $(SME_THIRDPARTY_MERGE_DIR)
	tar -xvf $(SME_THIRDPARTY_ORG_DIR)/$(PKG_GLIB_NAME).$(EXT_TYPE_TAR) -C $(SME_THIRDPARTY_MERGE_DIR)
	$(NC)cp -af $(SME_THIRDPARTY_PATCH_DIR)/$(PKG_GLIB_NAME) $(SME_THIRDPARTY_MERGE_DIR)/
	$(NS)touch $@
	$(NS)echo "end prepare $(PKG_GLIB_NAME)!"

$(LOCAL_MODULE)-clean:
	$(NS)echo "begin make clean $(PKG_GLIB_NAME) ..."
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgio-2.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgio-2.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgio-2.0.so.0.4600.2
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libglib-2.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libglib-2.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libglib-2.0.so.0.4600.2
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgmodule-2.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgmodule-2.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgmodule-2.0.so.0.4600.2
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgobject-2.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgobject-2.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgobject-2.0.so.0.4600.2
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgthread-2.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgthread-2.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgthread-2.0.so.0.4600.2
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME) uninstall $(MAKE_PARALLEL)
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME) clean $(MAKE_PARALLEL)
	$(NS)echo "end make clean $(PKG_GLIB_NAME)"

$(LOCAL_MODULE)-cfg-clean:
	$(NS)echo "begin make clean $(PKG_GLIB_NAME)-cfg ..."
	$(NC)rm -rf $(PRIVATE_PKG_GLIB_NAME_CONFIGURE)
	$(NS)echo "end make clean $(PKG_GLIB_NAME)-cfg ..."

$(LOCAL_MODULE)-prepare-clean:$(LOCAL_MODULE)-clean
	$(NS)echo "begin make clean $(PKG_GLIB_NAME)-prepare ..."
	$(NC)rm -rf $(PRIVATE_PKG_GLIB_NAME_CONFIGURE)
	$(NC)rm -rf $(PRIVATE_PKG_GLIB_NAME_PREPARE)
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GLIB_NAME)
	$(NS)echo "end make clean $(PKG_GLIB_NAME)-prepare ..."

include $(BUILD_OPENSOURCE_PKG)
