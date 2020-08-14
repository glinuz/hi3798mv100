#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/..
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================
CFLAGS += $(CFG_HI_SAMPLE_CFLAGS)
CFLAGS += -I$(HI_INCLUDE_DIR) -I$(SAMPLE_DIR)/common

objects :=  esplay ao audiocast tsplay es_ts_switch ipplay mosaic pip log memdev userproc cgms \
            mce capture fb higo tde jpeg \
            gpio pmoc ir e2prom i2c \
            otp flash uart network fs cipher snapshot 3dtv pq test/hdmi_tsplay test/video_test omxvdec

ifeq ($(CFG_HI_DIRECTFB_SUPPORT),y)
objects += directfb
endif

ifeq ($(CFG_HI_PWM_SUPPORT),y)
objects += pwm
endif

ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
objects += tuner demux dvbplay fullband iframe_dec factory_detect test/dolby test/sync_test
endif

ifeq ($(CFG_HI_PVR_SUPPORT),y)
ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
objects += pvr
endif
endif

ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
objects += keyled
endif

ifeq ($(CFG_HI_CIPLUS_SUPPORT),y)
objects += ci
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_HDCP),y)
objects += hdcpkey
endif

ifeq ($(CFG_HI_AENC_SUPPORT),y)
objects += aenc
endif

ifeq ($(CFG_HI_AI_SUPPORT),y)
objects += ai
endif

ifeq ($(CFG_HI_KARAOKE_SUPPORT),y)
objects += karaoke
endif

ifeq ($(CFG_HI_AUDIO_RESAMPLER_SUPPORT),y)
objects += resampler
endif

ifeq ($(CFG_HI_AUDIO_OMX_SUPPORT),y)
objects += omxadec
objects += omxaenc
endif

ifeq ($(CFG_HI_LOADER_SUPPORT),y)
objects += loader
endif

ifeq ($(CFG_HI_WIFI_SUPPORT),y)
objects += wifi
endif

ifeq ($(CFG_HI_3G_SUPPORT),y)
objects += 3g
endif

ifeq ($(CFG_HI_GPU_SUPPORT),y)
objects += gpu
endif

ifeq ($(CFG_HI_SCI_SUPPORT),y)
objects += sci
endif

ifeq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
objects += wdg
endif

ifeq ($(CFG_HI_AENC_SUPPORT),y)
ifeq ($(CFG_HI_VENC_SUPPORT),y)
objects += transcode
objects += avcast
endif
endif

ifeq ($(CFG_HI_VI_SUPPORT),y)
CFLAGS += -I$(MSP_DIR)/drv/include
ifeq ($(CFG_HI_VENC_SUPPORT),y)
ifneq ($(CFG_HI_VO_SINGLE_VIDEO_SUPPORT),y)
objects += vi_venc
ifeq ($(CFG_HI_VP_SUPPORT),y)
objects += vp
endif
endif
endif
ifeq ($(CFG_HI_AI_SUPPORT),y)
objects += hdmi_rx
endif
endif

ifeq ($(CFG_HI_VENC_SUPPORT),y)
objects += venc
endif

ifeq ($(CFG_HI_PLAYER_SUPPORT),y)
objects += localplay
endif

ifeq ($(CFG_HI_CAPTION_CC_SUPPORT),y)
objects += cc
endif

ifeq ($(CFG_HI_CAPTION_TTX_SUPPORT),y)
objects += teletext
endif

ifeq ($(CFG_HI_CAPTION_SUBT_SUPPORT),y)
objects += subtitle
endif

ifeq ($(CFG_HI_OPENSSL_SUPPORT),y)
objects += openssl
endif

ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
objects += advca
endif

ifeq ($(CFG_HI_DLNA_SUPPORT),y)
objects += hidlna
endif

ifeq ($(CFG_HI_BLUETOOTH_SUPPORT),y)
objects += castbluetooth
endif

ifeq ($(CFG_HI_TRANSCODER_SUPPORT),y)
objects += hitranscoder
endif

ifeq ($(CFG_HI_PLAYREADY_SUPPORT),y)
objects += playready
endif

ifeq ($(CFG_HI_ADVCA_SANDBOX_TYPE),LXC)
objects +=  lxc_ipc
endif

ifeq ($(CFG_HI_TEE_SUPPORT),y)
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a),)
objects += tee
endif
endif

objects_clean := $(addsuffix _clean, $(objects))

SAMPLE_RES := $(HI_OUT_DIR)/obj/sample
SAMPLE_RES64 := $(HI_OUT_DIR)/obj64/sample

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean $(objects) $(objects_clean) prepare sample_common sample_common_clean

all: $(objects)

clean: $(objects_clean)

$(objects): sample_common
	$(AT)make -C $@

$(objects_clean): sample_common_clean
	$(AT)make -C $(patsubst %_clean, %, $@) clean

sample_common: prepare
	$(AT)make -C common

sample_common_clean:
	$(AT)make -C common clean

prepare:
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(SAMPLE_RES) || mkdir -p $(SAMPLE_RES)
	$(AT)test -d $(SAMPLE_RES)/res || cp -rf $(SAMPLE_DIR)/res $(SAMPLE_RES)/
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(SAMPLE_RES64) || mkdir -p $(SAMPLE_RES64)
	$(AT)test -d $(SAMPLE_RES64)/res || cp -rf $(SAMPLE_DIR)/res $(SAMPLE_RES64)/
endif
