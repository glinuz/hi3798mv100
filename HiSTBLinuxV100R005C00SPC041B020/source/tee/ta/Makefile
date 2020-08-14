################################################################
# build
################################################################
.PHONY : all clean

all:
ifeq ($(strip $(WITH_HISI_SEC_MMZ)), true)
	$(MAKE) -C mmz all
endif

ifeq ($(strip $(WITH_HISI_SMMU)), true)
	$(MAKE) -C smmu all
endif

ifeq ($(strip $(WITH_HISI_DMX)), true)
	$(MAKE) -C demux all
endif

ifeq ($(strip $(WITH_HISI_PVR)), true)
	$(MAKE) -C pvr all
endif

ifeq ($(strip $(WITH_HISI_AVPLAY)), true)
	$(MAKE) -C avplay all
endif

ifeq ($(strip $(WITH_HISI_SEC_HDMI)), true)
	$(MAKE) -C hdmi all
endif

ifeq ($(strip $(WITH_HISI_VFMW)), true)
	$(MAKE) -C vfmw all
endif
ifeq ($(strip $(WITH_HISI_WIDEVINE)), true)
	$(MAKE) -C widevine all
endif

ifeq ($(strip $(WITH_HISI_PLAYREADY)), true)
	$(MAKE) -C playready all
endif

ifeq ($(strip $(WITH_HISI_TEST)), true)
	$(MAKE) -C test all
endif

ifeq ($(strip $(WITH_COMPONENT_VMX_ULTRA)), true)
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_VMXTA.HSL_HI3798CV200_I all
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_videomarkTA.HSL_HI3798CV200_I all
endif
ifeq ($(strip $(WITH_COMPONENT_VMX_ULTRA_VMXTAC_TEST)), true)
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_VMXTA.HSL_HI3798CV200_I all
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_videomarkTA.HSL_HI3798CV200_I all
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_clientId2TA.HSL_HI3798CV200_I all
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_clientId3TA.HSL_HI3798CV200_I all
endif
ifeq ($(strip $(WITH_COMPONENT_VMX_ULTRA_ITAC_TEST)), true)
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_VMXTA.HSL_HI3798CV200_I all
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_videomarkTA.HSL_HI3798CV200_I all
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_clientId2TA.HSL_HI3798CV200_I all
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_clientId3TA.HSL_HI3798CV200_I all
endif

ifeq ($(strip $(WITH_HISI_DEMO)), true)
	$(MAKE) -C demo all
endif

ifeq ($(strip $(WITH_HISI_VDP)), true)
	$(MAKE) -C task_hisi_vdp all
endif

clean:
ifeq ($(strip $(WITH_HISI_SEC_MMZ)), true)
	$(MAKE) -C mmz clean
endif
ifeq ($(strip $(WITH_HISI_SMMU)), true)
	$(MAKE) -C smmu clean
endif
ifeq ($(strip $(WITH_HISI_DMX)), true)
	$(MAKE) -C demux clean
endif

ifeq ($(strip $(WITH_HISI_VFMW)), true)
	$(MAKE) -C vfmw clean
endif

ifeq ($(strip $(WITH_HISI_PVR)), true)
	$(MAKE) -C pvr clean
endif

ifeq ($(strip $(WITH_HISI_AVPLAY)), true)
	$(MAKE) -C avplay clean
endif

#	$(MAKE) -C test clean
ifeq ($(strip $(WITH_HISI_WIDEVINE)), true)
	$(MAKE) -C widevine clean
endif

ifeq ($(strip $(WITH_HISI_PLAYREADY)), true)
	$(MAKE) -C playready clean
endif

ifeq ($(strip $(WITH_HISI_TEST)), true)
	$(MAKE) -C test clean
endif

ifeq ($(strip $(WITH_HISI_SEC_HDMI)), true)
	$(MAKE) -C hdmi clean
endif

ifeq ($(strip $(WITH_COMPONENT_VMX_ULTRA)), true)
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_VMXTA.HSL_HI3798CV200_I clean
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_videomarkTA.HSL_HI3798CV200_I clean
endif

ifeq ($(strip $(WITH_COMPONENT_VMX_ULTRA_VMXTAC_TEST)), true)
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_VMXTA.HSL_HI3798CV200_I clean
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_videomarkTA.HSL_HI3798CV200_I clean
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_clientId2TA.HSL_HI3798CV200_I clean
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_clientId3TA.HSL_HI3798CV200_I clean
endif
ifeq ($(strip $(WITH_COMPONENT_VMX_ULTRA_ITAC_TEST)), true)
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_VMXTA.HSL_HI3798CV200_I clean
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_videomarkTA.HSL_HI3798CV200_I clean
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_clientId2TA.HSL_HI3798CV200_I clean
	$(MAKE) -C component/vmx/ultra/HI3798CV200/vmxtac_clientId3TA.HSL_HI3798CV200_I clean
endif

ifeq ($(strip $(WITH_HISI_DEMO)), true)
	$(MAKE) -C demo clean
endif

ifeq ($(strip $(WITH_HISI_VDP)), true)
	$(MAKE) -C task_hisi_vdp clean
endif
