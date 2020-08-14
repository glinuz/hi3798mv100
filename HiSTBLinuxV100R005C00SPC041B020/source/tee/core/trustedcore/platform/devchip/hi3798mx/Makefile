
#by fangjian00208632 2015-03-13

###########################################################
# platform definition
###########################################################
ROOT      =.
PLATFORM_PATH = $(TRUSTEDCORE_DIR)/platform/$(PLATFORM_DIR)/$(PLATFORM_DIR_NAME)
LOCAL_CFLAGS := ${TRUSTEDCORE_LOCAL_CFLAGS}
LOCAL_ASFLAGS := ${TRUSTEDCORE_LOCAL_ASFLAGS}

ARCH_PATH := armv7
ifeq ($(WITH_ARMV8_SUPPORT) , true)
ARCH_PATH = armv8/aarch32
endif

DRV_SRCS :=

################################################
## include path
#################################################
TRUSTEDCORE_INS = \
	-I$(TRUSTEDCORE_DIR)/platform \
	-I$(TRUSTEDCORE_DIR)/platform/tzarch/include \
	-I$(TRUSTEDCORE_DIR)/include/config \
	-I$(TRUSTEDCORE_DIR)/include/kernel \
	-I$(TRUSTEDCORE_DIR)/include/TEE \
	-I$(TRUSTEDCORE_DIR)/include/TEE/libhwsecurec \
	-I$(TRUSTEDCORE_DIR)/drivers/mem

TEE_INCLUDES := \
	-I$(PLATFORM_PATH)/pm \
	-I$(HI_TEE_DRV_DIR)/include \
	-I$(HI_TEE_API_DIR)/include \
	-I$(HI_TEE_DRV_DIR)/mem/sec_mmz/include

INCLUDE := \
	$(TRUSTEDCORE_INS) \
	$(TEE_INCLUDES)

TZARCH_SRCS = \
	../../tzarch/common/sre_vector.o \
	../../tzarch/common/monitor/cache-v7.o

TZARCH_SRCS += ../../tzarch/$(ARCH_PATH)/tzinit/sre_reset_vector.o \
	       ../../tzarch/$(ARCH_PATH)/monitor/cpu_idle_task.o \
	       ../../tzarch/$(ARCH_PATH)/monitor/cpu_suspend_task.o \
	       ../../tzarch/$(ARCH_PATH)/monitor/monitor_api.o \
	       ../../tzarch/$(ARCH_PATH)/monitor/monitor.o

ifneq ($(WITH_ARMV8_SUPPORT) , true)
TZARCH_SRCS += ../../tzarch/$(ARCH_PATH)/monitor/secondary_monitor.o
endif

CONFIG_SRCS := ../../init/sre_config.o

#################################################
## init
#################################################
INIT_SRCS += \
	init/init.o \
	init/page_table_chip.o

#################################################
## stub code: should be removed.
#################################################
STUB_SRCS = init/stub.o

#################################################
## pm
#################################################
PM_SRCS += pm/sre_pm.o

#################################################
## tzasc
#################################################
TZASC_INCLUDES := -I$(PLATFORM_PATH)/securemem_cfg
TZASC_SRCS := securemem_cfg/secure_mem_cfg.o

INCLUDE += $(TZASC_INCLUDES)
DRV_SRCS += $(TZASC_SRCS)

#################################################
## mmz
#################################################
MMZ_INCLUDES := \
	-I$(HI_TEE_DRV_DIR)/mem/sec_mmz/include \
	-I$(HI_TEE_DRV_DIR)/mem/hi_smmu/include
INCLUDE += $(MMZ_INCLUDES)
## trustedcore src
#################################################
TRUSTEDCORE_SRCS = \
	${TZARCH_SRCS} \
	$(CONFIG_SRCS) \
	$(INIT_SRCS) \
	$(STUB_SRCS) \
	$(PM_SRCS) \
	$(DRV_SRCS)
	
LOCAL_C_INCLUDES := $(INCLUDE)
LOCAL_SRC_FILES := $(TRUSTEDCORE_SRCS)

LOCAL_CFLAGS += -O2 -fPIC -fno-omit-frame-pointer
LOCAL_CFLAGS += -march=armv7-a
ifeq ($(WITH_ARMV8_SUPPORT) , true)
LOCAL_MODULE:= $(OUT)/lib_trustedcore_platform_64bit.a
else
LOCAL_MODULE:= $(OUT)/lib_trustedcore_platform.a
endif

ifeq ($(strip $(TRUSTEDCORE_ASFLAG_SET)), true)
LOCAL_ASFLAGS += -march=armv7-a -fpie
endif

#include $(TRUSTEDCORE_BUILD_RAW_EXECUTABLE)
# add for enum size may different in different toolchain
LOCAL_CFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables
LOCAL_ASFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables



##########################################################
# project path definition
##########################################################
ALL_OBJ = $(LOCAL_SRC_FILES)
LOCAL_OUT = $(OUT)/platform/$(notdir $(CURDIR))
ALL_OUT_OBJ = $(foreach VAR,$(ALL_OBJ),$(addprefix $(LOCAL_OUT)/,$(VAR)))

CFLAGS += $(LOCAL_C_INCLUDES)
CFLAGS += $(LOCAL_CFLAGS)
#LDFLAGS += $(LOCAL_LDFLAGS)
##########################################################
# build globaltask.img
##########################################################
.PHONY : all prepare clean

all:$(ALL_OBJ)
	@echo $(ALL_OBJ)
	$(RM) $(LOCAL_MODULE)
	$(AR2) crsP $(LOCAL_MODULE) $(ALL_OUT_OBJ)
	@echo "Make $(LOCAL_MODULE) Success!"

$(ALL_OBJ): prepare

prepare:
	$(MKDIR) $(foreach VAR,$(ALL_OUT_OBJ),$(dir $(VAR)))
clean:
	$(RM) $(LOCAL_MODULE)  $(ALL_OUT_OBJ) $(dir $(ALL_OUT_OBJ)) $(ALL_OBJ)

%.o : %.c | prepare
	$(CC2) -c $(CFLAGS) $< -o $(LOCAL_OUT)/$@

%.o : %.S | prepare
	$(CC2) $(CFLAGS) -c  $< -o $(LOCAL_OUT)/$@

%.o : %.s | prepare
	$(CC2) $(CFLAGS) -c  $< -o $(LOCAL_OUT)/$@



