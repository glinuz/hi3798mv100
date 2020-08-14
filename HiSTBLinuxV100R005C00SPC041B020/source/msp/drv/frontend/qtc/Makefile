ifndef SRC_ROOT
export SRC_ROOT := $(PWD)/../../../..
endif

ifndef LINUXROOT
export LINUXROOT := $(SRC_ROOT)/osdrv/kernel/linux-2.6.35
endif

DRV_SRC_BASE=$(SRC_ROOT)/msp/ecs/drv/qtc_mv200
EEXTRA_CFLAGS += -I$(DRV_SRC_BASE)
EXTRA_CFLAGS += -I$(KCOM_DIR)/include -I$(KCOM_DIR)/drv/include
EXTRA_CFLAGS += -I$(KECS_DIR)/include -I$(KECS_DIR)/drv/include
EXTRA_CFLAGS += $(CFG_CFLAGS) -DSDK_VERSION=$(CFG_SDK_VERSION)

MOD_NAME := hi_qtc
obj-m := $(MOD_NAME).o

$(MOD_NAME)-y += qtc_test.o
$(MOD_NAME)-y += sv_unf_osal.o

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

all:
	make -C $(LINUXROOT) M=$(DRV_SRC_BASE) modules

clean:
	find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

