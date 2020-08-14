/******************************************************************************

Copyright (C), 2004-2007, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_psi_si.h
Version       : Initial
Author        : Hisilicon multimedia software group
Created       : 2007-04-10
Last Modified :
Description   : Hi3560 PSI/SI function define
Function List :
History       :
* Version   Date         Author     DefectNum    Description
* main\1    2007-04-10   Hs         NULL         Create this file.
******************************************************************************/
#ifndef __HI_PSI_SI_H__
#define __HI_PSI_SI_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define PRINT_PSISI(fmt...)     sample_common_printf(fmt)

typedef struct hiPROG_INFO_S
{
    HI_U16 ProgID;         /*Progam ID*/
    HI_U16 ProgPID;        /*PMT ID*/
} PROG_INFO;

typedef struct hiPAT_TBL_S
{
    HI_U32 ProgNum;
    HI_U32 TsID;
    HI_U8 *pat_buf;         /*PROG_INFO*/
} PAT_TBL;

typedef struct hiCAT_TBL_S
{
    HI_U32 len;
    HI_U8 *cat_buf;
} CAT_TBL;

typedef struct hiNIT_TSDESC_S
{
    HI_U16 TsID;            /*TS ID*/
    HI_U16 NetID;           /*net work ID*/
    HI_U16 TsDescLen;       /*TS desc len*/
    HI_U16 Reserved;
    struct hiNIT_TSDESC_S *Next;
    HI_U8 *TsDesc;          /*TS Desc*/
} NIT_TSDESC;

typedef struct hiNIT_TBL_S
{
    HI_U32 NetworkID;       /*Network ID*/
    HI_U32 NetDescLen;
    HI_U8 *NetDesc;
    HI_U32 StreamDescLen;
    HI_U8 *StreamDesc;
} NIT_TBL;

typedef struct hiPMT_Stream_S
{
    HI_U32 StreamType;
    HI_U32 ElementPid;
    HI_U32 ESInfoLen;
    struct hiPMT_Stream_S *Next;
    HI_U8 *ESDesc;
} PMT_Stream;

typedef struct hiPMT_Prog_S
{
    HI_U32 ProgID;
    HI_U32 PCR_PID;
    HI_U32 ProgDescLen;     /*Program Desc Len*/
    HI_U8 *ProgDescPtr;     /*Program Infor*/
    HI_U8 *ESDescPtr;       /*PMT_Stream*/
} PMT_Prog;

typedef struct hiPMT_TBL_S
{
    HI_U8 *pmt_buf;         /*PMT_Prog*/
} PMT_TBL;

typedef enum hiRUN_STATE_E
{
    UnDefined = 0,
    NotRun,
    StartInSeconds,
    Pause,
    Running,
    Run_Reserved1,
    Run_Reserved2,
    Run_Reserved3
} RUN_STATE_E;

typedef enum hiCA_MODE_E
{
    CA_NotNeed = 0,
    CA_Need
} CA_MODE_E;

typedef enum hiDESC_TAG_E
{
    Ser_Desc = 0x48,
    Short_evt_Desc = 0x4D,
    Ext_evt_Desc = 0x4E,
    TmShft_evt_Desc = 0x4F
} DESC_TAG_E;

typedef enum hiSERVICE_TYPE_E
{
    SvcType_Reserved1 = 0,
    Digital_TV,
    Digital_Radio,
    TeleText,
    Nvod_Refer,
    Nvod_TimeShift,
    Mosaic,
    PAL_Encode,
    SECAM_Encode,
    D2_MAC,
    ACodec_Digital_Radio,
    ACodec_Mosaic,
    Data_Broadcast,
    SvcType_Reserved5,
    RcsMap,
    RcsFls,
    DvbMhp,
    MPEG2_HD_Digital_TV,
    ACodec_SD_Digital_TV = 0x16,
    ACodec_SD_Nvod_TmShft,
    ACodec_SD_Nvod_Refer,
    ACodec_HD_Digital_TV,
    ACodec_HD_Nvod_TmShft,
    ACodec_HD_Nvod_Refer,
    INVALID_SER_TYPE = 0x100
} SERVICE_TYPE_E;

typedef struct hiSER_DESC_S
{
    SERVICE_TYPE_E SerType;
    HI_U32         PrvdNameLen;
    HI_U32         SerNameLen;
    HI_U8 *        PrvdName;
    HI_U8 *        SerName;
} SER_DESC_S;

typedef struct hiDESC_STRU_S
{
    DESC_TAG_E DescTag;
    HI_U32     DescLen;
    struct hiDESC_STRU_S *Next;
    HI_U8 *Desc;
} DESC_STRU_S;

typedef struct hiSER_INFO_S
{
    HI_U32      SerID;     /*Service ID*/
    HI_U32      SchdFlag;
    HI_U32      PrsntFlwFlag;
    RUN_STATE_E RunState;
    CA_MODE_E   CAMode;
    struct hiSER_INFO_S *Next;
    DESC_STRU_S *DescPtr;
} SER_INFO;

typedef struct hiSDT_TBL_S
{
    HI_U32 TsID;
    HI_U32 NetID;
    HI_U8 *ser_buf;         /*SER_INFO*/
} SDT_TBL;

typedef struct hiSHOTR_ETV_DESC_S
{
    HI_U8  Lang[4];
    HI_U32 EvtNameLen;
    HI_U32 TextLen;
    HI_U8 *EvtName;
    HI_U8 *Text;
} SHOTR_ETV_DESC_S;

typedef struct hiEVT_INFO_S
{
    HI_U32 EvtID;
    HI_U8  StartTime[4];
    HI_U8  Duration[4];
    HI_U16 RunState;        //program run state
    HI_U16 CAMode;
    struct hiEVT_INFO_S *Next;
    DESC_STRU_S *DescPtr;
} EVT_INFO;

typedef struct hiEIT_TBL_S
{
    HI_U32 TsID;
    HI_U32 NetID;
    HI_U8 *evt_buf;         /*EVT_INFO*/
} EIT_TBL;

HI_S32	HI_API_PSISI_GetPcrPid(HI_U32 u32DmxId, HI_U32 ProgID, HI_U32 *pPCR_PID);
HI_S32	HI_API_PSISI_GetPatTbl(HI_U32 u32DmxId, PAT_TBL **tbladdr, HI_U32 u32TimeOutMs);
HI_VOID HI_API_PSISI_FreePatTbl(HI_VOID);
HI_S32	HI_API_PSISI_GetCatTbl(HI_U32 u32DmxId, CAT_TBL **tbladdr, HI_U32 u32TimeOutms);
HI_VOID HI_API_PSISI_FreeCatTbl(HI_VOID);
HI_S32	HI_API_PSISI_GetNitTbl(HI_U32 u32DmxId, NIT_TBL **tbladdr, HI_U32 CurrFlag, HI_U32 u32TimeOutms);
HI_VOID HI_API_PSISI_FreeNitTbl(HI_VOID);
HI_S32	HI_API_PSISI_GetPmtTbl(HI_U32 u32DmxId, PMT_TBL **tbladdr, HI_U32 ProgID, HI_U32 u32TimeOutms);
HI_VOID HI_API_PSISI_FreePmtTbl(HI_VOID);
HI_S32	HI_API_PSISI_GetSdtTbl(HI_U32 u32DmxId, SDT_TBL **tbladdr, HI_U32 CurrFlag, HI_U32 TsID, HI_U32 NetID, HI_U32 u32TimeOutms);
HI_VOID HI_API_PSISI_FreeSdtTbl(HI_VOID);
HI_S32	HI_API_PSISI_GetEitTbl(HI_U32 u32DmxId, EIT_TBL **tbladdr, HI_U32 CurrFlag, HI_U32 SerID, HI_U32 TsID, HI_U32 NetID,
                               HI_U32 u32TimeOutms);
HI_VOID HI_API_PSISI_FreeEitTbl(HI_VOID);
HI_VOID HI_API_PSISI_Init(HI_VOID);
HI_VOID HI_API_PSISI_Destroy(HI_VOID);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_PSI_SI_H__ */
