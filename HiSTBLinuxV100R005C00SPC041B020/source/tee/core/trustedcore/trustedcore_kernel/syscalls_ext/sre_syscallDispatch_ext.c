#include "sre_task.h"
#include "sre_base.h"
#include "sre_typedef.h"
#include "sre_syscalls_id_ext.h"
#include "sre_syscalls_id.h"
#include "sre_typedef.h"
#include "se_hal.h"
#include "hisi_disp.h"
#include "sre_taskmem.h"
#include "sre_securemmu.h"
#include "hisi_tui_touchscreen.h"
#include "sre_access_control.h"
#include "demo.h"
#ifdef FEATURE_IRIS
#include "iris_mmap.h"
#endif
#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_KIRIN_PLATFORM)
#include "tee_fingerprint.h"
#endif
#if ((TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3650) || (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI6250))
#include "secureapi.h"
#endif
#include "sre_sharedmem.h"
#if (TRUSTEDCORE_PLATFORM_CHOOSE != WITH_DEVCHIP_PLATFORM)
#include "bsp_modem_call.h"
#include "tui_ext.h"
#endif

#define PRODCUT_MAX_LEN 64
#define NOTIFY_ENTRY_SIZE 512

#define DIE_ID_SIZE (5*4) //define in efuse/hisi_efuse.h

extern int SECURE_TEE_Mmap(paddr_t phy_addr , unsigned int size, unsigned int *virt_addr, unsigned int secure_mode, unsigned int cache_mode);
extern int SECURE_TEE_Unmap(unsigned int virt_addr, unsigned int size);
extern int SECURE_ISSecureMemory(paddr_t addr, unsigned int size);
extern UINT32 hisi_secboot_copy_soc_data(UINT32 soc_type, UINT32 offset, const paddr_t src_addr, UINT32 len);
extern UINT32 hisi_secboot_soc_verification(UINT32 SoC_Type, UINT32 vrlAddress, paddr_t imageAddress, UINT32 lock_state);
extern UINT32 hisi_secboot_soc_reset(UINT32 soc_type);
extern UINT32 hisi_secboot_soc_set(UINT32 soc_type, int set_type);
extern void hisi_secboot_get_vrl_addr(UINT32 vrl_address);
extern unsigned int sre_get_rtc_time();
extern void sre_set_rtc_time(unsigned int time);
extern void send_simu_tp(void *tp_emui);
extern INT32 is_device_rooted(void);
extern int get_vsim_sharemem(unsigned int *addr, unsigned int *size);
extern int p61_factory_test(void);
#if (TRUSTEDCORE_PLATFORM_CHOOSE != WITH_DEVCHIP_PLATFORM)
extern int SecBoot_get_secinfo_dieid(unsigned int *pBuffer);
#endif
extern void uart_printf_func(const char *fmt, ...);

static const char *target_product_value = TARGET_PRODUCT_VAL; /*lint !e40 */

#ifdef TEE_SUPPORT_HIVCODEC
extern int SEC_VDEC_Init(unsigned int, unsigned int);
extern int SEC_VDEC_Exit(unsigned int);
extern int SEC_VDEC_Control(int, unsigned int, unsigned int, unsigned int);
extern int SEC_VDEC_Suspend(void);
extern int SEC_VDEC_Resume(void);
extern int SEC_VDEC_RunProcess(unsigned int, unsigned int);
extern int SEC_VDEC_ReadProc(unsigned int, int);
extern int SEC_VDEC_WriteProc(unsigned int, int);
extern int SEC_VDEC_GetChanImage(int, unsigned int);
extern int SEC_VDEC_ReleaseChanImage(int, unsigned int);
#endif

int get_target_product(char *target_product, int *size)
{
	if (NULL == target_product || NULL == size)
		return -1;

	if (*size < 1 || *size > PRODCUT_MAX_LEN)
		return -1;

	if (strncpy_s(target_product, (uint32_t)(*size - 1), target_product_value,
			(uint32_t)(*size - 1)))
		return -1;

	int des_len = strlen(target_product_value);

	if (*size > des_len)
		*size = des_len;

	return 0;
}

int swi_handler_ext(int swi_id, TSK_REGS_S *regs, UINT64 permissions)
{

	UINT32 uwRet = 0;
	/* According to ARM AAPCS arguments from 5-> in a function call
	 * are stored on the stack, which in this case is pointer by
	 * user sp. Our own TrustedCore also push FP and LR on the stack
	 * just before SWI, so skip them */
	UINT32  *args = (UINT32 *)(regs->usr_sp + 8);
#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_DEVCHIP_PLATFORM)
	if (0 == swi_handler_chip(swi_id, regs, permissions)) {
		return 0;
	}
#endif

	HANDLE_SYSCALL(swi_id) {
#if ((TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3650) || (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI6250))
		SYSCALL_PERMISSION(SW_SYSCALL_SECURE_TEE_GETID, permissions, VDEC_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r1, sizeof(IMG_UINT32));
		ACCESS_WRITE_RIGHT_CHECK(regs->r1, sizeof(IMG_UINT32));
		uwRet = (UINT32)SECURE_TEE_GetId((IMG_VIDEO_CORE)regs->r0, (IMG_UINT32 *)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_SECURE_TEE_RELEASEID, permissions, VDEC_GROUP_PERMISSION)
		uwRet = (UINT32)SECURE_TEE_ReleaseId((IMG_UINT32)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_SECURE_TEE_SENDMESSAGE, permissions, VDEC_GROUP_PERMISSION)
		uwRet = (UINT32)SECURE_TEE_SendMessage((IMG_UINT32)regs->r0, (IMG_BYTE *)regs->r1,
				(IMG_UINT16)regs->r2, (MSG_ENDPOINT)regs->r3, (IMG_BYTE *)args[0],
				(IMG_UINT32)args[1]);
		regs->r0 = uwRet;
		SYSCALL_END
#endif
		SYSCALL_PERMISSION(SW_SYSCALL_SECURE_TEE_MMAP, permissions, VDEC_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r2, sizeof(UINT32));
		ACCESS_WRITE_RIGHT_CHECK(regs->r2, sizeof(UINT32));
		uwRet = (UINT32)SECURE_TEE_Mmap((paddr_t)regs->r0, (unsigned int)regs->r1,
				(unsigned int *)regs->r2, (unsigned int)regs->r3, (unsigned int)args[0]);
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_SECURE_TEE_UNMAP, permissions, VDEC_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(unsigned int));
		uwRet = (UINT32)SECURE_TEE_Unmap((unsigned int)regs->r0, (unsigned int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_SECURE_ISSEUCREMEM, permissions, VDEC_GROUP_PERMISSION)
		uwRet = (UINT32)SECURE_ISSecureMemory((paddr_t)regs->r0, (unsigned int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_TEE_HAL_GET_DIEID, permissions,
				GENERAL_GROUP_PERMISSION) /*lint !e835 !e587 !e774 */
		ACCESS_CHECK(regs->r0, DIE_ID_SIZE);
		ACCESS_WRITE_RIGHT_CHECK(regs->r0, DIE_ID_SIZE);
#if (TRUSTEDCORE_PLATFORM_CHOOSE != WITH_DEVCHIP_PLATFORM)
		uwRet = (UINT32)SecBoot_get_secinfo_dieid((unsigned int *)regs->r0);
		regs->r0 = uwRet;
#endif
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_DRIVER_TEST, permissions,
				   GENERAL_GROUP_PERMISSION) /*lint !e835 !e587 !e774 */
		uwRet = (UINT32)driver_dep_test();
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_SCARD_CONNECT, permissions,
				   GENERAL_GROUP_PERMISSION) /*lint !e835 !e587 !e774 */
		ACCESS_CHECK(regs->r2, sizeof(unsigned int));
		ACCESS_WRITE_RIGHT_CHECK(regs->r2, sizeof(unsigned int));
		if (regs->r2) {
			ACCESS_CHECK(regs->r1, *((unsigned int *)(regs->r2)));
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, *((unsigned int *)(regs->r2)));
		}
		uwRet = (UINT32)scard_connect((int)regs->r0, (void *)regs->r1, (unsigned int *)regs->r2);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SCARD_DISCONNECT, permissions,
				GENERAL_GROUP_PERMISSION) /*lint !e835 !e587 !e774 */
		uwRet = (UINT32)scard_disconnect((int)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SCARD_TRANSMIT, permissions,
				GENERAL_GROUP_PERMISSION) /*lint !e835 !e587 !e774 */
		ACCESS_CHECK(regs->r1, regs->r2);
		ACCESS_READ_RIGHT_CHECK(regs->r1, regs->r2);
		ACCESS_CHECK(args[0], sizeof(unsigned int));
		ACCESS_WRITE_RIGHT_CHECK(args[0], sizeof(unsigned int));
		if (args[0]) {
			ACCESS_CHECK(regs->r3, *(unsigned int *)args[0]);
			ACCESS_WRITE_RIGHT_CHECK(regs->r3, *(unsigned int *)args[0]);
		}
		uwRet = (UINT32)scard_transmit((int)regs->r0, (unsigned char *)regs->r1, (unsigned int)regs->r2, (unsigned char *)regs->r3, (unsigned int *)args[0]);
		regs->r0 = uwRet;
		SYSCALL_END
#ifdef TEE_SUPPORT_TUI
		SYSCALL_PERMISSION(SW_SYSCALL_FB_SEC_CFG, permissions, TUI_GROUP_PERMISSION)
		uwRet = (UINT32)hisi_fb_cfg_sec((int)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_FB_SEC_DISPLAY, permissions, TUI_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(dss_layer_t));
		ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(dss_layer_t));
		if (regs->r0) {
			ACCESS_CHECK(((dss_layer_t *)regs->r0)->img.vir_addr, ((dss_layer_t *)regs->r0)->img.buf_size);
			ACCESS_READ_RIGHT_CHECK(((dss_layer_t *)regs->r0)->img.vir_addr, ((dss_layer_t *)regs->r0)->img.buf_size);
			ACCESS_WRITE_RIGHT_CHECK(((dss_layer_t *)regs->r0)->img.vir_addr, ((dss_layer_t *)regs->r0)->img.buf_size);
		}
		uwRet = (UINT32)hisi_pan_display_sec((dss_layer_t *)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_TP_GET_DATA, permissions, TUI_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(struct ts_tui_finger));
		ACCESS_WRITE_RIGHT_CHECK(regs->r0, sizeof(struct ts_tui_finger));
		uwRet = (UINT32)hisi_tui_get_tpdata((struct ts_tui_finger *)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_TP_SET_KB_RECT, permissions, TUI_GROUP_PERMISSION)
		if (regs->r0) {
			ACCESS_CHECK(regs->r0, sizeof(struct tp_rect));
			ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(struct tp_rect));
		}
		set_kb_rect((int)regs->r0);
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_FB_ACTIVE_FLAG, permissions, TUI_GROUP_PERMISSION)
		uwRet = (UINT32)hisi_wait_vactive_flag();
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_FB_RELEASE_FLAG, permissions, TUI_GROUP_PERMISSION)
		uwRet = (UINT32)hisi_wait_release_flag();
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_FB_GETINFO, permissions, TUI_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(struct hisi_panel_info));
		ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(struct hisi_panel_info));
		uwRet = (UINT32)hisi_get_disp_info((struct hisi_panel_info *)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_TUI_INIT_SDRIVER, permissions, TUI_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(struct tui_tp_t));
		ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(struct tui_tp_t));
		ACCESS_WRITE_RIGHT_CHECK(regs->r0, sizeof(struct tui_tp_t));
		if (regs->r0) {
			ACCESS_CHECK((UINT32)((struct tui_tp_t *)(regs->r0))->notify_addr, NOTIFY_ENTRY_SIZE);
			ACCESS_READ_RIGHT_CHECK((UINT32)((struct tui_tp_t *)regs->r0)->notify_addr, NOTIFY_ENTRY_SIZE);
			ACCESS_WRITE_RIGHT_CHECK((UINT32)((struct tui_tp_t *)regs->r0)->notify_addr, NOTIFY_ENTRY_SIZE);
		}
		uwRet = (UINT32)init_tui_sdriver((int)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_FB_SEC_SET, permissions, TUI_GROUP_PERMISSION)
		uwRet = (UINT32)hisi_fb_alpha_set((int)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_TP_SET_SIMU_DATA, permissions, TUI_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(struct emui_tp));
		ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(struct emui_tp));
		send_simu_tp((void *)regs->r0);
		SYSCALL_END
#endif

#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_KIRIN_PLATFORM)
		SYSCALL_PERMISSION(SW_SYSCALL_P61_FAC_TEST, permissions,
				GENERAL_GROUP_PERMISSION) /*lint !e835 !e587 !e774 */
		uwRet = (UINT32)p61_factory_test();
		regs->r0 = uwRet;
		SYSCALL_END
#endif
		SYSCALL_PERMISSION(SW_SYSCALL_IS_DEVICE_ROOTED, permissions,
				GENERAL_GROUP_PERMISSION) /*lint !e835 !e587 !e774 */
		uwRet = (UINT32)is_device_rooted();
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_GET_SHAREMEM_INFO, permissions,
				GENERAL_GROUP_PERMISSION) /*lint !e835 !e587 !e774 */
		ACCESS_CHECK(regs->r1, regs->r2);
		ACCESS_WRITE_RIGHT_CHECK(regs->r1, regs->r2);
		uwRet = (UINT32)SRE_Get_SharedMem_Info((module_types)regs->r0,
				(unsigned int *)regs->r1, (unsigned int)regs->r2);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_GET_VSIM_SHAREMEM, permissions, VSIM_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(UINT32));
		ACCESS_WRITE_RIGHT_CHECK(regs->r0, sizeof(UINT32));
		ACCESS_CHECK(regs->r1, sizeof(UINT32));
		ACCESS_WRITE_RIGHT_CHECK(regs->r1, sizeof(UINT32));
		uwRet = (UINT32)get_vsim_sharemem((UINT32 *)regs->r0, (UINT32 *)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END
#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_KIRIN_PLATFORM)
		SYSCALL_PERMISSION(SW_SYSCALL_FP_COMMAND_INFO, permissions, FP_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(fingerprint_command_info));
		ACCESS_WRITE_RIGHT_CHECK(regs->r0, sizeof(fingerprint_command_info));
		uwRet = (UINT32)driver_fingerprint_command((fingerprint_command_info *)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_FP_SPI_TRANSACTION, permissions, FP_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(spi_transaction_info));
		ACCESS_CHECK(regs->r1, sizeof(spi_transaction_info));
		ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(spi_transaction_info));
		ACCESS_READ_RIGHT_CHECK(regs->r1, sizeof(spi_transaction_info));
		if ((void *)(regs->r0) != NULL) {
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->reg_addr), ((spi_transaction_info *)regs->r0)->reg_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->reg_addr), ((spi_transaction_info *)regs->r0)->reg_len);
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->buf_addr), ((spi_transaction_info *)regs->r0)->buf_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->buf_addr), ((spi_transaction_info *)regs->r0)->buf_len);
		}
		if ((void *)(regs->r1) != NULL) {
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->reg_addr), ((spi_transaction_info *)regs->r1)->reg_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->reg_addr), ((spi_transaction_info *)regs->r1)->reg_len);
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->buf_addr), ((spi_transaction_info *)regs->r1)->buf_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->buf_addr), ((spi_transaction_info *)regs->r1)->buf_len);
		}
		uwRet = (UINT32)driver_spi_full_duplex((spi_transaction_info *)regs->r0, (spi_transaction_info *)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_FP_SPI_FULL_DUPLEX_WITH_SPEED_TRANSACTION, permissions, FP_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(spi_transaction_info));
		ACCESS_CHECK(regs->r1, sizeof(spi_transaction_info));
		ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(spi_transaction_info));
		ACCESS_READ_RIGHT_CHECK(regs->r1, sizeof(spi_transaction_info));
		if ((void *)(regs->r0) != NULL) {
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->reg_addr), ((spi_transaction_info *)regs->r0)->reg_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->reg_addr), ((spi_transaction_info *)regs->r0)->reg_len);
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->buf_addr), ((spi_transaction_info *)regs->r0)->buf_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->buf_addr), ((spi_transaction_info *)regs->r0)->buf_len);
		}
		if ((void *)(regs->r1) != NULL) {
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->reg_addr), ((spi_transaction_info *)regs->r1)->reg_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->reg_addr), ((spi_transaction_info *)regs->r1)->reg_len);
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->buf_addr), ((spi_transaction_info *)regs->r1)->buf_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->buf_addr), ((spi_transaction_info *)regs->r1)->buf_len);
		}
		uwRet = (UINT32)driver_spi_full_duplex_with_speed((spi_transaction_info *)regs->r0, (spi_transaction_info *)regs->r1, (int)regs->r2);
		regs->r0 = uwRet;
		SYSCALL_END
		SYSCALL_PERMISSION(SW_SYSCALL_FP_SPI_HALF_DUPLEX_WITH_SPEED_TRANSACTION, permissions, FP_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r0, sizeof(spi_transaction_info));
		ACCESS_CHECK(regs->r1, sizeof(spi_transaction_info));
		ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(spi_transaction_info));
		ACCESS_READ_RIGHT_CHECK(regs->r1, sizeof(spi_transaction_info));
		if ((void *)(regs->r0) != NULL) {
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->reg_addr), ((spi_transaction_info *)regs->r0)->reg_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)((spi_transaction_info *)regs->r0)->reg_addr, ((spi_transaction_info *)regs->r0)->reg_len);
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->buf_addr), ((spi_transaction_info *)regs->r0)->buf_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r0)->buf_addr), ((spi_transaction_info *)regs->r0)->buf_len);
		}
		if ((void *)(regs->r1) != NULL) {
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->reg_addr), ((spi_transaction_info *)regs->r1)->reg_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->reg_addr), ((spi_transaction_info *)regs->r1)->reg_len);
			ACCESS_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->buf_addr), ((spi_transaction_info *)regs->r1)->buf_len);
			ACCESS_WRITE_RIGHT_CHECK((unsigned int)(((spi_transaction_info *)regs->r1)->buf_addr), ((spi_transaction_info *)regs->r1)->buf_len);
		}
		uwRet = (UINT32)driver_spi_half_duplex_with_speed((spi_transaction_info *)regs->r0, (spi_transaction_info *)regs->r1, (int)regs->r2);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_BSP_MODEM_CALL, permissions, MDMCALL_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r2, regs->r3);
		ACCESS_WRITE_RIGHT_CHECK(regs->r2, regs->r3);
		uwRet = (UINT32)bsp_modem_call((unsigned int)regs->r0, (unsigned int)regs->r1,
				(void *)regs->r2, (unsigned int)regs->r3);
		regs->r0 = uwRet;
		SYSCALL_END


		SYSCALL_PERMISSION(SW_SYSCALL_SRE_GET_RTC_TIME, permissions, VSIM_GROUP_PERMISSION)
		uwRet = sre_get_rtc_time();
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SRE_SET_RTC_TIME, permissions, VSIM_GROUP_PERMISSION)
		sre_set_rtc_time((unsigned int)regs->r0);
		SYSCALL_END
#endif

#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_BALONG_V722_PLATFORM)
		SYSCALL_PERMISSION(SW_SYSCALL_BSP_MODEM_CALL, permissions, MDMCALL_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r2, regs->r3);
		ACCESS_WRITE_RIGHT_CHECK(regs->r2, regs->r3);
		uwRet = (UINT32)bsp_modem_call((unsigned int)regs->r0, (unsigned int)regs->r1,
				(void *)regs->r2, (unsigned int)regs->r3);
		regs->r0 = uwRet;
		SYSCALL_END
#endif
		SYSCALL_PERMISSION(SW_SYSCALL_GET_TARGET_PRODUCT, permissions,
				GENERAL_GROUP_PERMISSION) /*lint !e835 !e587 !e774 */
		if (regs->r1) {
			ACCESS_CHECK(regs->r1, sizeof(int));
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, sizeof(int));
			ACCESS_CHECK(regs->r0, *(UINT32 *)regs->r1);
			ACCESS_WRITE_RIGHT_CHECK(regs->r0, *(UINT32 *)regs->r1);
		}
		uwRet = (UINT32)get_target_product((char *)regs->r0, (int *)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_COPY_SOC_DATA_TYPE, permissions, SECBOOT_GROUP_PERMISSION)
#ifndef ARM_PAE
		uwRet = hisi_secboot_copy_soc_data(regs->r0, regs->r1, regs->r2, regs->r3);
#else
		uwRet = hisi_secboot_copy_soc_data(regs->r0, regs->r1,
						   regs->r2 | (paddr_t)regs->r3 << 32, args[0]);
#endif
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_VERIFY_SOC_DATA_TYPE, permissions, SECBOOT_GROUP_PERMISSION)
#ifndef ARM_PAE
		uwRet = hisi_secboot_soc_verification((int)regs->r0, (unsigned int)regs->r1, (unsigned int)regs->r2, (int)regs->r3);
#else
		uwRet = hisi_secboot_soc_verification(regs->r0, (unsigned int)regs->r1,
				regs->r2 | (paddr_t)regs->r3 << 32, (int)args[0]); /*lint !e732 */
#endif
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SOC_IMAGE_RESET, permissions, SECBOOT_GROUP_PERMISSION)
		uwRet = hisi_secboot_soc_reset(regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SOC_IMAGE_SET, permissions, SECBOOT_GROUP_PERMISSION)
		uwRet = hisi_secboot_soc_set(regs->r0, (unsigned int)regs->r1); /*lint !e713 */
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SOC_GET_VRL_ADDR, permissions, SECBOOT_GROUP_PERMISSION)
		hisi_secboot_get_vrl_addr(regs->r0);
		SYSCALL_END

#ifdef TEE_SUPPORT_HIVCODEC
		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_INIT, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_Init((unsigned int)regs->r0, (unsigned int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_EXIT, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_Exit((unsigned int)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_CONTROL, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_Control((int)regs->r0, (unsigned int)regs->r1, (unsigned int)regs->r2, (unsigned int)regs->r3);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_SUSPEND, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_Suspend();
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_RESUME, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_Resume();
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_RUNPROCESS, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_RunProcess((unsigned int)regs->r0, (unsigned int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_READPROC, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_ReadProc((unsigned int)regs->r0, (int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_WRITEPROC, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_WriteProc((unsigned int)regs->r0, (int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_GETCHANIMAGE, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_GetChanImage((int)regs->r0, (unsigned int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_VDEC_RELEASECHANIMAGE, permissions, VDEC_GROUP_PERMISSION)
		uwRet = SEC_VDEC_ReleaseChanImage((int)regs->r0, (unsigned int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END
#endif
#ifdef FEATURE_IRIS
		SYSCALL_PERMISSION(SW_SYSCALL_IRIS_TEE_MMAP, permissions, IRIS_GROUP_PERMISSION)
		ACCESS_CHECK(regs->r2, sizeof(UINT32));
		ACCESS_WRITE_RIGHT_CHECK(regs->r2, sizeof(UINT32));
		uwRet = (UINT32)iris_tee_mmap((unsigned int)regs->r0, (unsigned int)regs->r1, (unsigned int *)regs->r2, (unsigned int)regs->r3, (unsigned int)args[0]);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_IRIS_TEE_UNMAP, permissions, IRIS_GROUP_PERMISSION)
		ACCESS_CHECK((unsigned int)regs->r0, sizeof(unsigned int));
		uwRet = (UINT32)iris_tee_unmap((unsigned int)regs->r0, (unsigned int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END

		SYSCALL_PERMISSION(SW_SYSCALL_IRIS_TEE_ISSECUREMEMORY, permissions, IRIS_GROUP_PERMISSION)
		uwRet = (UINT32)iris_tee_isSecureMemory((unsigned int)regs->r0, (unsigned int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END
#endif
	default:
		return -1;
	}
	return 0;

}
