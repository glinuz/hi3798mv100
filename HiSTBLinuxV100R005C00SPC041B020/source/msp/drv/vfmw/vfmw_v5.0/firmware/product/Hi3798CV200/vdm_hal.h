

/******************************************************************************

  ???? (C), 2001-2011, ????????

******************************************************************************
    ? ? ?   : vdmv300plus_hal.h
    ? ? ?   : ??
    ?    ?   :
    ????   :
    ????   :
    ????   : VDMV300plus????


 ????   :
    1.?    ? : 2009-03-04
    ?    ?   :
    ????   :

******************************************************************************/

#ifndef _VDM_HAL_HEADER_
#define _VDM_HAL_HEADER_

#ifdef __cplusplus
extern "C" {
#endif

#include "decparam.h"
#include "vdm_drv.h"

#ifdef VDH_DISTRIBUTOR_ENABLE
#ifdef ENV_SOS_KERNEL
#include "vdh_secure.h"
#else
#include "vdh_normal.h"
#endif
#endif
/************************************************************************/
/*   ??                                                               */
/************************************************************************/

#define    VDMHAL_OK                   	0
#define    VDMHAL_ERR                  	-1
#define    VDMHAL_NOT_ARRANGE           2 //l00273086
#define    VDMHAL_FUN_FALSE     		0
#define    VDMHAL_FUN_TRUE     		    1
#define    MAX_IMG_WIDTH_IN_MB         	512 // 120//5632
#define    MAX_IMG_HALF_HEIGHT_IN_MB  	256 // 34//4224
#define    MAX_IMG_HEIGHT_IN_MB        	(MAX_IMG_HALF_HEIGHT_IN_MB*2)
#define    MAX_HOR_SIZE                	(MAX_IMG_WIDTH_IN_MB*16)
#define    MAX_VER_SIZE                	(MAX_IMG_HEIGHT_IN_MB*16)
#define    MAX_MB_NUM_IN_PIC           	(MAX_IMG_WIDTH_IN_MB*MAX_IMG_HEIGHT_IN_MB)
#define    MAX_FS_NUM                   64
#define    MIN_DNRFSP_NUM               6
#define    MIN_DBDR_INFO_NUM            6

// VDM MAKE REPORT????
#define    VDM_RET_RIGHT                0
#define    VDM_RET_ERROR                1

#define    MAX_SLICE_SLOT_NUM           136

#define    FIRST_REPAIR   0
#define    SECOND_REPAIR  1
#define    ALIGN_LEN                    (64)

// define para
#define   DNR_FRAME                      0
#define   DNR_TOP_FIELD                  1
#define   DNR_BOTTOM_FIELD               2
#define   DNR_BAFF                       3
#define   DNR_FIELD_PAIR                 4

#define   MAX_OCCUPIED_NUM               4  //VPSS在对标清的流做去隔行操作时，会占用3帧
/************************************************************************/
/*  ?                                                                  */
/************************************************************************/
/* ????? */
#define RD_VREG( reg, dat, VdhId )               \
    do {                    \
        if (VdhId < MAX_VDH_NUM)                \
        {                    \
            dat = *((volatile SINT32*)((SINT8*)g_HwMem[VdhId].pVdmRegVirAddr + reg)); \
        }                    \
        else                 \
        {                    \
            dprint(PRN_ALWS,"%s: RD_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\n", __func__, VdhId, MAX_VDH_NUM); \
        }                    \
    } while(0)

#define WR_VREG( reg, dat, VdhId )               \
    do {                     \
        if (VdhId < MAX_VDH_NUM)                \
        {                    \
            *((volatile SINT32*)((SINT8*)g_HwMem[VdhId].pVdmRegVirAddr + reg)) = dat; \
        }                    \
        else                 \
        {                    \
            dprint(PRN_ALWS,"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\n", __func__, VdhId, MAX_VDH_NUM); \
        }                    \
    } while(0)

#define RD_BPD_VREG( reg, dat )               \
    do {                    \
    dat = *((volatile SINT32*)((SINT8*)g_HwMem[0].pBpdRegVirAddr + reg)); \
    } while(0)

#define WR_BPD_VREG( reg, dat )               \
    do {                    \
    *((volatile SINT32*)((SINT8*)g_HwMem[0].pBpdRegVirAddr + reg)) = dat; \
    } while(0)

/* ????? */
#define RD_MSGWORD( vir_addr, dat )          \
    do {                 \
        dat = *((volatile SINT32*)((SINT8*)vir_addr));                      \
    } while(0)

#define WR_MSGWORD( vir_addr, dat )          \
    do {                 \
        *((volatile SINT32*)((SINT8*)(vir_addr))) = dat;                      \
    } while(0)

/* ???? */
#define  VDMHAL_ASSERT_RET( cond, else_print )     \
    do {               \
        if( !(cond) )            \
        {               \
            dprint(PRN_FATAL,"%s: %s\n", __func__, else_print ); \
            return VDMHAL_ERR;          \
        }               \
    }while(0)

#define  VDMHAL_ASSERT( cond, else_print )      \
    do {               \
        if( !(cond) )            \
        {               \
            dprint(PRN_FATAL,"%s: %s\n", __func__, else_print ); \
            return;             \
        }               \
    }while(0)

/*#########################################################################################
       structures
 ##########################################################################################*/
typedef enum
{
    VDM_VERSION_VDHV100 = 1,
    VDM_VERSION_VDHV200_R001,
    VDM_VERSION_VDHV200_R002,
    VDM_VERSION_VDHV200_R003,
    VDM_VERSION_BUTT
} VDM_VERSION_E;

typedef enum
{
    FREQ_MODE_0 = 0,    // 148MHz
    FREQ_MODE_1,        // 118.8MHz
    FREQ_MODE_2,        // 198MHz
    FREQ_MODE_3,        // 214.2MHz
    FREQ_MODE_KEEP      // Keep Last Freq
} VDM_FREQMODE_E;

typedef struct
{
    UADDR  frameAddr;
    UINT32  bufSize;
    UINT8 isValid;
} FRAME_BUF_RECORD_S;

typedef struct
{
    UADDR  pmvAddr;
    UINT8 isValid;
    UINT8 isInUse;
} PMV_RECORD_S;
typedef struct
{
    UADDR  hdrAddr;
    UINT32 isValid;
    UINT32 isInUse;
} HDR_RECORD_S;

typedef struct
{
    SINT32    VahbStride;
    SINT32    TotalMemUsed;
    // frame buffer
    FRAME_BUF_RECORD_S frameBufRec[MAX_FRAME_NUM];

#ifdef VFMW_MODULE_LOWDLY_SUPPORT
    //add by l00225186 fordsp 2013-04-25
    UADDR     LineNumAddr[MAX_FRAME_NUM];
#endif

    FRAME_BUF_RECORD_S    HDR_Metadata_Addr[MAX_FRAME_NUM];
    UADDR     VpssInfoAddr[MAX_FRAME_NUM];
    UINT32    VpssInfoStride;
    UINT32    VpssInfoSize;
    UINT32    VpssInfoNum;
    //compress info
    SINT32    HeadStride;
    SINT32    HeadInfoSize;
    //HEVC 10bit
    SINT32    VahbStride_2bit;
    SINT32    LumaStride_2bit;
    SINT32    LumaOffset_2bit;
    SINT32    ChromOffset_2bit;
    SINT32    ChromOffset;
    SINT32    LumaPixelOffset;
    SINT32    ChromaPixelOffset;
    SINT32    ValidFrameNum;

#ifdef VFMW_BTL_SUPPORT
    // btl buffer
    UADDR     BtlAddr[MAX_FRAME_NUM];
    SINT32    ValidBTLFrameNum;
#endif

    // PMV buffer
    PMV_RECORD_S pmvRec[MAX_PMV_SLOT_NUM];

    SINT32    HafPmvOffset;
    SINT32    ValidPMVNum;

    UINT32 configuredFrameNum;
    UINT32 configuredPmvNum;
	UINT32 configuredHdrNum;
	//VP9
    UADDR    ProbTabAddr;
    UADDR    ProbCntAddr;
} VDMHAL_MEM_ARRANGE_S;
/*记录vfmw需要申请的内存信息大小，包括帧存信息，pmv信息，hdr的metadata信息*/
typedef struct
{
    UINT32  u32OneFrameTotalSize;/*所有数据的总大小*/
    UINT32  u32ImageSize;/*帧存信息大小*/
	UINT32  u32ImageNum;/*需要申请的帧存个数*/
    UINT32  u32PmvSize;/*pmv信息大小*/
	UINT32  u32PmvNum;/*需要申请的pmv个数*/

    UINT32  u32HdrMetadataSize;/*hdr metadata信息大小*/
} VFMW_FRAME_SIZE_INFO_S;

#define    MPEG2_DUMMY_BITS              24     // 40

#define    SHORT_HEADER_ID       1
#define    NON_SHORT_HEADER_ID   2
#define RSHIFT(a,b) ( (a)>0 ? (((a) + ((1<<(b))>>1))>>(b)) : (((a) + ((1<<(b))>>1)-1)>>(b)))

/* Filter strength tables */
/* default strength specified by RV_Default_Deblocking_Strength (0) */
#define RV8_Default_Deblocking_Strength     0

/* Filter strength tables */
/* default strength specified by RV_Default_Deblocking_Strength (0) */
#define RV9_Default_Deblocking_Strength     0



typedef struct hiVDMHAL_FUN_PTR_S
{
    /*VDM_DRV*/
    VOID (*pfun_VDMDRV_SetClockSkip)(UINT32 SkipValue);
    VOID (*pfun_VDMDRV_OpenHardware)(SINT32 VdhId);
    VOID (*pfun_VDMDRV_CloseHardware)(SINT32 VdhId);

    VOID (*pfun_DSPDRV_OpenHardware)(SINT32 VdhId);
    VOID (*pfun_DSPDRV_CloseHardware)(SINT32 VdhId);

    VOID (*pfun_SCDDRV_OpenHardware)(SINT32 ScdId);
    VOID (*pfun_SCDDRV_CloseHardware)(SINT32 ScdId);
    VOID (*pfun_BPDDRV_OpenHardware)(SINT32 BpdId);
    VOID (*pfun_BPDDRV_CloseHardware)(SINT32 BpdId);
    VOID (*pfun_MMUDRV_OpenHardware)(SINT32 MmuId);
    VOID (*pfun_MMUDRV_CloseHardware)(SINT32 MmuId);
    /*VDM_HAL*/
    VOID (*pfun_VDMHAL_GetCharacter)(VOID);
    SINT32 (*pfun_VDMHAL_GetHalMemSize)(VOID);
    SINT32 (*pfun_VDMHAL_CalcFsSize)(SINT32 ChanID, SINT32 Width, SINT32 Height, SINT32 UserDec, VID_STD_E VidStd, VFMW_FRAME_SIZE_INFO_S *pstVfmwFrameSizeInfo);
    SINT32 (*pfun_VDMHAL_OpenHAL)(VDMHAL_OPENPARAM_S *pOpenParam);
    VOID (*pfun_VDMHAL_CloseHAL)(SINT32 VdhId);
    SINT32 (*pfun_VDMHAL_ArrangeMem)( UADDR  MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 FrameNum, SINT32 UserDec, SINT32 ChanID, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange );
    SINT32 (*pfun_VDMHAL_ArrangeMem_BTL)( UADDR  MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 s32MaxRefFrameNum, SINT32 s32DisplayFrameNum, SINT32 UserDec, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange , SINT32 s32Btl1Dt2DEnable, SINT32 s32BtlDbdrEnable );
    VOID (*pfun_VDMHAL_SetSmmuPageTableAddr)( UINT32 VdhId );
    VOID (*pfun_VDMHAL_EnableSmmu)( VOID );
    VOID (*pfun_VDMHAL_ResetVdm)( SINT32 VdhId );
    VOID (*pfun_VDMHAL_GlbReset)( VOID );
    VOID (*pfun_VDMHAL_GlbResetX)( SINT32 VdhId );
    VOID (*pfun_VDMHAL_ClearIntState)( SINT32 VdhId );
    VOID (*pfun_VDMHAL_ClearMMUIntState)(SINT32 vdh_id);
    VOID (*pfun_VDMHAL_MaskInt)( SINT32 VdhId );
    VOID (*pfun_VDMHAL_EnableInt)( SINT32 VdhId );
    SINT32 (*pfun_VDMHAL_CheckReg)(REG_ID_E reg_id, SINT32 VdhId);
    UINT32 (*pfun_VDMHAL_ReadMMUMask)(SINT32 VdhId);
    VOID (*pfun_VDMHAL_WriteMMUMask)(UINT32 mask, SINT32 VdhId);
    VOID (*pfun_VDMHAL_StartHwRepair)(SINT32 VdhId, VOID *pTask);
    VOID (*pfun_VDMHAL_StartHwDecode)(SINT32 VdhId, VOID *pTask);
    SINT32 (*pfun_VDMHAL_PrepareDec)( VID_STD_E VidStd, VOID *pDecParam, SINT32 VdhId, VOID *pTask);
    SINT32 (*pfun_VDMHAL_IsVdmReady)(SINT32 VdhId);
    SINT32 (*pfun_VDMHAL_IsVdmRun)(SINT32 VdhId);
    SINT32 (*pfun_VDMHAL_PrepareRepair)( VID_STD_E VidStd, VOID *pDecParam, SINT32 RepairTime, SINT32 VdhId, VOID *pTask );
    SINT32 (*pfun_VDMHAL_MakeDecReport)(MAKE_DEC_REPORT_S *pMakeDecReport);
    SINT32 (*pfun_VDMHAL_BackupInfo)(BACKUP_INFO_S *pBackUpInfo);
    VOID (*pfun_VDMHAL_Write1DYuv)( VOID *fpYuv, UINT8 *Yaddress, UINT32 Width, UINT32 Height, UINT32 chroma_idc);
    VOID (*pfun_VDMHAL_WriteScdEMARID)(VOID);
    SINT32 (*pfun_VDMHAL_GetRpuSize)(VOID);

#if 0
    VOID (*pfun_VDMHAL_EnableSmmu)(UINT32 VdhId);
#endif
} VDMHAL_FUN_PTR_S;

extern VDMHAL_FUN_PTR_S g_vdm_hal_fun_ptr;
#if 1
#define  VDMHAL_NULL_FUN_PTR     		-1
#define  VDMHAL_NULL_FUN_PRINT   dprint(PRN_FATAL,"VDMHAL_NULL_FUN_PRINT,L%d\n", __LINE__)

/*clk_en & cancel_reset should be treated differently*/
#define VDMDRV_SetClockSkip(SkipValue) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMDRV_SetClockSkip)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMDRV_SetClockSkip(SkipValue); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMDRV_OpenHardware(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMDRV_OpenHardware)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMDRV_OpenHardware(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMDRV_CloseHardware(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMDRV_CloseHardware)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMDRV_CloseHardware(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define DSPDRV_OpenHardware(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_DSPDRV_OpenHardware)  \
        { \
            g_vdm_hal_fun_ptr.pfun_DSPDRV_OpenHardware(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define DSPDRV_CloseHardware(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_DSPDRV_CloseHardware)  \
        { \
            g_vdm_hal_fun_ptr.pfun_DSPDRV_CloseHardware(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)


#define SCDDRV_OpenHardware(ScdId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_SCDDRV_OpenHardware)  \
        { \
            g_vdm_hal_fun_ptr.pfun_SCDDRV_OpenHardware(ScdId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define SCDDRV_CloseHardware(ScdId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_SCDDRV_CloseHardware)  \
        { \
            g_vdm_hal_fun_ptr.pfun_SCDDRV_CloseHardware(ScdId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

/*the same reason as VDMHAL_GetHalMemSize*/
#define VDMHAL_CloseHAL(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_CloseHAL)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_CloseHAL(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_ResetVdm(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_ResetVdm)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_ResetVdm(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_SetSmmuPageTableAddr(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_SetSmmuPageTableAddr)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_SetSmmuPageTableAddr(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_EnableSmmu() \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_EnableSmmu)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_EnableSmmu(); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_GlbReset( ) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_GlbReset)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_GlbReset( ); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_GlbResetX(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_GlbResetX)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_GlbResetX(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_ClearIntState(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_ClearIntState)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_ClearIntState(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_ClearMMUIntState(VdhId) \
    do \
    { \
        if (NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_ClearMMUIntState)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_ClearMMUIntState(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    } while(0)

#define VDMHAL_MaskInt(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_MaskInt)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_MaskInt(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_EnableInt(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_EnableInt)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_EnableInt(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_StartHwRepair(VdhId, pTask) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwRepair)  \
        { \
        	g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwRepair(VdhId, pTask); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_StartHwDecode(VdhId, pTask) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwDecode)  \
        { \
        	g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwDecode(VdhId, pTask); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_MakeDecReport(pMakeDecReport) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_MakeDecReport)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_MakeDecReport(pMakeDecReport); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_BackupInfo(pBackUpInfo) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_BackupInfo)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_BackupInfo(pBackUpInfo); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_Write1DYuv(fpYuv, Yaddress, Width, Height, chroma_idc) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_Write1DYuv)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_Write1DYuv(fpYuv, Yaddress,  Width, Height, chroma_idc); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_WriteScdEMARID() \
    do \
    { \
        if (NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_WriteScdEMARID) \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_WriteScdEMARID(); \
        } \
        else \
        { \
            VDMHAL_NULL_FUN_PRINT; \
        } \
    } while (0)

#define BPDDRV_OpenHardware(BpdId) \
    (g_vdm_hal_fun_ptr.pfun_BPDDRV_OpenHardware? \
     g_vdm_hal_fun_ptr.pfun_BPDDRV_OpenHardware(BpdId): \
     VDMHAL_NULL_FUN_PTR)

#define VDMHAL_GetCharacter() \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_GetCharacter? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_GetCharacter(): \
     VDMHAL_NULL_FUN_PTR)

/*hal_mem_size of different chips should be treated differently, for example tables of VP6&VP8*/
#define VDMHAL_GetHalMemSize() \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_GetHalMemSize? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_GetHalMemSize(): \
     VDMHAL_NULL_FUN_PTR)

#define VDMHAL_CalcFsSize(ChanId, Width, Height, VidStd, pVfmwFrameSizeInfo) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_CalcFsSize? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_CalcFsSize(ChanId, Width, Height, 0, VidStd, pVfmwFrameSizeInfo): \
     VDMHAL_NULL_FUN_PTR)
/*the same reason as VDMHAL_GetHalMemSize*/
#define VDMHAL_OpenHAL(pOpenParam) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_OpenHAL? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_OpenHAL(pOpenParam): \
     VDMHAL_NULL_FUN_PTR)

/*arranging memory should be dealed with carefully, for example 4 mb 1d*/
#define VDMHAL_ArrangeMem( MemAddr, MemSize, Width, Height, PmvNum, FrameNum, UserDec, eFlag, pVdmMemArrange ) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_ArrangeMem? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_ArrangeMem(MemAddr, MemSize, Width, Height, PmvNum, FrameNum, UserDec, eFlag, pVdmMemArrange ):\
     VDMHAL_NULL_FUN_PTR)

#define VDMHAL_CheckReg(reg_id, VdhId) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_CheckReg? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_CheckReg(reg_id, VdhId): \
     VDMHAL_FUN_FALSE)

#define VDMHAL_ReadMMUMask(VdhId) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_ReadMMUMask ? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_ReadMMUMask(VdhId): \
     VDMHAL_FUN_FALSE)

#define VDMHAL_WriteMMUMask(mask, VdhId) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_WriteMMUMask? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_WriteMMUMask(mask, VdhId): \
     VDMHAL_FUN_FALSE)

#define VDMHAL_PrepareDec(VidStd, pDecParam, VdhId, pTask) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_PrepareDec? \
	 g_vdm_hal_fun_ptr.pfun_VDMHAL_PrepareDec(VidStd, pDecParam, VdhId, pTask): \
     VDMHAL_NULL_FUN_PTR)

#define VDMHAL_IsVdmReady(VdhId) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_IsVdmReady? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_IsVdmReady(VdhId): \
     VDMHAL_FUN_FALSE)

#define VDMHAL_IsVdmRun(VdhId) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_IsVdmRun? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_IsVdmRun(VdhId): \
     VDMHAL_FUN_FALSE)

#define VDMHAL_PrepareRepair(VidStd, pDecParam, RepairTime, VdhId, pTask) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_PrepareRepair? \
	 g_vdm_hal_fun_ptr.pfun_VDMHAL_PrepareRepair(VidStd, pDecParam, RepairTime, VdhId, pTask): \
     VDMHAL_NULL_FUN_PTR)

#define VDMHAL_ArrangeMem_BTL( MemAddr, MemSize, Width, Height, PmvNum, s32MaxRefFrameNum, s32DisplayFrameNum, UserDec, pVdmMemArrange, s32Btl1Dt2DEnable, s32BtlDbdrEnable ) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_ArrangeMem_BTL? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_ArrangeMem_BTL( MemAddr, MemSize, Width, Height, PmvNum, s32MaxRefFrameNum, s32DisplayFrameNum, UserDec, pVdmMemArrange , s32Btl1Dt2DEnable, s32BtlDbdrEnable):\
     VDMHAL_NULL_FUN_PTR)

#define BPDDRV_CloseHardware(BpdId) \
    (g_vdm_hal_fun_ptr.pfun_BPDDRV_CloseHardware? \
     g_vdm_hal_fun_ptr.pfun_BPDDRV_CloseHardware(BpdId): \
     VDMHAL_NULL_FUN_PTR)

#define VDMHAL_GetRpuSize() \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_GetRpuSize? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_GetRpuSize(): \
     VDMHAL_NULL_FUN_PTR)

#define MMUDRV_OpenHardware(MmuId) \
    (g_vdm_hal_fun_ptr.pfun_MMUDRV_OpenHardware? \
     g_vdm_hal_fun_ptr.pfun_MMUDRV_OpenHardware(MmuId): \
     VDMHAL_NULL_FUN_PTR)

#define MMUDRV_CloseHardware(MmuId) \
    (g_vdm_hal_fun_ptr.pfun_MMUDRV_CloseHardware? \
     g_vdm_hal_fun_ptr.pfun_MMUDRV_CloseHardware(MmuId): \
     VDMHAL_NULL_FUN_PTR)

#endif


#ifdef __cplusplus
}
#endif


#endif
