#########################################################################
# NOTE: This file is included by trustedcore/TEE_ext/Android.mk
#       Use to build user API for TA.
#########################################################################

DEVCHIP_TEE_C := ../platform/devchip/$(PLATFORM_DIR_NAME)/tee
DEVCHIP_TEE_H := ${LOCAL_PATH}/../platform/devchip/$(PLATFORM_DIR_NAME)/tee

DEVCHIP_TEE_INS := \
	${LOCAL_PATH}/../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user \
	${LOCAL_PATH}/../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/asm_mem \
	${LOCAL_PATH}/../platform/devchip/$(PLATFORM_DIR_NAME)/ext/kernel \
	$(DEVCHIP_TEE_H)/api/mem/include \
	$(DEVCHIP_TEE_H)/api/include \
	$(DEVCHIP_TEE_H)/drv/include \
	$(DEVCHIP_TEE_H)/drv/timer

DEVCHIP_TEE_SRCS := \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/sre_syscalls_chip.S \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/tee_symbol_chip.c

#########################################################################
# asm_memmove
#########################################################################		
DEVCHIP_ASM_MEM_SRCS := \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/tee_external_api.c \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/asm_mem/memcpy.S \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/asm_mem/memmove.S

DEVCHIP_TEE_SRCS += $(DEVCHIP_ASM_MEM_SRCS)

#########################################################################
# cipher
#########################################################################
ifeq ($(strip $(WITH_HISI_CIPHER)), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/cipher/hi_tee_cipher.c
endif

#########################################################################
# mmz
#########################################################################
ifeq ($(WITH_HISI_SEC_MMZ), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/mem/hi_tee_mmz.c 
endif

#########################################################################
# smmu
#########################################################################
ifeq ($(WITH_HISI_SMMU), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/mem/hi_tee_mem.c 
endif

#########################################################################
# vdp
#########################################################################
ifeq ($(WITH_HISI_VDP), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/vdp/hi_tee_vdp.c 
endif

#########################################################################
# hdmi
#########################################################################
ifeq ($(WITH_HISI_SEC_HDMI), true)
DEVCHIP_TEE_SRCS += \
	$(DEVCHIP_TEE_C)/api/hdmi/hi_tee_hdmi.c \
	$(DEVCHIP_TEE_C)/api/hdmi/hi_tee_drv_hdmi.c \
	$(DEVCHIP_TEE_C)/api/monitor/hi_tee_monitor.c \
	$(DEVCHIP_TEE_C)/api/display/hi_tee_display.c
endif

#########################################################################
# dmx
#########################################################################
ifeq ($(WITH_HISI_DMX), true)
DEVCHIP_TEE_SRCS += \
	$(DEVCHIP_TEE_C)/api/demux/tee_descrambler.c \
	$(DEVCHIP_TEE_C)/api/demux/tee_demux_utils.c
endif

#########################################################################
# otp
#########################################################################
ifeq ($(WITH_HISI_OTP), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/otp/hi_tee_otp.c 
endif

#########################################################################
# advca
#########################################################################
ifeq ($(WITH_HISI_ADVCA), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/advca/hi_tee_advca.c 
endif

INCLUDE += $(DEVCHIP_TEE_INS)
LIB_SRCS += $(DEVCHIP_TEE_SRCS)
