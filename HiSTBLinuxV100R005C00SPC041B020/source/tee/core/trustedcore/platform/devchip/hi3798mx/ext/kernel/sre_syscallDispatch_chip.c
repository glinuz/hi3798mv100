#include "sre_task.h"
#include "sre_base.h"
#include "sre_typedef.h"
#include "sre_syscalls_id_chip.h"
#include "sre_typedef.h"
#include "sre_taskmem.h"
#include "sre_securemmu.h"
#include "sre_access_control.h"
#ifdef WITH_HISI_CIPHER
#include "hi_type.h"
#include "tee_drv_cipher_define.h"
#include "hi_tee_cipher_ioctl.h"
#include "tee_drv_cipher_intf.h"
#endif
#ifdef WITH_HISI_OTP
#include "tee_drv_otp.h"
#endif

#ifdef WITH_HISI_ADVCA
#include "tee_drv_advca.h"
#include "tee_drv_advca_ext.h"
#endif

#ifdef WITH_HISI_SMMU
#include "hi_smmu.h"
#endif

#ifdef WITH_HISI_VDP
#include "drv_vdp_intf.h"
#include "hi_tee_vdp.h"
#endif

#ifdef WITH_HISI_DMX
#include "tee_drv_demux.h"
#endif
#ifdef WITH_HISI_PVR
#include "tee_drv_pvr.h"
#endif
#ifdef WITH_HISI_SEC_HDMI
#include "tee_drv_hdmi.h"
#endif

#ifdef WITH_HISI_SEC_MMZ
/* SEC MMZ */
#include "mmz-userdev.h"
#include "mmz_struct.h"
extern int MMZ_MALLOC(void *pBuf);
extern int MMZ_FREE(void *pBuf);
extern int MMZ_NEW(void *pBuf, char *mmz_name, char *mmb_name);
extern int MMZ_DEL(unsigned long addr);
extern int MMZ_MAP(unsigned long addr, unsigned long cached);
extern int MMZ_UNMAP(unsigned long addr);
extern int MMZ_FLUSH(unsigned long addr);
/* SEC MEM */
extern int TEE_IsSecMem(unsigned long phys_addr, unsigned long size);
extern int TEE_IsNonSecMem(unsigned long phys_addr, unsigned long size);
#endif

int swi_handler_chip(int swi_id, TSK_REGS_S *regs, UINT64 permissions)
{

    UINT32 uwRet = 0;
    /* According to ARM AAPCS arguments from 5-> in a function call
     * are stored on the stack, which in this case is pointer by
     * user sp. Our own TrustedCore also push FP and LR on the stack
     * just before SWI, so skip them */
    UINT32  *args = (UINT32 *)(regs->usr_sp + 8);

    HANDLE_SYSCALL(swi_id)
	{
#ifdef WITH_HISI_VFMW
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_INIT, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Init(regs->r0);
			regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_EXIT, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Exit();
		    regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_RESUME, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Resume();
			regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_SUSPEND, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Suspend();
			regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_CONTROL, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Control((int)regs->r0, regs->r1, regs->r2);
			regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_THREAD, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Thread();
			regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_GET_IMAGE, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Get_Image((int)regs->r0, regs->r1);
		    regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_RELEASE_IMAGE, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Release_Image((int)regs->r0, regs->r1);
		    regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_READ_PROC, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Read_Proc(regs->r0, (int)regs->r1);
		    regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_VFMW_WRITE_PROC, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = SEC_VFMW_Write_Proc(regs->r0, (int)regs->r1);
			regs->r0 = uwRet;
		SYSCALL_END;
#endif

#ifdef WITH_HISI_VDP
        SYSCALL_PERMISSION(SW_SYSCALL_VDP_DRV_IOCTL, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = vdp_drv_Ioctl((int)regs->r0, (void *)regs->r1);   
        regs->r0 = uwRet;
        SYSCALL_END;
#endif 
#ifdef WITH_HISI_SEC_MMZ
		SYSCALL_PERMISSION(SW_SYSCALL_MMZ_NEW, permissions, GENERAL_GROUP_PERMISSION)
			ACCESS_CHECK(regs->r0, sizeof(HI_MMZ_BUF_S));
			ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(HI_MMZ_BUF_S));
			ACCESS_CHECK(regs->r1, HIL_MMZ_NAME_LEN);
			ACCESS_READ_RIGHT_CHECK(regs->r1, HIL_MMZ_NAME_LEN);
			ACCESS_CHECK(regs->r2, HIL_MMB_NAME_LEN);
			ACCESS_READ_RIGHT_CHECK(regs->r2, HIL_MMB_NAME_LEN);
			uwRet = MMZ_NEW(regs->r0, (char *)regs->r1, (char *)regs->r2);
			regs->r0 = uwRet;
        SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_MMZ_DEL, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = MMZ_DEL(regs->r0);
			regs->r0 = uwRet;
        SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_MMZ_MALLOC, permissions, GENERAL_GROUP_PERMISSION)
			ACCESS_CHECK(regs->r0, sizeof(HI_MMZ_BUF_S));
			ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(HI_MMZ_BUF_S));
			ACCESS_WRITE_RIGHT_CHECK(regs->r0, sizeof(HI_MMZ_BUF_S));
			uwRet = MMZ_MALLOC(regs->r0);
			regs->r0 = uwRet;
        SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_MMZ_FREE, permissions, GENERAL_GROUP_PERMISSION)
			ACCESS_CHECK(regs->r0, sizeof(HI_MMZ_BUF_S));
			ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(HI_MMZ_BUF_S));
			uwRet = MMZ_FREE(regs->r0);
			regs->r0 = uwRet;
			SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_MMZ_MAP, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = MMZ_MAP((unsigned long)regs->r0, (unsigned long)regs->r1);
			regs->r0 = uwRet;
			SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_MMZ_UNMAP, permissions, GENERAL_GROUP_PERMISSION)
			ACCESS_CHECK(regs->r0, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r0, 0);
			uwRet = MMZ_UNMAP((unsigned long)regs->r0);
			regs->r0 = uwRet;
			SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_MMZ_FLUSH, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = MMZ_FLUSH((unsigned long)regs->r0);
			regs->r0 = uwRet;
			SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_TEE_ISSECMMZ, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = TEE_IsSecMMZ((unsigned long)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_TEE_ISSECMEM, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = TEE_IsSecMem((unsigned long)regs->r0, (unsigned long)regs->r1);
			regs->r0 = uwRet;
			SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_TEE_ISNONSECMEM, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = TEE_IsNonSecMem((unsigned long)regs->r0, (unsigned long)regs->r1);
			regs->r0 = uwRet;
			SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_DEBUG, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = call_debug((unsigned long)regs->r0, (unsigned long)regs->r1, (unsigned long)regs->r2, (unsigned long)regs->r3);
		regs->r0 = uwRet;
		SYSCALL_END;
#endif
#ifdef WITH_HISI_CIPHER
		SYSCALL_PERMISSION(SW_SYSCALL_CIPHER_IOCTL, permissions, GENERAL_GROUP_PERMISSION)
        ACCESS_CHECK(regs->r1, TEE_CIPHER_IOC_SIZE(regs->r0));
        ACCESS_WRITE_RIGHT_CHECK(regs->r1, TEE_CIPHER_IOC_SIZE(regs->r0));
		uwRet = TEE_CIPHER_Ioctl(regs->r0, (void*)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_CIPHER_PBKDF2, permissions, GENERAL_GROUP_PERMISSION)
        HI_TEE_CIPHER_PBKDF2_S *pstInfo;
        pstInfo = (HI_TEE_CIPHER_PBKDF2_S*)regs->r0;
        ACCESS_CHECK(regs->r0, sizeof(HI_TEE_CIPHER_PBKDF2_S));
        ACCESS_WRITE_RIGHT_CHECK(regs->r0, sizeof(HI_TEE_CIPHER_PBKDF2_S));
        ACCESS_CHECK(regs->r1, pstInfo->key_length);
        ACCESS_WRITE_RIGHT_CHECK(regs->r1, pstInfo->key_length);
		uwRet = CRYS_PKCS5_PBKDF2_HMAC1(regs->r0, regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;
#endif

#ifdef WITH_HISI_OTP
		SYSCALL_PERMISSION(SW_SYSCALL_OTP_INIT, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = OTP_Init();            
		regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_OTP_DeInit, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = OTP_DeInit();            
		regs->r0 = uwRet;
		SYSCALL_END;		
		SYSCALL_PERMISSION(SW_SYSCALL_OTP_GetChipID, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = OTP_GetChipID((HI_U32 *) regs->r0);            
		regs->r0 = uwRet;
		SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_OTP_Reset, permissions, GENERAL_GROUP_PERMISSION)
        	OTP_SetReset();            
        regs->r0 = 0;
        SYSCALL_END;		
		SYSCALL_PERMISSION(SW_SYSCALL_OTP_ReadWord, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = OTP_ReadWord((HI_U32) regs->r0, (HI_U32 *) regs->r1);            
		regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_OTP_ReadByte, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = OTP_ReadByte((HI_U32) regs->r0, (HI_U8 *) regs->r1);           
		regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_OTP_WriteWord, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = OTP_WriteWord((HI_U32) regs->r0, (HI_U32) regs->r1);           
		regs->r0 = uwRet;
		SYSCALL_END;
		SYSCALL_PERMISSION(SW_SYSCALL_OTP_WriteByte, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = OTP_WriteByte((HI_U32) regs->r0, (HI_U8) regs->r1);           
		regs->r0 = uwRet;
		SYSCALL_END; 
#endif
#ifdef WITH_HISI_ADVCA

		SYSCALL_PERMISSION(SW_SYSCALL_ADVCA_IOCTL, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = TEE_ADVCA_Ioctl(regs->r0, (void*)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;

#endif
#ifdef WITH_HISI_SMMU
		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_MAPTOSMMU, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_sec_maptosmmu((unsigned long)regs->r0, (unsigned long)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_UNMAPFROMSMMU, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_sec_unmapfromsmmu((unsigned long)regs->r0, (unsigned long)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_MAPTOSMMU_AND_SETFLAG, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_sec_maptosmmu_and_setflag((unsigned long)regs->r0, (unsigned long)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_UNMAPFROMSMMU_AND_CLRFLG, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_sec_unmapfromsmmu_and_clrflg((unsigned long)regs->r0, (unsigned long)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_ALLOC, permissions, GENERAL_GROUP_PERMISSION)
			ACCESS_CHECK(regs->r0, HIL_MAX_NAME_LEN);
			ACCESS_READ_RIGHT_CHECK(regs->r0, HIL_MAX_NAME_LEN);
			uwRet = hisi_sec_alloc((const char *)regs->r0, (unsigned long)regs->r1, (unsigned long)regs->r2, (int)regs->r3);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_FREE, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_sec_free((unsigned long)regs->r0, (int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_MAP_TO_CPU, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_sec_map_to_cpu((unsigned long)regs->r0, (int)regs->r1, (int)regs->r2);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_UNMAP_FROM_CPU, permissions, GENERAL_GROUP_PERMISSION)
			ACCESS_CHECK(regs->r0, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r0, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r0, 0);
			uwRet = hisi_sec_unmap_from_cpu((void *)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_MAP_TO_SEC_SMMU, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_sec_map_to_sec_smmu((unsigned long)regs->r0, (unsigned long)regs->r1, (int)regs->r2);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_SEC_UNMAP_FROM_SEC_SMMU, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_sec_unmap_from_sec_smmu((unsigned long)regs->r0, (int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_NONSEC_MEM_MAP_TO_SEC_CPU, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_nonsec_mem_map_to_sec_cpu((unsigned long)regs->r0, (int)regs->r1, (int)regs->r2);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_NOSEC_MEM_UNMAP_FROM_SEC_CPU, permissions, GENERAL_GROUP_PERMISSION)
			ACCESS_CHECK(regs->r0, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r0, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r0, 0);
			uwRet = hisi_nosec_mem_unmap_from_sec_cpu((void *)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_NOSEC_MEM_MAP_TO_SEC_SMMU, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_nosec_mem_map_to_sec_smmu((unsigned long)regs->r0, (int)regs->r1);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_NOSEC_MEM_UNMAP_FROM_SEC_SMMU, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = hisi_nosec_mem_unmap_from_sec_smmu((unsigned long)regs->r0);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_AGENT_CLOSED, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = agent_closed();
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_SEC_IOCTL, permissions, GENERAL_GROUP_PERMISSION)
			uwRet = sec_ioctl((unsigned long)regs->r0, (unsigned long)regs->r1, (unsigned long)regs->r2);
		regs->r0 = uwRet;
		SYSCALL_END;

		SYSCALL_PERMISSION(SW_SYSCALL_HISI_MEM_FLUSH, permissions, GENERAL_GROUP_PERMISSION)
			ACCESS_CHECK(regs->r0, regs->r1);
			ACCESS_READ_RIGHT_CHECK(regs->r0, regs->r1);
			ACCESS_WRITE_RIGHT_CHECK(regs->r0, regs->r1);
		    	hisi_mem_flush((void *)regs->r0, (unsigned long)regs->r1);
		SYSCALL_END;
#endif

#ifdef WITH_HISI_DMX
	SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_NEW_DESC, permissions, GENERAL_GROUP_PERMISSION)
	        ACCESS_CHECK(regs->r1, sizeof(HI_TEE_DMX_DESCRAMBLER_ATTR_S));
			ACCESS_READ_RIGHT_CHECK(regs->r1, sizeof(HI_TEE_DMX_DESCRAMBLER_ATTR_S));
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, sizeof(HI_TEE_DMX_DESCRAMBLER_ATTR_S));

            ACCESS_CHECK(regs->r2, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r2, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r2, 0);
            
            uwRet = TEE_DRV_DMX_CreateDescrambler(regs->r0, (HI_TEE_DMX_DESCRAMBLER_ATTR_S *)regs->r1, (HI_HANDLE *)regs->r2, regs->r3);
            regs->r0 = uwRet;
        SYSCALL_END;

	    SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_DEL_DESC, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_DestroyDescrambler(regs->r0);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_ATTACH_DESC, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_AttachDescrambler(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_DETACH_DESC, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_DetachDescrambler(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_GET_DESC_ATTR, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r1, sizeof(HI_TEE_DMX_DESCRAMBLER_ATTR_S));
			ACCESS_READ_RIGHT_CHECK(regs->r1, sizeof(HI_TEE_DMX_DESCRAMBLER_ATTR_S));
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, sizeof(HI_TEE_DMX_DESCRAMBLER_ATTR_S));
            
            uwRet = TEE_DRV_DMX_GetDescramblerAttr(regs->r0, (HI_TEE_DMX_DESCRAMBLER_ATTR_S *)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_SET_DESC_ATTR, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r1, sizeof(HI_TEE_DMX_DESCRAMBLER_ATTR_S));
			ACCESS_READ_RIGHT_CHECK(regs->r1, sizeof(HI_TEE_DMX_DESCRAMBLER_ATTR_S));
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, sizeof(HI_TEE_DMX_DESCRAMBLER_ATTR_S));
            
            uwRet = TEE_DRV_DMX_SetDescramblerAttr(regs->r0, (HI_TEE_DMX_DESCRAMBLER_ATTR_S *)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_SET_DESC_EVEN, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r1, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r1, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, 0);
            
            uwRet = TEE_DRV_DMX_SetDescramblerEvenKey(regs->r0, (HI_U8*)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_SET_DESC_ODD, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r1, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r1, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, 0);
            
            uwRet = TEE_DRV_DMX_SetDescramblerOddKey(regs->r0, (HI_U8*)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_SET_DESC_EVEN_IV, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r1, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r1, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, 0);
            
            uwRet = TEE_DRV_DMX_SetDescramblerEvenIVKey(regs->r0, (HI_U8*)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_SET_DESC_ODD_IV, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r1, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r1, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, 0);
            
            uwRet = TEE_DRV_DMX_SetDescramblerOddIVKey(regs->r0, (HI_U8*)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_GET_KEYID, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r1, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r1, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r1, 0);
            
            uwRet = TEE_DRV_DMX_GetDescramblerKeyHandle(regs->r0, (HI_HANDLE *)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_NEW_SC, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_CreateScrambler(regs->r0, (HI_TEE_DMX_SCRAMBLER_ATTR_S *)regs->r1, (HI_HANDLE *)regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_DEL_SC, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_DestroyScrambler(regs->r0);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_GET_SC_ATTR, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_GetScramblerAttr(regs->r0, (HI_TEE_DMX_SCRAMBLER_ATTR_S *)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_SET_SC_ATTR, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_SetScramblerAttr(regs->r0, (HI_TEE_DMX_SCRAMBLER_ATTR_S *)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_SET_SC_EVEN, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_SetScramblerEvenKey(regs->r0, (HI_U8*)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_SET_SC_ODD, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_SetScramblerOddKey(regs->r0, (HI_U8*)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_ATTACH_SC, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_AttachScrambler(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_DETACH_SC, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_DetachScrambler(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_GET_SC_KEYID, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_GetScramblerKeyHandle(regs->r0, (HI_HANDLE *)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_GET_CHNID, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r2, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r2, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r2, 0);
            
            uwRet = TEE_DRV_DMX_GetChannelHandle(regs->r0, regs->r1, (HI_HANDLE *)regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_REG_CHAN, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_RegisterChannel(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_UNREG_CHAN, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_UnregisterChannel(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

	    SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_REG_OQ, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_RegisterOq(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_UNREG_OQ, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_UnregisterOq(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

    	SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_REG_RAM_PORT, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_RegisterRamPort(regs->r0);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_UNREG_RAM_PORT, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_UnregisterRamPort(regs->r0);
            regs->r0 = uwRet;
        SYSCALL_END;
        
        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_LOCK_CHAN, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_LockChannel(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_UNLOCK_CHAN, permissions, GENERAL_GROUP_PERMISSION)
            TEE_DRV_DMX_UnlockChannel(regs->r0, regs->r1);
            regs->r0 = 0;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_REG_VID_SECBUF, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_RegisterVidSecBuf(regs->r0, regs->r1, regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_UNREG_VID_SECBUF, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_UnregisterVidSecBuf(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_REG_AUD_SECBUF, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_RegisterAudSecBuf(regs->r0, regs->r1, regs->r2, regs->r3, args[0]);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_UNREG_AUD_SECBUF, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_UnregisterAudSecBuf(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_FIXUP_AUD_SECBUF, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_FixupAudSecBuf(regs->r0, regs->r1, regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_PARSER_PES_HEADER, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r3, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r3, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r3, 0);
            
            ACCESS_CHECK(args[0], 0);
			ACCESS_READ_RIGHT_CHECK(args[0], 0);
			ACCESS_WRITE_RIGHT_CHECK(args[0], 0);
            
            ACCESS_CHECK(args[1], 0);
			ACCESS_READ_RIGHT_CHECK(args[1], 0);
			ACCESS_WRITE_RIGHT_CHECK(args[1], 0);
            
            ACCESS_CHECK(args[2], 0);
			ACCESS_READ_RIGHT_CHECK(args[2], 0);
			ACCESS_WRITE_RIGHT_CHECK(args[2], 0);
            
            uwRet = TEE_DRV_DMX_ParserPesHeader(regs->r0, regs->r1, regs->r2, (HI_U32 *)regs->r3, (HI_U32 *)args[0], (HI_U32 *)args[1], (HI_U8 *)args[2]);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_PARSER_DISP_CONTROL, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r3, sizeof(Disp_Control_t));
			ACCESS_READ_RIGHT_CHECK(regs->r3, sizeof(Disp_Control_t));
			ACCESS_WRITE_RIGHT_CHECK(regs->r3, sizeof(Disp_Control_t));
            
            uwRet = TEE_DRV_DMX_ParserPesDispCtrlInfo(regs->r0, regs->r1, regs->r2, (Disp_Control_t *)regs->r3);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_GET_PES_HEADER_LEN, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r3, 0);
			ACCESS_READ_RIGHT_CHECK(regs->r3, 0);
			ACCESS_WRITE_RIGHT_CHECK(regs->r3, 0);
            
            uwRet = TEE_DRV_DMX_GetPesHeaderLen(regs->r0, regs->r1, regs->r2, (HI_U32 *)regs->r3);
            regs->r0 = uwRet;
        SYSCALL_END;   

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_REG_REC_SECBUF, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_RegisterRecSecBuf(regs->r0, regs->r1, regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END; 

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_UNREG_REC_SECBUF, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_UnregisterRecSecBuf(regs->r0, regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_PARSER_SC_DATA, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_DMX_ParserSCData(regs->r0, regs->r1, regs->r2, regs->r3, args[0]);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_DEMUX_PARSER_FILTER_SC_DATA, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(args[1], 0);
			ACCESS_READ_RIGHT_CHECK(args[1], 0);
			ACCESS_WRITE_RIGHT_CHECK(args[1], 0);
            
            uwRet = TEE_DRV_DMX_ParserFilterSCData(regs->r0, regs->r1, regs->r2, regs->r3, args[0], (HI_U32 *)args[1]);
            regs->r0 = uwRet;
        SYSCALL_END;        
#endif
#ifdef WITH_HISI_PVR
        SYSCALL_PERMISSION(SW_SYSCALL_PVR_DeInit, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_PVR_DeInit();
            regs->r0 = uwRet;
        SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_PVR_OPEN_CHANNEL, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r0, sizeof(HI_U32));
            ACCESS_READ_RIGHT_CHECK(regs->r0, sizeof(HI_U32));
            uwRet = TEE_DRV_PVR_OpenChannel((HI_U32 *)regs->r0, (HI_S32)regs->r1, (TEE_PVR_TYPE_E)regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END; 
        SYSCALL_PERMISSION(SW_SYSCALL_PVR_CLOSE_CHANNEL, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_PVR_CloseChannel((HI_U32)regs->r0);
            regs->r0 = uwRet;
        SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_PVR_PROC_TSDATA, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_PVR_RecProcTsData((HI_U32)regs->r0, (HI_U32)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END; 
        SYSCALL_PERMISSION(SW_SYSCALL_PVR_COPY_TO_REE, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r2, sizeof(HI_U32));
            ACCESS_READ_RIGHT_CHECK(regs->r2, sizeof(HI_U32));
            uwRet = TEE_DRV_PVR_RecCopyToRee((HI_U32)regs->r0, (HI_U32)regs->r1, (HI_U32 *)regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_PVR_COPY_TO_TEE, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_PVR_PlayCopyToTee((HI_U32)regs->r0, (HI_U32)regs->r1, (HI_U32)regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_PVR_GET_OUTPUTBUF, permissions, GENERAL_GROUP_PERMISSION)
            ACCESS_CHECK(regs->r1, sizeof(HI_U32));
            ACCESS_READ_RIGHT_CHECK(regs->r1, sizeof(HI_U32));
            ACCESS_CHECK(regs->r2, sizeof(HI_U32));
            ACCESS_READ_RIGHT_CHECK(regs->r2, sizeof(HI_U32)); 
            uwRet = TEE_DRV_PVR_RecGetOutputBufInfo((HI_U32)regs->r0, (HI_U32*)regs->r1, (HI_U32 *)regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END;
        SYSCALL_PERMISSION(SW_SYSCALL_PVR_GET_TEE_STATE, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = TEE_DRV_PVR_RecGetStatus((HI_U32)regs->r0, (HI_U32)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;
#endif
#ifdef WITH_HISI_SEC_HDMI

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_SETSRM, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_SetSRM((HI_DRV_HDMI_DEVICE_ID_E)regs->r0, (HI_U8 *)regs->r1);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_SETHDCPSTRATEGY, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_SetHDCPStrategy((HI_DRV_HDMI_DEVICE_ID_E)regs->r0, (HI_DRV_HDMI_HDCP_LEVEL_E)regs->r1);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_GETHDCPSTRATEGY, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_GetHDCPStrategy((HI_DRV_HDMI_DEVICE_ID_E)regs->r0, (HI_DRV_HDMI_HDCP_LEVEL_E *)regs->r1);
            regs->r0 = uwRet;
            SYSCALL_END;           

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_GETHDCPSTATUS, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_GetHDCPStatus((HI_DRV_HDMI_DEVICE_ID_E)regs->r0, (HI_DRV_HDMI_HDCP_STATUS_E *)regs->r1);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_SETHDCPMUTE, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_SetHDCPMute((HI_DRV_HDMI_DEVICE_ID_E)regs->r0, (HI_BOOL)regs->r1);
            regs->r0 = uwRet;
            SYSCALL_END;
        
        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_HDCP22_INIT, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_HDCP22_Init((HI_DRV_HDMI_DEVICE_ID_E)regs->r0);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_TEEPROC, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_TeeProc((HI_DRV_HDMI_DEVICE_ID_E)regs->r0);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_SETHDCPCAPS, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_SetHDCPCaps((HI_DRV_HDMI_DEVICE_ID_E)regs->r0, (HI_DRV_HDMI_HDCP_CAPS_S *)regs->r1);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_IOCTRL, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_IOCTRL((HI_DRV_HDMI_DEVICE_ID_E)regs->r0, (HI_VOID *)regs->r1,(HI_U32)regs->r2);
            regs->r0 = uwRet;
            SYSCALL_END;


        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_GETHDCPCAPS, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_GetHDCPCaps((HI_DRV_HDMI_DEVICE_ID_E)regs->r0, (HI_DRV_HDMI_HDCP_CAPS_S *)regs->r1);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_INIT, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_Init((HI_DRV_HDMI_DEVICE_ID_E)regs->r0);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_DEINIT, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_DeInit((HI_DRV_HDMI_DEVICE_ID_E)regs->r0);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_START, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_Start((HI_DRV_HDMI_DEVICE_ID_E)regs->r0);
            regs->r0 = uwRet;
            SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_HI_DRV_HDMI_STOP, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_HDMI_Stop((HI_DRV_HDMI_DEVICE_ID_E)regs->r0);
            regs->r0 = uwRet;
            SYSCALL_END;

#endif
#ifdef WITH_STB_BEIDOU
        SYSCALL_PERMISSION(SW_SYSCALL_BEIDOU_GET_SW_VER, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = Beidou_GetSoftwareVersion((char * )regs->r0, (unsigned int)regs->r1);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_BEIDOU_GET_POS_PARAM, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = Beidou_GetPositionParameters((int *)regs->r0, (int *)regs->r1, (unsigned int *)regs->r2);
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_BEIDOU_CALC_DISTANCE, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = Beidou_CalculateDistance((int)regs->r0, (int)regs->r1, (int)regs->r2, (int)regs->r3);
            regs->r0 = uwRet;
        SYSCALL_END;
#endif

#ifdef WITH_STB_I2C_DRIVER
        SYSCALL_PERMISSION(SW_SYSCALL_I2C_INIT, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_I2C_Init();
            regs->r0 = uwRet;
        SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_I2C_DEINIT, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_I2C_DeInit();
            regs->r0 = uwRet;
        SYSCALL_END;


        SYSCALL_PERMISSION(SW_SYSCALL_I2C_WRITE, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_I2C_Write(
	    (HI_U32 )regs->r0,
	    (HI_U8)regs->r1,
	    (HI_U32)regs->r2,
	    (HI_U32)regs->r3,
	    (HI_U8 *)args[0],
	    (HI_U32 )args[1]);
	     regs->r0 = uwRet;
	 SYSCALL_END;


        SYSCALL_PERMISSION(SW_SYSCALL_I2C_READ, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_I2C_Read(
	    (HI_U32 )regs->r0,
	    (HI_U8)regs->r1,
	    (HI_U32)regs->r2,
	    (HI_U32)regs->r3,
	    (HI_U8 *)args[0],
	    (HI_U32 )args[1]);
	     regs->r0 = uwRet;
	 SYSCALL_END;

        SYSCALL_PERMISSION(SW_SYSCALL_I2C_SET_SPEED, permissions, GENERAL_GROUP_PERMISSION)
            uwRet = HI_DRV_I2C_SetRate(
	    (HI_U32)regs->r0,
	    (HI_U32)regs->r1);
	    regs->r0 = uwRet;
	 SYSCALL_END;
#endif
            default:
                return -1;
    }
    return 0;

}
