
#by fangjian00208632 2015-03-13
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

###########################################################
# platform definition
###########################################################
MAKE      =make
AR        =$(CROSS_COMPILE)ar
LD        =$(CROSS_COMPILE)ld
CC        =$(CROSS_COMPILE)gcc
CPP       =$(CROSS_COMPILE)g++
RM        =rm -f
COPY      =cp
ROOT      =$(shell pwd)

#Config Trusted Storage Path must end with '/'
CFG_TEE_STORAGE_DIR ?= /root/
LIB_SEC_DIR = $(ROOT)/libc_sec
LIB_SEC_INC_DIR = $(LIB_SEC_DIR)/include
LIB_SEC_SRC_DIR = $(LIB_SEC_DIR)/src
WITH_DEVCHIP_PLATFORM := true

#############################
## libteec.so
#############################
LOCAL_SRC_FILES := src/tee_client_api.o \
                   src/tee_client_storage_api.o \
                   src/tee_client_app_load.o \
                   src/tee_client_app_load_old.o

LOCAL_C_INCLUDES := -I$(ROOT)/inc \
                    -I$(LIB_SEC_INC_DIR)
                    
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
LOCAL_SRC_FILES += sec_mmz/src/sec_mmz.o
LOCAL_C_INCLUDES += -I$(ROOT)/sec_mmz/inc
endif

##########################################################
# CFLAGS definition
##########################################################
CFLAGS += -Os -Wall -fPIC
ifeq ($(CFG_HI_CPU_ARCH), arm)
CFLAGS += -march=armv7-a -mthumb
endif
CFLAGS += $(LOCAL_C_INCLUDES)
CFLAGS += $(LOCAL_CFLAGS)

ifeq ($(WITH_DEVCHIP_PLATFORM),true)
CFLAGS += -DWITH_DEVCHIP_PLATFORM
endif

ifeq ($(O),)
ifeq ($(firstword $(subst -, ,$(CROSS_COMPILE))),aarch64)
OUT_DIR := $(HI_OUT_DIR)/obj64/$(subst $(SDK_DIR)/,,$(CURDIR))/

TARGET_SLIB_OUT_DIR = $(HI_STATIC_LIB64_DIR)
TARGET_DLIB_OUT_DIR = $(HI_SHARED_LIB64_DIR)
else
OUT_DIR := $(HI_OUT_DIR)/obj/$(subst $(SDK_DIR)/,,$(CURDIR))/

TARGET_SLIB_OUT_DIR = $(HI_STATIC_LIB_DIR)
TARGET_DLIB_OUT_DIR = $(HI_SHARED_LIB_DIR)
endif
else
OUT_DIR := ${O}
TARGET_SLIB_OUT_DIR = $(HI_STATIC_LIB_DIR)
TARGET_DLIB_OUT_DIR = $(HI_SHARED_LIB_DIR)
endif
ALL_OBJ := $(addprefix $(OUT_DIR),$(LOCAL_SRC_FILES))

LDFLAGS +=

LIB_NAME := libteec
LIB_C_SEC_NAME := libc_sec

##########################################################
# build
##########################################################
.PHONY : all clean libc_sec libteec teecd test libc_sec_clean libteec_clean teecd_clean test_clean

all: teecd libteec libc_sec
	test -d $(TARGET_SLIB_OUT_DIR) || mkdir -p $(TARGET_SLIB_OUT_DIR)
	$(COPY) $(OUT_DIR)$(LIB_NAME).a $(TARGET_SLIB_OUT_DIR)/
	$(COPY) $(OUT_DIR)$(LIB_C_SEC_NAME).a $(TARGET_SLIB_OUT_DIR)/
	test -d $(TARGET_DLIB_OUT_DIR) || mkdir -p $(TARGET_DLIB_OUT_DIR)
	$(COPY) $(OUT_DIR)$(LIB_NAME).so $(TARGET_DLIB_OUT_DIR)/
	$(COPY) $(OUT_DIR)$(LIB_C_SEC_NAME).so $(TARGET_DLIB_OUT_DIR)/

test: libteec 

clean: libteec_clean libc_sec_clean teecd_clean test_clean
	$(RM) $(TARGET_SLIB_OUT_DIR)/$(LIB_NAME).a $(TARGET_DLIB_OUT_DIR)/$(LIB_NAME).so
	$(RM) $(TARGET_SLIB_OUT_DIR)/$(LIB_C_SEC_NAME).a $(TARGET_DLIB_OUT_DIR)/$(LIB_C_SEC_NAME).so

##########################################################
# 1. build libsec
##########################################################
LIBSEC_SRC_FILE = $(wildcard libc_sec/src/*.c)
LIBSEC_SRC_FILE_OUT = $(addprefix $(OUT_DIR),$(LIBSEC_SRC_FILE))
LIBSEC_OBJ_FILE = $(patsubst %.c, %.o, ${LIBSEC_SRC_FILE_OUT})

CFLAGS += -Os -Wall -fPIC
ifeq ($(CFG_HI_CPU_ARCH), arm)
CFLAGS += -march=armv7-a -mthumb
endif
LOCAL_CFLAGS = -Wall -s -DNDEBUG -O1 -DSECUREC_SUPPORT_STRTOLD=1
LOCAL_CFLAGS += -fPIC
ifeq ($(CFG_HI_CPU_ARCH), arm)
	LOCAL_CFLAGS += -march=armv7-a -mthumb
endif
libc_sec : $(LIBSEC_OBJ_FILE)
	$(CC) $(LDFLAGS) -shared -o $(OUT_DIR)$(LIB_C_SEC_NAME).so $(LIBSEC_OBJ_FILE)
	$(AR) -rc -o $(OUT_DIR)$(LIB_C_SEC_NAME).a $(LIBSEC_OBJ_FILE)
	@echo "Make $(LIB_C_SEC_NAME) Success!"

libc_sec_clean:
	$(RM) -rf $(OUT_DIR)$(LIB_C_SEC_NAME)* $(LIBSEC_OBJ_FILE)

$(OUT_DIR)%.o : %.c
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

##########################################################
# 1. build libteec
##########################################################
libteec: $(ALL_OBJ)
	$(CC) $(LDFLAGS) -shared -o $(OUT_DIR)$(LIB_NAME).so $(ALL_OBJ)
	$(AR) -rc -o $(OUT_DIR)$(LIB_NAME).a $(ALL_OBJ)
	@echo "Make $(LIB_NAME) Success!"

libteec_clean:
	$(RM) $(OUT_DIR)$(LIB_NAME)* $(ALL_OBJ)

$(OUT_DIR)%.o : %.c
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

##########################################################
# build test
##########################################################
test: libteec libc_sec
	make -C $(ROOT)/test/ all

test_install:
	make -C $(ROOT)/test/ install

test_uninstall:
	make -C $(ROOT)/test/ uninstall

test_clean:
	make -C $(ROOT)/test/ clean
#############################
## 2. build teecd
#############################
#add by q00209673 for bootloaderlockstatus
WITH_K3V3_FPGA=false
ifeq ($(strip $(WITH_K3V3_FPGA)), true)
TEECD_CFLAGS += -DK3V3_FPGA
endif
RPMB_FEATURE_ENABLED=false
ifeq ($(RPMB_FEATURE_ENABLED),true)
TEECD_CFLAGS += -DTEE_SUPPORT_RPMB_DEVICE
endif

ifeq ($(WITH_DEVCHIP_PLATFORM),true)
TEECD_CFLAGS += -DWITH_DEVCHIP_PLATFORM
endif
WITH_ANDROID_PLATFORM_HISI := false
ifeq ($(WITH_ANDROID_PLATFORM_HISI),true)
TEECD_CFLAGS += -D_ANDROID_PLATFORM_HISI
endif

TEECD_SRC_FILES:= src/tee_agent.o \
		  src/tee_get_app_cert.o \
		  src/tee_ca_daemon.o
TEECD_C_INCLUDES := -I$(ROOT)/inc \
		    -I$(ROOT)/libc_sec/include

#LOCAL_STATIC_LIBRARIES := libxml2 libc libm libcutils liblog libicuuc_static libcrypto_static
#ifneq ($(strip $(WITH_K3V3_FPGA)), true)
#LOCAL_STATIC_LIBRARIES += libnve
#endif
TEECD_CFLAGS += $(TEECD_C_INCLUDES)
TEECD_CFLAGS += -Os -Wall
ifeq ($(CFG_HI_CPU_ARCH), arm)
TEECD_CFLAGS += -march=armv7-a -mthumb
endif
TEECD_CFLAGS += -DCFG_TEE_STORAGE_DIR=\"$(CFG_TEE_STORAGE_DIR)\"

TEECD_OBJ = $(addprefix $(OUT_DIR), $(TEECD_SRC_FILES))
TEECD_EXE = $(OUT_DIR)teecd

teecd:libc_sec $(TEECD_OBJ)
	$(CC) -static -o $(TEECD_EXE) $(TEECD_OBJ) -L$(OUT_DIR) -lc_sec -lpthread

teecd_clean:
	$(RM) $(TEECD_EXE) $(TEECD_OBJ)

$(TEECD_OBJ): $(OUT_DIR)%.o : %.c
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) -c $(TEECD_CFLAGS) $< -o $@

