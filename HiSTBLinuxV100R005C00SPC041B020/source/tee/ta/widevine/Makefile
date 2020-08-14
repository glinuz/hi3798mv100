
###########################################################
# platform definition
###########################################################
ROOT      =.
LOCAL_CFLAGS =
LOCAL_ASFLAGS =
LOCAL_CFLAGS += ${TRUSTEDCORE_LOCAL_CFLAGS}
LOCAL_ASFLAGS += ${TRUSTEDCORE_LOCAL_ASFLAGS}

LOCAL_CFLAGS += -O2 -fstrict-aliasing -funswitch-loops -fno-short-enums -W -Wall -mfloat-abi=softfp -mfpu=neon -fno-omit-frame-pointer -fstack-protector-all -Wstack-protector -MD 
ifeq ($(strip $(WITH_STB_PLATFORM)), true)
LOCAL_CFLAGS += -DSTB=1
else
LOCAL_CFLAGS += -DDPT=1
endif

widevine_PATH=task_modular_wv_N

ifeq ($(strip $(WITH_HISI_SMMU)), true)
LOCAL_C_INCLUDES += \
    -I$(HI_TEE_OS_DIR)/trustedcore/include/TEE \
    -I$(HI_TEE_OS_DIR)/trustedcore/include/kernel \
    -I$(HI_TEE_OS_DIR)/trustedcore/platform/${PLATFORM_DIR_NAME}/TEE_ext \
    -I$(HI_TEE_API_DIR)/include \
    -I$(ROOT)/../../../../source/common/include \
    -I$(ROOT)/../../../../source/common/api/include \
    -I$(ROOT)/../../../../source/msp/include
else
LOCAL_C_INCLUDES += \
    -I$(ROOT)/../../include \
    -I$(HI_TEE_OS_DIR)/trustedcore/platform/${PLATFORM_DIR_NAME}/trustedcore_kernel/syscalls_ext \
    -I$(ROOT)/../../include_kernel \
    -I$(ROOT)/../../include_kernel/config \
    -I$(ROOT)/../../include_kernel/ph \
    -I$(ROOT)/../../TEE_ext/api/include \
    -I$(ROOT)/../../thirdparty/cipher_code/ \
    -I$(ROOT)/../../../../../../source/common/include \
    -I$(ROOT)/../../../../../../source/msp/include
endif

LOCAL_C_INCLUDES += \
    -I$(ROOT)/provision \
    -I$(ROOT)/$(widevine_PATH)/crypto \
    -I$(ROOT)/$(widevine_PATH)/ \
    -I$(ROOT)/task_widevine/ \
    -I$(ROOT)/task_widevine/hal  \
    -I$(ROOT)/hal/include \
    -I$(ROOT)/


ifeq ($(strip $(WITH_HISI_SMMU)), true)
LOCAL_C_INCLUDES += \
    -I$(ROOT)/hal/stb/tee
else
LOCAL_C_INCLUDES += \
    -I$(ROOT)/hal/stb/tvp
endif


LOCAL_SRC_FILES :=  $(ROOT)/task_widevine_tzserver.o \
			$(ROOT)/task_widevine/task_widevine.o \
			$(ROOT)/task_widevine/task_wv_tzserver.o \
			$(ROOT)/task_widevine/widevine_hal.o \
			$(ROOT)/task_widevine/hal/hi_cipher.o \
			$(ROOT)/task_widevine/hal/wvcrc.o \
			$(ROOT)/$(widevine_PATH)/task_modular_wv_tzserver.o \
			$(ROOT)/$(widevine_PATH)/hi_crypto.o \
			$(ROOT)/$(widevine_PATH)/hi_oem_engine_mock.o \
			$(ROOT)/$(widevine_PATH)/hi_oem_keybox_mock.o \
			$(ROOT)/$(widevine_PATH)/hi_oem_key_mock.o \
			$(ROOT)/$(widevine_PATH)/hi_oem_mock.o \
			$(ROOT)/$(widevine_PATH)/hi_oem_usage_table_mock.o \
			$(ROOT)/$(widevine_PATH)/hi_oem_wvcrc.o \
			$(ROOT)/$(widevine_PATH)/hi_wv_util.o \
			$(ROOT)/$(widevine_PATH)/crypto/aes.o \
			$(ROOT)/$(widevine_PATH)/crypto/asn1parse.o \
			$(ROOT)/$(widevine_PATH)/crypto/bignum.o \
			$(ROOT)/$(widevine_PATH)/crypto/cmac.o \
			$(ROOT)/$(widevine_PATH)/crypto/md.o \
			$(ROOT)/$(widevine_PATH)/crypto/md_wrap.o \
			$(ROOT)/$(widevine_PATH)/crypto/oid.o \
			$(ROOT)/$(widevine_PATH)/crypto/pkparse.o \
			$(ROOT)/$(widevine_PATH)/crypto/rsa.o \
			$(ROOT)/$(widevine_PATH)/crypto/sha1.o \
			$(ROOT)/$(widevine_PATH)/crypto/sha256.o \
			$(ROOT)/provision/widevine_provision.o \
			$(ROOT)/provision/tea_decrypt.o
# $(ROOT)/sec_mmz_sample.o

####drmutil###
LOCAL_C_INCLUDES += \
    -I$(ROOT)/../drmutil/hal/include \
    -I$(ROOT)/../drmutil/hal/stb/tee

LOCAL_SRC_FILES += \
    $(ROOT)/../drmutil/hal/stb/tee/hi_haldrmcipher.o \
    $(ROOT)/../drmutil/hal/stb/tee/hi_haldrmotp.o \
    $(ROOT)/../drmutil/hal/dpt/hi_haldrmsmmu.o \
    $(ROOT)/../drmutil/hal/dpt/hi_haldrmutil.o


ifeq ($(strip $(WITH_HISI_SMMU)), true)
LOCAL_SRC_FILES += \
    $(ROOT)/hal/stb/tee/hi_halwvcipher.o \
    $(ROOT)/hal/stb/tee/hi_halwvotp.o
else
LOCAL_SRC_FILES += \
    $(ROOT)/hal/stb/tvp/hi_halwvcipher.o \
    $(ROOT)/hal/stb/tvp/hi_halwvotp.o
endif

LOCAL_MODULE:= $(OUT)/task_wv_server.elf


ifeq ($(strip $(WITH_HISI_SMMU)), true)
TRUSTEDCORE_ELF_MAIN_ENTRY_PATH := $(TRUSTEDCORE_DIR)/release/internal_tasks/elf_main_entry.o
else
TRUSTEDCORE_ELF_MAIN_ENTRY_PATH :=$(ROOT)/../elf_main_entry.o
endif

ALL_OBJ = $(LOCAL_SRC_FILES)
LOCAL_OUT = $(OUT)/internal_tasks/$(notdir $(CURDIR))
ALL_OUT_OBJ = $(foreach VAR,$(ALL_OBJ),$(addprefix $(LOCAL_OUT)/,$(VAR)))

CFLAGS += $(LOCAL_CFLAGS) $(LOCAL_C_INCLUDES) $(TRUSTEDCORE_LOCAL_CFLAGS)
LDFLAGS += -r -d --strip-debug --whole-archive  -T $(TA_LINK_SCRIPT) $(TRUSTEDCORE_ELF_MAIN_ENTRY_PATH) --no-whole-archive
ifeq ($(strip $(WITH_HISI_SMMU)), true)
CFLAGS += -DHI_SMMU_SUPPORT
endif

##########################################################
# build demo_task1
##########################################################
.PHONY : all prepare clean
ifeq ($(strip $(CONFIG_TRUSTEDCORE_RELEASE)), )
all: $(ALL_OBJ)
	@echo $(ALL_OUT_OBJ)
	$(LD1) $(LDFLAGS) -o $(LOCAL_MODULE) $(ALL_OUT_OBJ) `$(CC1) -mthumb-interwork -print-libgcc-file-name`
	@echo "Make $(LOCAL_MODULE) Success!"
	@echo $(WITH_HISI_SMMU)
else
all:
	cp $(TRUSTEDCORE_RELEASE_DIR)/$(HI_CHIP_TYPE)/$(CFG_HI_TVP_MEM_LAYOUT)/$(notdir $(LOCAL_MODULE)) $(LOCAL_MODULE)
	@echo "Make $(LOCAL_MODULE) Success!"
endif
	
prepare:
	$(MKDIR) $(foreach VAR,$(ALL_OUT_OBJ),$(dir $(VAR)))

$(ALL_OBJ): prepare

clean:
	$(RM) $(LOCAL_MODULE) $(ALL_OUT_OBJ) $(ALL_OBJ)

%.o : %.c | prepare
	$(CC1) -c $(CFLAGS) $< -o $(LOCAL_OUT)/$@

