#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#       variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
objects := freetype

ifeq ($(CFG_HI_AUDIO_RESAMPLER_SUPPORT),y)
objects += resampler
endif

ifeq ($(CFG_HI_HACODEC_SUPPORT),y)
objects += ha_codec
endif

ifeq ($(CFG_HI_ZLIB_SUPPORT),y)
objects += zlib
endif

ifeq ($(CFG_HI_BLUETOOTH_SUPPORT),y)
objects += bluetooth
endif

ifeq ($(CFG_HI_ALSA_SUPPORT),y)
objects += alsa
endif

ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
objects += advca
endif

ifeq ($(CFG_HI_CURL_SUPPORT),y)
objects += curl
endif

ifeq ($(CFG_HI_CAPTION_SUBT_SUPPORT),y)
objects += subtitle
endif

ifeq ($(CFG_HI_CAPTION_SO_SUPPORT),y)
objects += subtoutput
endif

ifeq ($(CFG_HI_CAPTION_TTX_SUPPORT),y)
objects += teletext
endif

ifeq ($(CFG_HI_CAPTION_CC_SUPPORT),y)
objects += cc
endif

ifeq ($(CFG_HI_WIFI_SUPPORT),y)
objects += wifi
endif

ifeq ($(CFG_HI_3G_SUPPORT),y)
objects += 3g
endif

ifeq ($(CFG_HI_FAT_SUPPORT),y)
objects += hifat
endif

ifeq ($(CFG_HI_NTFS_SUPPORT),y)
objects += ntfs
endif

ifeq ($(CFG_HI_VP_SUPPORT),y)
objects += vp
endif

ifeq ($(CFG_HI_EXFAT_SUPPORT),y)
objects += exfat
endif

ifeq ($(CFG_HI_PLAYREADY_SUPPORT),y)
objects += playready
endif

ifeq ($(CFG_HI_OPENSSL_SUPPORT),y)
objects += openssl
endif

ifeq ($(CFG_HI_WIDEVINE_SUPPORT),y)
objects += widevine
endif

ifeq ($(CFG_HI_OPERA_SUPPORT),y)
objects += operabase
endif

objects_clean:=$(addsuffix _clean,$(objects))
objects_install:=$(addsuffix _install,$(objects))
objects_uninstall:=$(addsuffix _uninstall,$(objects))

depends += $(objects)

ifeq ($(CFG_HI_LOADER_SUPPORT),y)
depends += loader
endif

ifeq ($(CFG_HI_PLAYER_SUPPORT),y)
depends += hiplayer
endif

ifeq ($(CFG_GSTREAMER_FULL_SUPPORT),y)
depends += gstreamer
endif

ifeq ($(CFG_HI_KARAOKE_SUPPORT),y)
depends += karaoke
endif

depends_clean       := $(addsuffix _clean,$(depends))

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#==================================================================
#                          all
#==================================================================
.PHONY: all clean

all: $(depends)

clean: $(depends_clean)

#==================================================================
#                          modules
#==================================================================
.PHONY: $(depends) $(objects_clean)

$(objects):
	$(AT)make -C $@ all

$(objects_clean):
	$(AT)make -C $(patsubst %_clean,%,$@) clean

loader: freetype $(if $(findstring $(CFG_HI_CURL_SUPPORT),y),curl,)
	$(AT)make -C loader all

loader_clean:
	$(AT)make -C loader clean

hiplayer:
	$(AT)test ! -d hiplayer || make -C hiplayer all

hiplayer_clean:
	$(AT)test ! -d hiplayer || make -C hiplayer clean

gstreamer:
	$(AT)test ! -d gstreamer || make -C gstreamer all

gstreamer_clean:
	$(AT)test ! -d gstreamer || make -C gstreamer clean

karaoke: alsa
	$(AT)test -d karaoke && make -C karaoke all

karaoke_clean:
	$(AT)test -d karaoke && make -C karaoke clean
