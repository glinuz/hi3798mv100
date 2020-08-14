################################################################################
PREFIX           ?= $(shell pwd)/../rootfs
WIFI_DEVICE      ?= usb_ar9271
WIFI_MODE        ?= ap
################################################################################
BUILD_DIR         = $(shell pwd)
TARGET_DIR        = $(PREFIX)/usr/sbin
UTILS :=
ifeq ($(findstring sta,$(WIFI_MODE)),sta)
UTILS += sta_connect.sh
endif
ifeq ($(findstring ap,$(WIFI_MODE)),ap)
UTILS += ap_start.sh start_route.sh
endif
UTILS := $(addprefix $(TARGET_DIR)/,$(UTILS))
################################################################################
all: $(UTILS)

$(TARGET_DIR)/%.sh: $(BUILD_DIR)/%.sh
	@test -d $(@D) || mkdir -p $(@D)
	@cp -vf $(BUILD_DIR)/$(notdir $@) $@
	@chmod 777 $@

#################################################################################

clean:
	@-rm -fv $(UTILS)

distclean: clean

################################################################################
.PHONY: clean distclean all
################################################################################
