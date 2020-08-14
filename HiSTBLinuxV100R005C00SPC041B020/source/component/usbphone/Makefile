#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(CURDIR)/../../..

include $(SDK_DIR)/base.mak
endif

USBPHONE_DIR = $(SDK_DIR)/source/component/usbphone

#===============================================================================
# local variable
#===============================================================================
CFLAGS  +=  -I$(COMMON_UNF_INCLUDE)                 \
            -I$(COMMON_API_INCLUDE)                 \
            -I$(USBPHONE_DIR)/include               

LIBS := libhi_usbphone

OBJS := $(USBPHONE_DIR)/src/hi_usbphone.o  

HEADER_FILES := $(USBPHONE_DIR)/include/hi_usbphone.h

#===============================================================================
# rules
#===============================================================================
.PHONY : all clean install uninstall

all : $(LIBS)

clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS).a
	$(AT)rm -rf $(LIBS).so

install: all
	$(AT)cp -f  $(LIBS).a           $(HI_STATIC_LIB_DIR)/
	$(AT)cp -f  $(LIBS).so          $(HI_SHARED_LIB_DIR)/
	$(AT)cp -f  $(HEADER_FILES)     $(HI_INCLUDE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_STATIC_LIB_DIR)/$(LIBS).a
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/$(LIBS).so
	$(AT)rm -rf $(foreach file, $(HEADER_FILES), $(HI_INCLUDE_DIR)/$(notdir $(file)))

$(LIBS) : $(OBJS)
	$(AT)$(AR) -rsv $@.a $^
	$(AT)$(CC) -shared -o $@.so $^
	
