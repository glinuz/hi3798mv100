#ifndef __DRV_VENC_EFL_H__
#define __DRV_VENC_EFL_H__

#include "hi_type.h"
#include "drv_venc_buf_mng.h"
#include "hi_unf_common.h"
#include "hi_drv_video.h"
#include "drv_venc_queue_mng.h"
#include "hi_drv_mmz.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

//#define __VENC_DRV_DBG__
//#define __VENC_RC_DBG__
//#define __OMXVENC_ONE_IN_ONE_OUT__
#define VEDU_TR_STEP 1
/*************************************************************************************/

#define MAX_VEDU_CHN 8

#define MAX_VEDU_QUEUE_NUM 6
#define MAX_VEDU_STRM_QUEUE_NUM 20
#define MSG_QUEUE_NUM     100
#define INVAILD_CHN_FLAG   (-1)

#define VEDU_MAX_ENC_WIDTH   (1920)
#define VEDU_MIN_ENC_WIDTH   (176)
#define VEDU_MAX_ENC_HEIGHT  (1088)
#define VEDU_MIN_ENC_HEIGHT  (144)

#define OMX_OUTPUT_PORT_BUFFER_MAX (20)

#ifdef __VENC_DPT_ONLY__
#define VEDU_MMZ_ALIGN       (256)
#else
#define VEDU_MMZ_ALIGN       (64)
#endif

#define VEDU_MAX_INPUT_FRAME_RATE  (60)

/*******************************************************************/

#define __VEDU_NEW_RC_ALG__

#define D_VENC_ALIGN_UP(val, align) ( (val+((align)-1))&~((align)-1) )
typedef HI_S32 (*VE_IMAGE_FUNC)(HI_S32 handle, HI_DRV_VIDEO_FRAME_S* pstImage);
typedef HI_S32 (*VE_IMAGE_OMX_FUNC)(HI_S32 handle, venc_user_buf* pstImage);
typedef HI_S32 (*VE_CHANGE_INFO_FUNC)(HI_HANDLE handle, HI_U32 u32Width, HI_U32 u32Height, HI_U32 u32FrmRate);
typedef HI_S32 (*VE_DETACH_FUNC)(HI_HANDLE handle, HI_HANDLE hSrc);

/*define VDEC public buffer structure for MMZ/SMMU*/
typedef struct
{
	HI_U8* pu8StartVirAddr;    /*VirAddr*/
	HI_U32 u32StartPhyAddr;    /*PhyAddr or SmmuAddr*/
	HI_U32 u32Size;
} VENC_BUFFER_S;

typedef struct
{
    HI_U32 ProfileIDC;
    HI_U32 FrameWidthInMb;
    HI_U32 FrameHeightInMb;
    HI_U8  FrameCropLeft;
    HI_U8  FrameCropRight;
    HI_U8  FrameCropTop;
    HI_U8  FrameCropBottom;
} VeduEfl_H264e_SPS_S;

typedef struct
{
    HI_U32  ConstIntra;
    HI_S32  ChrQpOffset;
    HI_U32  H264HpEn;
    HI_U32  H264CabacEn;
    HI_S32* pScale8x8;
} VeduEfl_H264e_PPS_S;

typedef struct
{
    HI_U32 slice_type; /* 0-P, 2-I */
    HI_U32 frame_num;
    HI_U32 NumRefIndex; /* 0 or 1 */
} VeduEfl_H264e_SlcHdr_S;

typedef struct
{
    HI_U32 Pframe;   /* 1-P, 0-I */
    HI_U32 TR;       /* temporal ref */
    HI_U32 Wframe;
    HI_U32 Hframe;
    HI_U32 SrcFmt;
    HI_U32 PicQP;
} VeduEfl_H263e_PicHdr_S;

typedef struct
{
    HI_U32 Wframe;
    HI_U32 Hframe;
} VeduEfl_MP4e_VOL_S;

typedef struct
{
    HI_U32 Pframe;        /* 1-P, 0-I */
    HI_U32 PicQP;
    HI_U32 Fcode;
} VeduEfl_MP4e_VopHdr_S;

typedef struct
{
    HI_U32 PacketLen;     /* 64 aligned */
    HI_U32 InvldByte;     /* InvalidByteNum */
    HI_U8  Type;
    HI_U8  bBotField;
    HI_U8  bField;
    HI_U8  bLastSlice;
    HI_U32 ChnID;
    HI_U32 PTS0;
    HI_U32 PTS1;
    HI_U32 Reserved[10];
} VeduEfl_NaluHdr_S;

typedef struct
{
    HI_U32              handle;
    VE_IMAGE_FUNC       pfGetImage;
    VE_IMAGE_OMX_FUNC   pfGetImage_OMX;
    VE_IMAGE_FUNC       pfPutImage;
    VE_CHANGE_INFO_FUNC pfChangeInfo;
    VE_DETACH_FUNC      pfDetachFunc;
} VeduEfl_SrcInfo_S;

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
    HI_U32 TooFewBufferWaitCnt;
    HI_U32 TooFewBufferQueFailCnt;
    HI_U32 QueueFrmByAttachCnt;
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
    //HI_U32 u32AvgFrmRate;

} VeduEfl_StatInfo_S;

typedef struct
{
    HI_BOOL bOMXChn;
    HI_BOOL bAutoRequestIfrm;
	HI_BOOL bPrependSpsPps;
	HI_BOOL bRCSkipFrmEn;
	
    HI_U32 Protocol;      /* VEDU_H264, VEDU_H263 or VEDU_MPEG4 */
    HI_U32 CapLevel;      /* VEDU buffer alloc*/
    HI_U32 Profile;       /* H264 Profile*/
    HI_U32 FrameWidth;    /* width	in pixel, 96 ~ 2048 */
    HI_U32 FrameHeight;   /* height in pixel, 96 ~ 2048 */

    HI_U32 RotationAngle; /* venc don't care */

    HI_U32 PackageSel;
    HI_U32 Priority;
    HI_U32 streamBufSize;

    HI_BOOL QuickEncode;
    HI_BOOL SlcSplitEn;    /* 0 or 1, slice split enable */
    HI_BOOL SlcSplitMod;   /* 0 or 1, 0:byte; 1:mb line */
    HI_U32 SplitSize;     /* <512 mb line @ H264, H263 don't care*/
    HI_U32 Gop;
    HI_U32 QLevel;        /* venc don't care */
	HI_U32 DriftRCThr;    /* the threshold of RC skip frame; vp default 20%*/
} VeduEfl_EncCfg_S;

typedef struct
{
    HI_U32 BitRate;       /* 32k ~ 20M, bits per second */
    HI_U32 OutFrmRate;    /* 1 ~ InFrmRate */
    HI_U32 InFrmRate;     /* 1 ~ 30  */

    HI_U32   MaxQp;         /* H264: 0 ~ 51, Mpeg4: 1 ~ 31 */
    HI_U32   MinQp;         /* MinQp <= MaxQp */
    HI_U32   Gop;         /* for new RC ALG*/
} VeduEfl_RcAttr_S;

typedef struct
{
    HI_VOID*   pVirtAddr[2];
    HI_U32     PhyAddr[2];
    HI_U32     SlcLen[2];  /* byte */
    HI_U32     PTS0;
    HI_U32     PTS1;
    HI_U32     bFrameEnd;
    HI_U32     NaluType;   /* 1(P),5(I),6(SEI),7(SPS) or 8(PPS), only used by H264	*/
    HI_U32     InvldByte;
} VeduEfl_NALU_S;

typedef struct
{
    HI_U32   Enable [8];    /* only used at H264  */
    HI_U32   AbsQpEn[8];
    HI_S32   Qp     [8];    /* -26 ~ 25 or 0 ~ 51 */
    HI_U32   Width  [8];    /* size in MB */
    HI_U32   Height [8];    /* size in MB */
    HI_U32   StartX [8];    /* size in MB */
    HI_U32   StartY [8];    /* size in MB */
    HI_U32   Keep   [8];

} VeduEfl_RoiAttr_S;

typedef struct
{
    HI_U32 osd_rgbfm;            /* 0 ~ 1 */
    HI_S32 osd_global_en   [8];
    HI_S32 osd_en          [8];
    HI_U32 osd_alpha0      [8];
    HI_U32 osd_alpha1      [8];
    HI_U32 osd_global_alpha[8];
    HI_U32 osd_x           [8]; /* pixel, 2 aligned */
    HI_U32 osd_y           [8]; /* pixel, 2 aligned */
    HI_U32 osd_w           [8]; /* pixel, 2 aligned */
    HI_U32 osd_h           [8]; /* pixel, 2 aligned */
    HI_U32 osd_addr        [8]; /* 16-byte aligned  */
    HI_U32 osd_stride      [8]; /* 64-byte aligned  */
    HI_U32 osd_layer_id    [8]; /* 0 ~ 7 */

    HI_S32 osd_absqp_en    [8]; /* only used at H264  */
    HI_S32 osd_qp          [8]; /* -26 ~ 25 or 0 ~ 51 */

    HI_U32 osd_invs_en     [8];
    HI_U32 osd_invs_w;          /* 1 ~ 4 */
    HI_U32 osd_invs_h;          /* 1 ~ 4 */
    HI_U32 osd_invs_thr;        /* 0 ~ 255 */
    HI_U32 osd_invs_mode;       /* 0 ~ 1 */

} VeduEfl_OsdAttr_S;

//////////// add by ckf77439

typedef struct
{
    HI_U32 skipFrame;
    HI_U32 preskipFrame;

    HI_S32 MbNum;//number of mb
    HI_S32 FrmNumInGop; //frame number in gop minus 1
    HI_S32 FrmNumSeq;//frame number in sequence

    /*******bits***********/
    HI_U32 GopBits;//bits in gop
    HI_S32 AveFrameBits;//average bits of one frame

    //  HI_S32 GopFrameNum;//frame num in gop
    HI_S32 GopBitsLeft;//left bits in gop
    HI_S32 PreGopBitsLeft;//left bits in previous gop
    HI_S32 TotalBitsLeft;//left bits in sequence
    HI_S32 TotalTargetBitsUsedInGop;//total target bits used in gop
    HI_S32 TotalBitsUsedInGop;//actual total used bits used in gop

    HI_S32 AvePBits;//bits of P frame
    HI_S32 ConsAvePBits;//constant bits of P frame

    HI_S32 ITotalBits[6];//bits of previous six I frames
    HI_S32 PTotalBits[6];//bits of previous six P frames
    HI_S32 PreType;//I or P frame of previous frame, 1--I frame, 0--P frame'
    HI_S32 PPreQp[6];//Qp of previous six P frames
    HI_S32 IPreQp[6];//Qp of previous six I frames
    HI_S32 PreTargetBits;

    HI_S32 SaveBitsMode;//mode of saving bits

    /*********IMB***********/
    HI_S32 NumIMBCurFrm;//number of I MB in current  frame
    HI_S32 NumIMB[6];//number of I MB of previous six frames
    HI_S32 AveOfIMB;//average number of I MB of previous six frames
    HI_S32 ImbRatioSeq;//sum of Imb Ratio in the sequence

    /**********scence change detect *************/
    HI_U32 IMBRatioCurScen[6];//number of I MB of previous six frames just for scence change judge
    HI_S32 CurZone[16];//histogram of scene change detect of current frame
    HI_S32 PreZone[16];//histogram of scene change detect of previous frame
    HI_S32 SceneChangeFlag;
    HI_S32 PreSceneChangeFlag;
    HI_S32 AveDiffZone;//average of previous six differences
    HI_S32 DiffZone[6];//previous six differences

    HI_S32 SenChaNum[6];//sence change num of previous gop
    HI_S32 CurSenChaNum;//sence change num of current gop

    HI_S32 PreAveY;//average of Y of previous frame
    HI_S32 AverageY[6];//Y of previous six frame

    /************RC Out**************/
    HI_S32 CurQp;
    HI_S32 TargetBits;


    HI_S32 InitialQp;
    HI_S32 PreQp;
    HI_S32 PreMeanQp;
    HI_S32 MeanQp[6];
    HI_S32 AveMeanQp;

    HI_S32 StillToMove;
    HI_S32 StillMoveNum;
    HI_S32 StillFrameNum;
    HI_S32 StillFrameNum2;

    /**********parameter set************/
    HI_S32 MinTimeOfP;
    HI_S32 MaxTimeOfP;
    HI_S32 DeltaTimeOfP;
    HI_S32 AveFrameMinusAveP;
    HI_S32 StillToMoveDelay;
    HI_S32 IQpDelta;
    HI_S32 PQpDelta;


    HI_S32 StreamBuffer[30];
    HI_S32 BufferBits;

} VeduEfl_Rc_S;

typedef struct
{
    HI_S32 ICatchEnable;
	HI_S32 PrependSpsPpsEnable;
    HI_S32 ICatchOn;
	HI_S32 AutoSkipFrameEn;
	HI_U32 DriftRcSkipThr;
    HI_S32 FrmCnt;
    /* channel parameter */
    HI_U32 Protocol;
    HI_U32  H264CabacEn;
    HI_U32  H264HpEn;


    HI_U32 PicWidth;           /* done for 16 aligned @ venc */
    HI_U32 PicHeight;          /* done for 16 aligned @ venc */
    HI_U32 YuvStoreType;
    HI_U32 YuvSampleType;
    HI_U32 RotationAngle;
    //HI_U32 FixedQP;
    HI_U16 SlcSplitEn;
    HI_U8  SlcSplitMod;       /* 0 or 1, byte or mb line ,just select 1*/
    HI_U32 SplitSize;
#if 0
    HI_U32  MaskIntEn;          /* 0: interrupt mode, 1: query mode */                         //
    HI_U32  RegConfigMode;                                                                     //32->8
    HI_U32  RegConfigAddr;
    HI_U8*  pDdrBase;                                                                          //??
#endif
    HI_U8  Priority;
    HI_BOOL QuickEncode;
    HI_BOOL OMXChn;

    /* frame buffer parameter */
    HI_U32 SStrideY;
    HI_U32 SStrideC;
    HI_U32  StoreFmt;           /* 0, semiplannar; 1, package; 2,planer */
    HI_U32 PackageSel;
    HI_U32 SrcYAddr;
    HI_U32 SrcCAddr;
    HI_U32 SrcVAddr;                  //just for input of planner
    HI_U32 RcnYAddr[2];
    HI_U32 RcnCAddr[2];
    HI_U32 RcnIdx;             /* 0 or 1, idx for rcn, !idx for ref */
#if 1

    HI_U32  RStrideY;
    HI_U32  RStrideC;
#endif
    /* stream buffer parameter */
    VENC_BUFFER_S StreamMMZBuf;

    HI_U32          StrmBufAddr;
    HI_U32          StrmBufRpAddr; /* phy addr for hardware */
    HI_U32          StrmBufWpAddr;
    HI_U32*         StrmBufRpVirAddr; /* phy addr for hardware */
    HI_U32*         StrmBufWpVirAddr;
    HI_U32          StrmBufSize;
    HI_U32          Vir2BusOffset; /* offset = vir - bus, @ stream buffer */
    VALG_CRCL_BUF_S stCycBuf;
    HI_VOID*        pStrmBufLock; /* lock stream buffer */

    HI_DRV_VIDEO_FRAME_S    stVpssFrame;  /*for omx vpss output frame info*/

    /* header parameter */
    HI_U8  SpsStream[16];
    HI_U8  PpsStream[320];
    HI_U32 SpsBits;
    HI_U32 PpsBits;
    HI_U32 SlcHdrStream [4];
    HI_U32 ReorderStream[2];
    HI_U32 MarkingStream[2];
    HI_U32 SlcHdrBits;         /* 8bit_0 | mark | reorder | slchdr */
    HI_U32 H263SrcFmt;         /* 1 ~ 6 */
    HI_U32 H264FrmNum;

    /*header for jpge*/
    HI_U8   pJfifHdr[698];
    HI_U8   pJpgeYqt[64];
    HI_U8   pJpgeCqt[64];
    HI_U32  QLevel;

    /* other parameter */
    HI_U32* pRegBase;
    volatile HI_U32  WaitingIsr;         /* to block STOP channel */
    volatile HI_U32  WaitingIsr_jpge;         /* to block STOP channel */
    HI_U32  PTS0;
    HI_U32  PTS1;
    HI_BOOL bNeverEnc;
    HI_BOOL bFirstNal2Send;    /*for omx_venc send output buffer flags*/

    /* read back reg */
    HI_U32 PicBits;
    HI_U32 TotalMbhBits;
    HI_U32 TotalTxtBits;
    HI_U32 TotalTxtCost0;
    HI_U32 TotalTxtCost1;
    HI_U32 MeanQP;
    HI_U32 VencEndOfPic;
    HI_U32 VencBufFull;
    HI_U32 VencPbitOverflow;
    HI_U32  Timer;
    HI_U32  IdleTimer;
#ifdef __VENC_S40V200_CONFIG__
    HI_U32  CrefldReqNum;
    HI_U32  CrefldDatNum;
#endif
    HI_U32  CfgErrIndex;
    HI_U32  IntraCloseCnt;
    HI_U32  FracCloseCnt;
    HI_U32  IntpSearchCnt;
    /* sequence rate control in */
    HI_U32 Gop;
    HI_U32 BitRate;
    HI_U32 ViFrmRate;
    HI_U32 VoFrmRate;
    HI_U32 VBRflag;    /* fixed now...todo */
    HI_U32 PicLevel;
    HI_U32  SkipFrameEn;                                                                      //
    //HI_U32 VBR_MinQp;
    HI_U32 MaxPPdelta;
    HI_U32 MaxIPdelta;

    /* frame rate control out */
    HI_U32      RcStart;
    HI_U32      TrCount;
    HI_U32      LastTR;
    HI_U32      InterFrmCnt;

    HI_U32 IntraPic;
    HI_U32 TargetBits;         /* targetBits of each frame */
    HI_U32 StartQp;
    HI_U32 MinQp;
    HI_U32 MaxQp;

    /* rate control mid */
    VALG_FIFO_S stBitsFifo;
    HI_U32      MeanBit;
	HI_U32      BitsFifo[FrmRateCntBufNum];   
    VeduEfl_Rc_S stRc;        //add by ckf77439  new version

    /* frame rate control mid */
    HI_U32      LastFrmRate[6];	  /*from the struction of Image*/
    HI_U32      LastSecInputFrmRate[2];   /* from Timer stat.*/
    HI_U32      LastSecFrameCnt;
    HI_U32      bFrmRateSataError;
    HI_S32  bMorePPS;
    HI_S32  ChrQpOffset;
    HI_S32  ConstIntra;
    HI_S32  CabacInitIdc;
    HI_S32  CabacStuffEn;

    HI_S32  DblkIdc;
    HI_S32  DblkAlpha;
    HI_S32  DblkBeta;

    HI_S32  IPCMPredEn;
    HI_S32  Intra4x4PredEn;
    HI_S32  Intra8x8PredEn;
    HI_S32  Intra16x16PredEn;
    HI_S32  I4ReducedModeEn;

    HI_S32  Inter8x8PredEn;
    HI_S32  Inter8x16PredEn;
    HI_S32  Inter16x8PredEn;
    HI_S32  Inter16x16PredEn;
    HI_S32  PskipEn;
    HI_S32  ExtedgeEn;
    HI_S32  TransMode;
    HI_S32  NumRefIndex;

    HI_S32  PixClipEn;
    HI_S32  LumaClipMin;
    HI_S32  LumaClipMax;
    HI_S32  ChrClipMin;
    HI_S32  ChrClipMax;


    HI_S32  HSWSize;
    HI_S32  VSWSize;
    HI_S32  RectMod   [6];
    HI_S32  RectWidth [6];
    HI_S32  RectHeight[6];
    HI_S32  RectHstep [6];
    HI_S32  RectVstep [6];
    HI_S32  StartThr1;
    HI_S32  StartThr2;
    HI_S32  IntraThr;
    HI_S32  LmdaOff16;
    HI_S32  LmdaOff1608;
    HI_S32  LmdaOff0816;
    HI_S32  LmdaOff8;
    HI_S32  CrefldBur8En;
    HI_S32  MdDelta;

#ifdef __VENC_S40V200_CONFIG__
    HI_S32  MctfStrength0; /* NA */
    HI_S32  MctfStrength1; /* NA */

    HI_S32  MctfStillEn;
    HI_S32  MctfMovEn;
    HI_S32  mctfStillMvThr;
    HI_S32  mctfRealMvThr;
    HI_S32  MctfStillCostThr;
    HI_S32  MctfLog2mctf;
    HI_S32  MctfLumaDiffThr;
    HI_S32  MctfChrDiffThr;
    HI_S32  MctfChrDeltaThr;
    HI_S32  MctfStiMadiThr1;
    HI_S32  MctfStiMadiThr2;
    HI_S32  MctfMovMadiThr;
    HI_S32  MctfMovMad1_m;
    HI_S32  MctfMovMad1_n;
    HI_S32  MctfMovMad2_m;
    HI_S32  MctfMovMad2_n;
    HI_S32  MctfStiMad1_m;
    HI_S32  MctfStiMad1_n;
    HI_S32  MctfStiMad2_m;
    HI_S32  MctfStiMad2_n;

#elif ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__) |(defined __VENC_98CV200_CONFIG__))

    HI_S32 MctfStillEn            ;
    HI_S32 MctfSmlmovEn           ;
    HI_S32 MctfBigmovEn           ;
    HI_S32 mctfStillMvThr         ;
    HI_S32 mctfRealMvThr          ;
    HI_S32 MctfStillCostThr       ;
    HI_S32 MctfStiLumaOrialpha    ;
    HI_S32 MctfSmlmovLumaOrialpha ;
    HI_S32 MctfBigmovLumaOrialpha ;
    HI_S32 MctfChrOrialpha        ;
    HI_S32 mctfStiLumaDiffThr0    ;
    HI_S32 mctfStiLumaDiffThr1    ;
    HI_S32 mctfStiLumaDiffThr2    ;
    HI_S32 mctfSmlmovLumaDiffThr0 ;
    HI_S32 mctfSmlmovLumaDiffThr1 ;
    HI_S32 mctfSmlmovLumaDiffThr2 ;
    HI_S32 mctfBigmovLumaDiffThr0 ;
    HI_S32 mctfBigmovLumaDiffThr1 ;
    HI_S32 mctfBigmovLumaDiffThr2 ;
    HI_S32 mctfStiLumaDiffK0      ;
    HI_S32 mctfStiLumaDiffK1      ;
    HI_S32 mctfStiLumaDiffK2      ;
    HI_S32 mctfSmlmovLumaDiffK0   ;
    HI_S32 mctfSmlmovLumaDiffK1   ;
    HI_S32 mctfSmlmovLumaDiffK2   ;
    HI_S32 mctfBigmovLumaDiffK0   ;
    HI_S32 mctfBigmovLumaDiffK1   ;
    HI_S32 mctfBigmovLumaDiffK2   ;
    HI_S32 mctfChrDiffThr0        ;
    HI_S32 mctfChrDiffThr1        ;
    HI_S32 mctfChrDiffThr2        ;
    HI_S32 mctfChrDiffK0          ;
    HI_S32 mctfChrDiffK1          ;
    HI_S32 mctfChrDiffK2          ;
    HI_S32 mctfOriRatio           ;
    HI_S32 mctfStiMaxRatio        ;
    HI_S32 mctfSmlmovMaxRatio     ;
    HI_S32 mctfBigmovMaxRatio     ;
    HI_S32 mctfStiMadiThr0        ;
    HI_S32 mctfStiMadiThr1        ;
    HI_S32 mctfStiMadiThr2        ;
    HI_S32 mctfSmlmovMadiThr0     ;
    HI_S32 mctfSmlmovMadiThr1     ;
    HI_S32 mctfSmlmovMadiThr2     ;
    HI_S32 mctfBigmovMadiThr0     ;
    HI_S32 mctfBigmovMadiThr1     ;
    HI_S32 mctfBigmovMadiThr2     ;
    HI_S32 mctfStiMadiK0          ;
    HI_S32 mctfStiMadiK1          ;
    HI_S32 mctfStiMadiK2          ;
    HI_S32 mctfSmlmovMadiK0       ;
    HI_S32 mctfSmlmovMadiK1       ;
    HI_S32 mctfSmlmovMadiK2       ;
    HI_S32 mctfBigmovMadiK0       ;
    HI_S32 mctfBigmovMadiK1       ;
    HI_S32 mctfBigmovMadiK2       ;
#endif

#if ((defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__)|(defined __VENC_98CV200_CONFIG__))
    //csc parameter
    HI_S32 csc_mode;  /*0: disable 1:601->709 2:709->601*/
    //HI_S32 csc_en;
    HI_S32 csc00;
    HI_S32 csc01;
    HI_S32 csc02;
    HI_S32 csc10;
    HI_S32 csc11;
    HI_S32 csc12;
    HI_S32 csc20;
    HI_S32 csc21;
    HI_S32 csc22;
    HI_S32 cscin0;
    HI_S32 cscin1;
    HI_S32 cscin2;
    HI_S32 cscout0;
    HI_S32 cscout1;
    HI_S32 cscout2;
    /****************************************  end  *****************************************/
#endif
#if (defined __VENC_98CV200_CONFIG__)  
   //add for smmu config
   HI_S32 ch00_rrmax;
   HI_S32 ch01_rrmax;
   HI_S32 ch02_rrmax;
   HI_S32 ch03_rrmax;
   HI_S32 ch04_rrmax;
   HI_S32 ch05_rrmax;
   HI_S32 ch06_rrmax;
   HI_S32 ch07_rrmax;
   HI_S32 ch08_rrmax;
   HI_S32 ch09_rrmax;
   HI_S32 ch10_rrmax;
   HI_S32 ch11_rrmax;
   HI_S32 ch12_rrmax;
   
   HI_S32 ch00_wrmax;
   HI_S32 ch01_wrmax;
   HI_S32 ch02_wrmax;
   HI_S32 ch03_wrmax;
   HI_S32 ch04_wrmax;
   HI_S32 ch05_wrmax;
   HI_S32 ch06_wrmax;
   HI_S32 ch07_wrmax;
   HI_S32 ch08_wrmax;
   HI_S32 ch09_wrmax;
   HI_S32 ch10_wrmax;
   HI_S32 ch11_wrmax;
   HI_S32 ch12_wrmax;
   HI_S32 ch13_wrmax;
   HI_S32 ch14_wrmax;   

   HI_S32 wtmax;   
   HI_S32 rtmax;   

   HI_S32 secure_en;

   HI_S32 vcpi_srcy_bypass;
   HI_S32 vcpi_srcc_bypass;
   HI_S32 vcpi_srcv_bypass;
   HI_S32 vcpi_refy_bypass;
   HI_S32 vcpi_refc_bypass;
   HI_S32 vcpi_rcny_bypass;
   HI_S32 vcpi_rcnc_bypass;
   HI_S32 vcpi_strm_bypass;

   //for smmu
   HI_S32 glb_bypass;
   HI_S32 int_en;
   //HI_S32 ptw_pf;
   
   HI_S32 auto_clk_gt_en;

#if 1  
   HI_S32 ints_tlbmiss_msk;           //安全源中断屏蔽
   HI_S32 ints_ptw_trans_msk;
   HI_S32 ints_tlbinvalid_msk;
   
   HI_S32 ints_tlbmiss_raw;            //安全源中断
   HI_S32 ints_ptw_trans_raw;
   HI_S32 ints_tlbinvalid_raw;
   
   HI_S32 ints_tlbmiss_stat;          //安全中断状态
   HI_S32 ints_ptw_trans_stat;
   HI_S32 ints_tlbinvalid_stat;
   
   HI_S32 ints_tlbmiss_clr;           //安全侧清中断
   HI_S32 ints_ptw_trans_clr;
   HI_S32 ints_tlbinvalid_clr;
   
   HI_S32 intns_tlbmiss_msk     ;      //非安全源中断屏蔽
   HI_S32 intns_ptw_trans_msk   ;
   HI_S32 intns_tlbinvalid_msk  ;
   
   HI_S32 intns_tlbmiss_raw     ;      //非 安全源中断
   HI_S32 intns_ptw_trans_raw   ;
   HI_S32 intns_tlbinvalid_raw  ;
   
   HI_S32 intns_tlbmiss_stat    ;       //非安全中断状态
   HI_S32 intns_ptw_trans_stat  ;
   HI_S32 intns_tlbinvalid_stat ;
   
   HI_S32 intns_tlbmiss_clr     ;        //非 安全侧清中断
   HI_S32 intns_ptw_trans_clr   ;
   HI_S32 intns_tlbinvalid_clr  ;
   
   HI_S32 fault_index_id        ;
   HI_S32 fault_strm_id         ;
   HI_S32 fault_index_id_wr     ;
   HI_S32 fault_str_id_wr       ;
   HI_S32 fault_index_id_rd     ;
   HI_S32 fault_str_id_rd       ;
   HI_S32 fault_tlbmiss_err     ;
   HI_S32 fault_tlbinvalid_err  ;
   HI_S32 fault_ptw             ;
   HI_S32 fault_tbu             ;
   HI_S32 fault_ptw_num         ;
   HI_S32 fault_tbu_num         ;
#endif

#endif


    HI_S32  StartQpType;
    HI_S32  RcQpDelta;
    HI_S32  RcMadpDelta;
    HI_S32  RcQpDeltaThr[12];
    HI_S32  ModLambda[40];
    HI_S32  Scale8x8[128];

    HI_S32  IntraLowpowEn;   /* register of low power work mode */
    HI_S32  fracLowpowEn;
    HI_S32  intpLowpowEn;
    HI_S32  fracRealMvThr;

    HI_S32  LowDlyMod;
    HI_U32  tunlReadIntvl;
    HI_U32  tunlcellAddr;
    HI_S32  RegLockEn;
    HI_S32  ClkGateEn;
    HI_S32  MemClkGateEn;
    HI_S32  TimeOutEn;
    HI_U32  TimeOut;
    HI_S32  PtBitsEn;
    HI_S32  PtBits;
	
    HI_S32  OutStdNum;

    //add for 98cv200
    HI_S32  wOutStdNum;
    HI_S32  rOutStdNum;
    //add end
    
    HI_S32  IMbNum;
    HI_S32  StartMb;
    VeduEfl_RoiAttr_S RoiCfg;
    VeduEfl_OsdAttr_S OsdCfg;

    /* attach vi or vo ,backup for vpss*/
    VeduEfl_SrcInfo_S     stSrcInfo;
    VeduEfl_SrcInfo_S     stSrcInfo_toVPSS;
    HI_DRV_VIDEO_FRAME_S  stImage;
    venc_user_buf         stImage_OMX;

    HI_U32                WithoutVPSS;
    /* statistic */
    VeduEfl_StatInfo_S stStat;

#if 0
    VALG_CRCL_QUE_BUF_S stCycQueBuf;      //add
#else
    queue_info_s* FrameDequeue;
    queue_info_s* FrameQueue;
#endif
    queue_info_s* MsgQueue_OMX;
    queue_info_s* StreamQueue_OMX;
    queue_info_s* FrameQueue_OMX;

    HI_U32 KernelVirAddr[6];
    VENC_BUFFER_S JpgMMZBuf;

#ifdef HI_SMMU_SUPPORT
    HI_U32 u32SmmuPageBaseAddr;
    HI_U32 u32SmmuErrWriteAddr;
	HI_U32 u32SmmuErrReadAddr;
#endif
} VeduEfl_EncPara_S;

typedef struct
{
    HI_U32 BusViY;      /* 16-byte aligned  */
    HI_U32 BusViC;      /* 16-byte aligned  */
    HI_U32 BusViV;
    HI_U32 ViYStride;   /* 16-byte aligned  */
    HI_U32 ViCStride;   /* 16-byte aligned  */

    HI_U32 PTS0;
    HI_U32 PTS1;
    //ADD
    HI_U32   RStrideY;      /* 16-byte aligned  */
    HI_U32   RStrideC;      /* 16-byte aligned  */
    HI_U32   TunlCellAddr;
} VeduEfl_EncIn_S;

typedef struct
{
    VeduEfl_EncPara_S* EncHandle;
} VeduEfl_ChnCtx_S;

typedef struct
{
    HI_U32   IpFree;       /* for channel control */
    VeduEfl_EncPara_S*   CurrHandle;   /* used in ISR */
    HI_U32  *pRegBase;
    HI_VOID *pChnLock;     /* lock ChnCtx[MAX_CHN] */
    //HI_VOID *pTask;

    HI_VOID *pTask_Frame;        //for both venc & omxvenc
    HI_VOID *pTask_Stream;       //juse for omxvenc
    HI_U32   StopTask;
    HI_U32   TaskRunning;  /* to block Close IP */
    HI_S32   CurrFreq;     /* for record the current frequency*/
} VeduEfl_IpCtx_S;

HI_S32	VENC_DRV_EflOpenVedu( HI_VOID );
HI_VOID	VENC_DRV_EflCloseVedu( HI_VOID );
HI_S32	VENC_DRV_EflCreateVenc( VeduEfl_EncPara_S** pEncHandle, VeduEfl_EncCfg_S* pEncCfg );
HI_S32	VENC_DRV_EflDestroyVenc( VeduEfl_EncPara_S* EncHandle );
HI_S32	VENC_DRV_EflAttachInput( VeduEfl_EncPara_S* EncHandle, VeduEfl_SrcInfo_S* pSrcInfo );
HI_S32	VENC_DRV_EflDetachInput( VeduEfl_EncPara_S* EncHandle, VeduEfl_SrcInfo_S* pSrcInfo );
HI_S32	VENC_DRV_EflStartVenc( VeduEfl_EncPara_S* EncHandle );
HI_S32	VENC_DRV_EflStopVenc( VeduEfl_EncPara_S* EncHandle );
HI_S32	VENC_DRV_EflRcGetAttr( VeduEfl_EncPara_S* EncHandle, VeduEfl_RcAttr_S* pRcAttr );
HI_S32	VENC_DRV_EflRcAttrInit( VeduEfl_EncPara_S* EncHandle, VeduEfl_RcAttr_S* pRcAttr );
HI_S32  VENC_DRV_EflRcSetAttr( VeduEfl_EncPara_S* EncHandle, VeduEfl_RcAttr_S* pRcAttr );
HI_S32	VENC_DRV_EflRcFrmRateCtrl( VeduEfl_EncPara_S* EncHandle, HI_U32 TR );
HI_S32	VENC_DRV_EflRcOpenOneFrm( VeduEfl_EncPara_S* EncHandle );
HI_S32	VENC_DRV_EflRcOpenOneFrm_2( VeduEfl_EncPara_S* EncHandle );
HI_S32	VENC_DRV_EflRcCloseOneFrm( VeduEfl_EncPara_S* EncHandle );
HI_S32	VENC_DRV_EflRcCloseOneFrm_2( VeduEfl_EncPara_S* EncHandle );
HI_S32	VENC_DRV_EflStartOneFrameVenc( VeduEfl_EncPara_S* EncHandle, VeduEfl_EncIn_S* pEncIn );
HI_S32	VENC_DRV_EflEndOneFrameVenc( VeduEfl_EncPara_S* EncHandle );
HI_VOID	VENC_DRV_EflRequestIframe( VeduEfl_EncPara_S* EncHandle );
HI_S32	VENC_DRV_EflSetResolution( VeduEfl_EncPara_S* EncHandle, HI_U32 FrameWidth, HI_U32 FrameHeight );
HI_S32	VENC_DRV_EflResumeVedu(HI_VOID);
HI_S32	VENC_DRV_EflGetBitStream( VeduEfl_EncPara_S* EncHandle, VeduEfl_NALU_S* pNalu );
HI_S32	VENC_DRV_EflSkpBitStream( VeduEfl_EncPara_S* EncHandle, VeduEfl_NALU_S* pNalu );
HI_S32	VENC_DRV_EflQueryStatInfo( VeduEfl_EncPara_S* EncHandle, VeduEfl_StatInfo_S* pStatInfo );
HI_S32	VENC_DRV_EflEncodeFrame(HI_VOID);
HI_U32	VENC_DRV_EflGetStreamLen( VeduEfl_EncPara_S* EncHandle );
HI_S32  VENC_DRV_EflQueueFrame( VeduEfl_EncPara_S* EncHandle, HI_DRV_VIDEO_FRAME_S*  pstFrame);
HI_S32  VENC_DRV_EflDequeueFrame( VeduEfl_EncPara_S* EncHandle, HI_DRV_VIDEO_FRAME_S*  pstFrame);
HI_S32  VENC_DRV_EflGetImage(HI_S32 EncUsrHandle, HI_DRV_VIDEO_FRAME_S*  pstFrame);
HI_S32  VENC_DRV_EflGetImage_OMX(HI_S32 EncUsrHandle, venc_user_buf* pstFrame);
HI_S32  VENC_DRV_EflPutImage(HI_S32 EncUsrHandle, HI_DRV_VIDEO_FRAME_S*  pstFrame);

HI_S32  VENC_DRV_EflCfgRegVenc( VeduEfl_EncPara_S* EncHandle );
HI_VOID VENC_DRV_EflSortPriority(HI_VOID);

HI_BOOL VENC_DRV_EflJudgeVPSS( VeduEfl_EncPara_S* EncHandle, HI_DRV_VIDEO_FRAME_S* pstFrameInfo);   //调用在获得帧信息后
HI_VOID VENC_DRV_EflWakeUpThread(HI_VOID);

HI_S32 VENC_DRV_EflQFrameByAttach( HI_U32 EncUsrHandle, HI_DRV_VIDEO_FRAME_S*  pstFrame);
HI_S32 VENC_DRV_EflRlsAllFrame( VeduEfl_EncPara_S* EncHandle);
HI_S32 VENC_DRV_Refresh_FromVPSS( VeduEfl_EncPara_S* EncHandle );
	
HI_VOID VENC_DRV_EflSceChaDetect(VeduEfl_Rc_S *pRc, HI_S32 w, HI_S32 h, HI_S32 IntraPic);
HI_S32  VEDU_DRV_EflRcInitQp(HI_S32 bits,HI_S32 w,HI_S32 h);
HI_S32  VENC_DRV_EflRcAverage(HI_S32 *pData,HI_S32 n);
HI_VOID VENC_DRV_EflRcCalIFrmQp(VeduEfl_Rc_S *pRc, HI_S32 w, HI_S32 h, HI_S32 times, HI_S32 GopLength,int FrameIMBRatio);
HI_VOID VENC_DRV_EflRcCalPFrmQp(VeduEfl_Rc_S* pRc, VeduEfl_EncPara_S* pEncPara, HI_S32 FrameIMBRatio);

HI_S32 VENC_DRV_DbgWriteYUV(HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR* pFileName);
HI_VOID VENC_DRV_EflSuspendVedu( HI_VOID );
HI_S32 VENC_DRV_EflFlushStrmHeader( VeduEfl_EncPara_S  *pEncPara );

HI_S32 VENC_DRV_EflAllocBuf2Jpge( VeduEfl_EncPara_S* EncHandle, HI_U32 CapLevel);
HI_S32 VENC_DRV_EflFreeBuf2Jpge( VeduEfl_EncPara_S* EncHandle );
	
/*************************************************************************************/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif //__DRV_VENC_EFL_H__
