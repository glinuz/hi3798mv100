#########################################################################
## NOTE: This file is included in trustedcore/trustedcore_kernel/Android.mk
#########################################################################

DEVCHIP_TEE_C := $(HI_TEE_API_DIR)/..
DEVCHIP_TEE_H := $(HI_TEE_API_DIR)/..

#########################################################################
# kernel include
#########################################################################
DEVCHIP_KERNEL_INS := \
	-I../platform/devchip/$(PLATFORM_DIR_NAME)/ext/kernel \
	-I$(DEVCHIP_TEE_H)/api/include \
	-I$(DEVCHIP_TEE_H)/drv/include \
	-I$(DEVCHIP_TEE_H)/drv/timer

ifeq ($(strip $(WITH_HISI_CIPHER)), true)
DEVCHIP_KERNEL_INS += \
	-I$(DEVCHIP_TEE_H)/drv/cipher \
	-I$(DEVCHIP_TEE_H)/drv/cipher/rsa \
	-I$(DEVCHIP_TEE_H)/drv/cipher/tee/include
endif

ifeq ($(WITH_HISI_SMMU), true)
DEVCHIP_KERNEL_INS += -I$(DEVCHIP_TEE_H)/drv/mem/hi_smmu/include
endif

ifeq ($(WITH_HISI_SEC_MMZ), true)
DEVCHIP_KERNEL_INS += \
	-I$(DEVCHIP_TEE_H)/drv/mem/sec_mmz/include \
	-I$(DEVCHIP_TEE_H)/drv/mem/sec_mmz
endif

DEVCHIP_DRV_INS :=
#########################################################################
# devchip driver include
#########################################################################
ifeq ($(WITH_HISI_VDP), true)
DEVCHIP_DRV_INS += \
	-I$(DEVCHIP_TEE_H)/drv/vdp \
	-I$(DEVCHIP_TEE_H)/drv/vdp/drv \
	-I$(DEVCHIP_TEE_H)/drv/vdp/hal \
	-I$(DEVCHIP_TEE_H)/drv/vdp/hal/98cv200 \
	-I$(DEVCHIP_TEE_H)/drv/vdp/hal/98cv200/inc
endif

#########################################################################
# hdmi
#########################################################################
ifeq ($(WITH_HISI_SEC_HDMI), true)
DEVCHIP_DRV_INS += \
	-I$(DEVCHIP_TEE_H)/drv/hdmi
endif

#########################################################################
# dmx
#########################################################################
ifeq ($(WITH_HISI_DMX), true)
DEVCHIP_DRV_INS += -I$(DEVCHIP_TEE_H)/drv/demux \
		   -I$(DEVCHIP_TEE_H)/api/demux
endif

#########################################################################
# pvr
#########################################################################
ifeq ($(WITH_HISI_PVR), true)
DEVCHIP_DRV_INS += -I$(DEVCHIP_TEE_H)/api/pvr
endif

#########################################################################
# kernel src
#########################################################################
DEVCHIP_KERNEL_SRCS := \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/kernel/sre_syscallDispatch_chip.o

INCLUDE += \
	$(DEVCHIP_KERNEL_INS) \
	$(DEVCHIP_DRV_INS)
TRUSTEDCORE_SRCS += $(DEVCHIP_KERNEL_SRCS)

#########################################################################
# devchip libs
#########################################################################
PLATFORM_DEVCHIP_STATIC_LIB := libstbdrv
