#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
objects := aacenc

objects_install := $(addsuffix _install,$(objects))
objects_uninstall := $(addsuffix _uninstall,$(objects))
objects_clean := $(addsuffix _clean,$(objects))
#===============================================================================
.PHONY: all clean install uninstall
#===============================================================================
all: $(objects)

clean: $(objects_clean)

install: $(objects_install)

uninstall: $(objects_uninstall)

#===============================================================================
.PHONY: $(objects) $(objects_clean) $(objects_install) $(objects_uninstall)
#===============================================================================
$(objects):
	$(AT)make -C $@

$(objects_clean):
	$(AT)make -C $(patsubst %_clean,%,$@) clean

$(objects_install):
	$(AT)make -C $(patsubst %_install,%,$@) install

$(objects_uninstall):
	$(AT)make -C $(patsubst %_uninstall,%,$@) uninstall
#===============================================================================
