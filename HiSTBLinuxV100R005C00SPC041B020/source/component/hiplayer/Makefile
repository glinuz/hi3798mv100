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
ifeq ($(CFG_HI_PLAYER_FULL_SUPPORT),y)
ifndef CFG_HI_ADVCA_FUNCTION
HIPLAYER_BASEDIR := full
else
HIPLAYER_BASEDIR := full_advca
endif
endif

HIPLAYER_INC_DIR := include

HIPLAYER_LIB_DIR := lib/$(HIPLAYER_BASEDIR)/$(CFG_HI_ARM_TOOLCHAINS_NAME)
HIPLAYER_LIB64_DIR := lib/$(HIPLAYER_BASEDIR)/$(CFG_HI_AARCH64_TOOLCHAINS_NAME)

DST_INCLUDE_DIR := $(HI_INCLUDE_DIR)

DST_SHARE_LIB_DIR := $(HI_SHARED_LIB_DIR)
DST_STATIC_LIB_DIR := $(HI_STATIC_LIB_DIR)
DST_EXTERN_LIB_DIR := $(HI_EXTERN_LIB_DIR)

DST_SHARE_LIB64_DIR := $(HI_SHARED_LIB64_DIR)
DST_STATIC_LIB64_DIR := $(HI_STATIC_LIB64_DIR)
DST_EXTERN_LIB64_DIR := $(HI_EXTERN_LIB64_DIR)

HEADERS :=  hi_svr_charset_common.h \
            hi_svr_charset.h \
            hi_svr_charset_norm.h \
            hi_svr_assa.h \
            hi_svr_format.h \
            hi_svr_metadata.h \
            hi_svr_player.h \
            hi_svr_player_errno.h \
            hi_svr_codec.h \
            hi_drm_engine.h \
            hi_drm_client.h \
            hi_drm_common.h \
            hi_drm_rights.h \
            hi_drm_constraints.h \
            hi_drm_metadata.h \
            hi_drm_info.h \
            hi_drm_support_info.h

STATIC_LIBS := libplayer.a libsubdec.a libhi_charsetMgr.a libhiplayer_utils.a

SHARE_LIBS  := libplayer.so libsubdec.so libhi_charsetMgr.so libhiplayer_utils.so

EXTERN_LIBS := libformat.so libffmpegformat.so libavformat.so.* libavutil.so.* libavcodec.so.* libbluray.so.1 \
    libHA.AUDIO.FFMPEG_ADEC.decode.so libHA.AUDIO.FFMPEG_WMAPRO.decode.so libHV.VIDEO.FFMPEG_VDEC.decode.so \
    libswscale.so.* libhi_charset.so libformat_open.so

SRC_HEADERS := $(addprefix $(HIPLAYER_INC_DIR)/,$(HEADERS))
DST_HEADERS := $(addprefix $(DST_INCLUDE_DIR)/,$(HEADERS))
###32bit lib ###################################################
SRC_SHARE_LIBS  := $(addprefix $(HIPLAYER_LIB_DIR)/,$(SHARE_LIBS))
DST_SHARE_LIBS  := $(addprefix $(DST_SHARE_LIB_DIR)/,$(SHARE_LIBS))

SRC_STATIC_LIBS := $(addprefix $(HIPLAYER_LIB_DIR)/,$(STATIC_LIBS))
DST_STATIC_LIBS := $(addprefix $(DST_STATIC_LIB_DIR)/,$(STATIC_LIBS))

SRC_EXTERN_LIBS  := $(addprefix $(HIPLAYER_LIB_DIR)/,$(EXTERN_LIBS))
DST_EXTERN_LIBS  := $(addprefix $(DST_EXTERN_LIB_DIR)/,$(EXTERN_LIBS))
####64bit lib ##################################################
SRC_SHARE_LIBS64  := $(addprefix $(HIPLAYER_LIB64_DIR)/,$(SHARE_LIBS))
DST_SHARE_LIBS64  := $(addprefix $(DST_SHARE_LIB64_DIR)/,$(SHARE_LIBS))

SRC_STATIC_LIBS64 := $(addprefix $(HIPLAYER_LIB64_DIR)/,$(STATIC_LIBS))
DST_STATIC_LIBS64 := $(addprefix $(DST_STATIC_LIB64_DIR)/,$(STATIC_LIBS))

SRC_EXTERN_LIBS64  := $(addprefix $(HIPLAYER_LIB64_DIR)/,$(EXTERN_LIBS))
DST_EXTERN_LIBS64  := $(addprefix $(DST_EXTERN_LIB64_DIR)/,$(EXTERN_LIBS))

#===============================================================================
# rules
#===============================================================================
.PHONY : all clean

all:
	$(AT)test -d $(DST_INCLUDE_DIR) || mkdir -p $(DST_INCLUDE_DIR)
	$(AT)cp -f $(SRC_HEADERS) $(DST_INCLUDE_DIR)/
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(DST_STATIC_LIB_DIR) || mkdir -p $(DST_STATIC_LIB_DIR)
	$(AT)cp -f $(SRC_STATIC_LIBS) $(DST_STATIC_LIB_DIR)/
	$(AT)test -d $(DST_SHARE_LIB_DIR) || mkdir -p $(DST_SHARE_LIB_DIR)
	$(AT)cp -f $(SRC_SHARE_LIBS) $(DST_SHARE_LIB_DIR)/
	$(AT)test -d $(DST_EXTERN_LIB_DIR) || mkdir -p $(DST_EXTERN_LIB_DIR)
	$(AT)cp -f $(SRC_EXTERN_LIBS) $(DST_EXTERN_LIB_DIR)/
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(DST_STATIC_LIB64_DIR) || mkdir -p $(DST_STATIC_LIB64_DIR)
	$(AT)cp -f $(SRC_STATIC_LIBS64) $(DST_STATIC_LIB64_DIR)/
	$(AT)test -d $(DST_SHARE_LIB64_DIR) || mkdir -p $(DST_SHARE_LIB64_DIR)
	$(AT)cp -f $(SRC_SHARE_LIBS64) $(DST_SHARE_LIB64_DIR)/
	$(AT)test -d $(DST_EXTERN_LIB64_DIR) || mkdir -p $(DST_EXTERN_LIB64_DIR)
	$(AT)cp -f $(SRC_EXTERN_LIBS64) $(DST_EXTERN_LIB64_DIR)/
endif

clean:
	$(AT)rm -f $(DST_HEADERS)
	$(AT)rm -f $(DST_SHARE_LIBS) $(DST_STATIC_LIBS) $(DST_EXTERN_LIBS)
	$(AT)rm -f $(DST_SHARE_LIBS64) $(DST_STATIC_LIBS64) $(DST_EXTERN_LIBS64)
