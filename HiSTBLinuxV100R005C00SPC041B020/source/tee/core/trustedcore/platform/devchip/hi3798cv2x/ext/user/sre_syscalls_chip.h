#include "sre_typedef.h"
#include "tee_internal_api.h"
#ifdef WITH_HISI_SEC_HDMI
#include "tee_drv_hdmi.h"
#endif
#ifdef WITH_HISI_SEC_MMZ
#include "mmz_struct.h"
#endif

/*VFMW begin*/
#ifdef WITH_HISI_VFMW
extern int __VFMW_Init(unsigned int pArgs);
extern int __VFMW_Exit(void);
extern int __VFMW_Resume(void);
extern int __VFMW_Suspend(void);
extern int __VFMW_Control(unsigned int ChanID, unsigned int eCmdID, unsigned int pArgs);
extern int __VFMW_Thread(void);
extern int __VFMW_Get_Image(unsigned int ChanID, unsigned int pImage);
extern int __VFMW_Release_Image(unsigned int ChanID, unsigned int pImage);
extern int __VFMW_Read_Proc(unsigned int Page, unsigned int Len);
extern int __VFMW_Write_Proc(unsigned int Option, unsigned int Value);
#endif

/*VFMW end*/

/*VDP start*/
#ifdef WITH_HISI_VDP
extern int __vdp_drv_Ioctl(int cmd ,void *arg);
#endif
#ifdef WITH_HISI_SEC_MMZ
/*MMZ*/
extern int __MMZ_MALLOC(HI_MMZ_BUF_S *pBuf);
extern int __MMZ_FREE(HI_MMZ_BUF_S *pBuf);
extern int __MMZ_NEW(HI_MMZ_BUF_S *pBuf, char *mmz_name, char *mmb_name);
extern int __MMZ_DEL(unsigned long addr);
extern int __MMZ_MAP(unsigned long addr, unsigned long cached);
extern int __MMZ_UNMAP(unsigned long addr);
extern int __MMZ_FLUSH(unsigned long addr);
extern int __TEE_IsSecMMZ(unsigned long phys_addr);

extern int __TEE_IsSecMem(unsigned long phys_addr, unsigned long size);
extern int __TEE_IsNonSecMem(unsigned long phys_addr, unsigned long size);
extern int __call_debug(unsigned long arg0, unsigned long arg1, unsigned long arg2, unsigned long arg3);
#endif

#ifdef WITH_HISI_CIPHER
extern int __TEE_CIPHER_Ioctl(unsigned int cmd, void *argp);
#endif

#ifdef WITH_HISI_OTP
extern int __OTP_Init(void);
extern int __OTP_DeInit(void);
extern int __OTP_Reset(void);
extern int __OTP_GetChipID(uint8_t *pChipID);
extern int __OTP_ReadWord(uint32_t Addr, uint32_t *pu32Value);
extern int __OTP_ReadByte(uint32_t Addr, uint8_t *pu8Value);
extern int __OTP_WriteWord(uint32_t Addr, uint32_t u32Value);
extern int __OTP_WriteByte(uint32_t Addr, uint8_t u8Value);
#endif
#ifdef WITH_HISI_ADVCA
extern int __TEE_ADVCA_Ioctl(unsigned int cmd, void *argp);

#endif

/* smmu  */
#ifdef WITH_HISI_SMMU
extern UINT32 __hisi_sec_maptosmmu(unsigned long buf_phys, unsigned long buf_size);
extern int __hisi_sec_unmapfromsmmu(unsigned long buf_phys, unsigned long buf_size);
extern UINT32 __hisi_sec_maptosmmu_and_setflag(unsigned long buf_phys, unsigned long buf_size);
extern int __hisi_sec_unmapfromsmmu_and_clrflg(unsigned long buf_phys, unsigned long buf_size);
extern UINT32 __hisi_sec_alloc(const char *bufname, unsigned long size, unsigned long align, int memtype);
extern int __hisi_sec_free(unsigned long sec_addr, int memtype);
extern UINT32 __hisi_sec_map_to_cpu(unsigned long sec_addr, int memtype, int cached);
extern int __hisi_sec_unmap_from_cpu(void *sec_virt);
extern UINT32 __hisi_sec_map_to_sec_smmu(unsigned long phys_addr, unsigned long size, int share_mem);
extern int __hisi_sec_unmap_from_sec_smmu(unsigned long sec_smmu, int share_mem);
extern UINT32 __hisi_nonsec_mem_map_to_sec_cpu(unsigned long nonsec_addr, int memtype, int cached);
extern int __hisi_nosec_mem_unmap_from_sec_cpu(void *va_addr);
extern UINT32 __hisi_nosec_mem_map_to_sec_smmu(unsigned long nonsec_addr, int memtype);
extern int __hisi_nosec_mem_unmap_from_sec_smmu(unsigned long sec_smmu);
extern int __agent_closed(void);
extern int __sec_ioctl(unsigned long cmd, unsigned long arg0, unsigned long arg1);
extern void __hisi_mem_flush(void *virt, unsigned long size);
#endif

#ifdef WITH_HISI_PVR
extern HI_S32 __TEE_PVR_DeInit(HI_VOID);
//extern HI_S32 __TEE_PVR_OpenChannel(HI_U32 *pu32ChnId, HI_S32 s32BufLen, TEE_PVR_TYPE_E enType);
extern HI_S32 __TEE_PVR_OpenChannel(HI_U32 *pu32ChnId, HI_S32 s32BufLen, HI_U32 enType);
extern HI_S32 __TEE_PVR_CloseChannel(HI_U32 u32ChnId);
extern HI_S32 __TEE_PVR_RecProcTsData(HI_U32 u32ChnId, HI_U32 u32NoSecureSmmuAddr);
extern HI_S32 __TEE_PVR_RecCopyToRee(HI_U32 u32SecureSmmuAddr, HI_U32 u32NoSecureSmmuAddr, HI_U32 *pu32DataLen);
extern HI_S32 __TEE_PVR_PlayCopyFromRee(HI_U32 u32ReeAddr, HI_U32 u32TeeAddr, HI_U32 u32DataLen);
extern HI_S32 __TEE_PVR_RecGetOutputBufInfo(HI_U32 u32ChnId, HI_U32 *pu32SecureSmmuAddr, HI_U32 *pu32DataLen);
extern HI_S32 __TEE_PVR_RecGetStatus(HI_U32 u32ChnId, HI_U32 u32SecureSmmuAddr);
#endif

#ifdef WITH_HISI_SEC_HDMI
extern HI_S32 __HI_DRV_HDMI_SetSRM(HI_DRV_HDMI_DEVICE_ID_E enHdmiId, HI_U8 *pSrm);
extern HI_S32 __HI_DRV_HDMI_SetHDCPStrategy(HI_DRV_HDMI_DEVICE_ID_E enHdmiId, HI_DRV_HDMI_HDCP_LEVEL_E enHdcpLevle);
extern HI_S32 __HI_DRV_HDMI_GetHDCPStrategy(HI_DRV_HDMI_DEVICE_ID_E enHdmiId, HI_DRV_HDMI_HDCP_LEVEL_E *penHdcpLevel);
extern HI_S32 __HI_DRV_HDMI_GetHDCPStatus(HI_DRV_HDMI_DEVICE_ID_E enHdmiId, HI_DRV_HDMI_HDCP_STATUS_E *penHdcpStatus);
extern HI_S32 __HI_DRV_HDMI_SetHDCPMute(HI_DRV_HDMI_DEVICE_ID_E enHdmiId, HI_BOOL bMute);
extern HI_S32 __HI_DRV_HDMI_HDCP22_Init(HI_DRV_HDMI_DEVICE_ID_E enHdmiId);
extern HI_S32 __HI_DRV_HDMI_TeeProc(HI_DRV_HDMI_DEVICE_ID_E enHdmi);
extern HI_S32 __HI_DRV_HDMI_SetHDCPCaps(HI_DRV_HDMI_DEVICE_ID_E enHdmiId, HI_DRV_HDMI_HDCP_CAPS_S *pstHdcpCaps);
extern HI_S32 __HI_DRV_HDMI_IOCTRL(HI_DRV_HDMI_DEVICE_ID_E enHdmiId, HI_VOID *pvData,HI_U32 u32Size);
extern HI_S32 __HI_DRV_HDMI_GetHDCPCaps(HI_DRV_HDMI_DEVICE_ID_E enHdmiId, HI_DRV_HDMI_HDCP_CAPS_S *pstHdcpCaps);
extern HI_S32 __HI_DRV_HDMI_Init(HI_DRV_HDMI_DEVICE_ID_E enHdmiId);
extern HI_S32 __HI_DRV_HDMI_DeInit(HI_DRV_HDMI_DEVICE_ID_E enHdmiId);
extern HI_S32 __HI_DRV_HDMI_Start(HI_DRV_HDMI_DEVICE_ID_E enHdmiId);
extern HI_S32 __HI_DRV_HDMI_Stop(HI_DRV_HDMI_DEVICE_ID_E enHdmiId);

#endif

#ifdef WITH_HISI_AVPLAY
extern HI_S32 __TEE_DRV_AVPLAY_Ree2TeeMemCopy(HI_U32 ReeAddr, HI_U32 TeeAddr, HI_U32 DataLen);
#endif
