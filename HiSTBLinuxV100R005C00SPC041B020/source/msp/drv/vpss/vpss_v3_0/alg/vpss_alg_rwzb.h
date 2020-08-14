#ifndef __VPSS_ALG_RWZB_H__
#define __VPSS_ALG_RWZB_H__

#include "vpss_common.h"

#define PIX_NUM_IN_PATTERN 8
#define MAXSAMPNUM 6 

#define PIX_MISMATCH_THR   2
#define PAT_MISMATCH_THR   2

typedef enum
{
    PAT_UNKNOWN = 0,
    PAT_CCITT033,  //CVBS 1
    PAT_CCITT18,   //CVBS 2
    PAT_M576i,     //YPbPr 3
    PAT_480,       //YPbPr 4
    PAT_720P50,    //YPbPr 5
    PAT_720P59,    //YPbPr 6
    PAT_1080,      //YPbPr 7
    PAT_CBAR576_75,//CVBS 8
    PAT_CCIR3311,   //CVBS 9
    PAT_MATRIX625,   //CVBS 10
    PAT_CBAR576_75_B, //YPbPr 11
    PAT_CBAR576_100, //YPbPr 12
    PAT_M576I_PULSEBAR, //YPbPr 13
    PAT_M576I_REDFIELD, //YPbPr 14
    PAT_M576I_DUOBOQUN, //YPbPr 15
    PAT_M576I_5TAIJIE, //YPbPr 16
    PAT_M576I_XIEBO, //YPbPr 17
    PAT_M576I_BOWTIE, //YPbPr 18
    PAT_CBARS100, //scrat 19
    PAT_CCITT033_NTSC, //CVBS 20
    PAT_ITS1_NTSC,     //CVBS 21
    PAT_CODEC43,     //CVBS-ntsc
    PAT_MATRX525,     //CVBS-ntsc
    PAT_CBAR704_576,  //CVBS_PAL
    PAT_M480I_NTSC,  //moto cvbs ntsc
    PAT_M576I_PAL, //CVBS_PAL
    PAT_M480P_NTSC, //CVBS_NTSC
    PAT_M480P_SHALLOW_NTSC, //CVBS_NTSC  
    PAT_M480P_CBWFRC_NTSC, //CVBS_NTSC	
    PAT_M480P_SHALLOW_PAL, //CVBS_PAL 
    PAT_M480I_50_FLAT_FIELD, //moto CVBS  50% flat field ntsc
    PAT_M480I_75_COLORBARS, //moto CVBS  75% color bars ntsc
    PAT_M480I_NTC7_COMBINATION,     //moto cvbs NTC7 Combination ntsc
    PAT_M480I_NTC7_COMPOSITE,     //moto cvbs NTC7 Composite ntsc
    PAT_M480I_TSG130A_MATRIX_10MBPS,     //moto cvbs TSG130A-Matrix-10Mbps ntsc
    PAT_M480I_NTSC_MATRIX,  //moto cvbs ntsc matrix
} DNR_PAT_E;

typedef struct hi_DET_PAT_S
{
    HI_S32 Comp; //0,1,2 presents y,u,v
    HI_S32 Px;
    HI_S32 Py;
    HI_U8 Pix[PIX_NUM_IN_PATTERN];
}DET_PAT_S;


typedef struct hi_DET_SAMP_S
{
    DNR_PAT_E type; 
    HI_U8  samplingNumb;
    HI_S32 height;
    HI_S32 width;
    DET_PAT_S sample[MAXSAMPNUM];
}DET_SAMP_S;

typedef struct hi_DET_SAMPNODE_S
{
    DET_SAMP_S *pSamp;
    LIST list;
}DET_SAMPNODE_S;

typedef struct hi_DET_INFO_S
{
    LIST  nowRWZB;
    LIST  *target;
    HI_U32 btFlag;
    HI_U32 isRWZB;
}DET_INFO_S;


typedef struct hiVPSS_RWZB_INFO_S{
    HI_U32 u32Height;
    HI_U32 u32Width;
    HI_U32 u32EnRwzb; 
    HI_U32 u32Mode; /*0 : Y  1: C*/
    HI_U32 u32Addr[6][2];
    HI_U8 u8Data[6][8];
	HI_U32 u32IsRwzb;
}VPSS_RWZB_INFO_S;



/*****************************************************************************
                                  RWZB
*****************************************************************************/
HI_S32 ALG_DetPic(DET_INFO_S *pDetInfo,
                     VPSS_RWZB_INFO_S *pstRwzbCfg);
                     
HI_S32 ALG_InitRwzbInfo(DET_INFO_S *pDetInfo);
HI_S32 ALG_DeInitRwzbInfo(DET_INFO_S *pDetInfo);


HI_VOID DetPic_Compare_Pattern(DET_INFO_S *pDetInfo,VPSS_RWZB_INFO_S *pstRwzbCfg);
HI_VOID DetPic_Config_Addr(DET_INFO_S *pDetInfo,VPSS_RWZB_INFO_S *pstRwzbCfg);
HI_S32 Init_RWZB(DET_INFO_S *pDetInfo, HI_U32 height, HI_U32 width);
HI_S32 Free_RWZB(DET_INFO_S *pDetInfo);


#endif
