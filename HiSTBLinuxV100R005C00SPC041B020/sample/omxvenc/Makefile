#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SDK_DIR)/sample/base.mak

#===============================================================================
# local variable
#===============================================================================
CFLAGS  += -I$(SDK_DIR)/source/msp/api/omx/include
CFLAGS  += -I$(SDK_DIR)/source/common/include 

LIBPATH := -L$(HI_SHARED_LIB_DIR) -L$(HI_EXTERN_LIB_DIR) -L$(HI_STATIC_LIB_DIR)

LIB = -Wl,--start-group -lpthread -ldl -lOMX_Core -lOMX.hisi.video.encoder -lhi_common  -lrt -lm -Wl,--end-group
		  
APP = sample_omxvenc           
OBJS = omx_venc_test.o queue.o

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall

all:$(APP)

%.o: %.c
	$(AT)$(CC) $(CFLAGS) -c -o $@ $^

$(APP): ${OBJS}
	$(AT)$(CC) -o $@ $^ ${LIBPATH} ${LIB}


clean:
	-$(AT)rm -f ${OBJS}
	-$(AT)rm -f $(APP)

	
install:

uninstall:
