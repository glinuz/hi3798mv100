#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
#ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716cv200 hi3716mv400 hi3718cv100 hi3719cv100 hi3718mv100 hi3719mv100),)
#EXTRA_CFLAGS += -DAIAO_ALSA_DRV_V1
#ALSA_DIR = alsa/dsp_mix
#endif

#ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3751v100 hi3798cv100 hi3796cv100 hi3798mv100 hi3796mv100 hi3716dv100 hi3798cv200_a hi3798cv200_b),)
EXTRA_CFLAGS += -DAIAO_ALSA_DRV_V2
EXTRA_CFLAGS += -DHI_ALSA_HRTIMER_SUPPORT
ALSA_DIR = alsa/dsp_mix_v2
#endif

ADSP_DRV_INCLUDE := $(MSP_DIR)/drv/adsp/adsp_v1_1/include

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/api/higo/include           \
                -I$(ADSP_DRV_INCLUDE)                   \
                -I$(CURDIR)

EXTRA_CFLAGS += -I$(MSP_DIR)/drv/aiao/aiao_v1_1/timer

#COMMON include
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/common/include
#AO include
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/ao
#AI include
ifeq ($(CFG_HI_AI_SUPPORT),y)
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/ai
EXTRA_CFLAGS += -DHI_AUDIO_AI_SUPPORT
endif
#ALSA include
ifeq ($(CFG_HI_SND_ALSA_AO_SUPPORT),y)
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/$(ALSA_DIR)
EXTRA_CFLAGS += -DHI_ALSA_AO_SUPPORT
endif

ifeq ($(CFG_HI_SND_ALSA_I2S_ONLY_SUPPORT),y)
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/$(ALSA_DIR)
endif

ifeq ($(CFG_HI_SND_ALSA_HDMI_ONLY_SUPPORT),y)
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/$(ALSA_DIR)
endif
ifeq ($(CFG_HI_SND_MUTECTL_SUPPORT),y)
EXTRA_CFLAGS += -DHI_SND_MUTECTL_SUPPORT
EXTRA_CFLAGS += -DHI_SND_MUTECTL_GPIO=$(CFG_HI_SND_MUTECTL_GPIO)
EXTRA_CFLAGS += -DHI_SND_MUTECTL_LEVEL=$(CFG_HI_SND_MUTECTL_LEVEL)
endif

ifeq ($(CFG_HI_SND_AMP_SUPPORT),y)
#EXTRA_CFLAGS += -DHI_SND_AMP_SUPPORT
endif

ifeq ($(CFG_HI_SND_AOE_HW_SUPPORT),y)
EXTRA_CFLAGS += -DHI_SND_AOE_HW_SUPPORT
endif

#===============================================================================
# HI_SND_DRV_TEMPLATE_OPT : stantard drvier template
#===============================================================================
EXTRA_CFLAGS += -DHI_SND_DRV_TEMPLATE_OPT

ifeq ($(CFG_HI_SND_SPDIF_DELAY_SUPPORT),y)
EXTRA_CFLAGS += -DHI_SOUND_SPDIF_COMPENSATION_SUPPORT
endif

#===============================================================================
# HI_SND_DRV_SUSPEND_SUPPORT : suspend & resume driver
#===============================================================================
EXTRA_CFLAGS += -DHI_SND_DRV_SUSPEND_SUPPORT

#EXTRA_CFLAGS += -DHI_SND_FPGA

ifeq ($(CFG_HI_SND_HBRA_PASSTHROUGH_SUPPORT),y)
EXTRA_CFLAGS += -DHI_SND_HBRA_PASSTHROUGH_SUPPORT
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a),)
EXTRA_CFLAGS += -DHI_SND_HDMI_I2S_SPDIF_MUX
endif

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

#EXTRA_CFLAGS += -DHIAO_DEBUG

MOD_NAME := hi_aiao

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y := drv_aiao_module.o

#COMMON SUPPORT
$(MOD_NAME)-y += common/src/audio_util.o \
		 common/src/hal_aiao.o \
		 common/src/hal_aiao_func.o \
		 common/src/hal_aiao_priv.o

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3751v100 hi3796cv100 hi3798cv100),)
$(MOD_NAME)-y += common/src/hal_tianlai_adac_v510.o
EXTRA_CFLAGS += -DHI_TIANLAI_V510
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716cv200 hi3716mv400 hi3718cv100 hi3719cv100 hi3718mv100 hi3719mv100 hi3798mv100 hi3796mv100 hi3716dv100 hi3716mv410 hi3716mv420),)
$(MOD_NAME)-y += common/src/hal_tianlai_adac_v500.o
EXTRA_CFLAGS += -DHI_TIANLAI_V500
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200_a hi3798cv200_b hi3798cv200 hi3798mv200 hi3796mv200),)
$(MOD_NAME)-y += common/src/hal_tianlai_adac_v600.o
EXTRA_CFLAGS += -DHI_TIANLAI_V600
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798mv200_a)
$(MOD_NAME)-y += common/src/hal_tianlai_adac_v700.o
EXTRA_CFLAGS += -DHI_TIANLAI_V700
endif

ifeq ($(CFG_HI_AIAO_VERIFICATION_SUPPORT),y)
EXTRA_CFLAGS += -DHI_AIAO_VERIFICATION_SUPPORT
$(MOD_NAME)-y += common/src/drv_aiao_veri.o
endif

#AO SUPPORT (default)
$(MOD_NAME)-y += ao/drv_ao.o \
		 ao/drv_ao_op.o \
		 ao/drv_ao_track.o \
		 ao/hal_aoe_func.o \
		 ao/hal_aoe.o \
		 ao/drv_ao_intf.o

ifeq ($(CFG_HI_SND_AFLT_SUPPORT),y)
EXTRA_CFLAGS += -DHI_SND_AFLT_SUPPORT
$(MOD_NAME)-y += ao/drv_ao_aef.o
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100 hi3798cv200_a hi3798cv200_b hi3798cv200 hi3716mv410 hi3716mv420 hi3798mv200 hi3798mv200_a),)
EXTRA_CFLAGS += -DHI_AIAO_TIMER_SUPPORT
$(MOD_NAME)-y +=timer/drv_timer.o
endif

#AI SUPPORT(optional)
ifeq ($(CFG_HI_AI_SUPPORT),y)
$(MOD_NAME)-y += ai/drv_ai_intf.o\
                 ai/drv_ai.o
endif

#ALSA SUPPORT(optional)
ifeq ($(CFG_HI_SND_ALSA_AO_SUPPORT),y)
#ALSA drvier V2
$(MOD_NAME)-y += $(ALSA_DIR)/hiaiao_card.o \
		 $(ALSA_DIR)/hiaiao_dma.o \
		 $(ALSA_DIR)/hiaiao_codec.o \
		 $(ALSA_DIR)/hiaiao_i2s.o \
		 $(ALSA_DIR)/alsa_aiao_proc_func.o \
		 $(ALSA_DIR)/drv_ao_func.o \
		 $(ALSA_DIR)/alsa_aiao_volume_func.o
endif

ifeq ($(CFG_HI_SND_ALSA_AI_SUPPORT),y)
$(MOD_NAME)-y +=$(ALSA_DIR)/drv_ai_func.o
endif
ifeq ($(CFG_HI_ALSA_TVL320_KERNEL_SUPPORT),y)
$(MOD_NAME)-y +=$(ALSA_DIR)/alsa_i2c.o    \
		$(ALSA_DIR)/aic31.o
endif

EXTRA_CFLAGS += -DHI_SND_CAST_SUPPORT

$(MOD_NAME)-y += ao/drv_ao_cast.o \
		 ao/hal_cast.o

ifeq ($(CFG_HI_SND_ALSA_I2S_ONLY_SUPPORT),y)
$(MOD_NAME)-y += alsa/i2s_only/hiaiao_hw_card.o \
		 alsa/i2s_only/hiaiao_hw_dma.o \
		 alsa/i2s_only/hiaiao_hw_codec.o \
		 alsa/i2s_only/hiaiao_hw_i2s.o \
		 alsa/i2s_only/alsa_aiao_hw_proc_func.o \
		 alsa/i2s_only/drv_ao_hw_func.o
endif
ifeq ($(CFG_HI_SND_ALSA_HDMI_ONLY_SUPPORT),y)
$(MOD_NAME)-y += alsa/hdmi_only/hiaiao_hdmi_card.o \
		 alsa/hdmi_only/hiaiao_hdmi_dma.o \
		 alsa/hdmi_only/hiaiao_hdmi_codec.o \
		 alsa/hdmi_only/hiaiao_hdmi_i2s.o \
		 alsa/hdmi_only/alsa_aiao_hdmi_proc_func.o \
		 alsa/hdmi_only/drv_ao_hdmi_func.o \
		 alsa/hdmi_only/hdmi_alsa_aiao_volume_func.o
endif
#I2S SUPPORT
ifneq ($(CFG_HI_CHIP_TYPE),hi3751v100)
EXTRA_CFLAGS += -DHI_I2S0_SUPPORT
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716cv200 hi3716mv400 hi3718cv100 hi3719cv100 hi3796cv100 hi3798cv100 hi3751v100),)
EXTRA_CFLAGS += -DHI_I2S1_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_DOUT0_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_SND_OUTPUTPORT_I2S0_SUPPORT
endif

#############sound dont support I2S1
#ifeq ($(CFG_HI_I2S1_DOUT0_SUPPORT),y)
#EXTRA_CFLAGS += -DHI_UNF_SND_OUTPUTPORT_I2S1_SUPPORT
#endif

ifeq ($(CFG_HI_I2S0_DIN0_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_AI_I2S0_SUPPORT
endif
ifeq ($(CFG_HI_I2S1_DIN0_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_AI_I2S1_SUPPORT
endif
ifeq ($(CFG_HI_SND_ALSA_AI_SUPPORT),y)
EXTRA_CFLAGS += -DHI_ALSA_AI_SUPPORT
endif
ifeq ($(CFG_HI_SND_ALSA_I2S_ONLY_SUPPORT),y)
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/alsa/i2s_only
EXTRA_CFLAGS += -DHI_ALSA_I2S_ONLY_SUPPORT
endif
ifeq ($(CFG_HI_SND_ALSA_HDMI_ONLY_SUPPORT),y)
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/alsa/hdmi_only
EXTRA_CFLAGS += -DHI_ALSA_HDMI_ONLY_SUPPORT
endif
ifeq ($(CFG_HI_ALSA_TVL320_KERNEL_SUPPORT),y)
EXTRA_CFLAGS += -DHI_ALSA_TLV320_SUPPORT
endif

#===============================================================================
# For Pinshare Set
#===============================================================================
ifeq ($(CFG_HI_I2S0_MCLK_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_MCLK_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_BCLK_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_BCLK_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_WS_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_WS_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_DOUT0_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_DOUT0_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_DOUT1_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_DOUT1_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_DOUT2_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_DOUT2_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_DOUT3_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_DOUT3_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_IN0_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_IN0_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_IN1_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_IN1_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_IN2_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_IN2_SUPPORT
endif

ifeq ($(CFG_HI_I2S0_IN3_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S0_IN3_SUPPORT
endif


ifeq ($(CFG_HI_I2S1_MCLK_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_MCLK_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_BCLK_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_BCLK_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_WS_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_WS_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_DOUT0_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_DOUT0_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_DOUT1_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_DOUT1_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_DOUT2_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_DOUT2_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_DOUT3_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_DOUT3_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_IN0_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_IN0_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_IN1_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_IN1_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_IN2_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_IN2_SUPPORT
endif

ifeq ($(CFG_HI_I2S1_IN3_SUPPORT),y)
EXTRA_CFLAGS += -DHI_UNF_I2S1_IN3_SUPPORT
endif

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=$(CURDIR) modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	$(AT)cp -f $(CURDIR)/$(MOD_NAME).ko $(HI_MODULE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/$(MOD_NAME).ko
