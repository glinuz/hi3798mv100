#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

PREFIX ?= $(HI_ROOTFS_DIR)

ifeq ($(CFG_HI_64BIT_SYSTEM),y)
OBJ_NAME := obj64
else
OBJ_NAME := obj
endif

ifneq ($(HI_OUT_DIR),)
GDB_DIR := $(HI_OUT_DIR)/$(OBJ_NAME)/source/rootfs/gdb
else
GDB_DIR := $(CURDIR)
endif

################################################################################
HOST := $(HI_KERNEL_TOOLCHAINS_NAME)
STRIP := $(HOST)-strip

BUILD_DIR       := $(GDB_DIR)/tmp

GDB             := gdb-7.8.1
NCURSES         := ncurses-6.0
TARGETS         := bin/gdbserver bin/gdb

################################################################################
.PHONY: all clean
################################################################################
all: $(addprefix $(PREFIX)/,$(TARGETS))

clean:
	$(AT)rm -rf $(GDB_DIR)/$(GDB) $(GDB_DIR)/$(NCURSES) $(BUILD_DIR)

$(PREFIX)/%: $(BUILD_DIR)/%
	$(AT)test -d $(@D) || mkdir -p $(@D)
	$(AT)install -v -t $(dir $@) $<

$(addprefix $(BUILD_DIR)/,$(TARGETS)): $(GDB_DIR)/$(GDB)/tar $(BUILD_DIR)/lib/libncurses.a
	$(AT)cd $(GDB_DIR)/$(GDB) && make -j32 && make install

$(BUILD_DIR)/lib/libncurses.a: $(GDB_DIR)/$(NCURSES)/tar
	$(AT)cd $(GDB_DIR)/$(NCURSES) && make -j32 && make install

################################################################################
$(GDB_DIR)/$(GDB)/tar:
	$(AT)test -d $(GDB_DIR) || mkdir -p $(GDB_DIR)
	$(AT)test -d $(GDB_DIR)/$(GDB) || tar -xjf $(OPEN_SOURCE_DIR)/$(GDB).tar.bz2 -C $(GDB_DIR)
	$(AT)cd $(GDB_DIR)/$(GDB) && ./configure --target=$(HOST) --host=$(HOST) --prefix=$(BUILD_DIR) \
        --disable-tui CFLAGS="-g -O2" LDFLAGS="-L$(BUILD_DIR)/lib"
	$(AT)touch $@

$(GDB_DIR)/$(NCURSES)/tar:
	$(AT)test -d $(GDB_DIR) || mkdir -p $(GDB_DIR)
	$(AT)test -d $(GDB_DIR)/$(NCURSES) || tar -xjf $(OPEN_SOURCE_DIR)/$(NCURSES).tar.bz2 -C $(GDB_DIR)
	$(AT)cd $(GDB_DIR)/$(NCURSES) && CPPFLAGS="-P" ./configure --host=$(HOST) --without-ada \
        --without-manpages --prefix=$(BUILD_DIR)
	$(AT)touch $@
