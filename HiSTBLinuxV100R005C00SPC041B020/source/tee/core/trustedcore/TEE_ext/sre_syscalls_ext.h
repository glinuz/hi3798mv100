#if ((TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3650) || (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI6250))
#include <img_systypes.h>
#include "img_types.h"
#include "secure_defs.h"
#endif
#include "sre_sharedmem.h"
#include "sre_securemmu.h"

#ifdef FEATURE_IRIS
extern int __iris_tee_mmap(unsigned int phy_addr , unsigned int size, unsigned int *virt_addr,unsigned int secure_mode, unsigned int cache_mode);
extern int __iris_tee_unmap(unsigned int virt_addr, unsigned int size);
extern int __iris_tee_isSecureMemory(unsigned int addr, unsigned int size);
#endif
extern int __bsp_efuse_read(unsigned int *buf, const unsigned int group, const unsigned int size);
extern int __bsp_efuse_write(unsigned int *buf, const unsigned int group, const unsigned int size);
extern int __bsp_modem_call(unsigned int func_cmd, unsigned int arg1, void *arg2, unsigned int arg3);

extern int __hisi_tui_get_tpdata(void *finger_data_buf);
extern void __set_kb_rect(unsigned int mode);
extern void __send_simu_tp(void *tp_t);

extern int __tee_hal_get_dieid(unsigned int *pBuffer);
unsigned int __scard_connect(int reader_id, void *p_atr, unsigned int *atr_len);
unsigned int __scard_disconnect(int reader_id);
unsigned int __scard_transmit(int reader_id, unsigned char *p_cmd , unsigned int cmd_len,
			      unsigned char *p_rsp , unsigned int *rsp_len);
extern int __driver_dep_test();
#if ((TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3650) || (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI6250))
extern IMG_RESULT __SECURE_TEE_GetId(IMG_VIDEO_CORE nCore, IMG_UINT32 *pui32SecureId);
extern IMG_RESULT __SECURE_TEE_ReleaseId(IMG_UINT32 ui32SecureId);
extern IMG_RESULT __SECURE_TEE_SendMessage(IMG_UINT32 ui32SecureId, IMG_BYTE *pbyMsg,IMG_UINT16 ui16Size, MSG_ENDPOINT nEndpoint,IMG_BYTE *pbyBuf,IMG_UINT32 ui32BufSize);
#endif
extern int __SECURE_TEE_Mmap(paddr_t phy_addr , unsigned int size, unsigned int *virt_addr, unsigned int secure_mode, unsigned int cache_mode);
extern int __SECURE_TEE_Unmap(unsigned int virt_addr, unsigned int size);
extern int __SECURE_ISSecureMemory(paddr_t addr, unsigned int size);
extern int __driver_p61_factory_test(void);
/* display */
extern int __hisi_fb_cfg_sec(int value);
extern int __hisi_fb_alpha_set(int value);
extern int __hisi_pan_display_sec(void *rect);
extern int __hisi_wait_vactive_flag(void);
extern int __hisi_wait_release_flag(void);
extern int __hisi_get_disp_info(void *hisi_disp_info);
extern int __is_device_rooted(void);
extern int __get_vsim_sharemem(unsigned int *addr, unsigned int *size);
extern int __driver_fingerprint_command(void *cmmand_info);
extern int __driver_spi_full_duplex(void *p_write_info, void *p_read_info);
extern int __driver_spi_full_duplex_with_speed(void *p_write_info, void *p_read_info, int speed);
extern int __driver_spi_half_duplex_with_speed(void *p_write_info, void *p_read_info, int speed);

extern int __get_target_product(char *targe_product, int *size);

/*rtc config*/
extern unsigned int __sre_get_rtc_time();
extern void __sre_set_rtc_time(unsigned int time);

/*skytone version api*/
extern unsigned int TEE_EXT_Get_Skytone_Version();

/* sharemem of fastboot api */
extern int __SRE_Get_SharedMem_Info(module_types type, unsigned int *buffer, unsigned int size);

/*hi_vcodec*/
extern int __SEC_VDEC_Init(unsigned int Args, unsigned int Share);
extern int __SEC_VDEC_Exit(unsigned int IsSecure);
extern int __SEC_VDEC_Control(int ChanID, unsigned int eCmdID, unsigned int Args, unsigned int ArgLen);
extern int __SEC_VDEC_Suspend(void);
extern int __SEC_VDEC_Resume(void);
extern int __SEC_VDEC_RunProcess(unsigned int Args, unsigned int ArgLen);
extern int __SEC_VDEC_ReadProc(unsigned int Page, int Count);
extern int __SEC_VDEC_WriteProc(unsigned int Option, int Value);
extern int __SEC_VDEC_GetChanImage(int ChanID, unsigned int Image);
extern int __SEC_VDEC_ReleaseChanImage(int ChanID, unsigned int Image);

int ss_crypto_use_cbc();
