
#ifndef _VDM_HAL_HEADER_
#define _VDM_HAL_HEADER_

#ifdef __cplusplus
extern "C" {
#endif

//#include "vfmw_osal.h"
#include "decparam.h"
#include "vdm_drv.h"
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

/************************************************************************/
/*  ?                                                                  */
/************************************************************************/
/* ????? */
#define RD_VREG( reg, dat, VdhId )               \
    do {                    \
        if (VdhId < MAX_VDH_NUM)                \
        {                    \
            dat = *((volatile SINT32*)((SINT32)g_HwMem[VdhId].pVdmRegVirAddr + reg)); \
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
            *((volatile SINT32*)((SINT32)g_HwMem[VdhId].pVdmRegVirAddr + reg)) = dat; \
        }                    \
        else                 \
        {                    \
            dprint(PRN_ALWS,"%s: WR_VREG but VdhId(%d) > MAX_VDH_NUM(%d)\n", __func__, VdhId, MAX_VDH_NUM); \
        }                    \
    } while(0)

#define RD_BPD_VREG( reg, dat )               \
    do {                    \
    dat = *((volatile SINT32*)((UADDR)g_HwMem[0].pBpdRegVirAddr + reg)); \
    } while(0)

#define WR_BPD_VREG( reg, dat )               \
    do {                    \
    *((volatile SINT32*)((UADDR)g_HwMem[0].pBpdRegVirAddr + reg)) = dat; \
    } while(0)

/* ????? */
#define RD_MSGWORD( vir_addr, dat )          \
    do {                 \
        dat = *((volatile SINT32*)((SINT32)vir_addr));                      \
    } while(0)

#define WR_MSGWORD( vir_addr, dat )          \
    do {                 \
        *((volatile SINT32*)((SINT32)(vir_addr))) = dat;                      \
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
    UINT32 isValid;
} FRAME_BUF_RECORD_S;

typedef struct
{
    UADDR  pmvAddr;
    UINT32 isValid;
    UINT32 isInUse;
} PMV_RECORD_S;

typedef struct
{
    SINT32    VahbStride;
    SINT32    TotalMemUsed;
    // frame buffer
#ifndef Hi3716MV410
    UADDR     FrameAddr[MAX_FRAME_NUM];
#else
    FRAME_BUF_RECORD_S frameBufRec[MAX_FRAME_NUM];
#endif
#ifdef VFMW_MODULE_LOWDLY_SUPPORT
    //add by l00225186 fordsp 2013-04-25
    UADDR     LineNumAddr[MAX_FRAME_NUM];
#endif
    //HEVC 10bit
    SINT32    VahbStride_2bit;
    SINT32    LumaStride_2bit;
    SINT32    LumaOffset_2bit;
    SINT32    ChromOffset_2bit;
    SINT32    ChromOffset;
    SINT32    LumaPixelOffset;
    SINT32    ChromaPixelOffset;
    SINT32    ValidFrameNum;
    // btl buffer
    UADDR     BtlAddr[MAX_FRAME_NUM];
    SINT32    ValidBTLFrameNum;
    // PMV buffer
#ifndef Hi3716MV410
    UADDR     PMVAddr[30];//这里实际上应该为MAX_FRAME_NUM才合理，但是如果改为MAX_FRAME_NUM，编译器会报错，所以暂时改为30,等待后续更好的方案
#else
    PMV_RECORD_S pmvRec[MAX_PMV_SLOT_NUM];
#endif
    SINT32    HafPmvOffset;
    SINT32    ValidPMVNum;

    UINT32 configuredFrameNum;
    UINT32 configuredPmvNum;
} VDMHAL_MEM_ARRANGE_S;
#if 0
/* ?????? */
typedef struct
{
    IMAGE     Image;
    SINT32    IsRef;
    SINT32    IsDisp;
} COMN_FRAME_STORE_S;

typedef struct
{
    COMN_FRAME_STORE_S  FrameStore[64];
    SINT32    ValidFrameNum;
} FRAME_POOL_S;
#endif

typedef struct hi_FS_S
{
    SINT8    *pu8VirAddr;
    SINT8     s32RefFlag;    /* 0: ??????,1: ??????, 2: ??????, 3: ??????  */
    SINT8     s32DispFlag;   /* 0: ??????,1: ??????, 2: ??????, 3: ??????  */
    /* ??????? */
    IMAGE     stImage;

    /* ???? */
    SINT32    s32FsID;
    SINT32    s32ErrLevel;

    /* ?? */
    UADDR     s32PhyAddr;

} FS_S;


/* ??? */
typedef struct hiFS_POOL_S
{
    FS_S      stFrameStore[MAX_FS_NUM];
    SINT32    s32ValidFsNum;
    /* ??? */
    SINT32    s32FrameWidth;
    SINT32    s32FrameHeight;
    SINT32    s32FrameStride;
} FS_POOL_S;


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
    VOID (*pfun_VDMDRV_OpenHardware)(SINT32 VdhId);
    VOID (*pfun_VDMDRV_CloseHardware)(SINT32 VdhId);

    VOID (*pfun_DSPDRV_OpenHardware)(SINT32 VdhId);
    VOID (*pfun_DSPDRV_CloseHardware)(SINT32 VdhId);

    VOID (*pfun_SCDDRV_OpenHardware)(SINT32 ScdId);
    VOID (*pfun_SCDDRV_CloseHardware)(SINT32 ScdId);
    SINT32 (*pfun_BPDDRV_OpenHardware)(VOID);
    SINT32 (*pfun_BPDDRV_CloseHardware)(VOID);
    /*VDM_HAL*/
    VOID (*pfun_VDMHAL_GetCharacter)(VOID);
    SINT32 (*pfun_VDMHAL_GetHalMemSize)(VOID);
    SINT32 (*pfun_VDMHAL_OpenHAL)(VDMHAL_OPENPARAM_S *pOpenParam);
    VOID (*pfun_VDMHAL_CloseHAL)(SINT32 VdhId);
    SINT32 (*pfun_VDMHAL_ArrangeMem)( UADDR  MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 FrameNum, SINT32 UserDec, SINT32 ChanID, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange );
    SINT32 (*pfun_VDMHAL_ArrangeMem_BTL)( UADDR  MemAddr, SINT32 MemSize, SINT32 Width, SINT32 Height, SINT32 PmvNum, SINT32 s32MaxRefFrameNum, SINT32 s32DisplayFrameNum, SINT32 UserDec, VDMHAL_MEM_ARRANGE_S *pVdmMemArrange , SINT32 s32Btl1Dt2DEnable, SINT32 s32BtlDbdrEnable );
    VOID (*pfun_VDMHAL_ResetVdm)( SINT32 VdhId );
    VOID (*pfun_VDMHAL_GlbReset)( VOID );
    VOID (*pfun_VDMHAL_GlbResetX)( SINT32 VdhId );
    VOID (*pfun_VDMHAL_ClearIntState)( SINT32 VdhId );
    VOID (*pfun_VDMHAL_MaskInt)( SINT32 VdhId );
    VOID (*pfun_VDMHAL_EnableInt)( SINT32 VdhId );
    SINT32 (*pfun_VDMHAL_CheckReg)(REG_ID_E reg_id, SINT32 VdhId);
    VOID (*pfun_VDMHAL_StartHwRepair)(SINT32 VdhId);
    VOID (*pfun_VDMHAL_StartHwDecode)(SINT32 VdhId);
    SINT32 (*pfun_VDMHAL_PrepareDec)( VID_STD_E VidStd, VOID *pDecParam, SINT32 VdhId );
    SINT32 (*pfun_VDMHAL_IsVdmReady)(SINT32 VdhId);
    SINT32 (*pfun_VDMHAL_IsVdmRun)(SINT32 VdhId);
    SINT32 (*pfun_VDMHAL_PrepareRepair)( VID_STD_E VidStd, VOID *pDecParam, SINT32 RepairTime, SINT32 VdhId );
    SINT32 (*pfun_VDMHAL_MakeDecReport)(MAKE_DEC_REPORT_S *pMakeDecReport);
    SINT32 (*pfun_VDMHAL_BackupInfo)(BACKUP_INFO_S *pBackUpInfo);
    VOID (*pfun_VDMHAL_Write1DYuv)( VOID *fpYuv, UINT8 *Yaddress, UINT32 Width, UINT32 Height, UINT32 chroma_idc);
    VOID (*pfun_VDMHAL_WriteScdEMARID)(VOID);
} VDMHAL_FUN_PTR_S;

extern VDMHAL_FUN_PTR_S g_vdm_hal_fun_ptr;
#if 1
#define  VDMHAL_NULL_FUN_PTR     		-1
#define  VDMHAL_NULL_FUN_PRINT   dprint(PRN_FATAL,"VDMHAL_NULL_FUN_PRINT,L%d\n", __LINE__)

/*clk_en & cancel_reset should be treated differently*/
#if 0
#define VDMDRV_OpenHardware(VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMDRV_OpenHardware? \
    g_vdm_hal_fun_ptr.pfun_VDMDRV_OpenHardware(VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define SCDDRV_OpenHardware() \
    g_vdm_hal_fun_ptr.pfun_SCDDRV_OpenHardware? \
    g_vdm_hal_fun_ptr.pfun_SCDDRV_OpenHardware(): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_CloseHAL(VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_CloseHAL? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_CloseHAL(VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_ResetVdm(VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_ResetVdm? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_ResetVdm(VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_ClearIntState(VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_ClearIntState? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_ClearIntState(VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_MaskInt(VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_MaskInt? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_MaskInt(VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_EnableInt(VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_EnableInt? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_EnableInt(VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_StartHwRepair(VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwRepair? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwRepair(VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_StartHwDecode(VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwDecode? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwDecode(VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_MakeDecReport(VidStd, pDecReport, VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_MakeDecReport? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_MakeDecReport(VidStd, pDecReport, VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_BackupInfo(pMb0QpInCurrPic, VdhId) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_BackupInfo? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_BackupInfo(pMb0QpInCurrPic, VdhId): \
    VDMHAL_NULL_FUN_PRINT
#define VDMHAL_Write1DYuv(fpYuv, Yaddress, Width, Height, chroma_idc) \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_Write1DYuv? \
    g_vdm_hal_fun_ptr.pfun_VDMHAL_Write1DYuv(fpYuv, Yaddress,  Width, Height, chroma_idc): \
    VDMHAL_NULL_FUN_PRINT
#endif
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

#define VDMHAL_StartHwRepair(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwRepair)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwRepair(VdhId); \
        }  \
        else  \
        {  \
            VDMHAL_NULL_FUN_PRINT;  \
        }  \
    }while(0)

#define VDMHAL_StartHwDecode(VdhId) \
    do \
    { \
        if(NULL != g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwDecode)  \
        { \
            g_vdm_hal_fun_ptr.pfun_VDMHAL_StartHwDecode(VdhId); \
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

#define BPDDRV_OpenHardware() \
    (g_vdm_hal_fun_ptr.pfun_BPDDRV_OpenHardware? \
     g_vdm_hal_fun_ptr.pfun_BPDDRV_OpenHardware(): \
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

#define VDMHAL_PrepareDec(VidStd, pDecParam, VdhId) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_PrepareDec? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_PrepareDec(VidStd, pDecParam, VdhId): \
     VDMHAL_NULL_FUN_PTR)

#define VDMHAL_IsVdmReady(VdhId) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_IsVdmReady? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_IsVdmReady(VdhId): \
     VDMHAL_FUN_FALSE)

#define VDMHAL_IsVdmRun(VdhId) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_IsVdmRun? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_IsVdmRun(VdhId): \
     VDMHAL_FUN_FALSE)

#define VDMHAL_PrepareRepair(VidStd, pDecParam, RepairTime, VdhId) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_PrepareRepair? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_PrepareRepair(VidStd, pDecParam, RepairTime, VdhId): \
     VDMHAL_NULL_FUN_PTR)

#define VDMHAL_ArrangeMem_BTL( MemAddr, MemSize, Width, Height, PmvNum, s32MaxRefFrameNum, s32DisplayFrameNum, UserDec, pVdmMemArrange, s32Btl1Dt2DEnable, s32BtlDbdrEnable ) \
    (g_vdm_hal_fun_ptr.pfun_VDMHAL_ArrangeMem_BTL? \
     g_vdm_hal_fun_ptr.pfun_VDMHAL_ArrangeMem_BTL( MemAddr, MemSize, Width, Height, PmvNum, s32MaxRefFrameNum, s32DisplayFrameNum, UserDec, pVdmMemArrange , s32Btl1Dt2DEnable, s32BtlDbdrEnable):\
     VDMHAL_NULL_FUN_PTR)

#define BPDDRV_CloseHardware() \
    (g_vdm_hal_fun_ptr.pfun_BPDDRV_CloseHardware? \
     g_vdm_hal_fun_ptr.pfun_BPDDRV_CloseHardware(): \
     VDMHAL_NULL_FUN_PTR)
#endif



#ifdef __cplusplus
}
#endif


#endif
