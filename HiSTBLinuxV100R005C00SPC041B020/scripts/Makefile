#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/..
include $(SDK_DIR)/base.mak
endif

#***********************************************************************************
#   varibale
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++
ifneq ($(CFG_HI_ADVCA_SUPPORT),y)
    CFG_HI_ADVCA_SUPPORT :=
endif

JPGE_SUPPORT := y
SATA_SUPPORT := y
USB3_SUPPORT := n

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716mv410 hi3716dv100),)
SATA_SUPPORT := n
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3716mv410)
JPGE_SUPPORT := n
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3798cv200 hi3798mv200 hi3798mv200_a),)
USB3_SUPPORT := y
endif

#***********************************************************************************
#   rule
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: all clean load unload
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++
all: load unload

load:
	$(AT)test -d $(HI_MODULE_DIR) || mkdir -p $(HI_MODULE_DIR)
	$(AT)rm -rf $(HI_MODULE_DIR)/load
ifneq ($(CFG_HI_MSP_BUILDIN),y)
    ifneq ($(CFG_HI_KEYLED_CT1642_KERNEL_SUPPORT),y)
		$(AT)echo "insmod hi_media.ko" >> $(HI_MODULE_DIR)/load
    endif

	$(AT)echo "insmod hi_mmz.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_common.ko" >> $(HI_MODULE_DIR)/load
	
	$(AT)echo "insmod hi_pdm.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_pq.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_hdmi.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_vou.ko" >> $(HI_MODULE_DIR)/load

	$(AT)echo "insmod hi_gpio.ko" >> $(HI_MODULE_DIR)/load

    ifeq ($(CFG_HI_GPIOI2C_SUPPORT),y)
		$(AT)echo "insmod hi_gpio_i2c.ko i2cmode=2" >> $(HI_MODULE_DIR)/load
    endif

	$(AT)echo "insmod hi_i2c.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_tde.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_otp.ko" >> $(HI_MODULE_DIR)/load

    ifneq ($(CFG_HI_NVR_SUPPORT),y)
		$(AT)echo "insmod hi_demux.ko" >> $(HI_MODULE_DIR)/load
    endif

	$(AT)echo "insmod hi_adec.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_vfmw.ko" >> $(HI_MODULE_DIR)/load

    ifeq ($(CFG_HI_VDEC_SVDEC_SUPPORT),y)
		$(AT)echo "insmod hi_svdec.ko" >> $(HI_MODULE_DIR)/load
    endif

	$(AT)echo "insmod hi_vpss.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_vdec.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_cipher.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_adsp.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_aiao.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod hi_sync.ko" >> $(HI_MODULE_DIR)/load

    ifeq ($(CFG_HI_PVR_SUPPORT),y)
		$(AT)echo "insmod hi_pvr.ko" >> $(HI_MODULE_DIR)/load
    endif

	$(AT)echo "insmod hi_mce.ko" >> $(HI_MODULE_DIR)/load

    ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
		$(AT)echo "insmod hi_tuner.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifeq ($(CFG_HI_PWM_SUPPORT),y)
		$(AT)echo "insmod hi_pwm.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifndef CFG_HI_ADVCA_FUNCTION
		$(AT)echo "# About hi_fb:" >> $(HI_MODULE_DIR)/load
		$(AT)echo "#  For 1920*1080, triple buffer, ARGB8888 format, use:" >> $(HI_MODULE_DIR)/load
		$(AT)echo "#  insmod hi_fb.ko video=\"hi_fb:vram0_size:16200,vram4_size:2430\"" >> $(HI_MODULE_DIR)/load
		$(AT)echo "#  For 1280*720, use:" >> $(HI_MODULE_DIR)/load
		$(AT)echo "#  insmod hi_fb.ko video=\"hi_fb:vram0_size:7200,vram2_size:2430\"" >> $(HI_MODULE_DIR)/load
		$(AT)echo "#  Insmod hi_fb.ko with no args will use defaule value,The defaule value is set by cfg.mak." >> $(HI_MODULE_DIR)/load
		$(AT)echo "#  vram0 is memory size for HD and vram4 is for SD0. The size is calculated as below:" >> $(HI_MODULE_DIR)/load
		$(AT)echo "#  vram0_size = (1920*1080*4*2/1024)KB; vram4_size = (720*576*2*2/1024)KB" >> $(HI_MODULE_DIR)/load
    endif
	$(AT)echo "insmod hi_fb.ko" >> $(HI_MODULE_DIR)/load

    ifeq ($(CFG_HI_GPU_MALI400_SUPPORT),y)
		$(AT)echo "insmod mali.ko" >> $(HI_MODULE_DIR)/load
    else ifeq ($(CFG_HI_GPU_MALI450_SUPPORT),y)
		$(AT)echo "insmod mali.ko" >> $(HI_MODULE_DIR)/load
    else ifeq ($(CFG_HI_GPU_MALI700_SUPPORT),y)
		$(AT)echo "insmod kds.ko" >> $(HI_MODULE_DIR)/load
		$(AT)echo "insmod mali_kbase.ko" >> $(HI_MODULE_DIR)/load
		$(AT)echo "insmod hi_dbe.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifneq ($(CFG_HI_IR_TYPE_LIRC),y)
		$(AT)echo "insmod hi_ir.ko" >> $(HI_MODULE_DIR)/load
    endif

	$(AT)echo "insmod hi_jpeg.ko" >> $(HI_MODULE_DIR)/load

    ifeq ($(JPGE_SUPPORT),y)
		$(AT)echo "insmod hi_jpge.ko" >> $(HI_MODULE_DIR)/load
    endif

	$(AT)echo "insmod hi_png.ko" >> $(HI_MODULE_DIR)/load

	$(AT)echo "insmod hi_advca.ko" >> $(HI_MODULE_DIR)/load

    ifeq ($(CFG_HI_VENC_SUPPORT),y)
		$(AT)echo "insmod hi_venc.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifeq ($(CFG_HI_AENC_SUPPORT),y)
		$(AT)echo "insmod hi_aenc.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
		$(AT)echo "insmod hi_keyled.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifeq ($(CFG_HI_SCI_SUPPORT),y)
		$(AT)echo "insmod hi_sci.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifeq ($(CFG_HI_VI_SUPPORT),y)
		$(AT)echo "insmod hi_vi.ko" >> $(HI_MODULE_DIR)/load
        ifeq ($(CFG_HI_HDMI_RX_SUPPORT),y)
			$(AT)echo "insmod hi_hdmirx.ko" >> $(HI_MODULE_DIR)/load
        endif
    endif

	$(AT)echo "insmod hi_pmoc.ko" >> $(HI_MODULE_DIR)/load

    ifeq ($(CFG_HI_SPI_BUS_SUPPORT),y)
		$(AT)echo "insmod hi_spi.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifeq ($(CFG_HI_ADAC_SLIC_SUPPORT),y)
		$(AT)echo "insmod hi_slic.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifeq ($(CFG_HI_CIPLUS_SUPPORT),y)
		$(AT)echo "insmod hi_ci.ko" >> $(HI_MODULE_DIR)/load
    endif

    ifeq ($(CFG_HI_CI_DEV_CIMAXPLUS),y)
		$(AT)echo "insmod hi_cimaxplus.ko" >> $(HI_MODULE_DIR)/load
    endif
endif   # End of ifneq ($(CFG_HI_MSP_BUILDIN),y)

ifeq ($(CFG_HI_IR_TYPE_LIRC),y)
	$(AT)echo "insmod lirc_dev.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod lirc_serial.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "mkdir -p /var/run/lirc" >> $(HI_MODULE_DIR)/load
	$(AT)echo "mkdir -p /var/log" >> $(HI_MODULE_DIR)/load
	$(AT)echo "cat > /var/log/lircd << EOF" >> $(HI_MODULE_DIR)/load
	$(AT)echo "EOF" >> $(HI_MODULE_DIR)/load
	$(AT)echo "ln -fs /dev/lirc0 /dev/lirc" >> $(HI_MODULE_DIR)/load
	$(AT)echo "lircd -d /dev/lirc0 -o /dev/lircd -r__up" >> $(HI_MODULE_DIR)/load
endif

ifeq ($(CFG_HI_ADAC_TVL320AIC31_SUPPORT),y)
	$(AT)echo "insmod hi_tlv320.ko" >> $(HI_MODULE_DIR)/load
endif

ifeq ($(CFG_HI_KERNEL_VERSION),linux-3.18.y)
	$(AT)echo "insmod ./usb/ehci-platform.ko" >> $(HI_MODULE_DIR)/load
	$(AT)echo "insmod ./usb/ohci-platform.ko" >> $(HI_MODULE_DIR)/load

    ifeq ($(USB3_SUPPORT),y)
		$(AT)echo "insmod ./usb/xhci-plat-hcd.ko" >> $(HI_MODULE_DIR)/load
    endif
endif

ifeq ($(SATA_SUPPORT),y)
	$(AT)echo "insmod ./sata/libahci.ko" >> $(HI_MODULE_DIR)/load
ifeq ($(CFG_HI_KERNEL_VERSION),linux-3.18.y)
	$(AT)echo "insmod ./sata/libahci_platform.ko" >> $(HI_MODULE_DIR)/load
endif
	$(AT)echo "insmod ./sata/ahci_platform.ko" >> $(HI_MODULE_DIR)/load
endif

ifeq ($(CFG_HI_NTFS_MODULE_SUPPORT),y)
	$(AT)echo "insmod tntfs.ko" >> $(HI_MODULE_DIR)/load
endif

ifeq ($(CFG_HI_DIRECTFB_MULTI_SUPPORT),y)
	$(AT)echo "insmod fusion.ko" >> $(HI_MODULE_DIR)/load
endif

	$(AT)echo "echo 2097152 > /proc/sys/net/core/rmem_max" >> $(HI_MODULE_DIR)/load

ifndef CFG_HI_ADVCA_FUNCTION
	$(AT)echo "echo 6 > /proc/sys/vm/dirty_ratio" >> $(HI_MODULE_DIR)/load # see source/msp/component/hifat/how to use.txt for detail
	$(AT)echo "echo 1 > /proc/sys/vm/overcommit_memory" >> $(HI_MODULE_DIR)/load
	$(AT)echo "echo 8192 > /proc/sys/vm/min_free_kbytes" >> $(HI_MODULE_DIR)/load
endif

ifeq ($(CFG_HI_DVFS_CPU_SUPPORT),y)
    ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716mv410 hi3716mv420),)
		$(AT)echo "echo 1000000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq" >> $(HI_MODULE_DIR)/load
    endif
    ifeq ($(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a)
		$(AT)echo "echo 1200000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq" >> $(HI_MODULE_DIR)/load
    endif

ifeq ($(CFG_HI_NVR_SUPPORT),y)
	$(AT)echo "echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor" >> $(HI_MODULE_DIR)/load
	$(AT)echo "echo 1200000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed" >> $(HI_MODULE_DIR)/load
else
	$(AT)echo "echo interactive > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor" >> $(HI_MODULE_DIR)/load
	$(AT)echo "echo 200000 > /sys/devices/system/cpu/cpufreq/interactive/timer_rate" >> $(HI_MODULE_DIR)/load
endif
endif

ifeq ($(CFG_HI_GPU_MALI700_SUPPORT),y)
    ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200),)
		$(AT)echo "echo 200000000 >/sys/devices/platform/soc/f9200000.gpu/devfreq/f9200000.gpu/min_freq " >> $(HI_MODULE_DIR)/load
		$(AT)echo "echo 400000000 >/sys/devices/platform/soc/f9200000.gpu/devfreq/f9200000.gpu/max_freq " >> $(HI_MODULE_DIR)/load
    endif
endif
ifeq ($(CFG_HI_CHIP_TYPE), hi3796mv200)
	$(AT)mv $(HI_MODULE_DIR)/load $(HI_MODULE_DIR)/load_bak
else
	$(AT)chmod +x $(HI_MODULE_DIR)/load
endif

unload:
	$(AT)test -d $(HI_MODULE_DIR) || mkdir -p $(HI_MODULE_DIR)
	$(AT)rm -rf $(HI_MODULE_DIR)/unload

ifeq ($(CFG_HI_DIRECTFB_MULTI_SUPPORT),y)
	$(AT)echo "rmmod fusion" >> $(HI_MODULE_DIR)/unload
endif

ifeq ($(CFG_HI_NTFS_MODULE_SUPPORT),y)
	$(AT)echo "rmmod tntfs" >> $(HI_MODULE_DIR)/unload
endif

ifeq ($(SATA_SUPPORT),y)
	$(AT)echo "rmmod ahci_platform" >> $(HI_MODULE_DIR)/unload
ifeq ($(CFG_HI_KERNEL_VERSION),linux-3.18.y)
	$(AT)echo "rmmod libahci_platform" >> $(HI_MODULE_DIR)/unload
endif
	$(AT)echo "rmmod libahci" >> $(HI_MODULE_DIR)/unload
endif

ifeq ($(CFG_HI_KERNEL_VERSION),linux-3.18.y)
    ifeq ($(USB3_SUPPORT),y)
		$(AT)echo "rmmod xhci-plat-hcd" >> $(HI_MODULE_DIR)/unload
    endif
	$(AT)echo "rmmod ohci-platform" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod ehci-platform" >> $(HI_MODULE_DIR)/unload
endif

ifeq ($(CFG_HI_ADAC_TVL320AIC31_SUPPORT),y)
	$(AT)echo "rmmod hi_tlv320" >> $(HI_MODULE_DIR)/unload
endif

ifeq ($(CFG_HI_IR_TYPE_LIRC),y)
	$(AT)echo "rmmod lirc_serial" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod lirc_dev" >> $(HI_MODULE_DIR)/unload
endif

ifneq ($(CFG_HI_MSP_BUILDIN),y)
    ifeq ($(CFG_HI_CI_DEV_CIMAXPLUS),y)
		$(AT)echo "rmmod hi_cimaxplus" >> $(HI_MODULE_DIR)/unload
    endif

    ifeq ($(CFG_HI_CIPLUS_SUPPORT),y)
		$(AT)echo "rmmod hi_ci" >> $(HI_MODULE_DIR)/unload
    endif

    ifeq ($(CFG_HI_ADAC_SLIC_SUPPORT),y)
		$(AT)echo "rmmod hi_slic" >> $(HI_MODULE_DIR)/unload
    endif

    ifeq ($(CFG_HI_SPI_BUS_SUPPORT),y)
		$(AT)echo "rmmod hi_spi" >> $(HI_MODULE_DIR)/unload
    endif

	$(AT)echo "rmmod hi_pmoc" >> $(HI_MODULE_DIR)/unload

    ifeq ($(CFG_HI_VI_SUPPORT),y)
        ifeq ($(CFG_HI_HDMI_RX_SUPPORT),y)
			$(AT)echo "rmmod hi_hdmirx" >> $(HI_MODULE_DIR)/unload
        endif
		$(AT)echo "rmmod hi_vi" >> $(HI_MODULE_DIR)/unload
    endif

    ifeq ($(CFG_HI_SCI_SUPPORT),y)
		$(AT)echo "rmmod hi_sci" >> $(HI_MODULE_DIR)/unload
    endif

    ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
		$(AT)echo "rmmod hi_keyled" >> $(HI_MODULE_DIR)/unload
    endif

    ifeq ($(CFG_HI_AENC_SUPPORT),y)
		$(AT)echo "rmmod hi_aenc" >> $(HI_MODULE_DIR)/unload
    endif

    ifeq ($(CFG_HI_VENC_SUPPORT),y)
		$(AT)echo "rmmod hi_venc" >> $(HI_MODULE_DIR)/unload
    endif

	$(AT)echo "rmmod hi_advca" >> $(HI_MODULE_DIR)/unload

	$(AT)echo "rmmod hi_png" >> $(HI_MODULE_DIR)/unload

    ifeq ($(JPGE_SUPPORT),y)
		$(AT)echo "rmmod hi_jpge" >> $(HI_MODULE_DIR)/unload
    endif

	$(AT)echo "rmmod hi_jpeg" >> $(HI_MODULE_DIR)/unload

    ifneq ($(CFG_HI_IR_TYPE_LIRC),y)
		$(AT)echo "rmmod hi_ir" >> $(HI_MODULE_DIR)/unload
    endif

    ifeq ($(CFG_HI_GPU_MALI400_SUPPORT),y)
		$(AT)echo "rmmod mali" >> $(HI_MODULE_DIR)/unload
    else ifeq ($(CFG_HI_GPU_MALI450_SUPPORT),y)
		$(AT)echo "rmmod mali" >> $(HI_MODULE_DIR)/unload
    else ifeq ($(CFG_HI_GPU_MALI700_SUPPORT),y)
		$(AT)echo "rmmod mali_kbase" >> $(HI_MODULE_DIR)/unload
		$(AT)echo "rmmod kds" >> $(HI_MODULE_DIR)/unload
    endif

	$(AT)echo "rmmod hi_fb" >> $(HI_MODULE_DIR)/unload

    ifeq ($(CFG_HI_PWM_SUPPORT),y)
		$(AT)echo "rmmod hi_pwm" >> $(HI_MODULE_DIR)/unload
    endif

    ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
		$(AT)echo "rmmod hi_tuner" >> $(HI_MODULE_DIR)/unload
    endif

	$(AT)echo "rmmod hi_mce" >> $(HI_MODULE_DIR)/unload

    ifeq ($(CFG_HI_PVR_SUPPORT),y)
		$(AT)echo "rmmod hi_pvr" >> $(HI_MODULE_DIR)/unload
    endif

	$(AT)echo "rmmod hi_sync" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_aiao" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_adsp" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_cipher" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_vdec" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_vpss" >> $(HI_MODULE_DIR)/unload

    ifeq ($(CFG_HI_VDEC_SVDEC_SUPPORT),y)
		$(AT)echo "rmmod hi_svdec" >> $(HI_MODULE_DIR)/unload
    endif

	$(AT)echo "rmmod hi_vfmw" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_adec" >> $(HI_MODULE_DIR)/unload

    ifneq ($(CFG_HI_NVR_SUPPORT),y)
		$(AT)echo "rmmod hi_demux" >> $(HI_MODULE_DIR)/unload
    endif

	$(AT)echo "rmmod hi_otp" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_tde" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_i2c" >> $(HI_MODULE_DIR)/unload

    ifeq ($(CFG_HI_GPIOI2C_SUPPORT),y)
		$(AT)echo "rmmod hi_gpio_i2c" >> $(HI_MODULE_DIR)/unload
    endif

	$(AT)echo "rmmod hi_gpio" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_vou" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_hdmi" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_pq" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_pdm" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_common" >> $(HI_MODULE_DIR)/unload
	$(AT)echo "rmmod hi_mmz" >> $(HI_MODULE_DIR)/unload

    ifneq ($(CFG_HI_KEYLED_CT1642_KERNEL_SUPPORT),y)
		$(AT)echo "rmmod hi_media" >> $(HI_MODULE_DIR)/unload
    endif
endif   # End of ifneq ($(CFG_HI_MSP_BUILDIN),y)
	$(AT)chmod +x $(HI_MODULE_DIR)/unload

clean:
	$(AT)rm -rf $(HI_MODULE_DIR)/load $(HI_MODULE_DIR)/unload

