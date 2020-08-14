#===============================================================================
#   export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
#   local variables
#===============================================================================
CFLAGS += -I$(MSP_DIR)/api/adec
CFLAGS += -I$(MSP_DIR)/api/vo
CFLAGS += -I$(MSP_DIR)/api/higo/include
CFLAGS += -I$(MSP_DIR)/api/tde/include
CFLAGS += -I$(MSP_DIR)/api/jpge/include
CFLAGS += -I$(MSP_DIR)/api/pvr/include

CFLAGS += -D_GNU_SOURCE -D_XOPEN_SOURCE=600
CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

MSP_OBJ_DIR := $(HI_OUT_DIR)/obj/source/msp/api
MSP_OBJ_DIR64 := $(HI_OUT_DIR)/obj64/source/msp/api

LIB_EXTRACT = $(shell if [ -f $(3) ] ; then \
        mkdir -p $(1) && cd $(1) && $(2)-ar -x $(3); \
    fi)

#--------------- HDMI ------------------
ifeq ($(CFG_HI_HDMI_SUPPORT_1_4),y)
HDMI_VER := hdmi_1_4
else ifeq ($(CFG_HI_HDMI_SUPPORT_2_0),y)
HDMI_VER := hdmi_2_0
endif

CFLAGS +=   -I$(MSP_DIR)/drv/hdmi/$(HDMI_VER) \
            -DCONFIG_HDMI_STB_SDK \
            -DDVI_SUPPORT

ifeq ($(CFG_HI_HDMI_SUPPORT_HDCP),y)
CFLAGS += -DHDMI_HDCP_SUPPORT
endif

ifeq ($(CFG_HI_HDR_SUPPORT),y)
CFLAGS += -DHDMI_HDR_SUPPORT
endif

SRCS := hdmi/$(HDMI_VER)/mpi_hdmi.c hdmi/$(HDMI_VER)/unf_hdmi.c

#--------------- HDMI RX -------------------
ifeq ($(CFG_HI_HDMI_RX_SUPPORT),y)
CFLAGS += -DHI_HDMI_RX_INSIDE

SRCS += hdmirx/mpi_hdmirx.c hdmirx/unf_hdmirx.c
endif

#--------------- GPIO ------------------
SRCS += gpio/unf_gpio.c

#--------------- I2C ------------------
SRCS += i2c/unf_i2c.c

#--------------- VO ------------------
ifeq ($(CFG_HI_HDR_SUPPORT),y)
CFLAGS += -I$(MSP_DIR)/api/vo/dolby/include
CFLAGS += -DHI_VDP_HDR_SUPPORT
endif

SRCS += vo/unf_vo.c \
        vo/unf_display.c \
        vo/unf_edid.c \
        vo/mpi_win.c \
        vo/mpi_disp_tran.c \
        vo/mpi_disp.c \
        vo/win_ratio.c \
        vo/mpi_edid.c

ifeq ($(CFG_HI_VDP_VIDEO_LINEDCMP),y)
CFLAGS += -DHI_VDP_VIDEO_LINEDCMP

SRCS += vo/vo_algbase/src/dcmp_implement.c

VO_DCMP_LIB := $(MSP_DIR)/api/vo/vo_algbase/src/line_dcmp/libdcmp.a

VO_DCMP_OBJS := $(shell $(CFG_HI_ARM_TOOLCHAINS_NAME)-ar -t $(VO_DCMP_LIB))

SRCS += $(addprefix vo/,$(VO_DCMP_OBJS))

$(call LIB_EXTRACT, $(MSP_OBJ_DIR)/vo, $(CFG_HI_ARM_TOOLCHAINS_NAME), $(VO_DCMP_LIB))
endif

#--------------- PDM ------------------
SRCS += pdm/unf_pdm.c pdm/mpi_pdm.c pdm/hi_db.c

#--------------- CIPHER ------------------
SRCS += cipher/unf_cipher.c cipher/mpi_cipher.c

#--------------- ADVCA ------------------
SRCS += advca/unf_advca.c

#--------------- FRONTEND ------------------
ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
SRCS += frontend/unf_tuner.c

ifeq ($(CFG_HI_DISEQC_SUPPORT),y)
CFLAGS += -DDISEQC_SUPPORT

SRCS += frontend/unf_diseqc.c frontend/unf_unicable.c
endif
endif

#--------------- SPI ------------------
ifeq ($(CFG_HI_SPI_BUS_SUPPORT),y)
SRCS += spi/unf_spi.c
endif

#--------------- IR ------------------
ifeq ($(CFG_HI_IR_TYPE_STD),y)
SRCS += ir/unf_ir_std.c
else ifeq ($(CFG_HI_IR_TYPE_S2),y)
SRCS += ir/unf_ir.c
else ifeq ($(CFG_HI_IR_TYPE_LIRC),y)
SRCS += ir/unf_ir_lirc.c
endif

#--------------- DEMUX ------------------
DEMUX_VER := demux_v2

CFLAGS += -I$(MSP_DIR)/drv/demux/$(DEMUX_VER)

SRCS += demux/$(DEMUX_VER)/mpi_demux.c \
        demux/$(DEMUX_VER)/unf_demux.c \
        demux/$(DEMUX_VER)/mpi_descrambler.c \
        demux/$(DEMUX_VER)/unf_descrambler.c

#--------------- TDE ------------------
SRCS += tde/unf_tde.c

#--------------- HWC ------------------
ifeq ($(CFG_HI_GFX2D_SUPPORT),y)
CFLAGS += -I$(MSP_DIR)/api/gfx2d/include
CFLAGS += -DHWC_VERSION=0x400

SRCS += gfx2d/hi_gfx2d_api.c
endif

#--------------- JPGE ------------------
SRCS += jpge/unf_jpge.c

#--------------- OTP ------------------
SRCS += otp/unf_otp.c otp/mpi_otp.c

#--------------- PQ ------------------
SRCS += pq/mpi_pq.c pq/unf_pq.c

ifneq ($(CFG_HI_LOADER_APPLOADER),y)
#--------------- PM ------------------
ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
CFLAGS += -DHI_KEYLED_SUPPORT
endif

SRCS += pm/unf_pmoc.c pm/pmoc_usb_wakeup.c

#--------------- AVPLAY ------------------
ifeq ($(CFG_HI_MSP_BUILDIN),y)
CFLAGS += -DHI_MSP_BUILDIN
endif

ifeq ($(CFG_HI_AVPLAY_PLAY_AND_REC_SUPPORT),y)
CFLAGS += -DHI_AVPLAY_PLAY_AND_REC_SUPPORT
endif

SRCS += avplay/mpi_avplay.c \
        avplay/unf_avplay.c \
        avplay/avplay_frc.c \
        avplay/frame_detect.c \
        avplay/bitstream.c

#--------------- SYNC ------------------
SRCS += sync/mpi_sync.c

#--------------- ADEC ------------------
CFLAGS += -DHI_ADEC_MAX_INSTANCE=$(CFG_HI_ADEC_MAX_INSTANCE)
CFLAGS += -DHI_ADEC_OUTPUTBUF_FRAMENUM=$(CFG_HI_ADEC_OUTPUTBUF_FRAMENUM)

SRCS += adec/mpi_adec.c \
        adec/mpi_adec_core.c \
        adec/mpi_adec_imple.c

ifeq ($(CFG_HI_ADEC_AUDSPECTRUM_SUPPORT),y)
CFLAGS += -DHI_ADEC_AUDSPECTRUM_SUPPORT
SRCS += adec/mpi_adec_waveform.c
endif

ifdef CFG_HI_ADVCA_FUNCTION
ADEC_IMPLE_LIBNAME := libhi_adecimple_advca.a
else
ADEC_IMPLE_LIBNAME := libhi_adecimple.a
endif

ifeq ($(HI_USER_SPACE_LIB),y)
ADEC_IMPLE_LIB := $(MSP_DIR)/api/adec/lib/$(CFG_HI_ARM_TOOLCHAINS_NAME)/$(ADEC_IMPLE_LIBNAME)

$(call LIB_EXTRACT, $(MSP_OBJ_DIR)/adec, $(CFG_HI_ARM_TOOLCHAINS_NAME), $(ADEC_IMPLE_LIB))
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
ADEC_IMPLE64_LIB := $(MSP_DIR)/api/adec/lib/$(CFG_HI_AARCH64_TOOLCHAINS_NAME)/$(ADEC_IMPLE_LIBNAME)

$(call LIB_EXTRACT, $(MSP_OBJ_DIR64)/adec, $(CFG_HI_AARCH64_TOOLCHAINS_NAME), $(ADEC_IMPLE64_LIB))
endif

#--------------- AO ------------------
CFLAGS += -I$(MSP_DIR)/drv/adsp/adsp_v1_1/include

ifeq ($(CFG_HI_AI_SUPPORT),y)
CFLAGS += -DHI_SOUND_AI_SUPPORT
endif

ifeq ($(CFG_HI_SND_SPDIF_DELAY_SUPPORT),y)
CFLAGS += -DHI_SOUND_SPDIF_COMPENSATION_SUPPORT
endif

SRCS += ao/unf_sound.c \
        ao/mpi_ao.c \
        ao/mpi_vir.c \
        ao/mpi_lowlatency.c

#--------------- VDEC ------------------
ifeq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a hi3796mv200),)
VDEC_VER := vdec_v1.0
VFMW_VER := vfmw_v4.0
else
VDEC_VER := vdec_v2.0
VFMW_VER := vfmw_v5.0
endif

CFLAGS +=   -I$(MSP_DIR)/drv/vfmw/$(VFMW_VER)  \
            -I$(MSP_DIR)/drv/vdec/$(VDEC_VER)  \
            -I$(MSP_DIR)/api/jpeg/inc/inc_soft \
            -I$(MSP_DIR)/api/jpeg/inc/inc_hard

ifeq ($(CFG_HI_SMMU_SUPPORT),y)
CFLAGS += -DVFMW_MMU_SUPPORT
endif

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
# Hi3716MV410 or Hi3716MV420 config start
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
ifeq ($(CFG_HI_VIDEO_PRE_CAP_1080P),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_MVC),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC=0
endif

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
# Hi3716MV410 or Hi3716MV420 config end
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

#===============================================================================
# 256M ddr memory config flags
#===============================================================================
ifeq ($(CFG_HI_VIDEO_PRE_CAP_1080P_IN_256),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_256=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_256=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_MVC_IN_256),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_256=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_256=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_2160P_IN_256),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_256=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_256=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_4K_IN_256),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_256=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_256=0
endif

#===============================================================================
# 512M ddr memory config flags
#===============================================================================
ifeq ($(CFG_HI_VIDEO_PRE_CAP_1080P_IN_512),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_512=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_512=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_MVC_IN_512),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_512=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_512=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_2160P_IN_512),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_512=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_512=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_4K_IN_512),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_512=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_512=0
endif

#===============================================================================
# 768M ddr memory config flags
#===============================================================================
ifeq ($(CFG_HI_VIDEO_PRE_CAP_1080P_IN_768),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_768=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_768=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_MVC_IN_768),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_768=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_768=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_2160P_IN_768),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_768=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_768=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_4K_IN_768),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_768=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_768=0
endif

#===============================================================================
# 1024M ddr memory config flags
#===============================================================================
ifeq ($(CFG_HI_VIDEO_PRE_CAP_1080P_IN_1024),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_1024=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_1024=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_MVC_IN_1024),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_1024=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_1024=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_2160P_IN_1024),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_1024=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_1024=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_4K_IN_1024),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_1024=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_1024=0
endif

#===============================================================================
# 2048M ddr memory config flags
#===============================================================================
ifeq ($(CFG_HI_VIDEO_PRE_CAP_1080P_IN_2048),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_2048=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_2048=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_MVC_IN_2048),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_2048=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_2048=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_2160P_IN_2048),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_2048=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_2048=0
endif

ifeq ($(CFG_HI_VIDEO_PRE_CAP_4K_IN_2048),y)
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_2048=1
else
CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_2048=0
endif

#===============================================================================
# memory config end
#===============================================================================
ifeq ($(CFG_HI_VDEC_MJPEG_SUPPORT),y)
CFLAGS += -DHI_VDEC_MJPEG_SUPPORT=1
else
CFLAGS += -DHI_VDEC_MJPEG_SUPPORT=0
endif

ifeq ($(CFG_HI_VDEC_REG_CODEC_SUPPORT),y)
CFLAGS += -DHI_VDEC_REG_CODEC_SUPPORT=1
else
CFLAGS += -DHI_VDEC_REG_CODEC_SUPPORT=0
endif

ifeq ($(CFG_HI_VDEC_USERDATA_CC_SUPPORT),y)
CFLAGS += -DHI_VDEC_USERDATA_CC_SUPPORT=1

CFLAGS += -DHI_VDEC_USERDATA_CC_BUFSIZE=$(CFG_HI_VDEC_USERDATA_CC_BUFSIZE)
else
CFLAGS += -DHI_VDEC_USERDATA_CC_SUPPORT=0
endif

ifeq ($(CFG_HI_VDEC_DFS_SUPPORT),y)
CFLAGS += -DHI_VDEC_DFS_SUPPORT=1
else
CFLAGS += -DHI_VDEC_DFS_SUPPORT=0
endif

ifeq ($(CFG_HI_TEE_SUPPORT),y)
CFLAGS += -DHI_TEE_SUPPORT
endif

SRCS += vdec/$(VDEC_VER)/hi_codec.c \
        vdec/$(VDEC_VER)/mpi_vdec_adapter.c \
        vdec/$(VDEC_VER)/mpi_vdec.c

ifeq ($(CFG_HI_VDEC_MJPEG_SUPPORT),y)
SRCS += vdec/$(VDEC_VER)/mpi_vdec_mjpeg.c
endif

#--------------- WDG ------------------
ifeq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
SRCS += wdg/unf_wdg.c
endif

#--------------- MCE ------------------
SRCS += mce/unf_mce.c
endif

#--------------- PVR ------------------
ifeq ($(CFG_HI_PVR_SUPPORT),y)
ifeq ($(CFG_HI_PVR_EXTRA_BUF_SUPPORT),y)
CFLAGS += -DHI_PVR_EXTRA_BUF_SUPPORT
CFLAGS += -DHI_PVR_FIFO_DIO
endif

ifeq ($(CFG_HI_TEE_SUPPORT),y)
CFLAGS += -I$(TEE_API_INCLUDE)
endif

SRCS += pvr/hi_pvr_fifo.c \
        pvr/hi_pvr_index.c \
        pvr/hi_pvr_intf.c \
        pvr/hi_pvr_play_ctrl.c \
        pvr/hi_pvr_rec_ctrl.c \
        pvr/hi_pvr_index_v0.c \
        pvr/hi_pvr_index_v1.c \
        pvr/unf_pvr.c \
        pvr/hi_pvr_smooth_ctrl.c

ifeq ($(CFG_HI_TEE_SUPPORT),y)
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a hi3796mv200),)
CFLAGS += -DHI_TEE_PVR_SUPPORT
SRCS += pvr/hi_pvr_tee.c
endif
endif

ifdef CFG_HI_ADVCA_FUNCTION
PVR_SMOOTH_LIBNAME := libhi_pvrsmooth_advca.a
else
PVR_SMOOTH_LIBNAME := libhi_pvrsmooth.a
endif

ifeq ($(HI_USER_SPACE_LIB),y)
PVR_SMOOTH_LIB := $(MSP_DIR)/api/pvr/lib/$(CFG_HI_ARM_TOOLCHAINS_NAME)/$(PVR_SMOOTH_LIBNAME)

$(call LIB_EXTRACT, $(MSP_OBJ_DIR)/pvr, $(CFG_HI_ARM_TOOLCHAINS_NAME), $(PVR_SMOOTH_LIB))
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
PVR_SMOOTH64_LIB := $(MSP_DIR)/api/pvr/lib/$(CFG_HI_AARCH64_TOOLCHAINS_NAME)/$(PVR_SMOOTH_LIBNAME)

$(call LIB_EXTRACT, $(MSP_OBJ_DIR64)/pvr, $(CFG_HI_AARCH64_TOOLCHAINS_NAME), $(PVR_SMOOTH64_LIB))
endif
endif

#--------------- AI ------------------
ifeq ($(CFG_HI_AI_SUPPORT),y)
SRCS += ai/unf_ai.c ai/mpi_ai.c
endif

#--------------- KEYLED ------------------
ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
SRCS += keyled/unf_keyled.c
endif

#--------------- AENC ------------------
ifeq ($(CFG_HI_AENC_SUPPORT),y)
ifeq ($(CFG_HI_AI_SUPPORT),y)
CFLAGS += -DHI_AENC_AI_SUPPORT
endif

SRCS += aenc/unf_aenc.c aenc/mpi_aenc.c aenc/mpi_aenc_core.c aenc/mpi_aenc_lsrc.c
endif

#--------------- CI ------------------
ifeq ($(CFG_HI_CIPLUS_SUPPORT),y)
SRCS += ci/unf_ci.c ci/mpi_ci.c
endif

#--------------- HDCP ------------------
ifeq ($(CFG_HI_HDMI_SUPPORT_HDCP),y)
SRCS += hdcp/unf_hdcp.c
endif

#--------------- SCI ------------------
ifeq ($(CFG_HI_SCI_SUPPORT),y)
SRCS += sci/unf_sci.c
endif

#--------------- VI ------------------
ifeq ($(CFG_HI_VI_SUPPORT),y)
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
CFLAGS += -DHI_VI_SD_CREATE_WINDOW
endif

SRCS += vi/mpi_vi.c vi/unf_vi.c
endif

#--------------- VENC ------------------
ifeq ($(CFG_HI_VENC_SUPPORT),y)
ifneq ($(findstring $(CFG_HI_CHIP_TYPE),  hi3798mv200 hi3798mv200_a),)
CFLAGS += -DHI_VENC_HEVC_SUPPORT 
endif
SRCS += venc/mpi_venc.c venc/unf_venc.c
endif

#--------------- PWM ------------------
ifeq ($(CFG_HI_PWM_SUPPORT),y)
SRCS += pwm/unf_pwm.c
endif
#-------------------------------------

LIB_NAME := libhi_msp

HEADER_FILES := $(wildcard $(MSP_UNF_INCLUDE)/*.h)          \
                $(MSP_DIR)/api/tde/include/hi_tde_api.h     \
                $(MSP_DIR)/api/tde/include/hi_tde_errcode.h \
                $(MSP_DIR)/api/tde/include/hi_tde_type.h    \
                $(MSP_DIR)/drv/hifb/include/hifb.h

include $(SDK_DIR)/hi_rules.mak
