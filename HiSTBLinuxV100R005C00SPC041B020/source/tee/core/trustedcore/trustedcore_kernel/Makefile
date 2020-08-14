
#by fangjian00208632 2015-03-13

###########################################################
# platform definition
###########################################################
ROOT      =.
LOCAL_CFLAGS += ${TRUSTEDCORE_LOCAL_CFLAGS}
LOCAL_ASFLAGS += ${TRUSTEDCORE_LOCAL_ASFLAGS}

LOCAL_CFLAGS += -DTARGET_PRODUCT_VAL=\"$(TARGET_PRODUCT)\"

ifeq ($(WITH_TUI_SUPPORT),true)
LOCAL_CFLAGS += -DTEE_SUPPORT_TUI
ifeq ($(strip $(TRUSTEDCORE_ASFLAG_SET)), true)
LOCAL_ASFLAGS += -DTEE_SUPPORT_TUI
endif
endif

################################################
## test config
#################################################
ifeq ($(SECURE_TESTSUITE),RTOSck_UT)
LOCAL_CFLAGS += -DTESTSUITE_RTOSck_UT
ifeq ($(strip $(TRUSTEDCORE_ASFLAG_SET)), true)
LOCAL_ASFLAGS += -DTESTSUITE_RTOSck_UT
endif
WITH_MONITOR_TEST := false
endif

ifeq ($(SECURE_TESTSUITE),RTOSck_PT)
LOCAL_CFLAGS += -DTESTSUITE_RTOSck_PT
ifeq ($(strip $(TRUSTEDCORE_ASFLAG_SET)), true)
LOCAL_ASFLAGS += -DTESTSUITE_RTOSck_PT
endif
endif

ifeq ($(SECURE_TESTSUITE),RTOSck_IT)
LOCAL_CFLAGS += -DTESTSUITE_RTOSck_IT
ifeq ($(strip $(TRUSTEDCORE_ASFLAG_SET)), true)
LOCAL_ASFLAGS += -DTESTSUITE_RTOSck_IT
endif
endif

#For S/R uart renable when linux kernel uart disable
WITH_DEBUG_EXTERN := false
ifeq ($(WITH_DEBUG_EXTERN), true)
LOCAL_CFLAGS += -DDEBUG_EXTERN
ifeq ($(strip $(TRUSTEDCORE_ASFLAG_SET)), true)
LOCAL_ASFLAGS += -DDEBUG_EXTERN
endif
endif


################################################
## include path
#################################################
PATH00 = -I$(ROOT)/../platform/tzarch/include \
	-I$(ROOT)/../include/config \
	-I$(ROOT)/syscalls_ext \
	-I$(ROOT)/../include/kernel \
	-I$(ROOT)/../platform/common/include \
	-I$(ROOT)/../platform \
	-I$(ROOT)/../platform/common/include/efuse \
	-I$(ROOT)/../platform/common/tzpc \
	-I$(ROOT)/../platform/common/gpio \
	-I$(ROOT)/../platform/common/display \
	-I$(ROOT)/../drivers/driver_demo \
	-I$(ROOT)/../drivers/video_decoder/driver \
	-I$(ROOT)/../drivers/video_decoder/driver/hwdefs \
	-I$(ROOT)/../drivers/video_decoder/include \
	-I$(ROOT)/../drivers/video_decoder/platform/hisi_k3v5 \
	-I$(ROOT)/../include/TEE 
	
ifeq ($(SECURE_TESTSUITE),RTOSck_UT)
PATH01 = -I$(ROOT)/../testcode/include
PATH02 = -I$(ROOT)/../testcode/iCunit
PATH03 = -I$(ROOT)/../testcode/UT_OLD/Test_Common/
endif
ifeq ($(SECURE_TESTSUITE),RTOSck_IT)
PATH01 = -I$(ROOT)/../testcode/include
PATH02 = -I$(ROOT)/../testcode/iCunit
PATH03 = -I$(ROOT)/../testcode/IT_OLD/Test_Common/
endif
PATH05 = -I$(ROOT)/../drivers/eSE


#################################################
## include patch
#################################################
INCLUDE := \
$(PATH00) \
$(PATH01) \
$(PATH02) \
$(PATH03) \
$(PATH04) \
$(PATH05)

INCLUDE += -I$(ROOT)/../platform/common/touchscreen
#################################################
## syscall ext code
#################################################
SYSCALL_EXT_SRCS = \
		   syscalls_ext/sre_syscallDispatch_ext.o
ifeq ($(WITH_MONITOR_TEST), true)
TZARCH_TEST_SRCS += tzarch/monitor/monitor_test.o \
tzarch/monitor/monitor_test_asm.o
LOCAL_CFLAGS += -DMONITOR_TEST
#ifneq ($(filter 4.7 4.7.%, $(TARGET_GCC_VERSION)),)
ifeq ($(strip $(TRUSTEDCORE_ASFLAG_SET)), true)
LOCAL_ASFLAGS += -DMONITOR_TEST
endif
INCLUDE += -I$(ROOT)/../testcode/include
endif


#################################################
## test suite code
#################################################
ifeq ($(SECURE_TESTSUITE),RTOSck_UT)
TEST_SRC = \
../testcode/UT_OLD/Test_Common/app_data_loacl.o \
../testcode/UT_OLD/Test_Common/app_data_sbishared.o \
../testcode/UT_OLD/Test_Common/app_data_shared.o \
../testcode/UT_OLD/Test_Common/osTest.o \
../testcode/UT_OLD/Test_Common/SRE_AppInit.o \
../testcode/iCunit/iCunit.o \
../testcode/UT_OLD/UT/ut_uni_hwi.o \
../testcode/UT_OLD/UT/ut_uni_timer.o \
../testcode/UT_OLD/UT/sre_dispatch_test.o \
../testcode/UT_OLD/UT/ut_sre_err.o \
../testcode/UT_OLD/UT/ut_srehunt.o \
../testcode/UT_OLD/UT/ut_sre_fsem.o \
../testcode/UT_OLD/UT/ut_sre_sem.o \
../testcode/UT_OLD/UT/ut_sre_msg.o \
../testcode/UT_OLD/UT/ut_sre_msg_init.o \
../testcode/UT_OLD/UT/ut_sre_msg_minor.o \
../testcode/UT_OLD/UT/ut_sre_swi.o \
../testcode/UT_OLD/UT/ut_sre_swi_init.o \
../testcode/UT_OLD/UT/ut_sre_swi_minor.o \
../testcode/UT_OLD/UT/ut_sre_task.o \
../testcode/UT_OLD/UT/ut_sre_task_init.o \
../testcode/UT_OLD/UT/ut_sre_task_minor.o \
../testcode/UT_OLD/UT/ut_sre_mem.o \
../testcode/UT_OLD/UT/ut_sre_mem_init.o \
../testcode/UT_OLD/UT/ut_sre_mem_minor.o \
../testcode/UT_OLD/UT/ut_sre_dynpoolmem.o \
../testcode/UT_OLD/UT/ut_sre_dynpoolmem_init.o \
../testcode/UT_OLD/UT/ut_sre_fscmem.o \
../testcode/UT_OLD/UT/ut_sre_fscmem_init.o \
../testcode/UT_OLD/UT/ut_uni_cachemmu.o
endif

ifeq ($(SECURE_TESTSUITE),RTOSck_IT)
TEST_SRC = \
../testcode/IT_OLD/Test_Common/app_data_loacl.o \
../testcode/IT_OLD/Test_Common/app_data_sbishared.o \
../testcode/IT_OLD/Test_Common/app_data_shared.o \
../testcode/IT_OLD/Test_Common/osTest.o \
../testcode/IT_OLD/Test_Common/SRE_AppInit.o \
../testcode/iCunit/iCunit.o \
../testcode/IT_OLD/IT/ipc/it_sre_fsem.o \
../testcode/IT_OLD/IT/ipc/it_sresem.o \
../testcode/IT_OLD/IT/ipc/it_sremsg.o \
../testcode/IT_OLD/IT/ipc/it_sremsg2.o \
../testcode/IT_OLD/IT/Kernel/it_srehunt.o \
../testcode/IT_OLD/IT/Kernel/it_sre_swi.o \
../testcode/IT_OLD/IT/Kernel/it_sre_timer.o \
../testcode/IT_OLD/IT/Kernel/It_sreerr.o \
../testcode/IT_OLD/IT/Kernel/it_srehwi.o \
../testcode/IT_OLD/IT/Kernel/it_sretask.o \
../testcode/IT_OLD/IT/mem/it_fscmem.o \
../testcode/IT_OLD/IT/mem/it_fscmem_rt.o \
../testcode/IT_OLD/IT/mem/it_uniprivpoolmem.o
#../testcode/IT_OLD/IT/Kernel/it_sre_cachemmu.o
endif

#################################################
## trustedcore src
#################################################
TRUSTEDCORE_SRCS = \
$(TEST_SRC) \
$(TZARCH_TEST_SRCS) \
$(SYSCALL_EXT_SRCS)

#################################################
## devchip compile
#################################################
ifeq ($(strip $(TRUSTEDCORE_PLATFORM_CHOOSE)), WITH_DEVCHIP_PLATFORM)
include $(ROOT)/../platform/$(PLATFORM_DIR)/$(PLATFORM_DIR_NAME)/ext/kernel/Linux_kernel.mk
endif

#################################################
## build script & libs
#################################################
ifeq ($(WITH_ARMV8_SUPPORT) , true)
PROTECT_STATIC_LIBRARIES := lib_trustedcore_driver_64bit.a
PROTECT_STATIC_LIBRARIES += lib_trustedcore_platform_64bit.a
else
PROTECT_STATIC_LIBRARIES := lib_trustedcore_driver.a
PROTECT_STATIC_LIBRARIES += lib_trustedcore_platform.a
endif

ifeq ($(strip $(TRUSTEDCORE_PLATFORM_CHOOSE)), WITH_K3V3_PLATFORM)
PROTECT_STATIC_LIBRARIES += lib_trustedcore_init.a
endif
#ifeq ($(strip $(WITH_STB_PLATFORM)), true)
#PROTECT_STATIC_LIBRARIES += lib_trustedcore_init.a
#endif
#ifeq ($(strip $(WITH_HPT_PLATFORM)), true)
#PROTECT_STATIC_LIBRARIES += lib_trustedcore_init.a
#endif

#################################################
## devchip static lib
#################################################
ifeq ($(strip $(TRUSTEDCORE_PLATFORM_CHOOSE)), WITH_DEVCHIP_PLATFORM)
LOCAL_WHOLE_STATIC_LIBRARIES += $(PLATFORM_DEVCHIP_STATIC_LIB)
endif


ifeq ($(strip $(CONFIG_TRUSTEDCORE_RELEASE)), true)
LOCAL_WHOLE_STATIC_LIBRARIES = $(addprefix $(TRUSTEDCORE_RELEASE_DIR)/$(HI_CHIP_TYPE)/$(CFG_HI_TVP_MEM_LAYOUT)/, $(PROTECT_STATIC_LIBRARIES))
else
LOCAL_WHOLE_STATIC_LIBRARIES = $(addprefix $(OUT)/, $(PROTECT_STATIC_LIBRARIES))
endif

LOCAL_WHOLE_STATIC_LIBRARIES += $(addprefix $(OUT)/drv/, libstbdrv.a)

TRUSTEDCORE_LIB_PATH = $(TRUSTEDCORE_DIR)/release/trustedcore_lib/libtrustedcore.a

TARGET_RTOSCK_LINK_SCRIPT := $(ROOT)/trustedcore.ld
TARGET_RTOSCK_LINK_SCRIPT = $(addprefix ${TRUSTEDCORE_DIR}/, ${TRUSTEDCORE_LINK_FILE})


LOCAL_CFLAGS += -O2 -W -Wall -fno-omit-frame-pointer
LOCAL_CFLAGS += -march=armv7-a

ifeq ($(strip $(TRUSTEDCORE_ASFLAG_SET)), true)
LOCAL_ASFLAGS += -march=armv7-a
endif

LOCAL_C_INCLUDES := $(INCLUDE)
LOCAL_SRC_FILES := $(TRUSTEDCORE_SRCS)

LOCAL_MODULE_ELF:= $(OUT)/rtosck.img.elf
LOCAL_MODULE:= $(OUT)/rtosck.img

#include $(TRUSTEDCORE_BUILD_RAW_EXECUTABLE)
# add for enum size may different in different toolchain
LOCAL_CFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables
LOCAL_ASFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables

CFLAGS += $(LOCAL_C_INCLUDES)
CFLAGS += $(LOCAL_CFLAGS)
#LDFLAGS += $(LOCAL_LDFLAGS)

##########################################################
# project
##########################################################
ALL_OBJ = $(LOCAL_SRC_FILES)
LOCAL_OUT = $(OUT)/$(notdir $(CURDIR))
ALL_OUT_OBJ = $(foreach VAR,$(ALL_OBJ),$(addprefix $(LOCAL_OUT)/,$(VAR)))

LDFLAGS += --script $(TARGET_RTOSCK_LINK_SCRIPT) \
		-Ttext $(TRUSTEDCORE_VIRT_BASE) -Map $(LOCAL_OUT)/rtosck.map
LDFLAGS += --whole-archive $(TRUSTEDCORE_LIB_PATH) --no-whole-archive
LDFLAGS += --whole-archive $(LOCAL_WHOLE_STATIC_LIBRARIES) --no-whole-archive
GROUP_LIBS += --start-group $(addprefix ${ROOT}/../, ${TRUSTEDCORE_THIRDPARTY_LIBS}) --end-group
##########################################################
# build rtosck.img
##########################################################
.PHONY : all prepare clean

all: $(ALL_OBJ)
	@echo $(ALL_OBJ)
	$(RM) $(LOCAL_MODULE) $(LOCAL_MODULE_ELF)
	$(LD2) $(LDFLAGS) -o $(LOCAL_MODULE_ELF) $(ALL_OUT_OBJ) $(GROUP_LIBS) `$(CC2) -mthumb-interwork -print-libgcc-file-name`
	$(OBJCOPY2) -O binary $(LOCAL_MODULE_ELF) $(LOCAL_MODULE)
	@echo "Make $(LOCAL_MODULE) Success!"

$(ALL_OBJ): prepare

prepare:
	$(MKDIR) $(foreach VAR,$(ALL_OUT_OBJ),$(dir $(VAR)))
clean:
	$(RM) $(LOCAL_MODULE) $(LOCAL_MODULE_ELF) $(ALL_OUT_OBJ) $(dir $(ALL_OUT_OBJ)) $(ALL_OBJ) $(LOCAL_OUT)/rtosck.map

%.o : %.c | prepare
	$(CC2) -c $(CFLAGS) $< -o $(LOCAL_OUT)/$@

%.o : %.S | prepare
	$(CC2) $(CFLAGS) -c  $< -o $(LOCAL_OUT)/$@

%.o : %.s | prepare
	$(CC2) $(CFLAGS) -c  $< -o $(LOCAL_OUT)/$@



