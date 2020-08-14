#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
ALSA_DIR = $(COMPONENT_DIR)/alsa
BLUETOOTH_DIR = $(COMPONENT_DIR)/bluetooth
BLUEZ_API_DIR = $(BLUETOOTH_DIR)/hi_api

CFLAGS  +=  -I$(COMMON_UNF_INCLUDE) \
            -I$(COMMON_API_INCLUDE) \
            -I$(BLUETOOTH_DIR) \
            -I$(BLUETOOTH_DIR)/include \
            -I$(BLUETOOTH_DIR)/include/bluetooth \
            -I$(BLUETOOTH_DIR)/include/dbus-1.0/dbus \
            -I$(BLUETOOTH_DIR)/lib/dbus-1.0/include \
            -I$(BLUETOOTH_DIR)/lib/dbus-1.0/include/dbus                

LIBS := libhi_bluetooth

OBJS := bluetooth_api.o hi_bluetooth.o  

HEADER_FILES := $(BLUEZ_API_DIR)/hi_bluetooth*.h

#===============================================================================
# rules
#===============================================================================
.PHONY : all clean install uninstall

all : $(LIBS)

clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS).a
	$(AT)rm -rf $(LIBS).so

install:
	$(AT)test -d $(HI_STATIC_LIB_DIR) || mkdir -p $(HI_STATIC_LIB_DIR)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)cp -f $(LIBS).a $(HI_STATIC_LIB_DIR)/
	$(AT)cp -f $(LIBS).so $(HI_SHARED_LIB_DIR)/
	$(AT)cp -f $(HEADER_FILES) $(HI_INCLUDE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_STATIC_LIB_DIR)/$(LIBS).a
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/$(LIBS).so
	$(AT)rm -rf $(foreach file, $(HEADER_FILES), $(HI_INCLUDE_DIR)/$(notdir $(file)))

$(LIBS) : $(OBJS)
	$(AT)$(AR) -rsv $@.a $^
	$(AT)$(CC) -shared -o $@.so $^	
