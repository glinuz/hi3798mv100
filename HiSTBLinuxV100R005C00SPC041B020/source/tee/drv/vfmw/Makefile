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
LOCAL_OUT := $(OUT)/drv/vfmw
else
LOCAL_OUT := $(shell pwd)
endif

################################################
## test config
#################################################
ifeq ($(SECURE_TESTSUITE),RTOSck_UT)
LOCAL_CFLAGS += -DTESTSUITE_RTOSck_UT
WITH_MONITOR_TEST := false
endif

ifeq ($(SECURE_TESTSUITE),RTOSck_PT)
LOCAL_CFLAGS += -DTESTSUITE_RTOSck_PT
endif

ifeq ($(SECURE_TESTSUITE),RTOSck_IT)
LOCAL_CFLAGS += -DTESTSUITE_RTOSck_IT
endif

WITH_DEBUG_EXTERN := false
ifeq ($(WITH_DEBUG_EXTERN), true)
LOCAL_CFLAGS += -DDEBUG_EXTERN
LOCAL_CFLAGS += -DDEBUG_SWITCH
endif

ifeq ($(CONFIG_MEM_LAYOUT),1G)
LOCAL_CFLAGS += -DCONFIG_SYS_MEM_1G
endif

ifeq ($(CONFIG_MEM_LAYOUT),2G)
LOCAL_CFLAGS += -DCONFIG_SYS_MEM_2G
endif

################################################
## include path
#################################################
ifdef COMMON_DRV_INCLUDE
PATH_SDK_COMMON = -I$(COMMON_DRV_INCLUDE)
else
PATH_SDK_COMMON = -I$(CURDIR)/../../../common/drv/include
endif

PATH_MMZ_INCLUDE = -I$(HI_TEE_DRV_DIR)/mem/sec_mmz/include

ifdef MSP_DIR
PATH_MSP_DIR = $(MSP_DIR)
else
PATH_MSP_DIR = $(CURDIR)/../../../msp
endif


#################################################
## include patch
#################################################
INCLUDE := \
$(PATH_SDK_COMMON)\
$(PATH_MMZ_INCLUDE)

#################################################
## build script
#################################################
LOCAL_CFLAGS += -O2 -g -W -Wall -fno-omit-frame-pointer

LOCAL_CFLAGS += -march=armv7-a

LOCAL_ASFLAGS += -march=armv7-a

ifeq ($(strip $(WITH_HI3798CV2X_PLATFORM)), true)

VFMW_PRODUCT := Hi3798CV200

ifneq ($(findstring $(HI_CHIP_TYPE),hi3798mv200 hi3798mv200_a),)
VFMW_PRODUCT := Hi3798MV200
endif

VFMW_NS := ./vfmw_v5.0

include $(VFMW_NS)/firmware/product/$(VFMW_PRODUCT)/SOS_CFG/vfmw_config.cfg
include $(VFMW_NS)/firmware/product/$(VFMW_PRODUCT)/SOS_CFG/vfmw_make.cfg

else ifeq ($(strip $(WITH_HI3798MX_PLATFORM)), true)
VFMW_PRODUCT := Hi3798MV100

VFMW_NS := $(PATH_MSP_DIR)/drv/vfmw/vfmw_v4.0
VFMW_S := ./vfmw_v4.0

include $(VFMW_S)/firmware/product/$(VFMW_PRODUCT)/HD_FULL/vfmw_config.cfg
include $(VFMW_S)/firmware/product/$(VFMW_PRODUCT)/HD_FULL/vfmw_make.cfg
endif

LOCAL_CFLAGS += $(VFMW_CFLAGS)


##########################################################
# project path definition
##########################################################
LIBS    := libsec_$(notdir $(LOCAL_OUT)).a
OBJS    := $(vfmw_obj_list)

CFLAGS += $(INCLUDE)
CFLAGS += $(LOCAL_CFLAGS)

ALL_OUT_OBJ = $(foreach VAR,$(OBJS),$(addprefix $(LOCAL_OUT)/,$(VAR)))
ALL_OUT_LIBS = $(foreach VAR,$(LIBS),$(addprefix $(LOCAL_OUT)/,$(VAR)))

#===============================================================================
# rules
#===============================================================================
include $(HI_TEE_DRV_DIR)/stb_drv.mak
