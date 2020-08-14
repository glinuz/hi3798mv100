
LINUX         ?=
ARCH          ?= arm
CROSS_COMPILE ?= arm-histbv300-linux-
PWD           := $(shell pwd)
CONFIG_SEC_MMZ ?= y

ccflags-y += -Idrivers/hisilicon/tee/tee_hisi/tzdriver

obj-$(CONFIG_SEC_MMZ) += secmmz.o

secmmz-y   := sec_mmz.o

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean

all:
	make -C $(LINUX) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) \
		M=$(PWD) modules V=1

clean:
	make -C $(LINUX) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) \
		M=$(PWD) clean



