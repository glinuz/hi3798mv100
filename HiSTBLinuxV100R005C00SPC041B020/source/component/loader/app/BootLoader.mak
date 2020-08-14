#===============================================================================
# for bootloader only
#===============================================================================
CFLAGS += -I$(BOOT_DIR)/product/driver/include
CFLAGS += -I$(BOOT_DIR)/fastboot/include
CFLAGS += -I$(BOOT_DIR)/product
CFLAGS += -I$(BOOT_DIR)/fastboot/arch/arm/include

toLoaderLib = libapp.a

all:
	make $(toLoaderLib)
#	cp -rvf $(CURDIR)/include/hi_loader_info.h $(SDK_DIR)/pub/include

$(toLoaderLib):$(LOADER_OBJS)
	@rm -f $(toLoaderLib)
	$(AR) -rc $@ $^
	rm *.o

$(LOADER_OBJS):$(LOADER_SRC)
	$(CC) -c  $^ $(CFLAGS) $(LIBPATH) $(LD_FLAGS)

clean:
	rm -f $(CURDIR)/../$(toLoaderLib)
	rm -f *.o *.a

