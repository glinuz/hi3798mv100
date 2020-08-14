#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR     := $(shell pwd)/../../../..
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

ANDROID_DIR := $(PRODUCT_DIR)/android

CFLAGS  += -I$(PRODUCT_DIR)/driver/include
ifeq ($(CFG_HI_MINIBOOT_SUPPORT),y)
CFLAGS  +=  -I$(BOOT_DIR)/arm/$(BOOTCONFIG)/include
CFLAGS  += -I$(BOOT_DIR)/app
else
CFLAGS  +=  -I$(BOOT_DIR)/arch/arm/include
endif
CFLAGS  += -I$(BOOT_DIR)/include
CFLAGS  += -I$(SOURCE_DIR)/common/include
CFLAGS  += -I$(SOURCE_DIR)/boot/product/driver/advca/common/include
CFLAGS  += -I$(SOURCE_DIR)/boot/product/driver/advca/common/ca
CFLAGS  += -I$(SOURCE_DIR)/boot/product/driver/advca/common/auth
CFLAGS  += -I$(SOURCE_DIR)/boot/product/driver/advca/common/cipher
CFLAGS  += -I$(SOURCE_DIR)/boot/product/driver/advca/common/alg
CFLAGS  += -I$(ANDROID_DIR)/

LIBS     := libandroid.a
OBJS     := customer_android.o fixmac.o recovery.o

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
