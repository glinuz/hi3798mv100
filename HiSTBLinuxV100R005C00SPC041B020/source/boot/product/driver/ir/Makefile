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
CFLAGS   += -I$(SDK_DIR)/source/common/include
CFLAGS   += -I$(BOOT_DIR)/include

LIBS     := libir.a

#OBJS += drv_ir.o
#OBJS += sample_test_ir.o
OBJS += cmd_ir.o	\
	nec.o		\
	rc6.o		\
	sony.o		\
	timer.o		\
	credit.o	\
	ir_drv.o	\
	ir_protocol_entry.o	\
	ir_utils.o	\
	rc5.o	\
	tc9012.o
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
