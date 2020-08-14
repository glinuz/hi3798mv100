#by fangjian00208632 2015-03-13
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../../../..

include $(SDK_DIR)/base.mak
endif
include $(SAMPLE_DIR)/base.mak

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

LDPATH    =$(ROOT)/../bin
OUT       =$(ROOT)/../bin
CFLAGS    = -I$(ROOT)/../inc \
            -I$(ROOT)/../../trustedcore/include/TEE \
            -I$(ROOT)/../../../api/include 
			
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)			
CFLAGS     +=  -I$(ROOT)/../sec_mmz/inc                
endif			
			
CFLAGS += -Os -Wall
ifeq ($(CFG_HI_CPU_ARCH), arm)
CFLAGS += -march=armv7-a -mthumb
endif
LDFLAGS   = -L$(LDPATH) -lteec -lc_sec -lpthread

ALL_EXE += \
		tee_sptest_gen_key \
		tee_test_agent \
		tee_test_CA \
		tee_test_cancellation \
		tee_test_elfload \
		tee_test_hdcp_storekey \
		tee_test_hdcp_write \
		tee_test_hwi_ipc \
		tee_test_invoke \
		tee_test_login \
		tee_test_loginread \
		tee_test_loginwrite1 \
		tee_test_loginwrite \
		tee_test_mem \
		tee_test_query_storekey \
		tee_test_secboot \
		tee_test_securekey_package \
		tee_test_secure_timer \
		tee_test_sess \
		tee_test_storage \
		tee_test_ut \
		tee_test_main \
		teec_permctrl \
		tee_test_beidou \
		hisi_demo_client \
		hisi_test_ca \
		tee_test_cipher \
		test_hello \
		test_otp \
		test_vdp \
		test_advca \
		tee_test_display

ifeq ($(CFG_HI_TEE_SUPPORT),y)
CFLAGS  += -I$(COMMON_API_INCLUDE)  -I$(COMMON_UNF_INCLUDE)  -I$(MSP_UNF_INCLUDE)      
LDFLAGS += -L$(HI_STATIC_LIB_DIR) -L$(HI_SHARED_LIB_DIR) $(SYS_LIBS) -lhi_common
CFLAGS += -DHI_TEE_SUPPORT
ifeq ($(CFG_HI_SMMU_SUPPORT),y)
CFLAGS += -DHI_SMMU_SUPPORT
endif
endif

WITH_DEVCHIP_PLATFORM := true
ifeq ($(WITH_DEVCHIP_PLATFORM),true)
CFLAGS += -DWITH_DEVCHIP_PLATFORM
endif

WITH_ANDROID_PLATFORM_HISI := false
ifeq ($(WITH_ANDROID_PLATFORM_HISI),true)
CFLAGS += -D_ANDROID_PLATFORM_HISI
endif

#tee_store_default_hdcp_key \
#tee_test_kernel_api \
#ALL_EXE_TARGET = $(foreach VAR,$(ALL_EXE),$(addprefix $(OUT)/,$(VAR)))
ALL_EXE_TARGET = $(addprefix $(OUT)/, $(ALL_EXE))

.PHONY : all clean

all:$(ALL_EXE_TARGET)
	@echo "$(ALL_EXE)"
	@echo "Make Successfull!"

install: all
	$(COPY) $(ALL_EXE_TARGET) $(BIN_DIR)

uninstall:
	$(RM) $(addprefix $(BIN_DIR)/, $(ALL_EXE))

$(ALL_EXE_TARGET) : $(OUT)/% : $(ROOT)/%.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	$(RM) $(addprefix $(OUT)/, $(ALL_EXE))
	@echo "clean Successfull!"
