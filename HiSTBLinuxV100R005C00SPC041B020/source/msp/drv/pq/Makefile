ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

ifeq ($(CFG_HI_PQ_V3_0),y)
PQ_DIR := pq_v3_0
endif

ifeq ($(CFG_HI_PQ_V4_0),y)
PQ_DIR := pq_v4_0
endif

obj-y += $(PQ_DIR)/

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#=================================================================
#     rules
#=================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(PQ_DIR) all

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install:
	$(AT)make -C $(PQ_DIR) install

uninstall:
	$(AT)make -C $(PQ_DIR) uninstall

