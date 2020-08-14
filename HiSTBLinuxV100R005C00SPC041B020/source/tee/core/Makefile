#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ifeq ($(CFG_HI_CHIP_TYPE), hi3751v620)
export PLATFORM_DIR_NAME := hi3751vx
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200_a hi3798mv200),)
export PLATFORM_DIR_NAME := hi3798cv2x
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
export PLATFORM_DIR_NAME := hi3798mx
endif

export HI_CHIP_TYPE := $(CFG_HI_CHIP_TYPE)
export HI_TEE_VFMW_SUPPORT := $(CFG_HI_TEE_VFMW_SUPPORT)
export HI_TEE_ADVCA_SUPPORT := $(CFG_HI_TEE_ADVCA_SUPPORT)
export HI_TEE_PVR_SUPPORT := $(CFG_HI_TEE_PVR_SUPPORT)
export HI_TEE_DMX_SUPPORT := $(CFG_HI_TEE_DEMUX_SUPPORT)
export HI_TEE_HDMI_SUPPORT := $(CFG_HI_TEE_HDMI_SUPPORT)
export HI_TEE_DEMO_SUPPORT := $(CFG_HI_TEE_DEMO_SUPPORT)
export HI_TEE_VDP_SUPPORT := $(CFG_HI_TEE_VDP_SUPPORT)
export HI_TEE_BEIDOU_SUPPORT := $(CFG_HI_TEE_BEIDOU_SUPPORT)
export HI_TEE_I2C_SUPPORT := $(CFG_HI_TEE_I2C_SUPPORT)
export HI_TEE_WIDEVINE_SUPPORT := $(CFG_HI_TEE_WIDEVINE_SUPPORT)
export HI_TEE_PLAYREADY_SUPPORT := $(CFG_HI_TEE_PLAYREADY_SUPPORT)
export HI_TEE_VMX_ULTRA_SUPPORT := $(CFG_HI_TEE_VMX_ULTRA_SUPPORT)
export HI_TEE_VMXTAC_TEST_SUPPORT := $(CFG_HI_TEE_VMXTAC_TEST_SUPPORT)
export HI_TEE_ITAC_TEST_SUPPORT := $(CFG_HI_TEE_ITAC_TEST_SUPPORT)
export HI_TEE_AVPLAY_SUPPORT := $(CFG_HI_TEE_AVPLAY_SUPPORT)

ifeq ("$(origin O)", "command line")
OUTPUT_DIR := $(O)
else
OUTPUT_DIR := $(HI_OUT_DIR)/obj/source/tee/core/trustedcore
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: all clean trustedcore teec
ifeq ($(O),)
all: trustedcore teec
else
all: trustedcore
endif

clean:
	$(AT)rm -rf $(OUTPUT_DIR) $(HI_IMAGE_DIR)/trustedcore.img $(HI_ROOTFS_DIR)/usr/sbin/teecd

trustedcore:
	$(AT)make -C trustedcore all HI_SDK_DIR=$(SDK_DIR) O=$(OUTPUT_DIR)
ifeq ($(CFG_PRODUCT_TYPE),linux)
	$(AT)test -d $(HI_IMAGE_DIR) || mkdir -p $(HI_IMAGE_DIR)
	$(AT)cp $(OUTPUT_DIR)/trustedcore.img $(HI_IMAGE_DIR)
endif

teec:
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)make -C libteec all CROSS_COMPILE=$(CFG_HI_ARM_TOOLCHAINS_NAME)-
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)make -C libteec all CROSS_COMPILE=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-
endif
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_ROOTFS_DIR)/usr/sbin/ || mkdir -p $(HI_ROOTFS_DIR)/usr/sbin
	$(AT)cp $(HI_OUT_DIR)/obj/source/tee/core/libteec/teecd $(HI_ROOTFS_DIR)/usr/sbin/
else ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_ROOTFS_DIR)/usr/sbin/ || mkdir -p $(HI_ROOTFS_DIR)/usr/sbin
	$(AT)cp $(HI_OUT_DIR)/obj64/source/tee/core/libteec/teecd $(HI_ROOTFS_DIR)/usr/sbin/
endif
