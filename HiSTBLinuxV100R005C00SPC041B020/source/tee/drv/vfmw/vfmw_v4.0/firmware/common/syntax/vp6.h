/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv/JzwTgKEHe/9N4yU5rqhEDua6TPiV0wKx6HFum6IuayljWKzO13FGopUUsk
T+OY0FJkLlUiGGBlOVVEof3965uoi8kpUgJ0Yd0GyA8rRuzNHhLkw4Vjz8/mUBT4jVkB9mu3
T9U12Jp1W6xVVsbyX8rLwXKx5TCg+BJ6TPW93skjGPid32Yyz50MRvx1ZIVPsA==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/***********************************************************************
*
* Copyright (c) 2009 HUAWEI - All Rights Reserved
*
* File: $vp6_vfmw.h$
* Date: $2009/06/18$
* Revision: $v1.0$
* Purpose: VP6 decoder header file
*
*
* Change History:
*
* Date             Author            Change
* ====             ======            ====== 
* 2010/07/27       l48485            Original
*
* Dependencies:
*
************************************************************************/

#ifndef __VP6_HEADER__
#define __VP6_HEADER__

#include "basedef.h"
#include "public.h"
#include "vfmw.h"
#include "bitstream.h"
#include "syn_cmn.h"
#ifdef VP6_ENABLE

#define HUFF_LUT_LEVELS   6
#define CURRENT_DECODE_VERSION  8
#define SIMPLE_PROFILE          0
#define BASE_FRAME              0
#define NORMAL_FRAME            1
#define NVOP_FRAME				3

#define BLOCK_HEIGHT_WIDTH      8
#define VP6_BLOCK_SIZE              (BLOCK_HEIGHT_WIDTH * BLOCK_HEIGHT_WIDTH)
#define MODETYPES       3  //3
#define MODEVECTORS     16
#define PROBVECTORXMIT  174
#define PROBIDEALXMIT   254
#define MV_NODES        17

// VP6 hufman table AC bands
#define VP6_AC_BANDS                    6
#define MAX_ENTROPY_TOKENS              (DCT_EOB_TOKEN + 1)  

#define DC_TOKEN_CONTEXTS               3 // 00, 0!0, !0!0
#define CONTEXT_NODES                   (MAX_ENTROPY_TOKENS-7)
#define PREC_CASES                      3
#define ZERO_RUN_PROB_CASES             14 

#define PROB_UPDATE_BASELINE_COST       7
#define ZRL_BANDS                       2
#define SCAN_ORDER_BANDS                16
#define SCAN_BAND_UPDATE_BITS           4

#define LONG_MV_BITS                    8
#define MAX_MODES                       10

//tokens                                value    extra bits(range+sign)
#define ZERO_TOKEN                      0               //0             Extra Bits 0+0
#define ONE_TOKEN                       1               //1             Extra Bits 0+1       
#define TWO_TOKEN                       2               //2             Extra Bits 0+1 
#define THREE_TOKEN                     3               //3             Extra Bits 0+1
#define FOUR_TOKEN                      4               //4             Extra Bits 0+1
#define DCT_VAL_CATEGORY1               5               //5-6           Extra Bits 1+1
#define DCT_VAL_CATEGORY2               6               //7-10          Extra Bits 2+1
#define DCT_VAL_CATEGORY3               7               //11-26         Extra Bits 4+1
#define DCT_VAL_CATEGORY4               8               //11-26         Extra Bits 5+1
#define DCT_VAL_CATEGORY5               9               //27-58         Extra Bits 5+1
#define DCT_VAL_CATEGORY6               10              //59+           Extra Bits 11+1 
#define DCT_EOB_TOKEN                   11              //EOB           Extra Bits 0+0


#define PROB_MODE_SAME_TBL_LEN			(4 * 16)   //64 bytes
#define PROB_MODE_TBL_LEN				(4 * MAX_MODES * 16) //480bytes
#define PROB_MV_TBL_LEN					(2 * 2 * 16) 	//64
#define MV_MODIFIED_SCAN_ORDER_TBL_LEN	(VP6_BLOCK_SIZE)	//64
#define EOB_OFFSET_TBL_LEN				(VP6_BLOCK_SIZE)	//64
#define HUFF_TREE_LEN       			(0x3C0)
#define	DC_NODE_CONTEXTS_TBL_LEN		(2 * DC_TOKEN_CONTEXTS * 16) //96
#define PROB_AC_TBL_LEN					(2 * PREC_CASES * VP6_AC_BANDS * 16) //576
#define PROB_ZERO_TBL_LEN				(ZRL_BANDS * 16)	//32

#define DC_HUFF_LUT_TBL_LEN				(2 * (1<<HUFF_LUT_LEVELS)) //128
#define ZERO_HUFF_LUT_TBL_LEN			(ZRL_BANDS * (1<<HUFF_LUT_LEVELS)) //128
#define	AC_HUFF_LUT_TBL_LEN				(2 * PREC_CASES * 4 * (1<<HUFF_LUT_LEVELS))

#define DCProbOffset(A,B) \
        ( (A) * (MAX_ENTROPY_TOKENS-1) \
        + (B) )

#define ACProbOffset(A,B,C,D) \
        ( (A) * PREC_CASES * VP6_AC_BANDS * (MAX_ENTROPY_TOKENS-1) \
        + (B) * VP6_AC_BANDS * (MAX_ENTROPY_TOKENS-1) \
        + (C) * (MAX_ENTROPY_TOKENS-1) \
        + (D) ) 

#define DcNodeOffset(A,B,C) \
        ( (A) * DC_TOKEN_CONTEXTS * 11 \
        + (B) * 11 \
        + (C) ) 

#define AcHuffProbOffset(A,B,C,D) \
		( (A) * PREC_CASES * VP6_AC_BANDS * (MAX_ENTROPY_TOKENS) \
        + (B) * VP6_AC_BANDS * (MAX_ENTROPY_TOKENS) \
        + (C) * (MAX_ENTROPY_TOKENS) \
        + (D) ) 
        
#define AcHuffLUTOffset(A,B,C,D) \
		( (A) * PREC_CASES * VP6_AC_BANDS * (1<<HUFF_LUT_LEVELS) \
        + (B) * VP6_AC_BANDS * (1<<HUFF_LUT_LEVELS) \
        + (C) * (1<<HUFF_LUT_LEVELS) \
        + (D) ) 

// Prediction filter modes:
// Note: when trying to use an enum here we ran into an odd compiler bug in
// the WriteFrameHeader() code. Also an enum type is implicitly an int which 
// is a bit big for something that can only have 3 values
#ifndef  VP6_VFMW_BIG_ENDIAN
#define  VP6_VFMW_ENDIAN32(x)   \
    ((x) << 24) |               \
    (((x) & 0x0000ff00) << 8) | \
    (((x) & 0x00ff0000) >> 8) | \
    (((x) >> 24) & 0x000000ff)
#else
#define  VP6_VFMW_ENDIAN32(x) (x)
#endif

typedef struct LineEq
{
    SINT32      M;
    SINT32      C;
} LINE_EQ;

typedef enum 
{
    VP6_VFMW_FALSE = 0,
    VP6_VFMW_TRUE  = 1
} VP6_TRUEFALSE;

typedef enum 
{
    VP6_INTRAPIC    = 0,
    VP6_INTERPIC    = 1
} VP6_PicCodType;

typedef enum
{
    VP6_CODE_INTER_NO_MV        = 0x0,      
    VP6_CODE_INTRA              = 0x1,       
    VP6_CODE_INTER_PLUS_MV      = 0x2,       
    VP6_CODE_INTER_NEAREST_MV   = 0x3,      
    VP6_CODE_INTER_NEAR_MV      = 0x4,      
    VP6_CODE_USING_GOLDEN       = 0x5,      
    VP6_CODE_GOLDEN_MV          = 0x6,      
    VP6_CODE_INTER_FOURMV       = 0x7,       
    VP6_CODE_GOLD_NEAREST_MV    = 0x8,       
    VP6_CODE_GOLD_NEAR_MV       = 0x9,      
    VP6_DO_NOT_CODE             = 0x10       
} VP6_MBCodMode_E;

typedef enum
{
    VP6_BILINEAR_ONLY_PM        = 0,      
    VP6_BICUBIC_ONLY_PM         = 1,       
    VP6_AUTO_SELECT_PM          = 2
} VP6_PredFilterMode_E;


typedef struct  
{
    UINT8 selector :1;   //1->token,0->index
    UINT8 value    :4;   
    UINT8 unused   :3;
} VP6_TOKENORPTR_S;

typedef struct
{
    VP6_TOKENORPTR_S value;
    UINT32 next;
    UINT32 freq;
} VP6_SORTNODE_S;

typedef struct  
{
    union
    {
        UINT8 l;
        VP6_TOKENORPTR_S left;
    } leftunion;
    
    union
    {
        UINT8 r;
        VP6_TOKENORPTR_S right;
    } rightunion;
} VP6_HUFFNODE_S;


typedef struct  
{
	UINT8 flag   :1;
    UINT8 length :3;
    UINT8 value  :4;    //for value is from 0 - e,range is 12 ,so only need 4 bit actually,   
} VP6_HUFFTABNODE_S;


typedef struct
{
    // The size of the surface we want to draw to
    UINT32 VideoFrameWidth;
    UINT32 VideoFrameHeight;

    SINT32 YStride;
    SINT32 UVStride;

    // The number of horizontal and vertical blocks encoded
    UINT32 HFragPixels;
    UINT32 VFragPixels;

    // The Intended Horizontal Scale
    UINT32 HScale;
    UINT32 HRatio;

    // The Intended Vertical Scale
    UINT32 VScale;
    UINT32 VRatio;

    // The way in which we intended
    UINT32 ScalingMode;
    UINT32 reserved;
    UINT32 ExpandedFrameWidth;
    UINT32 ExpandedFrameHeight;
} VP6_ScaleType;

// Frame Header type
typedef struct 
{
    UINT32 lowvalue;
    UINT32 range;
    UINT32 value;
    UINT32 count;
} VP6_BOOLCODER_S;


typedef struct
{
    UINT8   DctQMask;
    UINT8   FLimit;
    UINT8   MultiStream;
    UINT8   ScalingMode;
    UINT8   PredictionFilterVarThresh;
    UINT8   PredictionFilterMvSizeThresh;
    UINT8   PredictionFilterAlpha;
    UINT16  DcQuant;
    UINT16  AcQuant;

    UINT32  Vp3VersionNo;
    UINT32  VpProfile;

    UINT32  FrameType;
	
    UINT32  pic_height_in_mb;
    UINT32  pic_width_in_mb;
	
    UINT32  VMacroblockCount;
    UINT32  HMacroblockCount;
	
    UINT32  OutputVMacroblockCount;
    UINT32  OutputHMacroblockCount;

    UINT32  LastFrameQIndex;
    
    UINT32  Buff2Offset;
    UINT32  PredictionFilterMode;
    UINT32  RefreshGoldenFrame;

    UINT32  UseLoopFilter;       
    UINT32  UseHuffman;

    VP6_BOOLCODER_S  br;
    VP6_ScaleType    Configuration;
} VP6_PictureHeader_S;

typedef struct
{
    UINT8  buf_state; //0:available to VDH write; 1: available to VO read
    UINT8  ref_flag;
    IMAGE  imginf;
    UINT32 err_level;
} VP6_VOBUFINF;


typedef struct
{	
    /*huffman table*/
    UINT8 MvSignProbs[2];
    UINT8 IsMvShortProb[2];
    UINT8 probModeSame[4][MAX_MODES];
    UINT8 probMode[4][MAX_MODES][MAX_MODES-1]; 
    UINT8 probXmitted[4][2][MAX_MODES];
    
    UINT8 MvShortProbs[2][7];
    UINT8 MvSizeProbs[2][LONG_MV_BITS];
    
    UINT8 DcProbs[2*(MAX_ENTROPY_TOKENS-1)];
    UINT8 ZeroRunProbs[ZRL_BANDS][ZERO_RUN_PROB_CASES];
    UINT8 AcProbs[2*PREC_CASES*VP6_AC_BANDS*(MAX_ENTROPY_TOKENS-1)];
    
    UINT8 ModifiedScanOrder[VP6_BLOCK_SIZE];
    UINT8 EobOffsetTable[VP6_BLOCK_SIZE];
    UINT8 ScanBands[VP6_BLOCK_SIZE];
    UINT8 DcNodeContexts[2 * DC_TOKEN_CONTEXTS * 11];               

    UINT8 TreeTab[0x400];

	UINT8 	TblProbModeSame[PROB_MODE_SAME_TBL_LEN];
	UINT8 	TblProbMode[PROB_MODE_TBL_LEN];
	UINT8 	TblProbMv[PROB_MV_TBL_LEN];  //include IsMvShort,MvSign,Mvshort,Mvlong. 
	
	UINT8	TblModifiedScanOrder[MV_MODIFIED_SCAN_ORDER_TBL_LEN];
	UINT8	TblEobOffsetTable[EOB_OFFSET_TBL_LEN];
	UINT8	TblHuffTree[HUFF_TREE_LEN];
	
	UINT8	TblDcNodeContexts[DC_NODE_CONTEXTS_TBL_LEN];   //used for boolcoder
	UINT8   TblACProbs[PROB_AC_TBL_LEN];
	UINT8   TblZeroProbs[PROB_ZERO_TBL_LEN];
		
	UINT8 	TblDcHuffLUT[DC_HUFF_LUT_TBL_LEN];
	UINT8 	TblAcHuffLUT[AC_HUFF_LUT_TBL_LEN];
	UINT8 	TblZeroHuffLUT[ZERO_HUFF_LUT_TBL_LEN];

    UINT32 DcHuffProbs[2][12]; 
    UINT32 ZeroHuffProbs[2][14];
    UINT32 AcHuffProbs[2][3][6][12];   
    
    VP6_HUFFNODE_S TblZeroHuffTree[2][14];		
    VP6_HUFFNODE_S TblAcHuffTree[2][3][6][12];
    VP6_HUFFNODE_S TblDcHuffTree[2][12]; 
    
    VP6_HUFFTABNODE_S DcHuffLUT[2][64];
    VP6_HUFFTABNODE_S AcHuffLUT[2][3][6][64];
    VP6_HUFFTABNODE_S ZeroHuffLUT[2][64];	
     
    UINT32 HOldScaled;       
    UINT32 VOldScaled;       
    
    UINT32 UsedDecMode;      //当前正在使用的解码模式(IPB, IP, I) 	
    UINT32 RefNum;
    UINT32 Imgbuf_allocated;

	UINT8 	FrameType;
	UINT32  RefreshGoldenFrame;	
	SINT32  CurFsID;
	SINT32  GoldenFsID;
	SINT32  FwdFsID;
	SINT32  ToQueFsID;	
    UINT32  DDR_Stride;	
	VP6_PictureHeader_S OldHeaderInfo;
	VP6_PictureHeader_S NewHeaderInfo;
} VP6_CODECINFO;


typedef struct
{
    SYNTAX_EXTRA_DATA_S *pstExtraData;
    VP6_DEC_PARAM_S    *pVp6DecParam;
    VP6_DEC_PARAM_S    Vp6DecParam;
	STREAM_PARAM	   *pVp6StreamParam;
	STREAM_PARAM	   Vp6StreamParam;
    VP6_CODECINFO       Vp6CodecInfo;
    BS                 Bs;
    IMAGE_VO_QUEUE     ImageQue;
    SINT32        	   ChanID;
} VP6_CTX_S;


#ifdef __cplusplus
extern "C" {
#endif

SINT32 VP6DEC_Init(VP6_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
SINT32 VP6DEC_Destroy(VP6_CTX_S *pCtx);
SINT32 VP6DEC_Decode(VP6_CTX_S *pCtx, DEC_STREAM_PACKET_S *pPacket);
SINT32 VP6DEC_RecycleImage(VP6_CTX_S *pCtx, UINT32 ImgID);
SINT32 VP6DEC_GetRemainImg(VP6_CTX_S *pCtx);
SINT32 VP6DEC_VDMPostProc(VP6_CTX_S *pCtx, SINT32 ErrRatio);
SINT32 VP6DEC_GetImageBuffer(VP6_CTX_S *pCtx);

#ifdef __cplusplus
}
#endif

#endif
#endif //__VP6_H__
