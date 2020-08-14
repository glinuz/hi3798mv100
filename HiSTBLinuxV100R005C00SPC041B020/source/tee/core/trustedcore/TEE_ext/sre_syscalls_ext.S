#include "sre_syscalls_id_ext.h"
#include "sre_syscalls_id.h"
#ifdef FEATURE_IRIS
    .global __iris_tee_mmap
    .global __iris_tee_unmap
    .global __iris_tee_isSecureMemory
#endif
/*efuse begin*/
    .global __bsp_efuse_read
    .global __bsp_efuse_write
    .global __tee_hal_get_dieid
    .global __driver_dep_test
    .global __driver_p61_factory_test
/*efuse end*/
/*vdec begin*/
    .global __SECURE_TEE_GetId
    .global __SECURE_TEE_ReleaseId
    .global __SECURE_TEE_SendMessage
    .global __SECURE_TEE_Mmap
    .global __SECURE_TEE_Unmap
    .global __SECURE_ISSecureMemory

/*vdec end*/
    .global __scard_connect
    .global __scard_disconnect
    .global __scard_transmit
    .global __driver_fingerprint_command
    .global __driver_spi_full_duplex
    .global __driver_spi_full_duplex_with_speed
    .global __driver_spi_half_duplex_with_speed
    .global __hisi_tui_get_tpdata
    .global __set_kb_rect
    .global __send_simu_tp

/*display begin*/
    .global __hisi_fb_cfg_sec
    .global __hisi_pan_display_sec
    .global __hisi_wait_vactive_flag
    .global __hisi_wait_release_flag
    .global __hisi_get_disp_info
    .global __hisi_fb_alpha_set
/*display end*/

#ifdef TEE_SUPPORT_TUI
    .global __can_enter_tui_loop
    .global __init_tui_sdriver
#endif
    .global __is_device_rooted
    .global __get_vsim_sharemem

    .global __get_target_product

    .global __bsp_modem_call

/*rtc config api*/
    .global __sre_get_rtc_time
    .global __sre_set_rtc_time

/* sharemem with fastboot api */
    .global __SRE_Get_SharedMem_Info
#ifdef TEE_SUPPORT_HIVCODEC
/*hi_vcodec begin*/
    .global __SEC_VDEC_Init
    .global __SEC_VDEC_Exit
    .global __SEC_VDEC_Control
    .global __SEC_VDEC_Suspend
    .global __SEC_VDEC_Resume
    .global __SEC_VDEC_RunProcess
    .global __SEC_VDEC_ReadProc
    .global __SEC_VDEC_WriteProc
    .global __SEC_VDEC_GetChanImage
    .global __SEC_VDEC_ReleaseChanImage
/*hi_vcodec end*/
#endif

    .align 5
#ifdef FEATURE_IRIS
__iris_tee_mmap:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_IRIS_TEE_MMAP
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__iris_tee_unmap:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_IRIS_TEE_UNMAP
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__iris_tee_isSecureMemory:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_IRIS_TEE_ISSECUREMEMORY
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
#endif

__SECURE_TEE_GetId:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SECURE_TEE_GETID
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__SECURE_TEE_ReleaseId:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SECURE_TEE_RELEASEID
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__SECURE_TEE_SendMessage:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SECURE_TEE_SENDMESSAGE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SECURE_TEE_Mmap:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SECURE_TEE_MMAP
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__SECURE_TEE_Unmap:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SECURE_TEE_UNMAP
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__SECURE_ISSecureMemory:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SECURE_ISSEUCREMEM
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__bsp_efuse_read:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_EFUSE_READ
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__bsp_efuse_write:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_EFUSE_WRITE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__tee_hal_get_dieid:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_TEE_HAL_GET_DIEID
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__scard_connect:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SCARD_CONNECT
    sub sp, fp, #4
    pop {fp, lr}
    bx lr
__scard_disconnect:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SCARD_DISCONNECT
    sub sp, fp, #4
    pop {fp, lr}
    bx lr
__scard_transmit:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SCARD_TRANSMIT
    sub sp, fp, #4
    pop {fp, lr}
    bx lr
#ifdef TEE_SUPPORT_TUI
__hisi_tui_get_tpdata:
    push {fp, lr}
    add fp, sp, #4
    swi SW_TP_GET_DATA
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__set_kb_rect:
    push {fp, lr}
    add fp, sp, #4
    swi SW_TP_SET_KB_RECT
    sub sp, fp, #4
    pop {fp, lr}
    bx lr

__init_tui_sdriver:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_TUI_INIT_SDRIVER
    sub sp, fp, #4
    pop {fp, lr}
    bx lr
#endif

__driver_dep_test:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_DRIVER_TEST
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__driver_p61_factory_test:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_P61_FAC_TEST
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__hisi_fb_cfg_sec:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FB_SEC_CFG
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__hisi_fb_alpha_set:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FB_SEC_SET
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
#if (TRUSTEDCORE_PLATFORM_CHOOSE != WITH_BALONG_V722_PLATFORM && TRUSTEDCORE_PLATFORM_CHOOSE != WITH_DEVCHIP_PLATFORM)
__send_simu_tp:
    push {fp, lr}
    add fp, sp, #4
    swi SW_TP_SET_SIMU_DATA
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
#endif
__hisi_pan_display_sec:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FB_SEC_DISPLAY
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__hisi_wait_vactive_flag:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FB_ACTIVE_FLAG
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__hisi_wait_release_flag:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FB_RELEASE_FLAG
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__hisi_get_disp_info:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FB_GETINFO
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__is_device_rooted:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_IS_DEVICE_ROOTED
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SRE_Get_SharedMem_Info:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_GET_SHAREMEM_INFO
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__get_vsim_sharemem:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_GET_VSIM_SHAREMEM
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__driver_fingerprint_command:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FP_COMMAND_INFO
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__driver_spi_full_duplex:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FP_SPI_TRANSACTION
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__driver_spi_full_duplex_with_speed:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FP_SPI_FULL_DUPLEX_WITH_SPEED_TRANSACTION
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
__driver_spi_half_duplex_with_speed:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_FP_SPI_HALF_DUPLEX_WITH_SPEED_TRANSACTION
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__get_target_product:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_GET_TARGET_PRODUCT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__bsp_modem_call:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_BSP_MODEM_CALL
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__sre_get_rtc_time:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SRE_GET_RTC_TIME
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__sre_set_rtc_time:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SRE_SET_RTC_TIME
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

#ifdef TEE_SUPPORT_HIVCODEC
__SEC_VDEC_Init:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_INIT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SEC_VDEC_Exit:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_EXIT
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SEC_VDEC_Control:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_CONTROL
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SEC_VDEC_Suspend:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_SUSPEND
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SEC_VDEC_Resume:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_RESUME
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SEC_VDEC_RunProcess:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_RUNPROCESS
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SEC_VDEC_ReadProc:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_READPROC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SEC_VDEC_WriteProc:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_WRITEPROC
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SEC_VDEC_GetChanImage:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_GETCHANIMAGE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr

__SEC_VDEC_ReleaseChanImage:
    push {fp, lr}
    add fp, sp, #4
    swi SW_SYSCALL_SEC_VDEC_RELEASECHANIMAGE
    sub fp, sp, #4
    pop {fp, lr}
    bx lr
#endif
