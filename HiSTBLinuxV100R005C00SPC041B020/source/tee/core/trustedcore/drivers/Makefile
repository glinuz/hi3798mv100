
#by fangjian00208632 2015-03-13

###########################################################
# platform definition
###########################################################
ROOT      =$(shell pwd)
LOCAL_CFLAGS =
LOCAL_ASFLAGS =
LOCAL_CFLAGS += ${TRUSTEDCORE_LOCAL_CFLAGS}
LOCAL_ASFLAGS += ${TRUSTEDCORE_LOCAL_ASFLAGS}

COMMON_KRENEL_INCS :=\
    -I$(ROOT)/../include/kernel/ \
    -I$(ROOT)/../include/config/ \
    -I$(ROOT)/../include/TEE \
    -I$(ROOT)/../include/TEE/libhwsecurec

PLAT_SYS_INCS := -I$(ROOT)/../platform

#DRIVER_DEMO_SRC := \
#	driver_demo/demo.o
#SE_DRIVER := \
#	eSE/se_dummy.o
COMMON_SRCS:= \
	driver_demo/demo.o \
	eSE/se_dummy.o \
	dma/dma.o \
	secure_config/tzpc.o \
	secure_config/device_status.o \
    	gic/secure_gic_common.o \
	gic/secure_gic_plat.o \
	rtc/secure_rtc_common.o \
	serial/secure_serial_common.o \
	serial/secure_serial_io.o \
	serial/secure_serial_plat.o \
	serial/sre_log.o \
	timer/sre_hwtmr_init.o \
	timer/secure_timer_common.o \
	timer/secure_timer_plat.o \
	mem/sre_taskmem.o \
	mem/sre_addr_check.o \
	mem/sre_sharedmem.o \
	video_decrypt/vdec_mmap.o

ifneq ($(strip $(TRUSTEDCORE_PLATFORM_CHOOSE)), WITH_DEVCHIP_PLATFORM)
COMMON_SRCS +=secure_mmu/secure_page_table_plat.o
endif
#sre_taskmem_init.h
COMMON_INCS:= \
    -I$(ROOT)/mem/ \
    -I$(ROOT)/video_decrypt
ifeq ($(WITH_IRIS_SUPPORT),true)
COMMON_SRCS += iris/iris_mmap.o
LOCAL_C_INCLUDES += -I$(ROOT)/iris
endif

ASM_MEM_SRC = \
	asm_mem/memmove.o \
	asm_mem/memcpy.o \
	asm_mem/asm_mem_api.o

LOCAL_SRC_FILES := \
	$(COMMON_SRCS) \
	$(ASM_MEM_SRC)


LOCAL_C_INCLUDES +=\
    $(PLAT_SYS_INCS) \
    $(COMMON_KRENEL_INCS) \
    $(COMMON_INCS)

##########################################################
# project path definition
##########################################################
ALL_OBJ = $(LOCAL_SRC_FILES)
LOCAL_OUT = $(OUT)/$(notdir $(CURDIR))
ALL_OUT_OBJ = $(foreach VAR,$(ALL_OBJ),$(addprefix $(LOCAL_OUT)/,$(VAR)))


#include $(TRUSTEDCORE_BUILD_RAW_EXECUTABLE)
# add for enum size may different in different toolchain
LOCAL_CFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables
LOCAL_ASFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables

LOCAL_CFLAGS += -march=armv7-a

CFLAGS += $(LOCAL_C_INCLUDES)
CFLAGS += $(LOCAL_CFLAGS)
#LDFLAGS += $(LOCAL_LDFLAGS)

ifeq ($(WITH_ARMV8_SUPPORT) , true)
LOCAL_MODULE := $(OUT)/lib_trustedcore_driver_64bit.a
else
LOCAL_MODULE:= $(OUT)/lib_trustedcore_driver.a
endif
##########################################################
# build lib_trustedcore_driver.a
##########################################################
.PHONY : all prepare clean

all:$(ALL_OBJ)
	@echo $(ALL_OBJ)
	$(RM) $(LOCAL_MODULE)
	$(AR2) crsP $(LOCAL_MODULE) $(ALL_OUT_OBJ)
	@echo "Make $(LOCAL_MODULE) Success!"

$(ALL_OBJ):prepare

prepare:
	$(MKDIR) $(foreach VAR,$(ALL_OUT_OBJ),$(dir $(VAR)))
clean:
	$(RM) $(LOCAL_MODULE) $(ALL_OUT_OBJ) $(dir $(ALL_OUT_OBJ)) $(ALL_OBJ)

%.o : %.c | prepare
	$(CC2) -c $(CFLAGS) $< -o $(LOCAL_OUT)/$@

%.o : %.S | prepare
	$(CC2) $(CFLAGS) -c  $< -o $(LOCAL_OUT)/$@

%.o : %.s | prepare
	$(CC2) $(CFLAGS) -c  $< -o $(LOCAL_OUT)/$@



