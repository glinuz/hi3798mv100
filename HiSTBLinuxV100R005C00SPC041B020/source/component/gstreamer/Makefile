#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

ifndef GST_DIR
#export GST_DIR := $(PWD)
export GST_DIR := $(COMPONENT_DIR)/gstreamer
endif

OUT_DIR := $(SDK_DIR)/out/$(CFG_HI_CHIP_TYPE)/$(CFG_HI_OUT_DIR)

# temp variable ,build release version
#gst_build_type := rls
gst_build_type := dbg
gst_prebuild := false

all: install
	mkdir -p $(OUT_DIR)/include/gstreamer/
	cp -rf $(GST_DIR)/prebuild/include/* $(OUT_DIR)/include/gstreamer/
	mkdir -p $(OUT_DIR)/lib/share/
	cp -rf $(GST_DIR)/prebuild/lib/* $(OUT_DIR)/lib/share/
	mkdir -p $(OUT_DIR)/obj/rootfs/usr/lib
	cp -rf $(GST_DIR)/prebuild/lib/* $(OUT_DIR)/obj/rootfs/usr/lib/
	mkdir -p $(OUT_DIR)/obj/rootfs/usr/bin/
	cp -rf $(GST_DIR)/prebuild/bin/* $(OUT_DIR)/obj/rootfs/usr/bin/
	mkdir -p $(OUT_DIR)/obj/rootfs/usr/data/
	cp -rf $(GST_DIR)/prebuild/config/* $(OUT_DIR)/obj/rootfs/usr/data/

	@echo "============ gstreamer all success ============"

gstreamer:
	@echo "============  start complie gstreamer ============ "
ifeq ($(gst_prebuild),false)
	cd $(GST_DIR)/build/sme;pwd; \
	source ./envsetup.sh $(gst_build_type); \
	make all;
endif

	@echo "============= gstreamer complie success ============"
install: gstreamer
ifeq ($(gst_prebuild),false)
	mkdir -p $(GST_DIR)/prebuild
	mkdir -p $(GST_DIR)/prebuild/lib
	mkdir -p $(GST_DIR)/prebuild/config
	mkdir -p $(GST_DIR)/prebuild/include
	mkdir -p $(GST_DIR)/prebuild/bin
	cp $(GST_DIR)/source/sme/prebuilt/HiSTBLinux.$(CFG_HI_CHIP_TYPE)/$(gst_build_type)/libhwsecurec.so $(GST_DIR)/prebuild/lib -rf
	cp $(GST_DIR)/out/HiSTBLinux.$(CFG_HI_CHIP_TYPE)/$(gst_build_type)/lib/* $(GST_DIR)/prebuild/lib -rf
	cp $(GST_DIR)/out/HiSTBLinux.$(CFG_HI_CHIP_TYPE)/$(gst_build_type)/config/gstomx.conf $(GST_DIR)/prebuild/config -rf
	cp $(GST_DIR)/out/HiSTBLinux.$(CFG_HI_CHIP_TYPE)/$(gst_build_type)/bin/* $(GST_DIR)/prebuild/bin -rf

	cp $(GST_DIR)/source/sme/api/* $(GST_DIR)/prebuild/include -rf
	cp $(GST_DIR)/source/sme/source/osal/inc/osal_type.h  $(GST_DIR)/prebuild/include -rf
	cp $(GST_DIR)/source/sme/source/common/inc/sme_property.h $(GST_DIR)/prebuild/include -rf
	cp $(GST_DIR)/source/sme/source/common/inc/sme_metadata.h $(GST_DIR)/prebuild/include -rf
	cp $(GST_DIR)/source/sme/source/common/inc/sme_media_type.h $(GST_DIR)/prebuild/include -rf
endif

	@echo "============= gstreamer install success ============"

uninstall: clean
	cd $(GST_DIR); \
	rm -rf prebuild;
	@echo "============ gstreamer uninstall success ============"

clean:
	@echo "============ gstreamer clean start============"
	cd $(GST_DIR)/build/sme; make clean;
	cd $(GST_DIR); \
        rm -rf out;
	@echo "============ gstreamer clean success ============"

