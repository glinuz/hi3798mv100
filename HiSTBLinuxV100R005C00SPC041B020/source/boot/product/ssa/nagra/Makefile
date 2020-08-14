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
CFLAGS += -I$(SSA_DIR)/nagra/include
CFLAGS += -I$(SSA_DIR)/nagra/adp


#===============================================================================
# rules
#===============================================================================
source := $(wildcard *.c adp/*.c)

SUBLIBS := ssa_lib/libsbp_rel.a

########################################################################################
objs=$(patsubst %.c,%.o, $(source))

OBJ_LIBS := libobj.a

SUBLIBS += $(OBJ_LIBS)

LIBS := libssa.a

all:$(LIBS)

$(LIBS): $(OBJ_LIBS) $(VMX_STUB_LIB)
	$(AT)rm -rf $@
	$(AT)rm -rf ar.mak
	$(AT)echo "CREATE ../$@" >> ar.mak
	$(AT)$(foreach sublib,$(SUBLIBS),echo "ADDLIB $(sublib)" >> ar.mak;)
	$(AT)echo "SAVE" >> ar.mak
	$(AT)echo "END" >> ar.mak
	$(AT)$(AR) -M < ar.mak
	$(AT)$(RM) -f $(OBJ_LIBS)

$(OBJ_LIBS):$(objs)
	$(AT)echo "ar: $^ *************************"
	$(AT)$(AR) -rc -o $@ $^

%.o: %.c
	$(AT)echo "cc: $^ *************************"
	$(AT)$(CC) $(CFLAGS) -c $^ -o "$(*D)/$(*F).o"

install: all

uninstall:

clean:
	$(AT)$(RM) *.o -rvf
	$(AT)$(RM) -rf $(objs)
	$(AT)$(RM) -f $(OBJ_LIBS)
	$(AT)$(RM) -f $(SSA_DIR)/$(LIBS)
	$(AT)$(RM) -rf ar.mak
	$(AT)echo "make $@ over !"
