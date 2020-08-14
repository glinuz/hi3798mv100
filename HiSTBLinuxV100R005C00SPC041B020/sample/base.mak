#=============DIR======================================================================

#=============TOOL CHAINS===================================================================
AR=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ar
AS=$(CFG_HI_ARM_TOOLCHAINS_NAME)-as
LD=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ld
CPP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-cpp
CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc
NM=$(CFG_HI_ARM_TOOLCHAINS_NAME)-nm
STRIP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-strip
OBJCOPY=$(CFG_HI_ARM_TOOLCHAINS_NAME)-objcopy
OBJDUMP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-objdump

#==============COMPILE OPTIONS================================================================
CFG_HI_SAMPLE_CFLAGS := -Werror -Wall

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
CFG_HI_SAMPLE_CFLAGS += -mcpu=cortex-a7
else ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a),)
CFG_HI_SAMPLE_CFLAGS += -mcpu=cortex-a53
else
CFG_HI_SAMPLE_CFLAGS += -mcpu=cortex-a9
endif

CFG_HI_SAMPLE_CFLAGS+= -D_GNU_SOURCE -Wall -O2 -ffunction-sections -fdata-sections -Wl,--gc-sections
CFG_HI_SAMPLE_CFLAGS+= -DCHIP_TYPE_$(CFG_HI_CHIP_TYPE) -DCFG_HI_SDK_VERSION=$(CFG_HI_SDK_VERSION)
CFG_HI_SAMPLE_CFLAGS+= $(CFG_HI_BOARD_CONFIGS)

ifeq ($(CFG_HI_HDMI_RX_SUPPORT),y)
CFG_HI_SAMPLE_CFLAGS += -DHI_HDMI_RX_INSIDE
endif
ifeq ($(CFG_HI_ADVCA_FUNCTION),FINAL)
    CFG_HI_SAMPLE_CFLAGS += -DHI_ADVCA_FUNCTION_RELEASE
else
    CFG_HI_SAMPLE_CFLAGS += -DHI_ADVCA_FUNCTION_$(CFG_HI_ADVCA_FUNCTION)
endif


SYS_LIBS := -lpthread -lrt -lm -ldl -lstdc++

HI_LIBS := -lhi_common -lpng -lhigo -lhigoadp -lfreetype -ljpeg

HI_LIBS += -lhi_msp

ifeq ($(CFG_HI_TEE_SUPPORT),y)
HI_LIBS += -lteec -lc_sec
endif

ifeq ($(CFG_HI_AUDIO_RESAMPLER_SUPPORT),y)
HI_LIBS += -lhi_resampler
endif

ifeq ($(CFG_HI_HAEFFECT_BASE_SUPPORT),y)
HI_LIBS += -lhi_aef_base
endif

ifeq ($(CFG_HI_HAEFFECT_SRS_SUPPORT),y)
HI_LIBS += -lhi_aef_srs
endif

ifeq ($(CFG_HI_ZLIB_SUPPORT),y)
HI_LIBS += -lz
endif

ifeq ($(CFG_HI_CAPTION_SUBT_SUPPORT),y)
HI_LIBS += -lhi_subtitle
endif

ifeq ($(CFG_HI_CAPTION_SO_SUPPORT),y)
HI_LIBS += -lhi_so
endif

ifeq ($(CFG_HI_CAPTION_TTX_SUPPORT),y)
HI_LIBS += -lhi_ttx
endif

ifeq ($(CFG_HI_CAPTION_CC_SUPPORT),y)
HI_LIBS += -lhi_cc
endif

ifeq ($(CFG_HI_VP_SUPPORT),y)
HI_LIBS += -lhi_vp -lrtp
endif

COMMON_SRCS :=  hi_adp_demux.c \
                hi_adp_data.c \
                hi_adp_hdmi.c \
                hi_adp_mpi.c \
                hi_adp_search.c \
                hi_filter.c

ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
COMMON_SRCS += hi_adp_tuner.c
endif

ifeq ($(CFG_HI_PVR_SUPPORT),y)
COMMON_SRCS += hi_adp_pvr.c
endif


AT := @
