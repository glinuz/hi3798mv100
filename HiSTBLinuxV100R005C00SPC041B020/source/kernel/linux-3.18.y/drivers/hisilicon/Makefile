
obj-y += kapi/ misc/ soc/ clk/ atags/ clocksource/ flash_stats/ ir/

obj-$(CONFIG_BLK_DEV_RAM)		+= initmrd/
obj-$(CONFIG_BLK_DEV_LOOP)		+= initfile/
obj-$(CONFIG_VIRTUAL_INPUT)		+= vinput/
obj-$(CONFIG_CMA)			+= cma/
obj-$(CONFIG_USB_OTG_HISI)		+= usbotg/

#ccflags-y += -I$(srctree)/drivers/hisilicon/tee/include
ifdef CONFIG_ARCH_HI3798MX
obj-$(CONFIG_TEE)			+= sec_mmz/
endif
obj-$(CONFIG_TEE)			+= tee/
obj-$(CONFIG_SDCARD_FS)			+= sdcardfs/
obj-$(CONFIG_REGULATOR)			+= regulator/
obj-$(CONFIG_HISI_THERMAL)		+= ipa/
obj-$(CONFIG_HASH_VRF_DEV)		+= hash_vrf_dev/
