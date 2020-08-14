ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell pwd)/../../../..
include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

objects := api

ifneq ($(CFG_HI_ADVCA_SUPPORT), y)
objects += schedule
endif

ifeq ($(CFG_HI_LOADER_BOOTLOADER),y)
objects += app
endif

objects_clean:=$(addsuffix _clean,$(objects))
objects_install:=$(addsuffix _install,$(objects))
objects_uninstall:=$(addsuffix _uninstall,$(objects))

depends_clean:=$(addsuffix _clean,$(depends))
depends_install:=$(addsuffix _install,$(depends))
depends_uninstall:=$(addsuffix _uninstall,$(depends))

LIBS := libloader.a
SUBLIBS := $(foreach sub,$(objects),$(sub)/lib$(sub).a)

$(LIBS): $(objects) 
	$(AT)rm -rf $@
	$(AT)rm -rf ar.mak
	$(AT)echo "CREATE $@" >> ar.mak
	$(AT)$(foreach sublib,$(SUBLIBS),echo "ADDLIB $(sublib)" >> ar.mak;)
	$(AT)echo "SAVE" >> ar.mak
	$(AT)echo "END" >> ar.mak
	$(AT)$(AR) -M < ar.mak

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#==================================================================
#                          all
#==================================================================
.PHONY: all clean install uninstall prepare
prepare:
	$(AT)rm -rf api/include 
	$(AT)rm -rf api/src
	
	$(AT)mkdir api/include 
	$(AT)mkdir api/src 


	$(AT)cp -r $(COMPONENT_DIR)/loader/api/include/hi_loader_info_v2.h api/include/
	
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_debug.h api/src/	
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_osal.h api/src/
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_osal.c api/src/
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_crc32.h api/src/
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_crc32.c api/src/		
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_dbinfo.h api/src/
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_dbinfo.c api/src/
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_dbharden.h api/src/
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_dbharden.c api/src/

ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_cipher.c api/src/
	$(AT)cp -r $(COMPONENT_DIR)/loader/api/src/loader_cipher.h api/src/
endif

all: $(LIBS)
#	$(AT)$(STRIP) $(LIBS)

clean: $(objects_clean)
	$(AT)rm -rf $(LIBS)
	$(AT)rm -rf ar.mak

install: $(objects_install)

uninstall: $(objects_uninstall)

#==================================================================
#                          modules
#==================================================================
.PHONY: $(objects) $(objects_clean) $(objects_install) $(objects_uninstall)

$(objects): | prepare
	make -C $@ all

$(objects_clean):
	make -C $(patsubst %_clean,%,$@) clean

$(objects_install):
	make -C $(patsubst %_install,%,$@) install

$(objects_uninstall):
	$(AT)echo "uninstall loader"
	#make -C $(patsubst %_uninstall,%,$@) uninstall

