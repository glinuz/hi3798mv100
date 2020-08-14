#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================
CFLAGS :=   -I$(MSP_DIR)/api/jpeg/grc_cmm_inc               \
            -I$(MSP_DIR)/api/jpeg/inc/inc_test              \
            -I$(MSP_DIR)/drv/jpeg/include                   \
            -I$(MSP_DIR)/api/jpeg/src/src_hard              \
            -I$(MSP_DIR)/api/jpeg/inc/inc_soft              \
            -I$(MSP_DIR)/api/jpeg/inc/inc_hard              \
            -I$(COMMON_DIR)/api/include

CFLAGS += -DCONFIG_HI_SDK_JPEG_VERSION

ifeq ($(CFG_HI_CHIP_TYPE),hi3798cv200)
CFLAGS += -DCONFIG_GFX_MEM_MMZ
else
CFLAGS += -DCONFIG_GFX_MEM_MMZ
endif

ifeq ($(CFG_HI_SMMU_SUPPORT),y)
CFLAGS += -DHI_SMMU_SUPPORT
CFLAGS += -DCONFIG_GFX_MMU_SUPPORT
endif

SAMPLE_IMAGES := sample_dec_file_lines \
    sample_dec_file_phymem \
    sample_dec_file_virmem \
    sample_dec_phymem_lines \
    sample_dec_phymem_phymem \
    sample_dec_phymem_virmem \
    sample_dec_virmem_crop_lines \
    sample_dec_virmem_crop_phymem \
    sample_dec_virmem_crop_virmem \
    sample_dec_virmem_lines \
    sample_dec_virmem_phymem \
    sample_dec_virmem_virmem

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS)

include $(SAMPLE_DIR)/hi_sample_rules.mak