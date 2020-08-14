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

CFLAGS   += -I$(PRODUCT_DIR)/driver/include
CFLAGS   += -I$(PRODUCT_DIR)/driver/include_inc
CFLAGS   += -I$(BOOT_DIR)/arch/arm/include
CFLAGS   += -I$(BOOT_DIR)/include
CFLAGS	 += -I$(SDK_DIR)/source/common/include
CFLAGS	 += -I$(PRODUCT_DIR)/driver/hdmi
CFLAGS	 += -I$(PRODUCT_DIR)/logo/include
CFLAGS	 += -I$(PRODUCT_DIR)/driver/higo/include

LIBS    := ../libhdmi.a

OBJS    :=  hdmi.o              \
            txvidp.o            \
            infofrm.o           \
            hdmitx.o            \
            drv_hlviic.o        \
            vmtables.o          \
            txvptbl.o           \
            phy.o               \
            timer.o             \
            si_vmode.o          \
            de.o

ifeq ($(CFG_HI_BUILD_WITH_HDMI_EDID),y)
OBJS += si_edid.o
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3796cv100 hi3798cv100 hi3798mv100 hi3796mv100 hi3716dv100),)
CFLAGS += -DHI_HDMI_4K_SUPPORT

OBJS += vsdb.o
endif

#===============================================================================
# rules
#===============================================================================

.PHONY: all clean install uninstall $(LIBS)

all: $(LIBS)

clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS)

install: all

uninstall:

$(LIBS): $(OBJS)
	$(AT)$(AR) -rc $@ $^
