
obj-y		+= atags2fdt.o
obj-y		+= atags.o tag-param.o tag-sdkver.o tag-net.o tag-chiptrim.o
obj-$(CONFIG_HIMCIV200_SDIO_SYNOPSYS)	+= tag-emmc.o

CFLAGS_atags2fdt.o    = -I$(src)/../../../scripts/dtc/libfdt
CFLAGS_tag-net.o      = -I$(src)/../../../scripts/dtc/libfdt
CFLAGS_tag-chiptrim.o = -I$(src)/../../../scripts/dtc/libfdt
CFLAGS_tag-emmc.o     = -I$(src)/../../../scripts/dtc/libfdt
