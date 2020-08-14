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

COMMON_DIR  := $(PRODUCT_DIR)/driver/common

CFLAGS   += -I$(PRODUCT_DIR)/driver/include
CFLAGS   += -I$(PRODUCT_DIR)/driver/include_inc
CFLAGS   += -I$(SDK_DIR)/source/common/include
CFLAGS   += -I$(BOOT_DIR)/include

LIBS     := libkeyled.a

ifeq ($(CFG_HI_KEYLED_PT6964_SELECT),y)
    OBJS :=./keyled_pt6964/hi_keyled.o
ifdef CFG_HI_KEYLED_PT6964_CLOCK_GPIO
    CFLAGS += -DHI_KEYLED_PT6964_CLOCK_GPIO=$(CFG_HI_KEYLED_PT6964_CLOCK_GPIO)
    CFLAGS += -DHI_KEYLED_PT6964_STB_GPIO=$(CFG_HI_KEYLED_PT6964_STB_GPIO)
    CFLAGS += -DHI_KEYLED_PT6964_DINOUT_GPIO=$(CFG_HI_KEYLED_PT6964_DINOUT_GPIO)
endif
endif

ifeq ($(CFG_HI_KEYLED_GPIOKEY_SELECT),y)
    OBJS :=./keyled_gpiokey/hi_keyled.o
    CFLAGS += -DHI_KEYLED_GPIOKEY_POWER_GPIO=$(CFG_HI_KEYLED_GPIOKEY_POWER_GPIO)
    CFLAGS += -DHI_KEYLED_GPIOKEY_OK_GPIO=$(CFG_HI_KEYLED_GPIOKEY_OK_GPIO)
    CFLAGS += -DHI_KEYLED_GPIOKEY_MENU_GPIO=$(CFG_HI_KEYLED_GPIOKEY_MENU_GPIO)
    CFLAGS += -DHI_KEYLED_GPIOKEY_UP_GPIO=$(CFG_HI_KEYLED_GPIOKEY_UP_GPIO)
    CFLAGS += -DHI_KEYLED_GPIOKEY_DOWN_GPIO=$(CFG_HI_KEYLED_GPIOKEY_DOWN_GPIO)
    CFLAGS += -DHI_KEYLED_GPIOKEY_LEFT_GPIO=$(CFG_HI_KEYLED_GPIOKEY_LEFT_GPIO)
    CFLAGS += -DHI_KEYLED_GPIOKEY_RIGHT_GPIO=$(CFG_HI_KEYLED_GPIOKEY_RIGHT_GPIO)
endif

ifeq ($(CFG_HI_KEYLED_PT6961_SELECT),y)
    OBJS :=./keyled_pt6961/hi_keyled.o
ifdef CFG_HI_KEYLED_PT6961_CLOCK_GPIO
    CFLAGS += -DHI_KEYLED_PT6961_CLOCK_GPIO=$(CFG_HI_KEYLED_PT6961_CLOCK_GPIO)
    CFLAGS += -DHI_KEYLED_PT6961_STB_GPIO=$(CFG_HI_KEYLED_PT6961_STB_GPIO)
    CFLAGS += -DHI_KEYLED_PT6961_DIN_GPIO=$(CFG_HI_KEYLED_PT6961_DIN_GPIO)
    CFLAGS += -DHI_KEYLED_PT6961_DOUT_GPIO=$(CFG_HI_KEYLED_PT6961_DOUT_GPIO)
endif
endif

#ifeq ($(CFG_HI_KEYLED_CT1642_SELECT),y)
#    ifeq ($(findstring hi3716cv200,$(CFG_HI_CHIP_TYPE)), hi3716cv200)
#        OBJS :=./keyled_ct1642/hi_keyled_inner.o
#    else
#        OBJS :=./keyled_ct1642/hi_keyled.o
#    ifdef CFG_HI_KEYLED_CT1642_CLOCK_GPIO
#        CFLAGS += -DHI_KEYLED_CT1642_CLOCK_GPIO=$(CFG_HI_KEYLED_CT1642_CLOCK_GPIO)
#        CFLAGS += -DHI_KEYLED_CT1642_DAT_GPIO=$(CFG_HI_KEYLED_CT1642_DAT_GPIO)
#        CFLAGS += -DHI_KEYLED_CT1642_KEY_GPIO=$(CFG_HI_KEYLED_CT1642_KEY_GPIO)
#    endif
#    endif
#endif

ifeq ($(CFG_HI_KEYLED_CT1642_SELECT),y)
  OBJS :=./keyled_ct1642/hi_keyled_inner.o
endif

ifeq ($(CFG_HI_KEYLED_74HC164_SELECT),y)
  OBJS :=./keyled_std/hi_keyled.o
endif

ifeq ($(CFG_HI_KEYLED_FD650_SELECT),y)
  OBJS :=./keyled_fd650/hi_keyled.o
ifdef CFG_HI_KEYLED_FD650_CLOCK_GPIO
    CFLAGS += -DHI_KEYLED_FD650_CLOCK_GPIO=$(CFG_HI_KEYLED_FD650_CLOCK_GPIO)
    CFLAGS += -DHI_KEYLED_FD650_DINOUT_GPIO=$(CFG_HI_KEYLED_FD650_DINOUT_GPIO)
endif
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
