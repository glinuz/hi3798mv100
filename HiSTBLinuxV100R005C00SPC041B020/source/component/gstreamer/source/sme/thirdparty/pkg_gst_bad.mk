LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := pkg_gst_bad

#EXT_TYPE_GZ := tar.gz
EXT_TYPE_XZ := tar.xz
PKG_GST_BAD_NAME := gst-plugins-bad-1.2.3


LOCAL_INTERMEDIATE_TARGETS := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE) \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)_prepare \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)_configure \
	$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)_make \
	$(LOCAL_MODULE)-clean

PRIVATE_PKG_GST_BAD_NAME_PREPARE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)_prepare
PRIVATE_PKG_GST_BAD_NAME_CONFIGURE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)_configure
PRIVATE_PKG_GST_BAD_NAME_MAKE := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)_make
PRIVATE_PKG_GST_BAD_NAME_INSTALL := $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)_install
ifeq ($(ENV_SME_VERSION_TYPE), rls)
$(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE): PRIVATE_PKG_GST_BAD_DBG_FLG := --disable-debug
else
$(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE): PRIVATE_PKG_GST_BAD_DBG_FLG := --enable-debug
endif

$(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE): PRIVATE_PKG_HLS_CONFIG_FLG := --disable-hls
$(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE): PRIVATE_PKG_DASH_CONFIG_FLG := --disable-dash

ifeq ($(CFG_ENABLE_GST_BAD_HLS), y)
$(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE): PRIVATE_PKG_HLS_CONFIG_FLG := --enable-hls
endif

ifeq ($(CFG_ENABLE_GST_BAD_DASH), y)
$(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE): PRIVATE_PKG_DASH_CONFIG_FLG := --enable-dash
endif


PRIVATE_PKG_GST_BAD_DEPS := pkg_gst_base-deps pkg_gnutls

$(LOCAL_MODULE)-deps: $(PRIVATE_PKG_GST_BAD_DEPS) $(LOCAL_MODULE)

$(LOCAL_MODULE): $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE)

$(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/.$(LOCAL_MODULE):$(PRIVATE_PKG_GST_BAD_NAME_INSTALL)

$(PRIVATE_PKG_GST_BAD_NAME_INSTALL):$(PRIVATE_PKG_GST_BAD_NAME_MAKE)
	$(NS)echo "begin INSTALL $(PKG_GST_BAD_NAME)!"
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgsturidownloader-1.0.so $(TARGET_OUT_LIB_DIR)/libgsturidownloader-1.0.so
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgsturidownloader-1.0.so.0  $(TARGET_OUT_LIB_DIR)/libgsturidownloader-1.0.so.0
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/libgsturidownloader-1.0.so.0.203.0 $(TARGET_OUT_LIB_DIR)/libgsturidownloader-1.0.so.0.203.0
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_LIB_DIR)/libgsturidownloader-1.0.so.0.203.0
endif
ifeq ($(CFG_ENABLE_GST_BAD_HLS), y)
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstfragmented.so $(TARGET_OUT_PLUGIN_DIR)/libgstfragmented.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstfragmented.so
endif
endif
ifeq ($(CFG_ENABLE_GST_BAD_DASH), y)
	cp -af $(THIRDPARTY_OBJ_DIR)/lib/gstreamer-1.0/libgstdashdemux.so $(TARGET_OUT_PLUGIN_DIR)/libgstdashdemux.so
ifeq ($(STRIP_IN_RELEASE_VERSION), y)
	$(TARGET_STRIP) $(TARGET_OUT_PLUGIN_DIR)/libgstdashdemux.so
endif
endif
	$(NS)echo "end INSTALL $(PKG_GST_BAD_NAME)!"

$(PRIVATE_PKG_GST_BAD_NAME_MAKE):$(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE)
	$(NS)echo "begin make $(PKG_GST_BAD_NAME)!"
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME) $(MAKE_PARALLEL)
	make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME) install $(MAKE_PARALLEL)
	$(NS)echo "end make $(PKG_GST_BAD_NAME)!"


ifeq ($(CFG_ENABLE_NDK), y)
GST_BAD_LDFLAGS:=$(OPENSOURCE_LDFLAGS) -L$(CFG_COMPILE_SYSROOT)/usr/lib -L$(THIRDPARTY_OBJ_DIR)/lib
else
GST_BAD_LDFLAGS:=$(OPENSOURCE_LDFLAGS) -L$(TOPDIR)/$(CFG_RT_PLATFORM_SDK_DYNAMIC_LIB_DIR)
endif

$(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE):$(PRIVATE_PKG_GST_BAD_NAME_PREPARE)
	$(NS)echo "begin configure $(PKG_GST_BAD_NAME)..."
	chmod 777 $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)/configure
	cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME);ac_cv_path_install="/usr/bin/install -cp" ./configure --prefix="${THIRDPARTY_OBJ_DIR}" \
		--host=$(CFG_HOST_TYPE) \
		CFLAGS="$(OPENSOURCE_CFLAGS) --sysroot=$(CFG_COMPILE_SYSROOT)" \
		CXXFLAGS="$(OPENSOURCE_CXXFLAGS)" \
		--disable-static --enable-shared --disable-static-plugins \
		--disable-orc $(PRIVATE_PKG_GST_BAD_DBG_FLG) --disable-benchmarks --disable-opensles \
		--disable-examples --disable-accurip --disable-adpcmdec --disable-librfb --disable-curl  \
		--disable-adpcmenc --disable-aiff --disable-asfmux --disable-audiofxbad --disable-audiovisualizers \
		--disable-autoconvert --disable-bayer --disable-camerabin2 --disable-cdxaparse --disable-coloreffects \
		--disable-dataurisrc --disable-dccp --disable-debugutils --disable-dvbsuboverlay --disable-dvdspu \
		--disable-faceoverlay --disable-festival --disable-fieldanalysis --disable-freeverb --disable-frei0r \
		--disable-gaudieffects --disable-geometrictransform  --disable-gdp --disable-hdvparse --disable-id3tag \
		--disable-inter --disable-interlace --disable-ivfparse --disable-ivtc --disable-jp2kdecimator \
		--disable-jpegformat --disable-liveadder --disable-midi --disable-mpegdemux --disable-mpegtsdemux \
		--disable-mpegtsmux --disable-mpegpsmux --disable-mve --disable-mxf --disable-nuvdemux --disable-patchdetect \
		--disable-pcapparse --disable-pnm --disable-rawparse --disable-real --disable-removesilence --disable-sdi \
		--disable-sdp --disable-segmentclip --disable-siren --disable-smooth --disable-speed --disable-subenc \
		--disable-stereo --disable-tta --disable-videofilters --disable-videomeasure --disable-videoparsers \
		--disable-videosignal --disable-vmnc --disable-y4m --disable-yadif --disable-directsound --disable-wasapi \
		--disable-direct3d --disable-directdraw --disable-winscreencap --disable-directshow --disable-apple_media \
		--disable-bluez --disable-osx_video  --disable-avc --disable-quicktime --disable-dc1394  --disable-resindvd \
		--disable-faac --disable-faad --disable-fbdev --disable-flite --disable-gsm --disable-fluidsynth --disable-kate \
		--disable-ladspa --disable-lv2 --disable-libmms --disable-mythtv --disable-nas  --disable-neon --disable-ofa \
		--disable-openal --disable-opencv --disable-openjpeg --disable-opus --disable-pvr --disable-rsvg --disable-gsettings \
		--disable-soundtouch --disable-mpeg2enc --disable-mpg123 --disable-mplex --disable-musepack --disable-mythtv \
		--disable-timidity --disable-eglgles --disable-teletextdec --disable-wildmidi --disable-sdl --disable-sdltest \
		--disable-smoothstreaming --disable-sndfile --disable-soundtouch --disable-spc --disable-gme --disable-xvid \
		--disable-dvb --disable-wininet --disable-acm --disable-vdpau --disable-sbc --disable-schro --disable-zbar --disable-wayland \
		--disable-rtmp --disable-spandsp --disable-sndio --disable-decklink --disable-directfb --without-x --without-egl-window-system \
		$(PRIVATE_PKG_HLS_CONFIG_FLG) \
		$(PRIVATE_PKG_DASH_CONFIG_FLG) \
		ZLIB_CFLAGS="$(CFG_ZLIB_CFLAGS)" ZLIB_LIBS="$(CFG_ZLIBS)" \
		LDFLAGS="$(GST_BAD_LDFLAGS) -Wl,-rpath-link=$(THIRDPARTY_OBJ_DIR)/lib" LIBS="-lz" \
		--with-pkg-config-path=$(THIRDPARTY_PKG_CONFIG_DIR) \
		CXX=$(TARGET_CXX) CC=$(TARGET_CC)
	-$(NS)cd $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME);sed -i 's/m4 common docs tests po tools/m4 common/g' `grep 'm4 common docs tests po tools' -rl Makefile`
	$(NS)touch $@
	$(NS)echo "end configure $(PKG_GST_BAD_NAME)..."

$(PRIVATE_PKG_GST_BAD_NAME_PREPARE):
	$(NS)echo "begin prepare $(PKG_GST_BAD_NAME)..."
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)
	$(NC)mkdir -p $(SME_THIRDPARTY_MERGE_DIR)
	tar -xf $(SME_THIRDPARTY_ORG_DIR)/$(PKG_GST_BAD_NAME).$(EXT_TYPE_XZ) -C $(SME_THIRDPARTY_MERGE_DIR)
	$(NC)cp -af $(SME_THIRDPARTY_PATCH_DIR)/$(PKG_GST_BAD_NAME) $(SME_THIRDPARTY_MERGE_DIR)/
	$(NS)touch $@
	$(NS)echo "end prepare $(PKG_GST_BAD_NAME)!"

$(LOCAL_MODULE)-clean:
	$(NS)echo "begin make clean $(PKG_GST_BAD_NAME) ..."
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgsturidownloader-1.0.so
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgsturidownloader-1.0.so.0
	$(NC)rm -rf $(TARGET_OUT_LIB_DIR)/libgsturidownloader-1.0.so.0.203.0
ifeq ($(CFG_ENABLE_GST_BAD_HLS), y)
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstfragmented.so
endif
ifeq ($(CFG_ENABLE_GST_BAD_DASH), y)
	$(NC)rm -rf $(TARGET_OUT_PLUGIN_DIR)/libgstdashdemux.so
endif
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME) uninstall $(MAKE_PARALLEL)
	$(NC)make -C $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME) clean $(MAKE_PARALLEL)
	$(NS)echo "end make clean $(PKG_GST_BAD_NAME)"

$(LOCAL_MODULE)-cfg-clean:
	$(NS)echo "begin make clean $(PKG_GST_BAD_NAME)-cfg ..."
	$(NC)rm -rf $(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE)
	$(NS)echo "end make clean $(PKG_GST_BAD_NAME)-cfg ..."

$(LOCAL_MODULE)-prepare-clean:$(LOCAL_MODULE)-clean
	$(NS)echo "begin make clean $(PKG_GST_BAD_NAME)-prepare ..."
	$(NC)rm -rf $(PRIVATE_PKG_GST_BAD_NAME_CONFIGURE)
	$(NC)rm -rf $(PRIVATE_PKG_GST_BAD_NAME_PREPARE)
	$(NC)rm -rf $(SME_THIRDPARTY_MERGE_DIR)/$(PKG_GST_BAD_NAME)
	$(NS)echo "end make clean $(PKG_GST_BAD_NAME)-prepare ..."

include $(BUILD_OPENSOURCE_PKG)
