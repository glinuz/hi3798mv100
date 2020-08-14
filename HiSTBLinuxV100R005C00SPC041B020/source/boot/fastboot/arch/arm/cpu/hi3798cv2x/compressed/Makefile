################################################################################
#    Create By Czyong
################################################################################

PWD           = $(shell pwd)
TOPDIR        =
BINIMAGE      = $(TOPDIR)/fastboot-burn.bin

include $(TOPDIR)/config.mk

################################################################################
CC       := $(CROSS_COMPILE)gcc
AR       := $(CROSS_COMPILE)ar
LD       := $(CROSS_COMPILE)ld
OBJCOPY  := $(CROSS_COMPILE)objcopy

################################################################################
BOOT     := mini-boot

CFLAGS   := -g -Os -fno-strict-aliasing -fno-common \
	-D__KERNEL__ -DTEXT_BASE=$(TEXT_BASE) \
	-I$(TOPDIR)/include -Wall \
	-fno-builtin -ffreestanding \
	-pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux  \
	-mno-thumb-interwork -march=armv7-a $(MKFLAGS)

################################################################################
START    := start.o
COBJS    := arm_exception.o chip.o \
	startup.o cache-cp15.o uart.o mmu.o image_data.o tail.o

REG      := $(wildcard $(TOPDIR)/*.reg $(TOPDIR)/.reg)

sinclude $(TOPDIR)/platform.mk
################################################################################
.PHONY: $(BOOT).bin

$(BOOT).bin: $(BOOT).elf regfile
	$(OBJCOPY) -O srec $< $(BOOT).srec
	$(OBJCOPY) --gap-fill=0xff -O binary $< $@
	@chmod 754 $(BOOT).bin
	@cp -fv $@ $(TOPDIR)
	@echo $(BOOT).bin is Ready.

$(BOOT).elf: image_data.lzma $(START) $(COBJS)
	$(LD) -Bstatic -T mini-boot.lds -Ttext $(TEXT_BASE) $(START) \
		$(COBJS) -Map $(BOOT).map -o $@

.PHONY: regfile
regfile:
	@if [ "$(words $(REG))" = "0" ]; then ( \
		echo '***' Need '.reg' or '*.reg' file in directory $(TOPDIR); \
		exit 1; \
	) fi

################################################################################
start.o: start.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ $< -c

image_data.lzma: $(BINIMAGE)
	lzma -fkzc -7 $< > $@

%.o: %.c
	$(CC) $(CFLAGS) -Wall -Wstrict-prototypes \
		-fno-stack-protector -o $@ $< -c

%.o: %.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ $< -c

image_data.o: image_data.S image_data.lzma
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ $< -c

#############################################################################

SRC := start.S tail.S arm_exception.S chip.c \
	early_timer.c uart.S himci_boot_hi3798cv2x.c

$(SRC):
	ln -fs ../$@ ./

SRC += mmu.S cache-cp15.c

cache-cp15.c \
mmu.S:
	ln -sf ./../../../lib/$@ ./

startup.c: unlzma.c

SRC += unlzma.c
unlzma.c:
	ln -fs ./../../../../../lib/$@ ./

################################################################################
TMPS := $(COBJS) start.o $(SRC) \
	$(BOOT).map $(BOOT).elf $(BOOT).srec $(BOOT).bin $(BOOT).tmp \
	image_data.lzma

distclean: clean

clean:
	@rm -f $(TMPS)

################################################################################
.PHONY: clean
################################################################################
