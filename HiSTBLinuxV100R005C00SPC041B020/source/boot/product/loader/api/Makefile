ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell pwd)/../../../../..
include $(SDK_DIR)/base.mak
endif

ifneq ($(CFG_HI_MINIBOOT_SUPPORT),y)
CFLAGS += -I$(BOOT_DIR)/fastboot/include
else
CFLAGS += -I$(BOOT_DIR)/miniboot/include
endif

CFLAGS += -I$(BOOT_DIR)/product/driver/include
CFLAGS += -I$(COMPONENT_DIR)/loader/api/include
CFLAGS += -I$(COMPONENT_DIR)/loader/api/src
CFLAGS += -DHI_LOADER_BOOTLOADER

source := $(wildcard *.c src/*.c)
LIBS := libapi.a
OBJS := $(patsubst %.c,%.o, $(source))

all: $(LIBS)

install: all

$(LIBS): $(OBJS)
	$(AT)$(AR) -rc $@ $^

%.o: %.c
	$(AT)echo "cc: $^ *************************"
	$(AT)$(CC) $(CFLAGS) -c $^ -o "$(*D)/$(*F).o"

clean:
	$(AT)rm -f $(OBJS) $(LIBS)

