#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
ifneq ($(OUT),)
LOCAL_OUT := $(OUT)/drv/cipher
else
LOCAL_OUT := $(shell pwd)
endif

LIBS    := libsec_$(notdir $(LOCAL_OUT)).a
OBJS    := \
        tee_drv_cipher.o \
        tee_drv_cipher_intf.o \
        tee_hal_cipher.o \
        rsa/tee_drv_rsa.o \
        rsa/tee_drv_rsa_bignum.o \
        rsa/tee_drv_rsa_intf.o \
        tee/src/crys_aes.o \
        tee/src/crys_hmac.o \
        tee/src/crys_rsa.o \
        tee/src/crys_rsa_prim.o \
        tee/src/crys_hash.o \
        tee/src/crys_des.o \
        tee/src/crys_rsa_kg.o \
        tee/src/crys_rnd_rom.o \
        tee/src/crys_common_math.o \
        tee/src/crys_common_conv_endian.o \
        tee/src/crys_cipher_common.o \
        tee/src/crys_rsa_build.o \
				tee/src/crys_stub.o

CFLAGS += -I$(HI_TEE_DRV_DIR)/cipher
CFLAGS += -I$(HI_TEE_DRV_DIR)/cipher/rsa
CFLAGS += -I$(HI_TEE_DRV_DIR)/cipher/tee/include
CFLAGS += -I$(TRUSTEDCORE_DIR)/platform/devchip/$(PLATFORM_DIR_NAME)/pm
CFLAGS += \
	-I$(HI_TEE_DRV_DIR)/cipher/tee/include/ccdriver/shared/include \
	-I$(HI_TEE_DRV_DIR)/cipher/tee/include/ccdriver/shared/include/crys \
	-I$(HI_TEE_DRV_DIR)/cipher/tee/include/ccdriver/shared/include/pal \
	-I$(HI_TEE_DRV_DIR)/cipher/tee/include/ccdriver/shared/include/pal/secure_os \
	-I$(HI_TEE_DRV_DIR)/cipher/tee/include/ccdriver/shared/include/dx_util


ALL_OUT_OBJ = $(foreach VAR,$(OBJS),$(addprefix $(LOCAL_OUT)/,$(VAR)))
ALL_OUT_LIBS = $(foreach VAR,$(LIBS),$(addprefix $(LOCAL_OUT)/,$(VAR)))

#===============================================================================
# rules
#===============================================================================
include $(HI_TEE_DRV_DIR)/stb_drv.mak
