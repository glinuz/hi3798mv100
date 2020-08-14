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
MP3DEC_DIR = $(MSP_DIR)/drv/adec/adec_v1_1/mp3dec

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)
EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MP3DEC_DIR)                         \
                -I$(MP3DEC_DIR)/mp3dec_core/src              \
                -I$(MP3DEC_DIR)/mp3dec_core/include          \
                -I$(MP3DEC_DIR)/mp3dec_core/layer12          \
                -I$(MP3DEC_DIR)/src                     \
                -I$(MP3DEC_DIR)/include

EXTRA_CFLAGS += -DHI_ADEC_MAX_INSTANCE=$(CFG_HI_ADEC_MAX_INSTANCE)
EXTRA_CFLAGS += -DMPEG_LAYER12_SUPPORT
KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers
EXTRA_CFLAGS += -DINTERLEAVED_OUTPUT
ifeq (y,$(CFG_HI_MCE_SUPPORT))
EXTRA_CFLAGS += -DHI_MCE_SUPPORT
endif

HI_DRV_DIR := $(MSP_DIR)/drv

MOD_NAME := hi_adec

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y   :=  drv_adec.o drv_adec_k.o

ifeq ($(CFG_HI_MCE_SUPPORT),y)
$(MOD_NAME)-y +=  \
        ./mp3dec/src/mp3dec_wrap.o \
        ./mp3dec/mp3dec_core/src/pvmp3_normalize.o \
        ./mp3dec/mp3dec_core/src/pvmp3_alias_reduction.o \
        ./mp3dec/mp3dec_core/src/pvmp3_crc.o \
        ./mp3dec/mp3dec_core/src/pvmp3_decode_header.o \
        ./mp3dec/mp3dec_core/src/pvmp3_decode_huff_cw.o \
        ./mp3dec/mp3dec_core/src/pvmp3_getbits.o \
        ./mp3dec/mp3dec_core/src/pvmp3_dequantize_sample.o \
        ./mp3dec/mp3dec_core/src/pvmp3_framedecoder.o \
        ./mp3dec/mp3dec_core/src/pvmp3_get_main_data_size.o \
        ./mp3dec/mp3dec_core/src/pvmp3_get_side_info.o \
        ./mp3dec/mp3dec_core/src/pvmp3_get_scale_factors.o \
        ./mp3dec/mp3dec_core/src/pvmp3_mpeg2_get_scale_data.o \
        ./mp3dec/mp3dec_core/src/pvmp3_mpeg2_get_scale_factors.o \
        ./mp3dec/mp3dec_core/src/pvmp3_mpeg2_stereo_proc.o \
        ./mp3dec/mp3dec_core/src/pvmp3_huffman_decoding.o \
        ./mp3dec/mp3dec_core/src/pvmp3_huffman_parsing.o \
        ./mp3dec/mp3dec_core/src/pvmp3_tables.o \
        ./mp3dec/mp3dec_core/src/pvmp3_imdct_synth.o \
        ./mp3dec/mp3dec_core/src/pvmp3_mdct_6.o \
        ./mp3dec/mp3dec_core/src/pvmp3_dct_6.o \
        ./mp3dec/mp3dec_core/src/pvmp3_poly_phase_synthesis.o \
        ./mp3dec/mp3dec_core/src/pvmp3_equalizer.o \
        ./mp3dec/mp3dec_core/src/pvmp3_seek_synch.o \
        ./mp3dec/mp3dec_core/src/pvmp3_stereo_proc.o \
        ./mp3dec/mp3dec_core/src/pvmp3_reorder.o \
        ./mp3dec/mp3dec_core/layer12/layer12_frame.o \
        ./mp3dec/ha_mp3dec_wrap.o \
        ./mp3dec/mp3dec_core/src/pvmp3_polyphase_filter_window.o \
        ./mp3dec/mp3dec_core/src/pvmp3_mdct_18.o \
        ./mp3dec/mp3dec_core/src/pvmp3_dct_9.o \
        ./mp3dec/mp3dec_core/src/pvmp3_dct_16.o


        # ./mp3dec/mp3dec_core/src/asm/pvmp3_polyphase_filter_window_gcc.o \
        # ./mp3dec/mp3dec_core/src/asm/pvmp3_mdct_18_gcc.o \
        # ./mp3dec/mp3dec_core/src/asm/pvmp3_dct_9_gcc.o \
        # ./mp3dec/mp3dec_core/src/asm/pvmp3_dct_16_gcc.o

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

