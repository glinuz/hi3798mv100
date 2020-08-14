LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pkg_gst_core

#EXT_TYPE_GZ := tar.gz
EXT_TYPE_XZ := tar.xz
PKG_GST_CORE_NAME := gstreamer-1.2.3


LOCAL_INTERMEDIATE_TARGETS := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE) \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE)_prepare \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE)_configure \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE)_make \
	$(LOCAL_MODULE)-clean

PRIVATE_PKG_GST_CORE_NAME_PREPARE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE)_prepare
PRIVATE_PKG_GST_CORE_NAME_CONFIGURE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE)_configure
PRIVATE_PKG_GST_CORE_NAME_MAKE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE)_make
PRIVATE_PKG_GST_CORE_NAME_INSTALL := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE)_install
ifeq ($(ENV_SME_VERSION_TYPE), rls)
$(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE): PRIVATE_PKG_GST_CORE_DBG_FLG := --disable-debug
else
$(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE): PRIVATE_PKG_GST_CORE_DBG_FLG := --enable-debug
endif

PRIVATE_PKG_GST_CORE_DEPS := pkg_glib-deps

$(LOCAL_MODULE)-deps: $(PRIVATE_PKG_GST_CORE_DEPS) $(LOCAL_MODULE)

$(LOCAL_MODULE): $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE)

$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/.$(LOCAL_MODULE):$(PRIVATE_PKG_GST_CORE_NAME_INSTALL)

$(PRIVATE_PKG_GST_CORE_NAME_INSTALL):$(PRIVATE_PKG_GST_CORE_NAME_MAKE)
	$(NS)echo "begin INSTALL $(PKG_GST_CORE_NAME)!"
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstbase-1.0.so $(TARGET_OUT_LIB_DIR)/libgstbase-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstbase-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstbase-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstbase-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstbase-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstbase-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstcheck-1.0.so $(TARGET_OUT_LIB_DIR)/libgstcheck-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstcheck-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstcheck-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstcheck-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstcheck-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstcheck-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstcontroller-1.0.so $(TARGET_OUT_LIB_DIR)/libgstcontroller-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstcontroller-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstcontroller-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstcontroller-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstcontroller-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstcontroller-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstnet-1.0.so $(TARGET_OUT_LIB_DIR)/libgstnet-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstnet-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstnet-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstnet-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstnet-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstnet-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstreamer-1.0.so $(TARGET_OUT_LIB_DIR)/libgstreamer-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstreamer-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstreamer-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstreamer-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstreamer-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstreamer-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstcoreelements.so $(TARGET_OUT_PLUGIN_DIR)/libgstcoreelements.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstcoreelements.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/bin/gst-launch-1.0 $(TARGET_OUT_BIN_DIR)/gst-launch-1.0
	cp -af $(THIRDPARTY_OBJ_DIR)/bin/gst-inspect-1.0 $(TARGET_OUT_BIN_DIR)/gst-inspect-1.0
	cp -af $(THIRDPARTY_OBJ_DIR)/bin/gst-typefind-1.0 $(TARGET_OUT_BIN_DIR)/gst-typefind-1.0
	$(NS)echo "end INSTALL $(PKG_GST_CORE_NAME)!"

$(PRIVATE_PKG_GST_CORE_NAME_MAKE):$(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE)
	$(NS)echo "begin make $(PKG_GST_CORE_NAME)!"
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME) $(MAKE_PARALLEL)
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME) install $(MAKE_PARALLEL)
	$(NS)echo "end make $(PKG_GST_CORE_NAME)!"

ifeq ($(CFG_ENABLE_SME_MQ_CTRL), y)
$(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE): PRIVATE_PKG_GST_CORE_CFLAGS := -D__SME_MQ_CTRL__
$(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE): PRIVATE_PKG_GST_CORE_CXXFLAGS := -D__SME_MQ_CTRL__
else
$(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE): PRIVATE_PKG_GST_CORE_CFLAGS :=
$(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE): PRIVATE_PKG_GST_CORE_CXXFLAGS :=
endif

$(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE):$(PRIVATE_PKG_GST_CORE_NAME_PREPARE)
	$(NS)echo "begin configure $(PKG_GST_CORE_NAME)..."
	chmod 777 $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)/configure
	cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME);ac_cv_path_install="/usr/bin/install -cp" ./configure --prefix="${THIRDPARTY_OBJ_DIR}" \
		--host=$(CFG_HOST_TYPE) \
		CFLAGS="$(PRIVATE_PKG_GST_CORE_CFLAGS) $(OPENSOURCE_CFLAGS) --sysroot=$(CFG_COMPILE_SYSROOT)" \
		CPPFLAGS="$(PRIVATE_PKG_GST_CORE_CXXFLAGS) $(OPENSOURCE_CXXFLAGS)" \
		LDFLAGS="$(OPENSOURCE_LDFLAGS)" \
		--disable-static --enable-shared \
		--disable-loadsave --disable-gtk-doc \
		ac_cv_func_register_printf_function=no --disable-tests \
		--disable-valgrind $(PRIVATE_PKG_GST_CORE_DBG_FLG) \
		--disable-examples --disable-static-plugins --disable-benchmarks \
		--with-pkg-config-path=$(THIRDPARTY_PKG_CONFIG_DIR) \
		CXX=$(TARGET_CXX) CC=$(TARGET_CC)
	-$(NS)cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME);sed -i 's/docs po//g' `grep 'docs po' -rl Makefile`
	$(NS)touch $@
	$(NS)echo "end configure $(PKG_GST_CORE_NAME)..."

$(PRIVATE_PKG_GST_CORE_NAME_PREPARE):
	$(NS)echo "begin prepare $(PKG_GST_CORE_NAME)..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)
	$(NC)mkdir -p $(SME_THIRDPARTY_MERGE_DIR)
	tar -xf $(SME_THIRDPARTY_ORG_DIR)/$(PKG_GST_CORE_NAME).$(EXT_TYPE_XZ) -C $(SME_THIRDPARTY_MERGE_DIR)
	$(NC)cp -af $(SME_THIRDPARTY_PATCH_DIR)/$(PKG_GST_CORE_NAME) $(SME_THIRDPARTY_MERGE_DIR)/
	$(NS)touch $@
	$(NS)echo "end prepare $(PKG_GST_CORE_NAME)!"

$(LOCAL_MODULE)-clean:
	$(NS)echo "begin make clean $(PKG_GST_CORE_NAME) ..."
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstbase-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstbase-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstbase-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstcheck-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstcheck-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstcheck-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstcontroller-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstcontroller-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstcontroller-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstnet-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstnet-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstnet-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstreamer-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstreamer-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstreamer-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstcoreelements.so
	$(NC)rm -rf $(TARGET_OUT_BIN_DIR)/gst-launch-1.0
	$(NC)rm -rf  $(TARGET_OUT_BIN_DIR)/gst-inspect-1.0
	$(NC)rm -rf  $(TARGET_OUT_BIN_DIR)/gst-typefind-1.0
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME) uninstall $(MAKE_PARALLEL)
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME) clean $(MAKE_PARALLEL)
	$(NS)echo "end make clean $(PKG_GST_CORE_NAME)"

$(LOCAL_MODULE)-cfg-clean:
	$(NS)echo "begin make clean $(PKG_GST_CORE_NAME)-cfg ..."
	$(NC)rm -rf $(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE)
	$(NS)echo "end make clean $(PKG_GST_CORE_NAME)-cfg ..."

$(LOCAL_MODULE)-prepare-clean:$(LOCAL_MODULE)-clean
	$(NS)echo "begin make clean $(PKG_GST_CORE_NAME)-prepare ..."
	$(NC)rm -rf $(PRIVATE_PKG_GST_CORE_NAME_CONFIGURE)
	$(NC)rm -rf $(PRIVATE_PKG_GST_CORE_NAME_PREPARE)
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_CORE_NAME)
	$(NS)echo "end make clean $(PKG_GST_CORE_NAME)-prepare ..."

include $(BUILD_OPENSOURCE_PKG)
