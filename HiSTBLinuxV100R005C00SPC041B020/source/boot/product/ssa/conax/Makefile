################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR     := $(shell pwd)/../../../../..

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
ifeq ($(CFG_HI_MINIBOOT_SUPPORT),y)
BOOT_DIR := $(SDK_DIR)/source/boot/miniboot
else
BOOT_DIR := $(SDK_DIR)/source/boot/fastboot
endif

ifneq ($(O),)
PRODUCT_DIR := $(O)/product
else
PRODUCT_DIR := $(BOOT_DIR)/../product
endif

SSA_DIR := $(PRODUCT_DIR)/ssa
SUBLIBS := 

CFLAGS   += -I$(PRODUCT_DIR)/driver/include
CFLAGS   += -I$(PRODUCT_DIR)/driver/include_inc
CFLAGS   += -I$(BOOT_DIR)/arch/arm/include
CFLAGS   += -I$(BOOT_DIR)/include
CFLAGS	 += -I$(PRODUCT_DIR)/driver/higo/include
CFLAGS   += -I$(LOGO_DIR)/include
CFLAGS   += -I$(COMPONENT_DIR)/loader/api/include
CFLAGS   += -I$(COMPONENT_DIR)/loader/api/src

################################################################################
CFLAGS += -I$(SSA_DIR)/conax/include
CFLAGS += -I$(SSA_DIR)/conax/adp

#===============================================================================
# rules
#===============================================================================
source := $(wildcard *.c adp/*.c)

objs=$(patsubst %.c,%.o, $(source))

LIBS := libssa.a

all:$(LIBS)

$(LIBS):$(objs)
	$(AT)echo "ar: $^ *************************"
	$(AT)$(AR) -rc -o $(SSA_DIR)/$@ $^

%.o: %.c
	$(AT)echo "cc: $^ *************************"
	$(AT)$(CC) $(CFLAGS) -c $^ -o "$(*D)/$(*F).o"


install: all

uninstall:

clean:
	$(AT)$(RM) *.o -rvf
	$(AT)$(RM) -rf $(objs)
	$(AT)$(RM) -f $(SSA_DIR)/$(LIBS)
	$(AT)echo "make $@ over !"
