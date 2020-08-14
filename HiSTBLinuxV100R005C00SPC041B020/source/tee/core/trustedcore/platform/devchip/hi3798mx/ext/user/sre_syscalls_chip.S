#include "sre_syscalls_id_chip.h"
#include "sre_syscalls_id.h"
#ifdef WITH_HISI_VFMW
    .global __VFMW_Init
    .global __VFMW_Exit
    .global __VFMW_Resume
    .global __VFMW_Suspend
    .global __VFMW_Control
    .global __VFMW_Thread
    .global __VFMW_Get_Image
    .global __VFMW_Release_Image
    .global __VFMW_Read_Proc
    .global __VFMW_Write_Proc
#endif
    
/*vdp begin*/
#ifdef WITH_HISI_VDP
    .global __vdp_drv_Ioctl
#endif
/*vdp end*/
#ifdef WITH_HISI_SEC_MMZ
    .global __MMZ_NEW
    .global __MMZ_DEL
    .global __MMZ_MALLOC
    .global __MMZ_FREE
    .global __MMZ_MAP
    .global __MMZ_UNMAP
    .global __MMZ_FLUSH
    .global __TEE_IsSecMMZ
    .global __TEE_IsSecMem
    .global __TEE_IsNonSecMem
    .global __call_debug
#endif
#if defined(WITH_HISI_CIPHER)
    .global __TEE_CIPHER_Ioctl
    .global __CRYS_PKCS5_PBKDF2_HMAC1
#endif
#ifdef WITH_HISI_OTP
/*otp begin*/
    .global __OTP_Init
    .global __OTP_DeInit
    .global __OTP_Reset
    .global __OTP_GetChipID
    .global __OTP_ReadWord
    .global __OTP_ReadByte
    .global __OTP_WriteWord
    .global __OTP_WriteByte
    .global __OTP_Test
/*otp end*/
#endif
#ifdef WITH_HISI_ADVCA
    .global __TEE_ADVCA_Ioctl

#endif

#ifdef WITH_HISI_SMMU
    .global __hisi_sec_maptosmmu
    .global __hisi_sec_unmapfromsmmu
    .global __hisi_sec_maptosmmu_and_setflag
    .global __hisi_sec_unmapfromsmmu_and_clrflg
    .global __hisi_sec_alloc
    .global __hisi_sec_free
    .global __hisi_sec_map_to_cpu
    .global __hisi_sec_unmap_from_cpu
    .global __hisi_sec_map_to_sec_smmu
    .global __hisi_sec_unmap_from_sec_smmu
    .global __hisi_nonsec_mem_map_to_sec_cpu
    .global __hisi_nosec_mem_unmap_from_sec_cpu
    .global __hisi_nosec_mem_map_to_sec_smmu
    .global __hisi_nosec_mem_unmap_from_sec_smmu
    .global __agent_closed
    .global __sec_ioctl
    .global __hisi_mem_flush
#endif

#ifdef WITH_HISI_DMX 
    .global __TEE_DRV_DMX_CreateDescrambler
    .global __TEE_DRV_DMX_DestroyDescrambler
    .global __TEE_DRV_DMX_AttachDescrambler
    .global __TEE_DRV_DMX_DetachDescrambler
    .global __TEE_DRV_DMX_GetDescramblerAttr
    .global __TEE_DRV_DMX_SetDescramblerAttr
    .global __TEE_DRV_DMX_SetDescramblerEvenKey
    .global __TEE_DRV_DMX_SetDescramblerOddKey
    .global __TEE_DRV_DMX_SetDescramblerEvenIVKey
    .global __TEE_DRV_DMX_SetDescramblerOddIVKey
    .global __TEE_DRV_DMX_GetDescramblerKeyHandle
    .global __TEE_DRV_DMX_CreateScrambler
    .global __TEE_DRV_DMX_DestroyScrambler
    .global __TEE_DRV_DMX_AttachScrambler
    .global __TEE_DRV_DMX_DetachScrambler
    .global __TEE_DRV_DMX_GetScramblerAttr
    .global __TEE_DRV_DMX_SetScramblerAttr
    .global __TEE_DRV_DMX_SetScramblerEvenKey
    .global __TEE_DRV_DMX_SetScramblerOddKey
    .global __TEE_DRV_DMX_GetScramblerKeyHandle
    .global __TEE_DRV_DMX_GetChannelHandle
    .global __TEE_DRV_DMX_RegisterChannel
    .global __TEE_DRV_DMX_UnregisterChannel
    .global __TEE_DRV_DMX_RegisterOq
    .global __TEE_DRV_DMX_UnregisterOq
    .global __TEE_DRV_DMX_RegisterRamPort
    .global __TEE_DRV_DMX_UnregisterRamPort
    .global __TEE_DRV_DMX_LockChannel
    .global __TEE_DRV_DMX_UnlockChannel
    .global __TEE_DRV_DMX_RegisterVidSecBuf
    .global __TEE_DRV_DMX_UnregisterVidSecBuf
    .global __TEE_DRV_DMX_RegisterAudSecBuf
    .global __TEE_DRV_DMX_UnregisterAudSecBuf
    .global __TEE_DRV_DMX_FixupAudSecBuf
    .global __TEE_DRV_DMX_ParserPesHeader
    .global __TEE_DRV_DMX_ParserPesDispCtrlInfo
    .global __TEE_DRV_DMX_GetPesHeaderLen
    .global __TEE_DRV_DMX_RegisterRecSecBuf
    .global __TEE_DRV_DMX_UnregisterRecSecBuf
    .global __TEE_DRV_DMX_ParserSCData
    .global __TEE_DRV_DMX_ParserFilterSCData
#endif
#ifdef WITH_HISI_PVR 
    .global __TEE_PVR_DeInit
    .global __TEE_PVR_OpenChannel
    .global __TEE_PVR_CloseChannel
    .global __TEE_PVR_RecProcTsData
    .global __TEE_PVR_RecCopyToRee
    .global __TEE_PVR_PlayCopyFromRee
    .global __TEE_PVR_RecGetStatus
    .global __TEE_PVR_RecGetOutputBufInfo
#endif
    .align 5

 #ifdef WITH_HISI_VFMW
__VFMW_Init:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_VFMW_INIT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__VFMW_Exit:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_VFMW_EXIT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__VFMW_Resume:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_VFMW_RESUME
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__VFMW_Suspend:
   push {fp, lr}
   add fp, sp, #4
   swi SW_SYSCALL_VFMW_SUSPEND
   sub fp, sp, #4
   pop {fp, lr}
   bx lr

__VFMW_Control:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_VFMW_CONTROL
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__VFMW_Thread:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_VFMW_THREAD
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__VFMW_Get_Image:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_VFMW_GET_IMAGE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__VFMW_Release_Image:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_VFMW_RELEASE_IMAGE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__VFMW_Read_Proc:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_VFMW_READ_PROC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__VFMW_Write_Proc:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_VFMW_WRITE_PROC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
#endif
  
#ifdef WITH_HISI_SEC_MMZ
__MMZ_NEW:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_MMZ_NEW
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__MMZ_DEL:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_MMZ_DEL
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__MMZ_MALLOC:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_MMZ_MALLOC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__MMZ_FREE:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_MMZ_FREE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__MMZ_MAP:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_MMZ_MAP
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__MMZ_UNMAP:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_MMZ_UNMAP
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__MMZ_FLUSH:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_MMZ_FLUSH
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_IsSecMMZ:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_TEE_ISSECMMZ
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__mem_copy:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_MEM_COPY
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_IsSecMem:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_TEE_ISSECMEM
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_IsNonSecMem:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_TEE_ISNONSECMEM
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__call_debug:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEBUG
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
#endif

#if defined(WITH_HISI_CIPHER)
__TEE_CIPHER_Ioctl:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_CIPHER_IOCTL
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__CRYS_PKCS5_PBKDF2_HMAC1:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_CIPHER_PBKDF2
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

#endif

#ifdef WITH_HISI_OTP
__OTP_Init:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_OTP_INIT
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__OTP_DeInit:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_OTP_DeInit
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__OTP_Reset:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_OTP_Reset
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__OTP_GetChipID:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_OTP_GetChipID
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__OTP_ReadWord:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_OTP_ReadWord
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__OTP_ReadByte:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_OTP_ReadByte
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__OTP_WriteWord:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_OTP_WriteWord
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__OTP_WriteByte:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_OTP_WriteByte
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__OTP_Test:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_OTP_Test
    sub sp, fp, #4
    pop {fp, lr}
    bx lr
#endif

#ifdef WITH_HISI_ADVCA

__TEE_ADVCA_Ioctl:
	push {fp, lr}
	add fp, sp, #4
	swi SW_SYSCALL_ADVCA_IOCTL
	sub sp, fp, #4
	pop {fp, lr}
	bx lr
#endif

#ifdef WITH_HISI_SMMU
__hisi_sec_maptosmmu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_MAPTOSMMU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_sec_unmapfromsmmu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_UNMAPFROMSMMU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_sec_maptosmmu_and_setflag:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_MAPTOSMMU_AND_SETFLAG
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_sec_unmapfromsmmu_and_clrflg:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_UNMAPFROMSMMU_AND_CLRFLG
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_sec_alloc:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_ALLOC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_sec_free:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_FREE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_sec_map_to_cpu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_MAP_TO_CPU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_sec_unmap_from_cpu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_UNMAP_FROM_CPU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_sec_map_to_sec_smmu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_MAP_TO_SEC_SMMU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_sec_unmap_from_sec_smmu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_SEC_UNMAP_FROM_SEC_SMMU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__hisi_nonsec_mem_map_to_sec_cpu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_NONSEC_MEM_MAP_TO_SEC_CPU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_nosec_mem_unmap_from_sec_cpu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_NOSEC_MEM_UNMAP_FROM_SEC_CPU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_nosec_mem_map_to_sec_smmu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_NOSEC_MEM_MAP_TO_SEC_SMMU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_nosec_mem_unmap_from_sec_smmu:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_NOSEC_MEM_UNMAP_FROM_SEC_SMMU
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__agent_closed:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_AGENT_CLOSED
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__sec_ioctl:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_IOCTL
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_mem_flush:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HISI_MEM_FLUSH
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

#endif

#ifdef WITH_HISI_VDP
 __vdp_drv_Ioctl:
     push {fp, lr}
     add fp, sp, #4
     swi SW_SYSCALL_VDP_DRV_IOCTL
     sub fp, sp, #4
     pop {fp, lr}
     bx lr
#endif
#ifdef WITH_HISI_DMX
__TEE_DRV_DMX_CreateDescrambler:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_NEW_DESC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_DestroyDescrambler:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_DEL_DESC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_AttachDescrambler:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_ATTACH_DESC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_DetachDescrambler:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_DETACH_DESC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_GetDescramblerAttr:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_GET_DESC_ATTR
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_SetDescramblerAttr:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_SET_DESC_ATTR
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_SetDescramblerEvenKey:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_SET_DESC_EVEN
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_SetDescramblerOddKey:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_SET_DESC_ODD
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_SetDescramblerEvenIVKey:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_SET_DESC_EVEN_IV
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_SetDescramblerOddIVKey:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_SET_DESC_ODD_IV
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_GetDescramblerKeyHandle:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_GET_KEYID
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_CreateScrambler:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_NEW_SC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_DestroyScrambler:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_DEL_SC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_GetScramblerAttr:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_GET_SC_ATTR
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_SetScramblerAttr:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_SET_SC_ATTR
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_SetScramblerEvenKey:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_SET_SC_EVEN
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_SetScramblerOddKey:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_SET_SC_ODD
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_AttachScrambler:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_ATTACH_SC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_DetachScrambler:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_DETACH_SC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_GetScramblerKeyHandle:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_GET_SC_KEYID
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
    
__TEE_DRV_DMX_GetChannelHandle:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_GET_CHNID
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_RegisterChannel:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_REG_CHAN
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_UnregisterChannel:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_UNREG_CHAN
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_RegisterOq:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_REG_OQ
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_UnregisterOq:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_UNREG_OQ
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_RegisterRamPort:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_REG_RAM_PORT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_UnregisterRamPort:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_UNREG_RAM_PORT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_LockChannel:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_LOCK_CHAN
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_UnlockChannel:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_UNLOCK_CHAN
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_RegisterVidSecBuf:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_REG_VID_SECBUF
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_UnregisterVidSecBuf:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_UNREG_VID_SECBUF
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_RegisterAudSecBuf:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_REG_AUD_SECBUF
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_UnregisterAudSecBuf:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_UNREG_AUD_SECBUF
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_FixupAudSecBuf:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_FIXUP_AUD_SECBUF
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_ParserPesHeader:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_PARSER_PES_HEADER
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_ParserPesDispCtrlInfo:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_PARSER_DISP_CONTROL
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__TEE_DRV_DMX_GetPesHeaderLen:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_GET_PES_HEADER_LEN
    sub fp, sp, #4
    pop {fp, lr}
    bx lr    

__TEE_DRV_DMX_RegisterRecSecBuf:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_REG_REC_SECBUF
    sub fp, sp, #4
    pop {fp, lr}
    bx lr   
    
__TEE_DRV_DMX_UnregisterRecSecBuf:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_UNREG_REC_SECBUF
    sub fp, sp, #4
    pop {fp, lr}
    bx lr  

__TEE_DRV_DMX_ParserSCData:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_PARSER_SC_DATA
    sub fp, sp, #4
    pop {fp, lr}
    bx lr 

__TEE_DRV_DMX_ParserFilterSCData:
	push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DEMUX_PARSER_FILTER_SC_DATA
    sub fp, sp, #4
    pop {fp, lr}
    bx lr 
#endif

#ifdef WITH_HISI_PVR
__TEE_PVR_DeInit:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_PVR_DeInit
    sub fp, sp, #4
    pop {fp, lr}
    bx lr 
__TEE_PVR_OpenChannel:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_PVR_OPEN_CHANNEL
    sub fp, sp, #4
    pop {fp, lr}
    bx lr 
__TEE_PVR_CloseChannel:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_PVR_CLOSE_CHANNEL
    sub fp, sp, #4
    pop {fp, lr}
    bx lr  
__TEE_PVR_RecProcTsData:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_PVR_PROC_TSDATA
    sub fp, sp, #4
    pop {fp, lr}
    bx lr 
__TEE_PVR_RecCopyToRee:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_PVR_COPY_TO_REE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr 
__TEE_PVR_PlayCopyFromRee:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_PVR_COPY_TO_TEE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr 
__TEE_PVR_RecGetOutputBufInfo:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_PVR_GET_OUTPUTBUF
    sub fp, sp, #4
    pop {fp, lr}
    bx lr  
__TEE_PVR_RecGetStatus:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_PVR_GET_TEE_STATE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr      
#endif

#ifdef WITH_HISI_SEC_HDMI

    .global __HI_DRV_HDMI_SetSRM
    .global __HI_DRV_HDMI_SetHDCPStrategy
    .global __HI_DRV_HDMI_GetHDCPStrategy
    .global __HI_DRV_HDMI_GetHDCPStatus
    .global __HI_DRV_HDMI_SetHDCPMute
    .global __HI_DRV_HDMI_HDCP22_Init
    .global __HI_DRV_HDMI_TeeProc
    .global __HI_DRV_HDMI_SetHDCPCaps
    .global __HI_DRV_HDMI_GetHDCPCaps
    .global __HI_DRV_HDMI_Init
    .global __HI_DRV_HDMI_DeInit
    .global __HI_DRV_HDMI_Start
    .global __HI_DRV_HDMI_Stop
    .global __HI_DRV_HDMI_IOCTRL

__HI_DRV_HDMI_SetSRM:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_SETSRM
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_SetHDCPStrategy:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_SETHDCPSTRATEGY
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_GetHDCPStrategy:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_GETHDCPSTRATEGY
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_GetHDCPStatus:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_GETHDCPSTATUS
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_SetHDCPMute:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_SETHDCPMUTE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_HDCP22_Init:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_HDCP22_INIT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_TeeProc:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_TEEPROC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_SetHDCPCaps:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_SETHDCPCAPS
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_GetHDCPCaps:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_GETHDCPCAPS
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_Init:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_INIT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_DeInit:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_DEINIT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_Start:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_START
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_HDMI_Stop:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_STOP
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
	
__HI_DRV_HDMI_IOCTRL:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_HI_DRV_HDMI_IOCTRL
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
#endif

#ifdef WITH_STB_BEIDOU

    .global __Beidou_GetSoftwareVersion
    .global __Beidou_GetPositionParameters
    .global __Beidou_CalculateDistance

__Beidou_GetSoftwareVersion:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_BEIDOU_GET_SW_VER
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__Beidou_GetPositionParameters:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_BEIDOU_GET_POS_PARAM
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__Beidou_CalculateDistance:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_BEIDOU_CALC_DISTANCE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
#endif


#ifdef WITH_STB_I2C_DRIVER

    .global __HI_DRV_I2C_Init
    .global __HI_DRV_I2C_DeInit
    .global __HI_DRV_I2C_Write
    .global __HI_DRV_I2C_Read
    .global __HI_DRV_I2C_SetRate

__HI_DRV_I2C_Init:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_I2C_INIT
    sub sp, fp, #4
    pop {fp, lr}
    bx lr
__HI_DRV_I2C_DeInit:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_I2C_DEINIT
    sub sp, fp, #4
    pop {fp, lr}
    bx lr
__HI_DRV_I2C_Write:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_I2C_WRITE
    sub sp, fp, #4
    pop {fp, lr}
    bx lr
__HI_DRV_I2C_Read:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_I2C_READ
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__HI_DRV_I2C_SetRate:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_I2C_SET_SPEED
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

#endif