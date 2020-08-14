#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===================================================================
#DirectFB Version
#==================================================================
DIRECTFB_VER := 1.6.1
FUSION_VER   := 9.0.2
DFB_ADP_LIB_VER := directfb-1.6-0

DIRECTFB := DirectFB-$(DIRECTFB_VER)
FUSION   := linux-fusion-$(FUSION_VER)

ifeq ($(CFG_HI_DIRECTFB_MULTI_SUPPORT),y)
protype=enable
else
protype=disable
endif

ifeq ($(CFG_HI_DIRECTFB_DEBUG_SUPPORT),y)
comtype=enable
else
comtype=disable
endif

#===============================================================================
# local variable
#===============================================================================
DIRECTFB_OUT_DIR := $(HI_OUT_DIR)/obj/source/component/directfb
DIRECTFB_OUT_DIR64 := $(HI_OUT_DIR)/obj64/source/component/directfb

INSTALL_DIR := $(DIRECTFB_OUT_DIR)/directfb
INSTALL_DIR64 := $(DIRECTFB_OUT_DIR64)/directfb

ifeq ($(CFG_HI_CPU_ARCH),arm)
FUSION_OUT_DIR := $(DIRECTFB_OUT_DIR)
else
FUSION_OUT_DIR := $(DIRECTFB_OUT_DIR64)
endif

TARGETS :=

ifeq ($(HI_USER_SPACE_LIB),y)
TARGETS += directfb
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
TARGETS += directfb64
endif

ifeq ($(CFG_HI_DIRECTFB_MULTI_SUPPORT),y)
TARGETS += fusion
endif

#====================================================================
#jpeg
#====================================================================
JPEG_SOFT_INC_DIR := $(MSP_DIR)/api/jpeg/inc/inc_soft
LIBJPEG=libjpeg

#====================================================================
#png
#====================================================================
PNG_SOFT_INC_DIR := $(MSP_DIR)/api/png/libpng-1.6.16

LIBPNG_CFLAGS=-I$(HI_INCLUDE_DIR)
LIBPNG_LIBS=-lpng

#====================================================================
#freetype
#====================================================================
FREETYPE_CFLAGS=-I$(COMPONENT_DIR)/freetype/include/freetype2
FREETYPE_LIBS=-lfreetype

#=====================================================================================
#export env
#=====================================================================================
INCLUDES := \
            -I$(INSTALL_DIR)/include                \
            -I$(HI_INCLUDE_DIR)                     \
            -I$(JPEG_SOFT_INC_DIR)                  \
            -I$(PNG_SOFT_INC_DIR)                   \
            -I$(MSP_DIR)/api/jpeg/inc/inc_hard      \
            -I$(MSP_DIR)/png/release/include        \
            -I$(MSP_DIR)/api/png/include            \
            -I$(COMPONENT_DIR)/zlib/include         \
            -I$(COMPONENT_DIR)/directfb/include/include_$(FUSION_VER)

ifeq ($(HI_USER_SPACE_LIB),y)
LDFLAGS := -L$(INSTALL_DIR)/lib -L$(HI_SHARED_LIB_DIR)
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
LDFLAGS64 := -L$(INSTALL_DIR64)/lib -L$(HI_SHARED_LIB64_DIR)
endif

CPPFLAGS := $(INCLUDES)

#===================================================================
#CFLAGS
#==================================================================
ifneq ($(CFG_HI_KEYLED_74HC164_SUPPORT),)
CFLAGS := -DCFG_HI_KEYLED_74HC164_SUPPORT
endif
ifneq ($(CFG_HI_KEYLED_CT1642_SUPPORT),)
CFLAGS := -DCFG_HI_KEYLED_CT1642_SUPPORT
endif
ifneq ($(CFG_HI_KEYLED_PT6961_SUPPORT),)
CFLAGS := DCFG_HI_KEYLED_PT6961_SUPPORT
endif
ifneq ($(CFG_HI_KEYLED_PT6964_SUPPORT),)
CFLAGS := -DCFG_HI_KEYLED_PT6964_SUPPORT
endif

EXTRA_FLAGS :=

CFLAGS += $(EXTRA_FLAGS)

ifeq ($(CFG_HI_DIRECTFB_CA_SUPPORT),y)
CFLAGS += -DHI_ADVCA_FUNCTION_RELEASE -DCONFIG_SUPPORT_CA_RELEASE
endif
CFLAGS += -DPNG_ACCELERATION -DLIBPNG14

CXXFLAGS := -fno-rtti $(CFLAGS)
#===============================================================================
# rules
#===============================================================================
.PHONY: all clean directfb directfb64 fusion fusion_clean

all: $(TARGETS)
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)cp -rf  $(INSTALL_DIR)/include/directfb $(HI_INCLUDE_DIR)/
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)test -d $(HI_EXTERN_LIB_DIR) || mkdir -p $(HI_EXTERN_LIB_DIR)
	$(AT)cp -ralf $(INSTALL_DIR)/lib/lib*.so*  $(HI_SHARED_LIB_DIR)/
	$(AT)cp -rf $(INSTALL_DIR)/lib/$(DFB_ADP_LIB_VER) $(HI_EXTERN_LIB_DIR)
	$(AT)find $(HI_EXTERN_LIB_DIR)/$(DFB_ADP_LIB_VER)/ -name *.la | xargs rm -rf
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)cp -rf  $(INSTALL_DIR64)/include/directfb $(HI_INCLUDE_DIR)/
	$(AT)test -d $(HI_SHARED_LIB64_DIR) || mkdir -p $(HI_SHARED_LIB64_DIR)
	$(AT)test -d $(HI_EXTERN_LIB64_DIR) || mkdir -p $(HI_EXTERN_LIB64_DIR)
	$(AT)cp -ralf $(INSTALL_DIR64)/lib/lib*.so*  $(HI_SHARED_LIB64_DIR)/
	$(AT)cp -rf $(INSTALL_DIR64)/lib/$(DFB_ADP_LIB_VER)  $(HI_EXTERN_LIB64_DIR)
	$(AT)find $(HI_EXTERN_LIB64_DIR)/$(DFB_ADP_LIB_VER)/ -name *.la | xargs rm -rf
endif

clean:
	$(AT)rm -rf $(INSTALL_DIR) $(INSTALL_DIR64)
	$(AT)rm -rf $(DIRECTFB_OUT_DIR) $(DIRECTFB_OUT_DIR64)
	$(AT)rm -rf $(HI_INCLUDE_DIR)/directfb
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/libdirect*.so*
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/libfusion*.so*
	$(AT)rm -rf $(HI_EXTERN_LIB_DIR)/directfb-1.6-0
	$(AT)rm -rf $(HI_SHARED_LIB64_DIR)/libdirect*.so*
	$(AT)rm -rf $(HI_SHARED_LIB64_DIR)/libfusion*.so*
	$(AT)rm -rf $(HI_EXTERN_LIB64_DIR)/directfb-1.6-0

directfb: $(DIRECTFB_OUT_DIR)/$(DIRECTFB)/tar
	$(AT)cd $(DIRECTFB_OUT_DIR)/$(DIRECTFB) && make install

directfb64: $(DIRECTFB_OUT_DIR64)/$(DIRECTFB)/tar
	$(AT)cd $(DIRECTFB_OUT_DIR64)/$(DIRECTFB) && make install

$(DIRECTFB_OUT_DIR)/$(DIRECTFB)/tar:
	$(AT)test -d $(INSTALL_DIR) || mkdir -p $(INSTALL_DIR)
	$(AT)test -d $(DIRECTFB_OUT_DIR)/$(DIRECTFB) || tar xf $(OPEN_SOURCE_DIR)/$(DIRECTFB).tar.gz -C $(DIRECTFB_OUT_DIR)
	$(AT)cp -rf patch-for-Directfb/$(DIRECTFB).patch/* $(DIRECTFB_OUT_DIR)/$(DIRECTFB)/
	$(AT)cd $(DIRECTFB_OUT_DIR)/$(DIRECTFB) && sh ./configure --host=$(CFG_HI_ARM_TOOLCHAINS_NAME) --prefix=$(INSTALL_DIR) --build=x86 --$(protype)-multi --enable-freetype --$(comtype)-debug --with-gfxdrivers=tde --with-inputdrivers=keyboard,linuxinput,lirc,ps2mouse --without-setsockopt --without-tools --without-tests --disable-osx --disable-x11 --disable-x11vdpau --disable-mmx --disable-sse --disable-vnc --disable-mesa --disable-devmem  --disable-pnm --disable-imlib2 --disable-mpeg2 --disable-bmp --disable-jpeg2000 --disable-video4linux --disable-linotype --disable-svg CFLAGS="$(CFLAGS)" LIBJPEG="$(LIBJPEG)" LIBPNG_CFLAGS="$(LIBPNG_CFLAGS)" LIBPNG_LIBS="$(LIBPNG_LIBS)" FREETYPE_CFLAGS="$(FREETYPE_CFLAGS)" FREETYPE_LIBS="$(FREETYPE_LIBS)" INCLUDES="$(INCLUDES)" LDFLAGS="$(LDFLAGS)" CPPFLAGS="$(CPPFLAGS)" CXXFLAGS="$(CXXFLAGS)" CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc CPP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-cpp AR=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ar AS=$(CFG_HI_ARM_TOOLCHAINS_NAME)-as LD=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ld NM=$(CFG_HI_ARM_TOOLCHAINS_NAME)-nm STRIP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-strip OBJDUMP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-objdump
	$(AT)touch $@

$(DIRECTFB_OUT_DIR64)/$(DIRECTFB)/tar:
	$(AT)test -d $(INSTALL_DIR64) || mkdir -p $(INSTALL_DIR64)
	$(AT)test -d $(DIRECTFB_OUT_DIR64)/$(DIRECTFB) || tar xf $(OPEN_SOURCE_DIR)/$(DIRECTFB).tar.gz -C $(DIRECTFB_OUT_DIR64)
	$(AT)cp -rf patch-for-Directfb/$(DIRECTFB).patch/* $(DIRECTFB_OUT_DIR64)/$(DIRECTFB)/
	$(AT)cd $(DIRECTFB_OUT_DIR64)/$(DIRECTFB) && sh ./configure --host=$(CFG_HI_AARCH64_TOOLCHAINS_NAME) --prefix=$(INSTALL_DIR64) --build=x86 --$(protype)-multi --enable-freetype --$(comtype)-debug --with-gfxdrivers=tde --with-inputdrivers=keyboard,linuxinput,lirc,ps2mouse --without-setsockopt --without-tools --without-tests --disable-osx --disable-x11 --disable-x11vdpau --disable-mmx --disable-sse --disable-vnc --disable-mesa --disable-devmem  --disable-pnm --disable-imlib2 --disable-mpeg2 --disable-bmp --disable-jpeg2000 --disable-video4linux --disable-linotype --disable-svg CFLAGS="$(CFLAGS)" LIBJPEG="$(LIBJPEG)" LIBPNG_CFLAGS="$(LIBPNG_CFLAGS)" LIBPNG_LIBS="$(LIBPNG_LIBS)" FREETYPE_CFLAGS="$(FREETYPE_CFLAGS)" FREETYPE_LIBS="$(FREETYPE_LIBS)" INCLUDES="$(INCLUDES)" LDFLAGS="$(LDFLAGS64)" CPPFLAGS="$(CPPFLAGS)" CXXFLAGS="$(CXXFLAGS)" CC=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc CPP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-cpp AR=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ar AS=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-as LD=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ld NM=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-nm STRIP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-strip OBJDUMP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-objdump
	$(AT)touch $@

fusion: $(FUSION_OUT_DIR)/$(FUSION)/tar
	$(AT)make -C $(FUSION_OUT_DIR)/$(FUSION) SDK_DIR=$(SDK_DIR) FUSION_DIR=$(FUSION_OUT_DIR)/$(FUSION)

$(FUSION_OUT_DIR)/$(FUSION)/tar:
	$(AT)test -d $(FUSION_OUT_DIR) || mkdir -p $(FUSION_OUT_DIR)
	$(AT)test -d $(FUSION_OUT_DIR)/$(FUSION) || tar xf $(OPEN_SOURCE_DIR)/$(FUSION).tar.gz -C $(FUSION_OUT_DIR)
	$(AT)cp -rf ./patch-for-fusion/$(FUSION).patch/* $(FUSION_OUT_DIR)/$(FUSION)/
	$(AT)cp -rf ./patch-for-fusion/Makefile $(FUSION_OUT_DIR)/$(FUSION)/
	$(AT)cp -rf $(FUSION_OUT_DIR)/$(FUSION)/linux/drivers/char/fusion/debug.h $(FUSION_OUT_DIR)/$(FUSION)/linux/drivers/char/fusion/single
	$(AT)touch $@

fusion_clean:
	$(AT)rm -rf $(FUSION_OUT_DIR)/$(FUSION)
