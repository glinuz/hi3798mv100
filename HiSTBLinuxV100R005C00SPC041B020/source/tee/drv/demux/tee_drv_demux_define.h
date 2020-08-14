/***********************************************************************************
*              Copyright 2004 - 2015, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  tee_drv_demux_define.h
* Description:  Define basic definition.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20151012    NA                 NULL      Create this file.
***********************************************************************************/

#ifndef __TEE_DRV_DEMUX_DEFINE_H__
#define __TEE_DRV_DEMUX_DEFINE_H__

#include "hi_type.h"

#include "tee_demux_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * linux helpers utils.
 */
#define BITS_PER_BYTE		8
#define BITS_PER_LONG       32
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define DIV_ROUND_UP(n,d)   (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#define DECLARE_BITMAP(name,bits) \
                        unsigned long name[BITS_TO_LONGS(bits)]

#define unlikely(condition) (condition)

struct mutex {
    HI_VOID *rlock;
};

#define HI_FATAL_DEMUX(fmt,args...)    uart_printf_func("[%s]:%s[%d]:" fmt "", "FATAL-TEE_DEMUX", __func__, __LINE__, ## args)
#define HI_ERR_DEMUX(fmt, args...)     uart_printf_func("[%s]:%s[%d]:" fmt "", "ERROR-TEE_DEMUX", __func__, __LINE__, ## args)
#define HI_WARN_DEMUX(fmt, args...)    uart_printf_func("[%s]:%s[%d]:" fmt "", "WARN-TEE_DEMUX", __func__, __LINE__, ## args)
#define HI_INFO_DEMUX(fmt, args...)      
#define HI_DBG_DEMUX(fmt, args...)   

#define BUG_ON(condition) if(condition) {HI_FATAL_DEMUX("BUG ...\n");while(1);}

#define DMX_INVALID_DEMUX_ID            0xffffffff
#define DMX_INVALID_CHAN_ID             0xffff
#define DMX_INVALID_KEY_ID              0xffff

#define DMX_INVALID_PID                 0x1fff

#define DMX_KEY_MIN_LEN                 8
#define DMX_KEY_MAX_LEN                 16

#define DMX_KEY_HARDONLY_FLAG           0xffffffff

#define DMX_PES_HEADER_LENGTH           9
#define INVALID_PTS                     0xFFFFFFFFL

typedef struct ChanBufInfo {
    HI_U8 *BufStartVirAddr;
    HI_U32 BufStartAddr;
    HI_U32 BufSize;
    HI_U32 ShadowBufStartAddr;
    HI_BOOL FlushShadowBuf;
}ChanBufInfo_S;

typedef struct RecBufInfo {
    HI_U8 *BufStartVirAddr;
    HI_U32 BufStartAddr;
    HI_U32 BufSize;
}RecBufInfo_S;

typedef struct
{
    HI_U32          DmxId;
    HI_U32          KeyId;
    ChanBufInfo_S   ChanSecBufInfo;
    struct mutex    LockChn;
} DMX_ChanInfo_S;

typedef struct
{
    HI_U32         DmxId;
    HI_U32         KeyId;
    HI_U32         CaType;
    HI_U32         CaEntropy;
    HI_U32         DescType;
    HI_U32         KeyLen;
} DMX_KeyInfo_S;

typedef struct 
{
    HI_U32         Scrambler;
    HI_U32         ActiveKey;
}DMX_Encrypt_KeyInfo_S;

typedef struct
{
    RecBufInfo_S RecSecBufInfo;
}DMX_RecInfo_S;
                        
typedef struct hiDmx_Set_S
{
    HI_U32                      IoBase;
    HI_U32                      MmuIoBase;

    HI_U32                      RamPortCnt;
    
    DMX_ChanInfo_S              DmxChanInfo[DMX_CHANNEL_CNT];
    HI_U32                      DmxChanCnt;
    struct mutex                LockAllChn;
    DECLARE_BITMAP(ChnBitmap, DMX_CHANNEL_CNT);
    
    DMX_KeyInfo_S               DmxKeyInfo[DMX_KEY_CNT];
    DMX_Encrypt_KeyInfo_S       DmxEncryptKeyInfo[DMX_KEY_CNT];
    HI_U32                      DmxKeyCnt;
    struct mutex                LockAllKey;
    DECLARE_BITMAP(KeyBitmap, DMX_KEY_CNT);

    DMX_RecInfo_S               DmxRecInfo[DMX_REC_CNT];  
    HI_U32                      DmxRecCnt;

    HI_U32                      DmxOqCnt;
    
    HI_U32                      KeyCsa2HardFlag;
    HI_U32                      KeyCsa3HardFlag;
    HI_U32                      KeySpeHardFlag;
    HI_U32                      KeyAesCbcHardFlag;
    HI_U32                      KeyOtherHardFlag;
}Dmx_Set_S;

Dmx_Set_S *GetDmxSetByDmxid(HI_U32 DmxId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __TEE_DRV_DEMUX_DEFINE_H__ */