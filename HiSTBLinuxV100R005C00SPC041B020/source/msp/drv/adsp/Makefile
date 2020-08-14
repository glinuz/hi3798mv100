#=======================================
#	export variable
#=======================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

#=======================================
#	local variable
#=======================================
objects := adsp_v1_1

obj-y += $(addsuffix /,$(objects))

objects_clean       := $(addsuffix _clean, $(objects))
objects_install     := $(addsuffix _install, $(objects))
objects_uninstall   := $(addsuffix _uninstall, $(objects))
#=================================================================
#	  rules
#=================================================================
.PHONY: all clean install uninstall $(objects) $(objects_clean) $(objects_install) $(objects_uninstall)

all: $(objects)

clean: $(objects_clean)

install: $(objects_install)

uninstall: $(objects_uninstall)

$(objects):
	$(AT)make -C $@

$(objects_clean):
	$(AT)make -C $(patsubst %_clean,%,$@) clean

$(objects_install):
	$(AT)make -C $(patsubst %_install,%,$@) install

$(objects_uninstall):
	$(AT)make -C $(patsubst %_uninstall,%,$@) uninstall
