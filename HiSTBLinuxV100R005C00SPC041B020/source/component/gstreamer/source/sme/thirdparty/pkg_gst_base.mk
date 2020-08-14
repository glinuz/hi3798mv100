LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pkg_gst_base

#EXT_TYPE_GZ := tar.gz
EXT_TYPE_XZ := tar.xz
PKG_GST_BASE_NAME := gst-plugins-base-1.2.3


LOCAL_INTERMEDIATE_TARGETS := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE) \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE)_prepare \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE)_configure \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE)_make \
	$(LOCAL_MODULE)-clean

PRIVATE_PKG_GST_BASE_NAME_PREPARE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE)_prepare
PRIVATE_PKG_GST_BASE_NAME_CONFIGURE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE)_configure
PRIVATE_PKG_GST_BASE_NAME_MAKE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE)_make
PRIVATE_PKG_GST_BASE_NAME_INSTALL := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE)_install
ifeq ($(ENV_SME_VERSION_TYPE), rls)
$(PRIVATE_PKG_GST_BASE_NAME_CONFIGURE): PRIVATE_PKG_GST_CORE_DBG_FLG := --disable-debug
else
$(PRIVATE_PKG_GST_BASE_NAME_CONFIGURE): PRIVATE_PKG_GST_CORE_DBG_FLG := --enable-debug
endif

PRIVATE_PKG_GST_BASE_DEPS := pkg_gst_core-deps

$(LOCAL_MODULE)-deps: $(PRIVATE_PKG_GST_BASE_DEPS) $(LOCAL_MODULE)

$(LOCAL_MODULE): $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE)

$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/.$(LOCAL_MODULE):$(PRIVATE_PKG_GST_BASE_NAME_INSTALL)

$(PRIVATE_PKG_GST_BASE_NAME_INSTALL):$(PRIVATE_PKG_GST_BASE_NAME_MAKE)
	$(NS)echo "begin INSTALL $(PKG_GST_BASE_NAME)!"
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstallocators-1.0.so $(TARGET_OUT_LIB_DIR)/libgstallocators-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstallocators-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstallocators-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstallocators-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstallocators-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstallocators-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstapp-1.0.so $(TARGET_OUT_LIB_DIR)/libgstapp-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstapp-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstapp-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstapp-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstapp-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstapp-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstaudio-1.0.so $(TARGET_OUT_LIB_DIR)/libgstaudio-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstaudio-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstaudio-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstaudio-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstaudio-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstaudio-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstfft-1.0.so $(TARGET_OUT_LIB_DIR)/libgstfft-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstfft-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstfft-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstfft-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstfft-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstfft-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstpbutils-1.0.so $(TARGET_OUT_LIB_DIR)/libgstpbutils-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstpbutils-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstpbutils-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstpbutils-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstpbutils-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstpbutils-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstriff-1.0.so $(TARGET_OUT_LIB_DIR)/libgstriff-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstriff-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstriff-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstriff-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstriff-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstpbutils-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstrtp-1.0.so $(TARGET_OUT_LIB_DIR)/libgstrtp-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstrtp-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstrtp-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstrtp-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstrtp-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstrtp-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstrtsp-1.0.so $(TARGET_OUT_LIB_DIR)/libgstrtsp-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstrtsp-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstrtsp-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstrtsp-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstrtsp-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstrtsp-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstsdp-1.0.so $(TARGET_OUT_LIB_DIR)/libgstsdp-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstsdp-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstsdp-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstsdp-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstsdp-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstsdp-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgsttag-1.0.so $(TARGET_OUT_LIB_DIR)/libgsttag-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgsttag-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgsttag-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgsttag-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgsttag-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgsttag-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstvideo-1.0.so $(TARGET_OUT_LIB_DIR)/libgstvideo-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstvideo-1.0.so.0 $(TARGET_OUT_LIB_DIR)/libgstvideo-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgstvideo-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgstvideo-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgstvideo-1.0.so.0.203.0
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstadder.so $(TARGET_OUT_PLUGIN_DIR)/libgstadder.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstadder.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstapp.so $(TARGET_OUT_PLUGIN_DIR)/libgstapp.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstapp.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstaudioconvert.so $(TARGET_OUT_PLUGIN_DIR)/libgstaudioconvert.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstaudioconvert.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstaudiorate.so $(TARGET_OUT_PLUGIN_DIR)/libgstaudiorate.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstaudiorate.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstaudioresample.so $(TARGET_OUT_PLUGIN_DIR)/libgstaudioresample.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstaudioresample.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstaudiotestsrc.so $(TARGET_OUT_PLUGIN_DIR)/libgstaudiotestsrc.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstaudiotestsrc.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstencodebin.so $(TARGET_OUT_PLUGIN_DIR)/libgstencodebin.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstencodebin.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstgio.so $(TARGET_OUT_PLUGIN_DIR)/libgstgio.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstgio.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstplayback.so $(TARGET_OUT_PLUGIN_DIR)/libgstplayback.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstplayback.so
endif

ifeq ($(CFG_ENABLE_SUBTITLE_PATH), y)
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstsubparse.so $(TARGET_OUT_PLUGIN_DIR)/libgstsubparse.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstsubparse.so
endif
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgsttcp.so $(TARGET_OUT_PLUGIN_DIR)/libgsttcp.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgsttcp.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgsttypefindfunctions.so $(TARGET_OUT_PLUGIN_DIR)/libgsttypefindfunctions.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgsttypefindfunctions.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstvideoconvert.so $(TARGET_OUT_PLUGIN_DIR)/libgstvideoconvert.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstvideoconvert.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstvideorate.so $(TARGET_OUT_PLUGIN_DIR)/libgstvideorate.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstvideorate.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstvideoscale.so $(TARGET_OUT_PLUGIN_DIR)/libgstvideoscale.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstvideoscale.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstvideotestsrc.so $(TARGET_OUT_PLUGIN_DIR)/libgstvideotestsrc.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstvideotestsrc.so
endif
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstvolume.so $(TARGET_OUT_PLUGIN_DIR)/libgstvolume.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstvolume.so
endif
ifeq ($(CFG_ENABLE_ALSASINK), y)
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstalsa.so $(TARGET_OUT_PLUGIN_DIR)/libgstalsa.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstalsa.so
endif
endif
	$(NS)echo "end INSTALL $(PKG_GST_BASE_NAME)!"

$(PRIVATE_PKG_GST_BASE_NAME_MAKE):$(PRIVATE_PKG_GST_BASE_NAME_CONFIGURE)
	$(NS)echo "begin make $(PKG_GST_BASE_NAME)!"
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME) $(MAKE_PARALLEL)
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME) install $(MAKE_PARALLEL)
	$(NS)echo "end make $(PKG_GST_BASE_NAME)!"

PRIVATE_PKG_GST_BASE_CFLAGS :=
PRIVATE_PKG_GST_BASE_CXXFLAGS :=
ifeq ($(CFG_ENABLE_SME_MQ_CTRL), y)
PRIVATE_PKG_GST_BASE_CFLAGS += -D__SME_MQ_CTRL__
PRIVATE_PKG_GST_BASE_CXXFLAGS += -D__SME_MQ_CTRL__
endif

ifeq ($(CFG_ENABLE_HI_CHARSET_DETECT), y)
PRIVATE_PKG_GST_BASE_CFLAGS += -D__USE_HISI_CHAR_DET__
PRIVATE_PKG_GST_BASE_CXXFLAGS += -D__USE_HISI_CHAR_DET__
endif

ifeq ($(CFG_ENABLE_BITSTREAM), y)
PRIVATE_PKG_GST_BASE_CFLAGS += -DSME_AUDIO_BITSTREAM
PRIVATE_PKG_GST_BASE_CXXFLAGS += -DSME_AUDIO_BITSTREAM
endif

ifeq ($(CFG_ENABLE_ALSASINK), y)
PRIVATE_PKG_GST_BASE_ALSA_FLG := --enable-alsa \
	--with-alsa-inc-prefix=$(TOPDIR)/$(CFG_RT_ALSA_INC_DIR) \
	ALSA_CFLAGS=-I$(TOPDIR)/$(CFG_RT_ALSA_INC_DIR) \
	ALSA_LIBS="-L$(TOPDIR)/$(CFG_RT_ALSA_LIB_DIR) -l$(CFG_ALSA_LIB_NAME)"
else
PRIVATE_PKG_GST_BASE_ALSA_FLG := --disable-alsa
endif

ifeq ($(CFG_ENABLE_NDK), y)
GST_BASE_LDFLAGS:=$(OPENSOURCE_LDFLAGS) -L$(CFG_COMPILE_SYSROOT)/usr/lib -L$(THIRDPARTY_OBJ_DIR)/lib -ldl
GST_BASE_CFLAGS:=-I$(THIRDPARTY_OBJ_DIR)/include
else
GST_BASE_LDFLAGS:=$(OPENSOURCE_LDFLAGS) -L$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR) -ldl
GST_BASE_CFLAGS:=$(foreach inc_tmp, $(CFG_RT_PLATFORM_SDK_INC_DIR), -I$(TOPDIR)/$(inc_tmp))
endif

ifeq ($(CFG_ENABLE_SUBTITLE_PATH), y)
ifeq ($(CFG_ENABLE_HI_CHARSET_DETECT), y)
GST_BASE_LDFLAGS += -l$(CFG_HI_CHARSETMGR_LIB_NAME)
endif
endif

$(PRIVATE_PKG_GST_BASE_NAME_CONFIGURE):$(PRIVATE_PKG_GST_BASE_NAME_PREPARE)
	$(NS)echo "begin configure $(PKG_GST_BASE_NAME)..."
	chmod 777 $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/configure
	cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME);ac_cv_path_install="/usr/bin/install -cp" ./configure --prefix="${THIRDPARTY_OBJ_DIR}" \
		--host=$(CFG_HOST_TYPE) \
		CFLAGS="$(PRIVATE_PKG_GST_BASE_CFLAGS) $(GST_BASE_CFLAGS) $(OPENSOURCE_CFLAGS) --sysroot=$(CFG_COMPILE_SYSROOT)"\
		CPPFLAGS="$(PRIVATE_PKG_GST_BASE_CXXFLAGS) $(OPENSOURCE_CXXFLAGS)" \
		--disable-static --enable-shared \
		--disable-pango --disable-orc $(PRIVATE_PKG_GST_CORE_DBG_FLG) --disable-benchmarks \
		--disable-examples  --disable-freetypetest --disable-static-plugins \
		--disable-ogg --disable-vorbis --enable-external \
		--disable-x --disable-xvideo \
		--disable-theora \
		--with-default-audiosink=$(CFG_AUDIOSINK_NAME) \
		--with-default-videosink=$(CFG_VIDEOSINK_NAME)\
		ZLIB_CFLAGS="$(CFG_ZLIB_CFLAGS)" ZLIB_LIBS="$(CFG_ZLIBS)" \
		LDFLAGS="$(GST_BASE_LDFLAGS)" LIBS="-lz" \
		$(PRIVATE_PKG_GST_BASE_ALSA_FLG) \
		--with-pkg-config-path=$(THIRDPARTY_PKG_CONFIG_DIR) \
		CXX=$(TARGET_CXX) CC=$(TARGET_CC)
	-cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/docs;sed -i "s/install: install-recursive/install:/g" `grep "install: install-recursive" -rl Makefile`
	-cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)/po;sed -i "s/install: install-exec install-data/install:/g" `grep "install: install-exec install-data" -rl Makefile`
	$(NS)touch $@
	$(NS)echo "end configure $(PKG_GST_BASE_NAME)..."

$(PRIVATE_PKG_GST_BASE_NAME_PREPARE):
	$(NS)echo "begin prepare $(PKG_GST_BASE_NAME)..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)
	$(NC)mkdir -p $(SME_THIRDPARTY_MERGE_DIR)
	tar -xf $(SME_THIRDPARTY_ORG_DIR)/$(PKG_GST_BASE_NAME).$(EXT_TYPE_XZ) -C $(SME_THIRDPARTY_MERGE_DIR)
	$(NC)cp -af $(SME_THIRDPARTY_PATCH_DIR)/$(PKG_GST_BASE_NAME) $(SME_THIRDPARTY_MERGE_DIR)/
	$(NS)touch $@
	$(NS)echo "end prepare $(PKG_GST_BASE_NAME)!"

$(LOCAL_MODULE)-clean:
	$(NS)echo "begin make clean $(PKG_GST_BASE_NAME) ..."
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstallocators-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstallocators-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstallocators-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstapp-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstapp-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstapp-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstaudio-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstaudio-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstaudio-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstfft-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstfft-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstfft-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstpbutils-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstpbutils-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstpbutils-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstriff-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstriff-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstriff-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstrtp-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstrtp-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstrtp-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstrtsp-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstrtsp-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstrtsp-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstsdp-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstsdp-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstsdp-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgsttag-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgsttag-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgsttag-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstvideo-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstvideo-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgstvideo-1.0.so.0.203.0
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstadder.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstapp.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstaudioconvert.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstaudiorate.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstaudioresample.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstaudiotestsrc.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstencodebin.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstgio.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstplayback.so
ifeq ($(CFG_ENABLE_SUBTITLE_PATH), y)
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstsubparse.so
endif
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgsttcp.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgsttypefindfunctions.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstvideoconvert.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstvideorate.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstvideoscale.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstvideotestsrc.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstvolume.so
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstalsa.so
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME) uninstall $(MAKE_PARALLEL)
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME) clean $(MAKE_PARALLEL)
	$(NS)echo "end make clean $(PKG_GST_BASE_NAME)"

$(LOCAL_MODULE)-cfg-clean:
	$(NS)echo "begin make clean $(PKG_GST_BASE_NAME)-cfg ..."
	$(NC)rm -rf $(PRIVATE_PKG_GST_BASE_NAME_CONFIGURE)
	$(NS)echo "end make clean $(PKG_GST_BASE_NAME)-cfg ..."

$(LOCAL_MODULE)-prepare-clean:$(LOCAL_MODULE)-clean
	$(NS)echo "begin make clean $(PKG_GST_BASE_NAME)-prepare ..."
	$(NC)rm -rf $(PRIVATE_PKG_GST_BASE_NAME_CONFIGURE)
	$(NC)rm -rf $(PRIVATE_PKG_GST_BASE_NAME_PREPARE)
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BASE_NAME)
	$(NS)echo "end make clean $(PKG_GST_BASE_NAME)-prepare ..."

include $(BUILD_OPENSOURCE_PKG)
