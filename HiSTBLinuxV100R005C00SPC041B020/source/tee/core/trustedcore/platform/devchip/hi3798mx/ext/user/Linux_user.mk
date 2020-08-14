#########################################################################
## NOTE: This file is included in trustedcore/TEE_ext/Android.mk
#########################################################################

DEVCHIP_TEE_C := $(HI_TEE_API_DIR)/..
DEVCHIP_TEE_H := $(HI_TEE_API_DIR)/..

DEVCHIP_TEE_INS := \
	-I../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user \
	-I../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/asm_mem \
	-I../platform/devchip/$(PLATFORM_DIR_NAME)/ext/kernel \
	-I$(DEVCHIP_TEE_H)/api/mem/include \
	-I$(DEVCHIP_TEE_H)/api/include \
	-I$(DEVCHIP_TEE_H)/drv/include

DEVCHIP_TEE_SRCS := \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/sre_syscalls_chip.o \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/tee_symbol_chip.o \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/tee_config_chip.o

#########################################################################
## asm_memmove
#########################################################################		
DEVCHIP_ASM_MEM_SRCS := \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/tee_external_api.o \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/asm_mem/memcpy.o \
	../platform/devchip/$(PLATFORM_DIR_NAME)/ext/user/asm_mem/memmove.o

DEVCHIP_TEE_SRCS += $(DEVCHIP_ASM_MEM_SRCS)

#########################################################################
## cipher
#########################################################################
ifeq ($(strip $(WITH_HISI_CIPHER)), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/cipher/hi_tee_cipher.o
endif

#########################################################################
## mmz
#########################################################################
ifeq ($(WITH_HISI_SEC_MMZ), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/mem/hi_tee_mmz.o
endif

#########################################################################
## smmu
#########################################################################
ifeq ($(WITH_HISI_SMMU), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/mem/hi_tee_mem.o 
endif

#########################################################################
# vdp
#########################################################################
ifeq ($(WITH_HISI_VDP), true)
DEVCHIP_TEE_INS += -I$(DEVCHIP_TEE_H)/drv/vdp
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/vdp/hi_tee_vdp.o 
endif

#########################################################################
# hdmi
#########################################################################
ifeq ($(WITH_HISI_SEC_HDMI), true)
DEVCHIP_TEE_INS += -I$(DEVCHIP_TEE_H)/api/hdmi
DEVCHIP_TEE_SRCS += \
	$(DEVCHIP_TEE_C)/api/hdmi/hi_tee_hdmi.o \
	$(DEVCHIP_TEE_C)/api/hdmi/tee_hdmi.o
endif

#########################################################################
# dmx
#########################################################################
ifeq ($(WITH_HISI_DMX), true)
DEVCHIP_TEE_INS += -I$(DEVCHIP_TEE_H)/api/demux
DEVCHIP_TEE_SRCS += \
	$(DEVCHIP_TEE_C)/api/demux/tee_descrambler.o \
	$(DEVCHIP_TEE_C)/api/demux/tee_scrambler.o \
	$(DEVCHIP_TEE_C)/api/demux/tee_demux_utils.o
endif

#########################################################################
# otp
#########################################################################
ifeq ($(WITH_HISI_OTP), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/otp/hi_tee_otp.o 
endif

#########################################################################
# advca
#########################################################################
ifeq ($(WITH_HISI_ADVCA), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/advca/hi_tee_advca.o 
endif
ifeq ($(strip $(WITH_STB_BEIDOU)), true)
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/beidou/tee_beidou.o
endif

#########################################################################
# pvr
#########################################################################
ifeq ($(WITH_HISI_PVR), true)
DEVCHIP_TEE_INS += -I$(DEVCHIP_TEE_C)/api/pvr
DEVCHIP_TEE_SRCS += $(DEVCHIP_TEE_C)/api/pvr/tee_pvr_utils.o
endif

INCLUDE += $(DEVCHIP_TEE_INS)
LIB_SRCS += $(DEVCHIP_TEE_SRCS)
