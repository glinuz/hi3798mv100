#include "hi_type.h"

#ifndef __TEE_DRV_AVPLAY_H__
#define __TEE_DRV_AVPLAY_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define TEE_AVPLAY_MEMCPY  asm_memmove

#define HI_TEE_ERR_AVPLAY_DEV_NOT_EXIST         (HI_S32)(0x80310001)
#define HI_TEE_ERR_AVPLAY_NOT_DEV_FILE          (HI_S32)(0x80310002)
#define HI_TEE_ERR_AVPLAY_DEV_OPEN_ERR          (HI_S32)(0x80310003)
#define HI_TEE_ERR_AVPLAY_DEV_CLOSE_ERR         (HI_S32)(0x80310004)
#define HI_TEE_ERR_AVPLAY_NULL_PTR              (HI_S32)(0x80310005)
#define HI_TEE_ERR_AVPLAY_DEV_NO_INIT           (HI_S32)(0x80310006)
#define HI_TEE_ERR_AVPLAY_INVALID_PARA          (HI_S32)(0x80310007)
#define HI_TEE_ERR_AVPLAY_CREATE_ERR            (HI_S32)(0x80310008)
#define HI_TEE_ERR_AVPLAY_DESTROY_ERR           (HI_S32)(0x80310009)
#define HI_TEE_ERR_AVPLAY_INVALID_OPT           (HI_S32)(0x8031000A)
#define HI_TEE_ERR_AVPLAY_NOT_SUPPORT           (HI_S32)(0x8031000B)

#define TEE_FATAL_AVPLAY(fmt,args...) \
    uart_printf_func("[%s]:%s[%d]:" fmt "", "FATAL-TEE_AVPLAY", __func__, __LINE__, ## args)

#define TEE_ERR_AVPLAY(fmt, args...) \
    uart_printf_func("[%s]:%s[%d]:" fmt "", "ERROR-TEE_AVPLAY", __func__, __LINE__, ## args)

#define TEE_INFO_AVPLAY(fmt,args...) \
    uart_printf_func("[%s]:%s[%d]:" fmt "", "INFO-TEE_AVPLAY", __func__, __LINE__, ## args)

#define TEE_WARN_AVPLAY(fmt, args...) \
    uart_printf_func("[%s]:%s[%d]:" fmt "", "WARN-TEE_AVPLAY", __func__, __LINE__, ## args)

HI_S32 TEE_DRV_AVPLAY_Ree2TeeMemCopy(HI_U32 ReeAddr, HI_U32 TeeAddr, HI_U32 DataLen);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
