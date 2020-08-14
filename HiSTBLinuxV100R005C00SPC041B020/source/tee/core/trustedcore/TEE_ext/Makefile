ROOT      =.
LOCAL_CFLAGS =
LOCAL_ASFLAGS =
LOCAL_CFLAGS += ${TRUSTEDCORE_LOCAL_CFLAGS}
LOCAL_ASFLAGS += ${TRUSTEDCORE_LOCAL_ASFLAGS}
LOCAL_CFLAGS += -DSKYTONE_VERSION=$(TEE_SUPPORT_SKYTONE_VERSION)

ifeq ($(WITH_TUI_SUPPORT),true)
LOCAL_CFLAGS += -DTEE_SUPPORT_TUI
ifeq ($(strip $(TRUSTEDCORE_ASFLAG_SET)), true)
LOCAL_ASFLAGS += -DTEE_SUPPORT_TUI
endif
endif

ifeq ($(WITH_RPMB_PARTITION_INFO_FROM_ATF),true)
LOCAL_CFLAGS += -DTEE_GET_RPMB_PARTITION_FROM_ATF
endif

ifeq ($(strip $(TRUSTEDCORE_PLATFORM_CHOOSE)), WITH_DEVCHIP_PLATFORM)
	LOCAL_CFLAGS += -DTEE_DISABLE_CA_SIGN=2
else
ifdef GLOBAL_TAG_DISABLE_CA_SIGN
ifeq ($(strip $(GLOBAL_TAG_DISABLE_CA_SIGN)), 0)
	LOCAL_CFLAGS += -DTEE_DISABLE_CA_SIGN=0
else
	LOCAL_CFLAGS += -DTEE_DISABLE_CA_SIGN=1
endif #end ifeq GLOBAL_TAG_DISABLE_CA_SIGN 0

else # ifdef GLOBAL_TAG_DISABLE_CA_SIGN
	LOCAL_CFLAGS += -DTEE_DISABLE_CA_SIGN=0
endif
endif

PATH01 = -I$(ROOT)/../include/TEE
PATH02 = -I$(ROOT)/../include/kernel
PATH03 = -I$(ROOT)/../include/TEE/tui
PATH12 = -I$(ROOT)/../trustedcore_kernel/syscalls_ext
PATH13 = -I$(ROOT)/../drivers/video_decoder/platform/TEE/include
PATH14 = -I$(ROOT)/../drivers/video_decoder/include
PATH15 = -I$(ROOT)/../drivers/video_decoder/driver
PATH16 = -I$(ROOT)/../include
PATH17 = -I$(ROOT)

INCLUDE := \
$(PATH01) \
$(PATH02) \
$(PATH03) \
$(PATH12) \
$(PATH13) \
$(PATH14) \
$(PATH15) \
$(PATH16) \
$(PATH17) 

#################################################
## export symbol
#################################################
SYMBOL_SRC = \
	tee_symbol_ext.o
SYSCALLS_EXT_SRC = \
	sre_syscalls_ext.o
TEE_CONFIG_SRC= \
	tee_config.o
TEE_RPMB_EXT_SRC = \
        tee_rpmb_ext.o
TEE_SFS_EXT_SRC = \
	tee_sfs_ext.o
TEE_EXT_SRC = \
	tee_external_api.o
TEE_ECIES_EXT_SRC = \
	tee_ecies_ext.o

LIB_SRCS = \
$(SYMBOL_SRC) \
$(SYSCALLS_EXT_SRC) \
$(TEE_RPMB_EXT_SRC) \
$(TEE_SFS_EXT_SRC) \
$(TEE_EXT_SRC) \
$(TEE_ECIES_EXT_SRC)

ifeq ($(strip $(TRUSTEDCORE_PLATFORM_CHOOSE)), WITH_DEVCHIP_PLATFORM)
include $(ROOT)/../platform/${PLATFORM_DIR}/${PLATFORM_DIR_NAME}/ext/user/Linux_user.mk
INCLUDE += -I$(TRUSTEDCORE_DIR)/platform/$(PLATFORM_DIR)/dispatch
else
LIB_SRCS += $(TEE_CONFIG_SRC)
endif

GLOBALTASK_TEXT_BASE = 0

LOCAL_CFLAGS += -O2 -g -fPIC
LOCAL_CFLAGS += -fstack-protector --param ssp-buffer-size=4 -Wstack-protector
#LOCAL_CFLAGS += -O2 -g -W -Wall
LOCAL_CFLAGS += -march=armv7-a


LOCAL_C_INCLUDES := $(INCLUDE)
LOCAL_SRC_FILES := 	$(LIB_SRCS)

#################################################
## import libs
#################################################
TARGET_GLOBALTASK_LINK_SCRIPT := $(ROOT)/link.ld
GLOBALTASK_LIB_PATH = $(TRUSTEDCORE_DIR)/release/internal_tasks/libtee.a
GLOBALTASK_TUI_PATH = $(TRUSTEDCORE_DIR)/release/internal_tasks/libtui.a
GLOBALTASK_TUI_CORE_PATH = $(TRUSTEDCORE_DIR)/release/internal_tasks/libtui-core.a
GLOBALTASK_TUI_THIRD_PATH = $(TRUSTEDCORE_DIR)/release/internal_tasks/libthirdtui.a
GLOBALTASK_THIRD_PATH = $(TRUSTEDCORE_DIR)/release/internal_tasks/libthird.a
GLOBALTASK_O = $(TRUSTEDCORE_DIR)/release/internal_tasks/global_task.o



#include $(TRUSTEDCORE_BUILD_RAW_EXECUTABLE)
# add for enum size may different in different toolchain
LOCAL_CFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables
LOCAL_ASFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables


CFLAGS += $(LOCAL_C_INCLUDES)
CFLAGS += $(LOCAL_CFLAGS)
#LDFLAGS += $(LOCAL_LDFLAGS)

LOCAL_MODULE_ELF:= $(OUT)/globaltask.img.elf
LOCAL_MODULE:= $(OUT)/globaltask.img


##########################################################
# project path definition
##########################################################
ALL_OBJ = $(LOCAL_SRC_FILES)
LOCAL_OUT = $(OUT)/$(notdir $(CURDIR))
ALL_OUT_OBJ = $(foreach VAR,$(ALL_OBJ),$(addprefix $(LOCAL_OUT)/,$(VAR)))

LDFLAGS := --script $(TARGET_GLOBALTASK_LINK_SCRIPT) \
		-pie --section-start .globaltask=$(GLOBALTASK_TEXT_BASE) \
		-Map $(LOCAL_OUT)/globaltask.map

ifeq ($(WITH_TUI_SUPPORT),true)
LDFLAGS += --whole-archive $(GLOBALTASK_O) $(GLOBALTASK_LIB_PATH) $(GLOBALTASK_TUI_PATH) $(GLOBALTASK_TUI_THIRD_PATH) $(GLOBALTASK_TUI_CORE_PATH) --no-whole-archive
else
LDFLAGS += --whole-archive $(GLOBALTASK_O) $(GLOBALTASK_LIB_PATH) $(GLOBALTASK_THIRD_PATH) --no-whole-archive
endif

##########################################################
# build globaltask.img
##########################################################
.PHONY : all prepare clean
all: $(ALL_OBJ)
	@echo $(ALL_OBJ)
	$(LD2) $(LDFLAGS) -o $(LOCAL_MODULE_ELF) $(ALL_OUT_OBJ) `$(CC2) -mthumb-interwork -print-libgcc-file-name`
	$(OBJCOPY2) -O binary $(LOCAL_MODULE_ELF) $(LOCAL_MODULE)
	@echo "Make $(LOCAL_MODULE) Success!"

$(ALL_OBJ): prepare

prepare:
	$(MKDIR) $(foreach VAR,$(ALL_OUT_OBJ),$(dir $(VAR)))
clean:
	$(RM) -rf $(LOCAL_MODULE) $(LOCAL_MODULE_ELF) $(ALL_OUT_OBJ) $(dir $(ALL_OUT_OBJ)) $(ALL_OBJ) $(LOCAL_OUT)/globaltask.map

%.o : %.c | prepare
	$(CC2) -c $(CFLAGS) $< -o $(LOCAL_OUT)/$@

%.o : %.S | prepare
	$(CC2) $(CFLAGS) -c  $< -o $(LOCAL_OUT)/$@

%.o : %.s | prepare
	$(CC2) $(CFLAGS) -c  $< -o $(LOCAL_OUT)/$@



