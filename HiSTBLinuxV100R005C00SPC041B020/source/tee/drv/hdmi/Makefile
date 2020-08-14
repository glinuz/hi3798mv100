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
LOCAL_OUT := $(OUT)/drv/hdmi
else
LOCAL_OUT := $(shell pwd)
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE),  hi3798cv200),)
CTRL_DIR := imagev200
PRODUCT_DIR := product/hi3798cv200
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE),  hi3798mv200 hi3798mv200_a hi3796mv200),)
CHIP_NAME := hi379xmv200
CTRL_DIR := hisiv100
PRODUCT_DIR := product/hisiv100
endif

LIBS    := libsec_$(notdir $(LOCAL_OUT)).a
OBJS    :=  \
	tee_drv_hdmi_srm.o \
	tee_drv_hdmi_intf.o \
	tee_drv_hdmi_proc.o \
	hal/$(CTRL_DIR)/tee_drv_hdmi_hal.o \
	hal/$(CTRL_DIR)/tee_drv_hdmi_hdcp.o \

ifneq ($(findstring $(CFG_HI_CHIP_TYPE),  hi3798cv200),)	
OBJS += hal/$(CTRL_DIR)/tee_drv_hdmi_tx_reg.o
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE),  hi3798mv200 hi3798mv200_a hi3796mv200),)
OBJS += hal/$(CTRL_DIR)/tee_hdmi_hal_hdcp14.o\
		regs/tee_hdmi_reg_com.o \
		regs/$(CHIP_NAME)/tee_hdmi_reg_sec.o \
		regs/$(CHIP_NAME)/tee_hdmi_reg_hdcp2x.o\
		regs/$(CHIP_NAME)/tee_hdmi_reg_hdcp14.o\

endif

CFLAGS += -I$(HI_TEE_DRV_DIR)/hdmi
CFLAGS += -I$(HI_TEE_DRV_DIR)/hdmi/hal
CFLAGS += -I$(HI_TEE_DRV_DIR)/hdmi/hal/$(CTRL_DIR)
CFLAGS += -I$(HI_TEE_DRV_DIR)/hdmi/$(PRODUCT_DIR)
CFLAGS += -I$(HI_TEE_DRV_DIR)/hdmi/regs
CFLAGS += -I$(HI_TEE_DRV_DIR)/hdmi/regs/$(CHIP_NAME)
CFLAGS += -I$(TRUSTEDCORE_DIR)/include/TEE
CFLAGS += -I$(HI_TEE_DRV_DIR)/cipher/tee/include
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
