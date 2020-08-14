#include "drv_venc_buf_mng.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define RC_RECODE_USE   1
#define RC_FIRST_I_RECODE 1

#define GOP_THRESHOLD 1000
#define MODIFY_MINQP_FRAMENUM 200
#define ENLARGEMENT_FACTOR 6
#define MAX_INTER_LOADSIZE 5
#define MAX_INTRA_LOADSIZE 2

#define VENC_RC_FATAL(fmt...) //HI_FATAL_PRINT(HI_ID_VENC, fmt)
#define VENC_RC_ERR(fmt...) //HI_ERR_PRINT(HI_ID_VENC, fmt)
#define VENC_RC_WARN(fmt...) //HI_WARN_PRINT(HI_ID_VENC, fmt)
#define VENC_RC_INFO(fmt...) //HI_INFO_PRINT(HI_ID_VENC, fmt)
#define VENC_RC_DBG(fmt...) //HI_DBG_PRINT(HI_ID_VENC, fmt)

typedef struct
{
    HI_U32 GetFrameNumTry;
    HI_U32 PutFrameNumTry;
    HI_U32 GetStreamNumTry;
    HI_U32 PutStreamNumTry;

    HI_U32 GetFrameNumOK;
    HI_U32 PutFrameNumOK;
    HI_U32 GetStreamNumOK;
    HI_U32 PutStreamNumOK;

    HI_U32 BufFullNum;
    //HI_U32 SkipFrmNum;
    HI_U32 FrmRcCtrlSkip;
    HI_U32 SamePTSSkip;
    HI_U32 QuickEncodeSkip;
    HI_U32 TooFewBufferSkip;
    HI_U32 ErrCfgSkip;
    HI_U32 TooManyBitsSkip;

    HI_U32 QueueNum;
    HI_U32 VpssQueueBufNum;
    HI_U32 VpssImgBufNum;
    HI_U32 DequeueNum;                  /*OMX Channel not use this data*/
    HI_U32 StreamQueueNum;              /*just OMX Channel use this data*/
    HI_U32 MsgQueueNum;

    HI_U32 UsedStreamBuf;

    HI_U32 StreamTotalByte;

    HI_U32 u32RealSendInputRrmRate;       /*use to record curent Input FrameRate in use*/
    HI_U32 u32RealSendOutputFrmRate;      /*use to record curent Output FrameRate in use*/
    HI_U32 u32FrameType;

    HI_U32 u32TotalPicBits;
    HI_U32 u32TotalEncodeNum;
	HI_U32 RateControlRatio;           /*use to know whether the current bit rate is controlled*/
} VeduEfl_StatInfo_S;

typedef struct
{
    HI_U32 skipFrame;
    HI_U32 MinQp;
    HI_U32 MaxQp;
    HI_U32 PicWidth;           /* done for 16 aligned @ venc */
    HI_U32 PicHeight;          /* done for 16 aligned @ venc */
    HI_U32 RcStart;

    HI_U32 PicBits;
    HI_U32 VoFrmRate;
    HI_U32 ViFrmRate;
    HI_U32 BitRate;
    HI_U32 Gop;
    HI_U32 IsIntraPic;
    HI_U32 RcnIdx;
    HI_U32 PmeInfoIdx;
    HI_U32 H264FrmNum;
    HI_U32 VencBufFull;
    HI_U32 VencPbitOverflow;
    HI_S32 LowDlyMod;
    HI_U32 bFrmRateSataError;
    HI_U32 IFrmInsertFlag;

    HI_S32 FrmNumInGop; //frame number in gop minus 1
    HI_S32 FrmNumSeq;//frame number in sequence

    /*******bits***********/
    HI_U32 GopBits;//bits in gop
    HI_S32 AverageFrameBits;//average bits of one frame

    HI_S32 IPreQp;//Qp of previous six I frames
    HI_S32 PPreQP;
    HI_S32 PreTargetBits;

    /*********IMB***********/
    HI_S32 NumIMBCurFrm;//number of I MB in current  frame
    HI_S32 NumIMB[6];//number of I MB of previous six frames
    HI_S32 AveOfIMB;//average number of I MB of previous six frames

    /**********scence change detect *************/
    HI_U32 IMBRatioCurScen[6];//number of I MB of previous six frames just for scence change judge

    /************RC Out**************/
    HI_U32 CurQp;
    HI_U32 TargetBits;
    HI_U32 StartQP;
    HI_S32 InitialQp;

    /**********parameter set************/
    HI_S32 MinTimeOfP;
    HI_S32 MaxTimeOfP;
    HI_S32 DeltaTimeOfP;
    HI_S32 IQpDelta;
    HI_S32 PQpDelta;

    HI_S32 CurTimeOfP[2];    //0:Intra    1:Inter
    HI_U32 IPQPDelta;
    HI_S32 WaterlineInit;
    HI_S32 Waterline;
    HI_U32 LastFrameTypeIsIntra;
    /**********I/P bits set************/
    HI_U32 IPicBits[2];
    HI_U32 PPicBits[5];
    HI_U32 IPicBitsHead;
    HI_U32 PPicBitsHead;

    HI_U32 Recode;         //1: frame need to be recoded   0: not need;  just reencode one time
    HI_U32 ReStartQP;
    /**********instant bits set************/
    HI_U32 InstBits[65];     //the instant bitrate of last frame
    HI_U32 InstBitsLen;
    HI_U32 InstBitsSum;
    HI_U32 InstBitsHead;
    HI_U32 PreInstBitsSum;
    HI_U32 LastLen;
    HI_U32 LastGop;
    HI_U32 LastBitRate;
    /**********VBR set************/
    HI_BOOL IsVBRMode;
    HI_U32 VBR_SumOfQP;
    HI_S32 VBR_SumOfDeltaPicBits;
    HI_S32 LastSumOfDeltaPicBits;
    HI_U32 VBR_MinQP;
    HI_U32 VBR_MaxBitRate;
} VeduEfl_Rc_S;

HI_S32 VENC_DRV_RcOpenOneFrm(VeduEfl_Rc_S *pstRc);
HI_S32 VENC_DRV_RcCloseOneFrm(VeduEfl_Rc_S *pstRc, VALG_FIFO_S *pstBitsFifo, HI_U32 *TrCount , HI_U32 *InterFrmCnt);
HI_VOID VEDU_DRV_RCCalcMinQPForVBR(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCGetRecodingQP(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCGetVBRMaxBitRate(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCofFirstFrame(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCInitInfo(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCInitInstantBits(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCGetTargetFrameBits(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCInitFrameBits(VeduEfl_Rc_S* pstRc);
HI_U32  VEDU_DRV_EflRcInitQp(HI_U32 bits, HI_U32 w, HI_U32 h);
HI_VOID VEDU_DRV_RCCalculateCurQP(VeduEfl_Rc_S* pstRc);

HI_VOID VEDU_DRV_RCModifyQPDeltaOfIP(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCCalcQPDeltaByFrameBits(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCCalcQPDeltaByInstantBits(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCCalcQPDeltaByBitRate(HI_U32 u32CurInstBits, HI_U32 BitRate);
HI_S32 VEDU_DRV_RCCalcQPDeltaByWaterline(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCGetQPDeltaForVBR(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCCalcVBRQPDeltaByInstantBits(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCCalcVBRQPDeltaByPicBits(VeduEfl_Rc_S* pstRc);

HI_S32 VEDU_DRV_RCGetTimeOfP(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCUpdateFrameBits(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCCalcTimeOfP(VeduEfl_Rc_S* pstRc);
HI_VOID ModifyInfoForBitRatesChange(VeduEfl_Rc_S* pstRc);
HI_U32 VEDU_DRV_EflRCGetCurTimeOfP(VeduEfl_Rc_S* pstRc);
HI_U32 VEDU_DRV_EflRCCalcTargetFrameBits(VeduEfl_Rc_S* pstRc, HI_U32 CurTimeOfP);
HI_VOID ModifyInfoForGopChange(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCModifyQPForSpecialScene(VeduEfl_Rc_S* pstRc);
HI_S32  VENC_DRV_EflRcAverage(HI_S32* pData, HI_S32 n);//meiduli

HI_VOID VEDU_DRV_RCUpdateInfo(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCUpdateWaterline(VeduEfl_Rc_S* pstRc);
HI_VOID VEDU_DRV_RCUpdateInstantBits(VeduEfl_Rc_S* pstRc);
HI_VOID ModifyInfoForFrmRateChange(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCIsNeedRecoding(VeduEfl_Rc_S* pstRc);
HI_S32 VEDU_DRV_RCStartQPChange(VeduEfl_Rc_S* pstRc);

HI_S32 D_VENC_RC_CLIP3(HI_S32 x, HI_S32 y, HI_S32 z);
HI_VOID VEDU_DRV_RCUpdateIMBInfo(VeduEfl_Rc_S* pstRc);
HI_S32 VENC_DRV_EflRcAverage(HI_S32* pData, HI_S32 n);
HI_S32 VENC_DRV_CalRCRatio(HI_U32 CurInstBits, HI_U32 BitRate);  

