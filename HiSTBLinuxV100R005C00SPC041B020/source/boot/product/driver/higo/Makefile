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
BOOT_DIR  := $(SDK_DIR)/source/boot/miniboot
else
BOOT_DIR  := $(SDK_DIR)/source/boot/fastboot
endif

ifneq ($(O),)
PRODUCT_DIR := $(O)/product
else
PRODUCT_DIR := $(BOOT_DIR)/../product
endif

HIGO_DIR = .
HIFB_DIR = hifb

CFLAGS   += -DHI_BUILD_IN_BOOT -D__DISP_PLATFORM_BOOT__

ifeq ($(CFG_HI_MINIBOOT_SUPPORT),y)
CFLAGS   += -DHI_BUILD_IN_MINI_BOOT
endif

ifeq ($(CFG_HI_LOADER_BOOTLOADER),y)
CFLAGS   += -DHI_LOADER_BOOTLOADER
endif

#===============================================================================
#hifb adp olatform
#===============================================================================
HIFB_HAL_DIR  := hifb_v1_0
HIFB_ALG_DIR  := alg

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798hv100 hi3798mv200 hi3798mv200_a hi3796mv200),)
HIFB_HAL_DIR := hifb_v2_0
endif


TDE_HAL_DIR  := tde_v1_0

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a hi3796mv200),)
TDE_HAL_DIR  := tde_v2_0
endif

#===============================================================================
#tde include
#===============================================================================
CFLAGS   += -I$(MSP_DIR)/drv/tde/include         \
	    -I$(MSP_DIR)/drv/tde/adp/$(TDE_HAL_DIR) \
            -I$(MSP_DIR)/api/tde/include \
	    -I$(HIGO_DIR)/tde
#===============================================================================
# jpeg include
#===============================================================================
CFLAGS   += -I$(HIGO_DIR)/imagedecode                 \
	    -I$(MSP_DIR)/drv/jpeg/include             \
            -I$(MSP_DIR)/drv/jpeg/grc_cmm_inc
#===============================================================================
# gfx self include
#===============================================================================
CFLAGS   += -I$(HIGO_DIR)/gfx

#===============================================================================
# hifb include
#===============================================================================
CFLAGS   += -I$(HIGO_DIR)/hifb                           \
	    -I$(MSP_DIR)/drv/hifb/inc                    \
            -I$(MSP_DIR)/drv/hifb/include                \
            -I$(MSP_DIR)/drv/hifb/adp/inc                \
            -I$(MSP_DIR)/drv/hifb/adp/${HIFB_ALG_DIR}    \
            -I$(MSP_DIR)/drv/hifb/adp/${HIFB_HAL_DIR}

#===============================================================================
# others include
#===============================================================================
CFLAGS   += -I$(PRODUCT_DIR)/driver/include           \
            -I$(PRODUCT_DIR)/driver/include_inc       \
	    -I$(MSP_DIR)/drv/include

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a hi3796mv200),)
CFLAGS   += -I$(PRODUCT_DIR)/driver/disp/vdp_v4_0/component/
endif

CFLAGS   += -I$(BOOT_DIR)/include
CFLAGS   += -I$(BOOT_DIR)/include/linux/mtd

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798hv100 hi3798mv200 hi3798mv200_a hi3796mv200),)
CFLAGS += -DCONFIG_GFX_PQ
endif

ifeq ($(CFG_HI_SMMU_SUPPORT),y)
CFLAGS += -DCONFIG_GFX_MMU_CLOCK
endif

#===============================================================================
#tde src
#===============================================================================
SRC     += $(wildcard $(HIGO_DIR)/msp/drv/tde/*.c)
SRC     += $(wildcard $(HIGO_DIR)/msp/api/tde/*.c)
#===============================================================================
#jpeg src
#===============================================================================
SRC     += $(wildcard $(HIGO_DIR)/imagedecode/*.c)
SRC     += $(wildcard $(HIGO_DIR)/msp/drv/jpeg/*.c)
#===============================================================================
#gfx self src
#===============================================================================
SRC     += $(wildcard $(HIGO_DIR)/gfx/*.c)
SRC     += $(wildcard $(HIGO_DIR)/tde/*.c)
#===============================================================================
#hifb src
#===============================================================================
SRC     +=  $(HIGO_DIR)/msp/drv/hifb/adp/src/drv_hifb_wbc.c                   \
            $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_HAL_DIR}/drv_hifb_adp.c       \
            $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_HAL_DIR}/drv_hifb_hal.c

ifeq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798hv100 hi3798mv200 hi3798mv200_a hi3796mv200),)
SRC     +=  $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_ALG_DIR}/optm_alg_csc.c     \
            $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_ALG_DIR}/optm_alg_gzme.c    \
            $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_ALG_DIR}/optm_alg_gsharp.c  \
            $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_ALG_DIR}/optm_alg_zme_coef.c

endif

#===============================================================================
# obj file
#===============================================================================
OBJS := $(SRC:%.c=%.o)

#===============================================================================
# lib file name
#===============================================================================
LIBS := $(HIGO_DIR)/libhigo.a


#===============================================================================
# rules
#===============================================================================

.PHONY: all prepare clean install uninstall $(LIBS)


all: $(LIBS) 

prepare:
	$(AT)-test -e $(HIGO_DIR)/msp/drv/tde  || mkdir -p $(HIGO_DIR)/msp/drv/tde
	$(AT)-test -e $(HIGO_DIR)/msp/api/tde  || mkdir -p $(HIGO_DIR)/msp/api/tde
	$(AT)-test -e $(HIGO_DIR)/msp/drv/jpeg || mkdir -p $(HIGO_DIR)/msp/drv/jpeg
	$(AT)-test -e $(HIGO_DIR)/msp/drv/hifb/adp/src              ||  mkdir -p $(HIGO_DIR)/msp/drv/hifb/adp/src
	$(AT)-test -e $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_HAL_DIR}  ||  mkdir -p $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_HAL_DIR}
	$(AT)-test -e $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_ALG_DIR}  ||  mkdir -p $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_ALG_DIR}
	$(AT)cp -rf   $(MSP_DIR)/drv/jpeg/src/*.c     $(HIGO_DIR)/msp/drv/jpeg
	$(AT)cp -rf   $(MSP_DIR)/drv/jpeg/src_dec/*.c $(HIGO_DIR)/msp/drv/jpeg
	$(AT)cp -rf   $(MSP_DIR)/drv/tde/src/*.c $(HIGO_DIR)/msp/drv/tde
	$(AT)cp -rf   $(MSP_DIR)/drv/tde/adp/$(TDE_HAL_DIR)/*.c $(HIGO_DIR)/msp/drv/tde
	$(AT)cp -rf   $(MSP_DIR)/api/tde/*.c $(HIGO_DIR)/msp/api/tde
	$(AT)cp -rf   $(MSP_DIR)/drv/hifb/adp/src/drv_hifb_wbc.c       $(HIGO_DIR)/msp/drv/hifb/adp/src
	$(AT)cp -rf   $(MSP_DIR)/drv/hifb/adp/${HIFB_HAL_DIR}/*.c      $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_HAL_DIR}
ifeq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798hv100 hi3798mv200 hi3798mv200_a hi3796mv200),)
	$(AT)cp -rf   $(MSP_DIR)/drv/hifb/adp/${HIFB_ALG_DIR}/*.c  $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_ALG_DIR}
endif
	$(AT)rm -rf   $(HIGO_DIR)/msp/drv/tde/*.mod*
	$(AT)rm -rf   $(HIGO_DIR)/msp/drv/jpeg/*.mod*
	$(AT)rm -rf   $(HIGO_DIR)/msp/drv/hifb/adp/src/*.mod*
	$(AT)rm -rf   $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_HAL_DIR}/*.mod*
	$(AT)rm -rf   $(HIGO_DIR)/msp/drv/hifb/adp/${HIFB_ALG_DIR}/*.mod*

clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS)
	$(AT)rm -rf $(HIGO_DIR)/msp

install: all

uninstall:

$(LIBS):$(OBJS)
	$(AR) -rc $@ $^
	$(AT)rm -rf $(OBJS)

%.o:%.c
	$(AT)$(CC) $(CFLAGS) -c $^ -o $@
