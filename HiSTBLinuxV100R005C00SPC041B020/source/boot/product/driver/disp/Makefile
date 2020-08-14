#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR     := $(shell pwd)/../../../../..
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================

ifeq ($(CFG_HI_MINIBOOT_SUPPORT),y)
BOOT_DIR    := $(SDK_DIR)/source/boot/miniboot
else
BOOT_DIR    := $(SDK_DIR)/source/boot/fastboot
endif

ifneq ($(O),)
PRODUCT_DIR := $(O)/product
else
PRODUCT_DIR := $(BOOT_DIR)/../product
endif

COMMON_DIR  := $(PRODUCT_DIR)/driver/common
DISP_DIR    := $(PRODUCT_DIR)/driver/disp

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716cv200 hi3716mv400 hi3718cv100 hi3719cv100 hi3718mv100 hi3719mv100),)
HAL_DIR := cv200
DRIVER_PATH := vdp_v1_0
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100),)
HAL_DIR := 3798m
DRIVER_PATH := vdp_v2_0
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420),)
HAL_DIR := mv410
DRIVER_PATH := vdp_v3_0
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798cv200)
HAL_DIR := 98cv200
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798mv200)
HAL_DIR := 98mv200
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798mv200_a)
HAL_DIR := 98mv200_a
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798mv200 hi3798mv200_a),)
CFLAGS += -DFPGA_98CV200
DRIVER_PATH := vdp_v4_0
endif

CFLAGS  += -D__DISP_PLATFORM_BOOT__ -D__DISP_SELF_START__ -D__DISP_D0_FOLLOW_D1__

ifeq ($(CFG_HI_DISP_USE_QPLL_SOLELY), y)
CFLAGS += -DHI_DISP_DOUBLE_HD_SUPPORT
endif

ifeq ($(CFG_HIFB_CURSOR_SUPPORT), y)
EXTRA_CFLAGS += -DHI_GFX_USE_G3
endif

ifeq ($(CFG_HI_VIDEO_BUFF_TYPE0), y)
EXTRA_CFLAGS += -DHI_VIDEO_BUFF_TYPE0
endif

ifeq ($(CFG_HI_VDP_VDAC_FULL_DRIVE), y)
CFLAGS += -DHI_VO_VDAC_FULL_DRIVE
endif

ifeq ($(CFG_HI_VDP_VDAC_HALF_DRIVE), y)
CFLAGS += -DHI_VO_VDAC_HALF_DRIVE
endif

ifeq ($(CFG_HI_VDP_VDAC_QUARTER_DRIVE), y)
CFLAGS += -DHI_VO_VDAC_QUARTER_DRIVE
endif


CFLAGS  += -I$(PRODUCT_DIR)/driver/include
CFLAGS  += -I$(PRODUCT_DIR)/driver/include_inc
CFLAGS  += -I$(BOOT_DIR)/arch/arm/include
CFLAGS  += -I$(BOOT_DIR)/include
CFLAGS  += -I$(SDK_DIR)/source/boot/product/driver/include

CFLAGS  += -I$(DISP_DIR)/$(DRIVER_PATH)/alg
CFLAGS  += -I$(DISP_DIR)/$(DRIVER_PATH)/com
CFLAGS  += -I$(DISP_DIR)/$(DRIVER_PATH)/drv
CFLAGS  += -I$(DISP_DIR)/$(DRIVER_PATH)/hal/common
CFLAGS  += -I$(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)
CFLAGS  += -I$(DISP_DIR)/$(DRIVER_PATH)/hal/$(HAL_DIR)/inc
CFLAGS  += -I$(SDK_DIR)/source/common/drv/include/hi3798cv200

LIBS    := libdisp.a

include $(DISP_DIR)/include_src.mak

OBJS    := $(SRC:%.c=%.o) 

#===============================================================================
# rules
#===============================================================================

.PHONY: all clean install uninstall $(LIBS) prepare

all:  $(LIBS)

prepare:
	$(AT)rm -rf $(DISP_DIR)/$(DRIVER_PATH)
	cp -rf $(MSP_DIR)/drv/vo/$(DRIVER_PATH)  $(DISP_DIR)/	
	find . -name "*.o" -o -name "*.so" -o -name "*.a" |xargs rm -fr
	
clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS)
	$(AT)rm -rf $(DISP_DIR)/$(DRIVER_PATH)
	
install: all

uninstall:

$(LIBS): $(OBJS)
	$(AT)$(AR) -rc $@ $^
