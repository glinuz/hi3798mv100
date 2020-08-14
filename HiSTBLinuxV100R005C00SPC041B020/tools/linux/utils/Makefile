#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ifeq ("$(origin O)", "command line")
SERVER_UNTILS_INSTALL := $(O)
else
SERVER_UNTILS_INSTALL := $(HI_OUT_DIR)/tools/linux/utils
endif

export CC       := gcc

MKCRAMFS        := $(SERVER_UNTILS_INSTALL)/mkfs.cramfs
MKEXT4FS        := $(SERVER_UNTILS_INSTALL)/make_ext4fs
MKSQUASHFS      := $(SERVER_UNTILS_INSTALL)/mksquashfs
MKJFFS2         := $(SERVER_UNTILS_INSTALL)/mkfs.jffs2
MKYAFFS         := $(SERVER_UNTILS_INSTALL)/mkyaffs
FILECAP         := $(SERVER_UNTILS_INSTALL)/filecap
MKLOADERDB      := $(SERVER_UNTILS_INSTALL)/mkloaderdb
MKDEVICEINFO    := $(SERVER_UNTILS_INSTALL)/mkdeviceinfo
MKUBIFS         := $(SERVER_UNTILS_INSTALL)/mkfs.ubifs
UBINIZE         := $(SERVER_UNTILS_INSTALL)/ubinize
MKUBIIMG        := $(SERVER_UNTILS_INSTALL)/mkubiimg.sh
LZMA            := $(SERVER_UNTILS_INSTALL)/lzma
MKIMAGE         := $(SERVER_UNTILS_INSTALL)/mkimage
MKADVCA         := $(SERVER_UNTILS_INSTALL)/advca
MKBOOTIMG       := $(SERVER_UNTILS_INSTALL)/mkbootimg
CPIO            := $(SERVER_UNTILS_INSTALL)/cpio
MKBOOTARGS      := $(SERVER_UNTILS_INSTALL)/mkbootargs

export RULEATTR_LIB := $(CURDIR)/libs/libruleattr.a
export ZLIB     := $(CURDIR)/libs/libz.a
TOOLS_BIN_PATH	:= $(CURDIR)/bin

ifeq ($(CFG_PRODUCT_TYPE), linux)
TOOLS_LIST  :=  $(MKUBIFS) $(UBINIZE) $(MKUBIIMG)

TOOLS_LIST  +=  $(MKLOADERDB) $(MKDEVICEINFO) $(MKIMAGE) $(FILECAP) $(CPIO) \
                $(LZMA) $(MKBOOTIMG) $(MKCRAMFS) $(MKEXT4FS) \
                $(MKSQUASHFS) $(MKJFFS2) $(MKYAFFS) \
                $(MKBOOTARGS)

ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
#TOOLS_LIST  +=  $(MKADVCA)
endif
else ifeq ($(CFG_PRODUCT_TYPE), android)
TOOLS_LIST  :=  $(MKUBIFS) $(UBINIZE) $(MKUBIIMG) $(MKSQUASHFS) $(CPIO) $(MKIMAGE) $(MKBOOTARGS)
endif

#-------------------------------------------
.PHONY: all clean ruleattr

all: $(TOOLS_LIST)
	$(AT)test -d $(SERVER_UNTILS_INSTALL) || mkdir -p $(SERVER_UNTILS_INSTALL)
	$(AT)cp -f $(TOOLS_BIN_PATH)/* $(SERVER_UNTILS_INSTALL)

clean:
	$(AT)rm -rf $(SERVER_UNTILS_INSTALL)/*

$(MKCRAMFS) $(MKSQUASHFS) $(LZMA) $(MKEXT4FS) $(MKJFFS2) $(MKUBIFS) $(UBINIZE) $(MKUBIIMG) $(CPIO): ruleattr

ruleattr:
	$(AT)test -f $(RULEATTR_LIB) || (make -C ruleattr PREFIX=$(SERVER_UNTILS_INSTALL)/ruleattr && \
		cp -rf $(SERVER_UNTILS_INSTALL)/ruleattr/libruleattr.a $(RULEATTR_LIB))

$(MKYAFFS):
	$(AT)test -f $(TOOLS_BIN_PATH)/mkyaffs || ( \
		make -C mkyaffs2image PREFIX=$(SERVER_UNTILS_INSTALL)/mkyaffs2image -j1 && \
		cp -rf $(SERVER_UNTILS_INSTALL)/mkyaffs2image/tmp/mkyaffs2image/mkyaffs  $(TOOLS_BIN_PATH))

squashfs:
	$(AT)(test -f $(TOOLS_BIN_PATH)/mksquashfs && test -f $(TOOLS_BIN_PATH)/lzma) || ( \
		make -C squashfs4.3 PREFIX=$(SERVER_UNTILS_INSTALL)/squashfs4.3 -j1 && \
		cp -rf $(SERVER_UNTILS_INSTALL)/squashfs4.3/mksquashfs $(TOOLS_BIN_PATH) && \
		cp -rf $(SERVER_UNTILS_INSTALL)/squashfs4.3/lzma $(TOOLS_BIN_PATH))

$(MKSQUASHFS) $(LZMA): squashfs

ext4fs:
	$(AT)test -f $(TOOLS_BIN_PATH)/make_ext4fs || (make -C ext4_utils PREFIX=$(SERVER_UNTILS_INSTALL)/ext4_utils -j1 && \
	cp -rf $(SERVER_UNTILS_INSTALL)/ext4_utils/make_ext4fs $(TOOLS_BIN_PATH))

$(MKEXT4FS) : ext4fs

$(MKLOADERDB):
	$(AT)make -C mkloaderdb-tool PREFIX=$(SERVER_UNTILS_INSTALL)/mkloaderdb-tool -j1
	$(AT)cp -rf $(SERVER_UNTILS_INSTALL)/mkloaderdb-tool/mkloaderdb $(SERVER_UNTILS_INSTALL)

$(MKDEVICEINFO):
	$(AT)make -C mkdeviceinfo-tool PREFIX=$(SERVER_UNTILS_INSTALL)/mkdeviceinfo-tool -j1
	$(AT)cp -rf $(SERVER_UNTILS_INSTALL)/mkdeviceinfo-tool/mkdeviceinfo $(SERVER_UNTILS_INSTALL)

ubifs:
	$(AT)(test -f $(TOOLS_BIN_PATH)/mkfs.jffs2 && \
		test -f $(TOOLS_BIN_PATH)/mkfs.ubifs && \
		test -f $(TOOLS_BIN_PATH)/ubinize && \
		test -f $(TOOLS_BIN_PATH)/mkubiimg.sh) || \
		(make -C mtd-utils PREFIX=$(SERVER_UNTILS_INSTALL)/mtd-utils -j1 && \
		cp -rf $(SERVER_UNTILS_INSTALL)/mtd-utils/mkfs.jffs2 $(TOOLS_BIN_PATH) && \
		cp -rf $(SERVER_UNTILS_INSTALL)/mtd-utils/mkfs.ubifs $(TOOLS_BIN_PATH) && \
		cp -rf $(SERVER_UNTILS_INSTALL)/mtd-utils/ubinize $(TOOLS_BIN_PATH) && \
		cp -rf mtd-utils/mkubiimg.sh $(TOOLS_BIN_PATH))

$(MKJFFS2) $(MKUBIFS) $(UBINIZE) $(MKUBIIMG): ubifs

$(MKBOOTARGS):
	$(AT)test -f $(TOOLS_BIN_PATH)/mkbootargs || (make -C mkbootargs-tool PREFIX=$(SERVER_UNTILS_INSTALL)/mkbootargs-tool -j1 && \
		cp -rf $(SERVER_UNTILS_INSTALL)/mkbootargs-tool/mkbootargs $(TOOLS_BIN_PATH))

$(FILECAP):
	$(AT)test -f $(TOOLS_BIN_PATH)/filecap || (make -C libcap-ng PREFIX=$(SERVER_UNTILS_INSTALL)/libcap-ng -j1 && \
		cp -rf $(SERVER_UNTILS_INSTALL)/libcap-ng/libcap-ng-0.7.7/INSTALL_DIR/usr/bin/filecap $(TOOLS_BIN_PATH))

$(MKCRAMFS):
	$(AT)test -f $(TOOLS_BIN_PATH)/mkfs.cramfs || (make -C util-linux PREFIX=$(SERVER_UNTILS_INSTALL)/util-linux -j1 && \
		cp -rf $(SERVER_UNTILS_INSTALL)/util-linux/mkfs.cramfs $(TOOLS_BIN_PATH))

$(MKIMAGE):
	$(AT)test -f $(TOOLS_BIN_PATH)/mkimage || (make -C mkimage-tool PREFIX=$(SERVER_UNTILS_INSTALL)/mkimage-tool -j1 && \
		cp -rf $(SERVER_UNTILS_INSTALL)/mkimage-tool/mkimage $(TOOLS_BIN_PATH))

$(MKADVCA):
	$(AT)cp -rf $(SERVER_UNTILS_INSTALL)/advca/Linux_CA_Boot_Sign/ca_sign_tool $(SERVER_UNTILS_INSTALL)

$(MKBOOTIMG):
	$(AT)test -f $(TOOLS_BIN_PATH)/mkbootimg || (make -C mkbootimg-tool PREFIX=$(SERVER_UNTILS_INSTALL)/mkbootimg-tool -j1 && \
		cp -rf $(SERVER_UNTILS_INSTALL)/mkbootimg-tool/mkbootimg-tool/mkbootimg $(TOOLS_BIN_PATH))

$(CPIO):
	$(AT)test -f $(TOOLS_BIN_PATH)/cpio || (make -C cpio-tool PREFIX=$(SERVER_UNTILS_INSTALL)/cpio-tool -j1 && \
		cp -rf $(SERVER_UNTILS_INSTALL)/cpio-tool/cpio $(TOOLS_BIN_PATH))
