#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../../..
include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
objects := common db flash disp higo hdmi i2c gpio pdm pq

ifeq ($(CFG_HI_BUILD_WITH_FRONTEND), y)
objects += frontend
endif

ifeq ($(CFG_HI_GPIOI2C_SUPPORT), y)
objects += gpioi2c
endif

ifeq ($(CFG_HI_BUILD_WITH_DEMUX), y)
objects += demux
endif

ifeq ($(CFG_HI_BUILD_WITH_IR), y)
objects += ir
endif

ifeq ($(CFG_HI_BUILD_WITH_KEYLED), y)
objects += keyled
endif

ifeq ($(CFG_HI_ADVCA_SUPPORT), y)
objects += cipher
objects += advca
objects += pmoc
endif

ifeq ($(CFG_HI_PM_POWERUP_MODE1_SUPPORT), y)
objects += pmoc
endif

objects += otp

objects_clean:=$(addsuffix _clean,$(objects))
objects_install:=$(addsuffix _install,$(objects))
objects_uninstall:=$(addsuffix _uninstall,$(objects))

LIBS := libdriver.a
SUBLIBS := $(foreach sub,$(objects),$(sub)/lib$(sub).a)

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#==================================================================
#                          all
#==================================================================
.PHONY: all clean install uninstall $(LIBS)

all: $(LIBS)

clean:  $(objects_clean)
	$(AT)rm -rf $(LIBS)
	$(AT)rm -rf ar.mak

install: $(objects_install)

uninstall: $(objects_uninstall)

$(LIBS): $(objects)
	$(AT)rm -rf $@
	$(AT)rm -rf ar.mak
	$(AT)echo "CREATE $@" >> ar.mak
	$(AT)$(foreach sublib,$(SUBLIBS),echo "ADDLIB $(sublib)" >> ar.mak;)
	$(AT)echo "SAVE" >> ar.mak
	$(AT)echo "END" >> ar.mak
	$(AT)$(AR) -M < ar.mak

#==================================================================
#                          modules
#==================================================================
.PHONY: $(objects) $(objects_clean) $(objects_install) $(objects_uninstall)

prepare:
	make -C higo prepare
	make -C disp prepare
	make -C pq prepare

$(objects): | prepare
	make -C $@ all

$(objects_clean):
	make -C $(patsubst %_clean,%,$@) clean

$(objects_install):
	make -C $(patsubst %_clean,%,$@) install

$(objects_uninstall):
	make -C $(patsubst %_clean,%,$@) uninstall
