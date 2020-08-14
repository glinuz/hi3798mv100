#########################################################################
## NOTE: This file is included in trustedcore/trustedcore_kernel/Android.mk
#########################################################################

DEVCHIP_TEE_C := ../platform/devchip/$(PLATFORM_DIR_NAME)/tee
DEVCHIP_TEE_H := ${LOCAL_PATH}/../platform/devchip/$(PLATFORM_DIR_NAME)/tee

#########################################################################
# kernel include
#########################################################################
DEVCHIP_KERNEL_INS := \
	${LOCAL_PATH}/../platform/devchip/$(PLATFORM_DIR_NAME)/ext/kernel \
	$(DEVCHIP_TEE_H)/api/include \
	$(DEVCHIP_TEE_H)/drv/include \
	$(DEVCHIP_TEE_H)/drv/timer

ifeq ($(strip $(WITH_HISI_CIPHER)), true)
DEVCHIP_KERNEL_INS += \
	$(DEVCHIP_TEE_H)/drv/cipher \
	$(DEVCHIP_TEE_H)/drv/cipher/rsa \
	$(DEVCHIP_TEE_H)/drv/cipher/tee/include
endif

ifeq ($(WITH_HISI_SMMU), true)
DEVCHIP_KERNEL_INS += $(DEVCHIP_TEE_H)/drv/mem/hi_smmu/include
endif

ifeq ($(WITH_HISI_SEC_MMZ), true)
DEVCHIP_KERNEL_INS += \
	$(DEVCHIP_TEE_H)/drv/mem/sec_mmz/include \
	$(DEVCHIP_TEE_H)/drv/mem/sec_mmz
endif

DEVCHIP_DRV_INS :=
#########################################################################
# devchip driver include
#########################################################################
ifeq ($(WITH_HISI_VDP), true)
DEVCHIP_DRV_INS += \
	$(DEVCHIP_TEE_H)/drv/vdp \
	$(DEVCHIP_TEE_H)/drv/vdp/drv \
	$(DEVCHIP_TEE_H)/drv/vdp/hal \
	$(DEVCHIP_TEE_H)/drv/vdp/hal/98cv200 \
	$(DEVCHIP_TEE_H)/drv/vdp/hal/98cv200/inc
endif

#########################################################################
# hdmi
#########################################################################
ifeq ($(WITH_HISI_SEC_HDMI), true)
DEVCHIP_DRV_INS += \
	$(DEVCHIP_TEE_H)/drv/hdmi
endif

#########################################################################
# dmx
#########################################################################
ifeq ($(WITH_HISI_DMX), true)
DEVCHIP_DRV_INS += $(DEVCHIP_TEE_H)/drv/demux
endif

#########################################################################
# kernel src
#########################################################################
DEVCHIP_KERNEL_SRCS := \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/kernel/sre_syscallDispatch_chip.c

INCLUDE += \
	$(DEVCHIP_KERNEL_INS) \
	$(DEVCHIP_DRV_INS)
TRUSTEDCORE_SRCS += $(DEVCHIP_KERNEL_SRCS)

#########################################################################
# devchip libs
#########################################################################
PLATFORM_DEVCHIP_STATIC_LIB := libstbdrv
