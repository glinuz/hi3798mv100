#include "hi_type.h"
#include "tee_log.h"

#ifndef __TEE_PVR_H__
#define __TEE_PVR_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define TEE_PVR_MAX_INSTANCE                (4)                                 /*support 4 channels for recording*/
#define HI_TEE_ERROR_PVR_BASE               (0x81300000)                        /*general error*/
#define HI_TEE_ERROR_PVR_NOT_SUPPORT        (HI_TEE_ERROR_PVR_BASE + 1)         /*not support the operation*/
#define HI_TEE_ERROR_PVR_NULL_PTR           (HI_TEE_ERROR_PVR_BASE + 2)         /*null pointer*/
#define HI_TEE_ERROR_PVR_NO_ENOUGH_DATA     (HI_TEE_ERROR_PVR_BASE + 3)         /*the ts data is less than 47k, can't be got*/
#define HI_TEE_ERROR_PVR_INVALID_PARAM      (HI_TEE_ERROR_PVR_BASE + 4)         /*the inputed param is invalid*/
#define HI_TEE_ERROR_PVR_NO_MEM             (HI_TEE_ERROR_PVR_BASE + 5)         /*malloc memory failed*/
#define HI_TEE_ERROR_PVR_INVALID_LEN        (HI_TEE_ERROR_PVR_BASE + 6)         /*invalid data length from demux*/
#define HI_TEE_ERROR_PVR_LOW_BUFFER         (HI_TEE_ERROR_PVR_BASE + 7)         /*the un-used buffer is low*/
#define HI_TEE_ERROR_PVR_INDEX_MORE         (HI_TEE_ERROR_PVR_BASE + 8)         /*reserved*/
#define HI_TEE_ERROR_NO_CHANNEL             (HI_TEE_ERROR_PVR_BASE + 9)         /* no free channel left*/
#define HI_TEE_ERROR_BUSY                   (HI_TEE_ERROR_PVR_BASE + 10)        /*reserved*/
#define TEE_PVR_DATA_ALIGN                  (47*1024)                           /*align length*/
#define REE_PVR_MAX_INDEX_NUM               (256)                               /*max index num from demux*/
#define TEE_PVR_MAX_BUF_LENGTH              (2*47*1024*1024)                    /*max buffer length*/

typedef enum
{
    TEE_PVR_RECORD = 0, /*recording*/
    TEE_PVR_PLAYBACK,   /*playback, reserved*/
    TEE_PVR_BUTT
}TEE_PVR_TYPE_E;

#if 1
#define TEE_WARN_PVR(fmt, args...)      
#define TEE_DEBUG_PVR(fmt, args...) 
#else
#define TEE_WARN_PVR(fmt, args...)      uart_printf_func("[%s]:%s[%d]:" fmt "", "WARN-TEE_PVR", __func__, __LINE__, ## args)
#define TEE_DEBUG_PVR(fmt, args...)     uart_printf_func("[%s]:%s[%d]:" fmt "", "DEBUG-TEE_PVR", __func__, __LINE__, ## args)
#endif
#define TEE_FATAL_PVR(fmt,args...)      uart_printf_func("[%s]:%s[%d]:" fmt "", "FATAL-TEE_PVR", __func__, __LINE__, ## args)
#define TEE_ERR_PVR(fmt, args...)       uart_printf_func("[%s]:%s[%d]:" fmt "", "ERROR-TEE_PVR", __func__, __LINE__, ## args)

#define TEE_PVR_ENTER()\
    do{\
        TEE_DEBUG_PVR("ENTER>>>>\n");\
    }while(0)
#define TEE_PVR_EXIT()\
    do{\
        TEE_DEBUG_PVR("<<<<EXIT\n");\
    }while(0)

#if 0   //memcpy for all
#define TEE_PVR_MEMCPY  memcpy
#else
#if 1   //memcpy for secure-OS-kernel
#define TEE_PVR_MEMCPY  asm_memmove
#else   //memcpy for secure-OS-user
#define TEE_PVR_MEMCPY  ASM_MemMove
#endif
#endif

HI_S32 TEE_PVR_DeInit(HI_VOID);
HI_S32 TEE_PVR_RecOpenChannel(TEE_Param stParams[4]);
HI_S32 TEE_PVR_RecCloseChannel(TEE_Param stParams[4]);
HI_S32 TEE_PVR_RecProcTsData(TEE_Param stParams[4]);
HI_S32 TEE_PVR_RecCopyToRee(TEE_Param stParams[4]);
HI_S32 TEE_PVR_RecGetOutputBufInfo(TEE_Param stParams[4]);
HI_S32 TEE_PVR_RecGetStatus(TEE_Param stParams[4]);
HI_S32 TEE_PVR_PlayCopyFromRee(TEE_Param stParams[4]);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif
