#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR     := $(shell pwd)/../../../../../..
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

CFLAGS += -DHDMI_BUILD_IN_BOOT
ifeq ($(CFG_HI_MINIBOOT_SUPPORT),y)
CFLAGS += -DHDMI_BUILD_IN_MINIBOOT
endif
CFLAGS += -DCONFIG_HDMI_STB_SDK
CFLAGS += -DHDMI_SCDC_SUPPORT
#open it when emi is enable default
#CFLAGS += -DHDMI_EMI_ENABLE

ifeq ($(findstring fpga, $(CFG_HI_BOOT_REG_NAME)), fpga)
CFLAGS += -DHDMI_FPGA_SUPPORT
endif

####################################################################
# silicon image HDMI2.0 start
####################################################################
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200_a hi3798cv200_b ),)
HAL	         := imagev200
HAL_DIR      := hal/ctrl/$(HAL)
HDMI_PRODUCT := product/HifoneB02
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200),)
HAL          := imagev200
HAL_DIR      := hal/ctrl/$(HAL)
EMI_DIR      := hal/emi
HDMI_PRODUCT := product/Hi3798CV200
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798cv200_a hi3798cv200_b ),)
CFLAGS 	 += -DHDMI_SUPPORT_LOGIC_SILICON

CFLAGS   += -I$(PRODUCT_DIR)/driver/include
CFLAGS   += -I$(PRODUCT_DIR)/driver/include_inc
CFLAGS   += -I$(BOOT_DIR)/arch/arm/include
CFLAGS   += -I$(BOOT_DIR)/include
CFLAGS	 += -I$(SDK_DIR)/source/common/include
CFLAGS	 += -I$(SDK_DIR)/source/common/drv/include/$(CFG_HI_CHIP_TYPE)
CFLAGS	 += -I$(PRODUCT_DIR)/logo/include
CFLAGS	 += -I$(PRODUCT_DIR)/driver/higo/include
CFLAGS	 += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0
CFLAGS	 += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/hal
CFLAGS 	 += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/hal/emi
CFLAGS	 += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/osal/hisiv200
CFLAGS	 += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/$(HDMI_PRODUCT)

CFLAGS	 += -I$(HAL_DIR)
CFLAGS	 += -I$(HAL_DIR)/driver
CFLAGS	 += -I$(HAL_DIR)/driver/common
CFLAGS	 += -I$(HAL_DIR)/driver/cra
CFLAGS	 += -I$(HAL_DIR)/driver/tx
CFLAGS	 += -I$(HAL_DIR)/driver/tx/tx_hdmi
CFLAGS	 += -I$(HAL_DIR)/driver/tx/scdc
CFLAGS	 += -I$(HAL_DIR)/driver/tx/videopath
CFLAGS	 += -I$(HAL_DIR)/library/log
CFLAGS	 += -I$(HAL_DIR)/library/malloc
CFLAGS	 += -I$(HAL_DIR)/library/obj
CFLAGS	 += -I$(HAL_DIR)/library/time
CFLAGS	 += -I$(HAL_DIR)/platform

OBJS    := $(HAL_DIR)/hdmi_hal.o		
OBJS	+= $(HAL_DIR)/../../emi/hdmi_emi.o
OBJS    += $(HAL_DIR)/driver/common/si_drv_common.o \
           $(HAL_DIR)/driver/tx/si_drv_tx.o \
           $(HAL_DIR)/driver/tx/tx_hdmi/si_mod_tx_hdmi.o \
           $(HAL_DIR)/driver/tx/videopath/si_mod_videopath.o   \
           $(HAL_DIR)/driver/tx/videopath/si_mod_videopath_cra.o \
           $(HAL_DIR)/driver/cra/si_drv_cra.o	\
           $(HAL_DIR)/library/log/si_lib_log.o \
           $(HAL_DIR)/library/malloc/si_lib_malloc.o \
           $(HAL_DIR)/library/obj/si_lib_obj.o \
           $(HAL_DIR)/library/time/si_lib_time.o \
           $(HAL_DIR)/platform/platform.o \
           $(HAL_DIR)/platform/sii_time.o 									
OBJS    += $(HAL_DIR)/driver/tx/scdc/si_mod_tx_scdc.o
endif

####################################################################
# silicon image HDMI2.0 end
####################################################################


####################################################################
# sel-develop HDMI2.0 start
####################################################################
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hifonev300 hifonev200 hi3798mv200 hi3796mv200 hi3798mv200_a),)
CFLAGS 	 += -DHDMI_SUPPORT_LOGIC_HISIV100

CFLAGS   += -DHDMI_EMI_ENABLE

EMI_DIR	 := hal/emi
CTRL_DIR := hal/ctrl/hisiv100
PHY_DIR  := hal/phy/hisiv100
HDMI_PRODUCT := product/hisiv100

#LINUX_VERSION := osal/hisiv200

CFLAGS 	 += -I.
CFLAGS 	 += -Ihal
CFLAGS   += -I$(PRODUCT_DIR)/driver/include
CFLAGS   += -I$(PRODUCT_DIR)/driver/include_inc
CFLAGS   += -I$(BOOT_DIR)/arch/arm/include
CFLAGS   += -I$(BOOT_DIR)/include
CFLAGS	 += -I$(SDK_DIR)/source/common/include
CFLAGS	 += -I$(SDK_DIR)/source/common/drv/include/$(CFG_HI_CHIP_TYPE)
CFLAGS	 += -I$(PRODUCT_DIR)/logo/include
CFLAGS	 += -I$(PRODUCT_DIR)/driver/higo/include
CFLAGS	 += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0
CFLAGS	 += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/hal
CFLAGS 	 += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/hal/emi
CFLAGS   += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/osal/hisiv200
CFLAGS   += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/$(HDMI_PRODUCT)
CFLAGS   += -I$(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/$(HDMI_PRODUCT)/regs

CFLAGS	 += -I$(HAL_DIR)/driver/common
CFLAGS	 += -I$(CTRL_DIR)
CFLAGS	 += -I$(PHY_DIR)
CFLAGS	 += -I$(PHY_DIR)/regs
CFLAGS	 += -I$(CTRL_DIR)/regs
CFLAGS	 += -I$(CTRL_DIR)/include
CFLAGS	 += -I$(EMI_DIR)

CFLAGS	 += -nostdlib

#CFLAGS += -I$(HDMI_DRV_DIR)/hal/emi


OBJS += $(CTRL_DIR)/regs/hdmi_reg_aon.o\
        $(CTRL_DIR)/regs/hdmi_reg_audio_path.o\
        $(CTRL_DIR)/regs/hdmi_reg_ctrl.o\
        $(CTRL_DIR)/regs/hdmi_reg_mcu.o\
        $(CTRL_DIR)/regs/hdmi_reg_tx.o\
        $(CTRL_DIR)/regs/hdmi_reg_video_path.o\

OBJS += $(CTRL_DIR)/hdmi_hal_intf.o\
        $(CTRL_DIR)/hdmi_hal_ncts.o\
        $(CTRL_DIR)/hdmi_hal_ctrl.o\
        $(CTRL_DIR)/hdmi_hal_scdc.o\
        $(CTRL_DIR)/hdmi_hal_ddc.o\

				
OBJS += $(PHY_DIR)/hdmi_hal_phy.o \
		$(PHY_DIR)/regs/hdmi_reg_dphy.o\
				
#OBJS += $(HDMI_VER)/hal/emi/hdmi_emi.o
							
#OBJS += $(HDMI_VER)/$(LINUX_VERSION)/hdmi_osal.o 	

OBJS += $(HDMI_PRODUCT)/regs/hdmi_reg_crg.o
OBJS += $(HDMI_PRODUCT)/hdmi_product_define.o

endif
####################################################################
# sel-develop HDMI2.0 endi
####################################################################



OBJS +=	boot_hdmi_intf.o        \
        drv_hdmi_infoframe.o    \
        drv_hdmi_common.o       \

LIBS    := ../libhdmi.a

#===============================================================================
# rules
#===============================================================================

.PHONY: all prepare clean install uninstall $(LIBS)

all: $(LIBS)

prepare:
	$(AT)cp -rf $(MSP_DIR)/drv/hdmi/hdmi_2_0/hal .
	$(AT)cp -rf $(MSP_DIR)/drv/hdmi/hdmi_2_0/product .
	$(AT)cp -rf $(MSP_DIR)/drv/hdmi/hdmi_2_0/drv_hdmi_infoframe.c .
	$(AT)cp -rf $(MSP_DIR)/drv/hdmi/hdmi_2_0/drv_hdmi_infoframe.h .
	$(AT)cp -rf $(MSP_DIR)/drv/hdmi/hdmi_2_0/drv_hdmi_common.h .
	$(AT)cp -rf $(MSP_DIR)/drv/hdmi/hdmi_2_0/drv_hdmi_common.c .
	$(AT)find  hal/ -name "*.o" |xargs rm -rf {}
	$(AT)find  product/ -name "*.o" |xargs rm -rf {}
clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS)
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/hal
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/product 
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/drv_hdmi_infoframe.c 
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/drv_hdmi_infoframe.h 
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/drv_hdmi_common.h
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/drv_hdmi_common.c

install: all

uninstall:

$(LIBS): $(OBJS)
	$(AT)$(AR) -rc $@ $^
	$(AT)rm -rf $(OBJS) $(EXTRA_CFLAGS)
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/hal
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/product 
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/drv_hdmi_infoframe.c 
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/drv_hdmi_infoframe.h 
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/drv_hdmi_common.h 
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/drv_hdmi_common.c
	$(AT)rm -rf $(PRODUCT_DIR)/driver/hdmi/hdmi_2_0/drv_hdmi_platform.h





	
