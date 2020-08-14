
###########################################################
# platform definition
###########################################################
ROOT      =.

#LOCAL_CFLAGS += -fstrict-aliasing -funswitch-loops -fno-short-enums -W -Wall -mfloat-abi=softfp -mfpu=neon -fno-omit-frame-pointer -fstack-protector-all -Wstack-protector -MD 
LOCAL_CFLAGS += -W -Wall -fno-omit-frame-pointer -fno-short-enums

ifeq ($(strip $(WITH_HISI_SEC_HDMI)), true)
#LOCAL_CFLAGS  += -DWITH_PLAYREADY_SEC_HDMI
endif

PLAYREADY_COMPILELIB_USER_CFLAGS = -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3-d16

PLAYREADY_SOURCE_DIR= $(ROOT)
PLAYREADY_LIB_PRIVATE_SOURCE_DIR   := $(PLAYREADY_SOURCE_DIR)/src/PK30

ifeq ($(strip $(CONFIG_TRUSTEDCORE_RELEASE)), )
include $(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/playready_3_0.mk
endif

VPATH = $(TARGET_TEE_LIBS)
VPATH += $(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/oem/linux/lib
VPATH += $(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/modules/bcertformat/builder
VPATH += $(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/trustedexec/teeproxystub/stub/prndtx/stub
#VPATH += $(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/oem/common/trustedexec/version
C_DEFINES+= -D_PLAYREADY_M_IX86 $(_DRM_C_DEFINES)

###############################include *h LOCAL_C_INCLUDES ##########################################
CFG_INC =    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)                                 \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/blackbox                    \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/certs                       \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/inc                         \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/keyfile                     \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/keyfileformat               \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/modules/prnd/real           \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/modules/prnd/internal       \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/modules/secure_clock/real   \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/results/checkedin           \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/results                     \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/ndtinc                      \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/oem/ansi/inc                \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/oem/common/inc              \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/oem/common/hal/oemimpl      \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/oem/common/hal/shared       \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/oem/common/trustedexec      \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/tools/shared/common         \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/tools/shared/netio          \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/trustedexec/base            \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/trustedexec/prndinternal    \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/trustedexec/inc             \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/xmr                         \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/modules/contentspecific/h264/real 	\
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/modules/m2tsencryptor/real	\
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/trustedexec/teeproxystub/common    \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/trustedexec/debug \
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/trustedexec/teeproxystub/stub/common	\
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/trustedexec/aes128ctr	\
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/securecore/trustedexec/base	\
    -I$(PLAYREADY_LIB_PRIVATE_SOURCE_DIR)/trustedexec/teeproxystub/proxy/common \
    -I$(HI_TEE_OS_DIR)/trustedcore/include/TEE \
    -I$(HI_TEE_OS_DIR)/trustedcore/include/kernel \
    -I$(HI_TEE_OS_DIR)/trustedcore/platform/${PLATFORM_DIR_NAME}/TEE_ext \
    -I$(ROOT)/../../api/include \
    -I$(ROOT)/../../../../source/common/include \
    -I$(ROOT)/../../../../source/common/api/include \
    -I$(ROOT)/../../../../source/msp/include \
    -I$(HI_TEE_API_DIR)/mem/include\
    -I$(ROOT)/

PLAYREADY_PRIV_SRC=$(shell find $(VPATH) -maxdepth 1 -name "*.c")
PLAYREADY_PRIV_OBJS_TEMP := $(PLAYREADY_PRIV_SRC:%.c=%.o)
PLAYREADY_PRIV_OBJS := $(foreach VAR,$(PLAYREADY_PRIV_OBJS_TEMP),$(subst $(PLAYREADY_SOURCE_DIR),$(ROOT),$(VAR)))

LOCAL_SRC_FILES :=  $(ROOT)/task_playready.o
LOCAL_SRC_FILES +=  $(ROOT)/hi_pr_crypto.o
LOCAL_SRC_FILES +=  $(ROOT)/hi_pr_provision.o
LOCAL_SRC_FILES +=  $(ROOT)/hi_pr_ta.o
LOCAL_SRC_FILES +=  $(ROOT)/hi_pr_util.o
LOCAL_SRC_FILES +=  $(PLAYREADY_PRIV_OBJS)


####drmutil###
ifeq ($(strip $(WITH_HPT_PLATFORM)), true)
CFG_INC += \
    -I$(ROOT)/../drmutil/hal/include \
    -I$(ROOT)/../drmutil/hal/dpt

LOCAL_SRC_FILES += \
    $(ROOT)/../drmutil/hal/dpt/hi_haldrmcipher.o \
    $(ROOT)/../drmutil/hal/dpt/hi_haldrmotp.o \
    $(ROOT)/../drmutil/hal/dpt/hi_haldrmsmmu.o \
    $(ROOT)/../drmutil/hal/dpt/hi_haldrmutil.o

else ifeq ($(strip $(WITH_STB_PLATFORM)), true)
CFG_INC += \
    -I$(ROOT)/../drmutil/hal/include \
    -I$(ROOT)/../drmutil/hal/stb/tee

LOCAL_SRC_FILES += \
    $(ROOT)/../drmutil/hal/stb/tee/hi_haldrmcipher.o \
    $(ROOT)/../drmutil/hal/stb/tee/hi_haldrmotp.o \
    $(ROOT)/../drmutil/hal/dpt/hi_haldrmsmmu.o \
    $(ROOT)/../drmutil/hal/dpt/hi_haldrmutil.o

else
CFG_INC += \
    -I$(ROOT)/../drmutil/hal/include \
    -I$(ROOT)/../drmutil/hal/stb/tvp

LOCAL_SRC_FILES += \
    $(ROOT)/../drmutil/hal/stb/tvp/hi_haldrmcipher.o \
    $(ROOT)/../drmutil/hal/stb/tvp/hi_haldrmotp.o
endif

##############

# $(ROOT)/sec_mmz_sample.o

LOCAL_MODULE:= $(OUT)/task_playready.elf



TRUSTEDCORE_ELF_MAIN_ENTRY_PATH := $(TRUSTEDCORE_DIR)/release/internal_tasks/elf_main_entry.o

ALL_OBJ = $(LOCAL_SRC_FILES)
LOCAL_OUT = $(OUT)/internal_tasks/$(notdir $(CURDIR))
ALL_OUT_OBJ = $(foreach VAR,$(ALL_OBJ),$(addprefix $(LOCAL_OUT)/,$(VAR)))

CFLAGS += $(LOCAL_CFLAGS) $(CFG_INC) $(C_DEFINES)
CFLAGS += -DTVP
#OEM_TEE_BASE_Version
CFLAGS += -DOEM_TEE_VERSION_MAJOR=$(CFG_OEM_TEE_VERSION_MAJOR)
CFLAGS += -DOEM_TEE_VERSION_MINOR=$(CFG_OEM_TEE_VERSION_MINOR)
CFLAGS += -DOEM_TEE_VERSION_RELEASE=$(CFG_OEM_TEE_VERSION_RELEASE)
CFLAGS += -DOEM_TEE_VERSION_BUILD=$(CFG_OEM_TEE_VERSION_BUILD)
#CFLAGS += $(LOCAL_CFLAGS) $(EXTRA_CFLAGS)
LDFLAGS += -r -d --strip-debug --whole-archive  -T $(TA_LINK_SCRIPT) $(TRUSTEDCORE_ELF_MAIN_ENTRY_PATH) --no-whole-archive


##########################################################
# build demo_task1
##########################################################
.PHONY : all clean prepare

ifeq ($(strip $(CONFIG_TRUSTEDCORE_RELEASE)), )
all: $(ALL_OBJ)
	@echo ALL_OUT_OBJ = $(ALL_OUT_OBJ)
	$(LD1) $(LDFLAGS) -o $(LOCAL_MODULE) $(ALL_OUT_OBJ) `$(CC1) -mthumb-interwork -print-libgcc-file-name`
	@echo "Make $(LOCAL_MODULE) Success!"
else
all:
	cp $(TRUSTEDCORE_RELEASE_DIR)/$(HI_CHIP_TYPE)/$(CFG_HI_TVP_MEM_LAYOUT)/$(notdir $(LOCAL_MODULE)) $(LOCAL_MODULE)
	@echo "Make $(LOCAL_MODULE) Success!"
endif

clean:
	$(RM) $(LOCAL_MODULE) $(ALL_OUT_OBJ) $(ALL_OBJ)

prepare:
	@echo VPATH = $(VPATH)
	@echo PLAYREADY_PRIV_SRC = $(PLAYREADY_PRIV_SRC)
	$(MKDIR) $(foreach VAR,$(ALL_OUT_OBJ),$(dir $(VAR)))

$(ALL_OBJ): prepare

%.o : %.c
	$(CC1) -c $(CFLAGS) $< -o $(LOCAL_OUT)/$@

