/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file si_drv_tpg.c
*
* @brief TPG driver
*
*****************************************************************************/
//#define SII_DEBUG

/***** #include statements ***************************************************/

//#include <string.h>
//#include <math.h>

#include "si_datatypes.h"
#include "si_drv_tpg_api.h"
#include "si_lib_obj_api.h"
#include "si_lib_seq_api.h"
#include "si_drv_cra_api.h"
#include "si_lib_video_api.h"
#include "si_drv_vtg_api.h"
#include "si_drv_pll_vo_api.h"

/***** Register Module name **************************************************/

SII_LIB_OBJ_MODULE_DEF(drv_tpg);

/***** local macro definitions ***********************************************/

#define REG_ENABLE(val)                        ( SiiDrvCraPutBit8(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x00, 0x01<<0, (val)<<0) )
#define REG_OVERSAMP(val)                      ( SiiDrvCraPutBit8(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x00, 0x03<<1, (val)<<1) )
#define REG_SOLID(val)                         ( SiiDrvCraPutBit8(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x00, 0x01<<3, (val)<<3) )
#define REG_ADDRESS(val)                       ( SiiDrvCraWrReg16(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x08, (val)&0xFFFF)      )
#define REG_LINEGEN(val)                       ( SiiDrvCraWrReg24(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x0C, (val)&0xFFFFFF)    )
#define REG_LINEMAP(val)                       ( SiiDrvCraWrReg8( pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x10, (val)&0xFF)        )
#define REG_COLOR_Y(val)                       ( SiiDrvCraWrReg16(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x12, (val)&0x0FFF)      )
#define REG_COLOR_CB(val)                      ( SiiDrvCraWrReg16(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x14, (val)&0x0FFF)      )
#define REG_COLOR_CR(val)                      ( SiiDrvCraWrReg16(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x16, (val)&0x0FFF)      )
#define REG_JUDDER_INCREMENT(val)              ( SiiDrvCraPutBit8(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x01, 0x0F<<0, (val)<<0) )
#define REG_JUDDER_DECREMENT(val)              ( SiiDrvCraPutBit8(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x01, 0x0F<<4, (val)<<4) )
#define REG_JUDDER_REVCOUNT(val)               ( SiiDrvCraWrReg16(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x04, (val)&0xFFFF)      )
#define REG_JUDDER_BARWIDTH(val)               ( SiiDrvCraWrReg8( pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x06, (val)&0xFF)        )
#define REG_JUDDER_RESTART()                   ( SiiDrvCraWrReg8( pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x07, (0)&0xFF)          )
#define REG_CFG_ENABLE(val)                    ( SiiDrvCraPutBit8(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x00, 0x01<<0, (val)<<0) )
#define REG_CFG_SOLID_COLOR_ENABLE(val)        ( SiiDrvCraPutBit8(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x00, 0x01<<3, (val)<<3) )
#define REG_SOLID_COLOR_Y(val)                 ( SiiDrvCraWrReg16(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x18, (val)&0x0FFF)      )
#define REG_SOLID_COLOR_CB(val)                ( SiiDrvCraWrReg16(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x1A, (val)&0x0FFF)      )
#define REG_SOLID_COLOR_CR(val)                ( SiiDrvCraWrReg16(pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x1C, (val)&0x0FFF)      )
#define REG_FIX_PATTERN(val)                   ( SiiDrvCraWrReg8( pObj->pConfig->instTxCra,  pObj->pConfig->baseAddr+0x1E, (val)&0x0F)        )

#define LINEFLAG_JUDDER                        0x0010
#define LINEFLAG_TRANSP                        0x0020
#define LINEFLAG_GREYVAL                       0x0040

#define PI                                     SII_DEF_PI

#define BLANK_SCRN_COLOR                       sClr_Black
#define BLUE_SCRN_COLOR                        sClr_Blue1

#define LUMAGEN_BITDEPTH  12   /* bit */

#define TPG_BANK_SELECT_BIT         			3
#define TPG_INDEX_NUM_OF_BITS                   4

/***** local type definitions ************************************************/

typedef struct
{
	SiiDrvTpgConfig_t*      pConfig;
	SiiLibVideoTiming_t     timing;
	SiiDrvTpgMode_t         mode;
	SiiDrvTpgPattern_t      pattern;
	float                   fBarPos;
	uint16_t                absPos;
	uint8_t                 barWidth;
	SiiLibVideoClrSpc_t     clrSpc;
	SiiLibVideoAspRat_t     fPictAspRat;
	bool_t                  bFullRange;
	uint8_t                 upSamp;
	float                   fAlphaBS;
	SiiInst_t               seqInst;
	SiiInst_t               vtgInst;
	SiiInst_t				instTxCra;
} TpgObj_t;

/***** local prototypes ******************************************************/

static void sUpdateMode( TpgObj_t* pObj );
static void sUpdatePattern( TpgObj_t* pObj );
//static void sUpdateDemoBarPosition( void );
//static void sUpdateDemoBar( void );
static void sUpdateBlankScreenColor( TpgObj_t* pObj );
static void sBlack2BlueScreenHandler( SiiInst_t inst );
static void sBlankEnable( TpgObj_t* pObj );
static void sBlankDisable( TpgObj_t* pObj );
static void sRgb2Yc(TpgObj_t* pObj, SiiLibVideoYUV_t* pYuv, const SiiLibVideoRGB_t* pRgb );
static void sSetLumaLut(TpgObj_t* pObj, uint8_t indx, uint16_t luma );
static void sSetChromaLut(TpgObj_t* pObj, uint8_t indx, uint16_t cb, uint16_t cr );
static void sSetColorLut( TpgObj_t* pObj, uint8_t indx, const SiiLibVideoRGB_t* pRgb );
static void sSetLineGen(TpgObj_t* pObj, uint8_t line, uint16_t a, uint16_t b );
static void sSetLineColor8Bars100( TpgObj_t* pObj );
static void sSetCheckerBoard( TpgObj_t* pObj );
static void sSetColor8Bars100( TpgObj_t* pObj );
static void sSet256GrayRamp( TpgObj_t* pObj );
static void sSetSimp92( TpgObj_t* pObj );
static void sInt2Float( SiiLibVideoRGB_t* pRgb );
static void sSetFrmGeomPattern( TpgObj_t* pObj );
static void sSetFlexWindowPattern( TpgObj_t* pObj, float fRed, float fGreen, float fBlue, float fSize );
//static void sSetFullTransparent( void );

/***** local data objects ****************************************************/

/*                                             R,       G,       B       */
//static  SiiLibVideoRGB_t sClr_Curtains  = {(float) 0.06256, (float)0.06256, (float)0.06256 };
static  SiiLibVideoRGB_t sClr_UltraBlk  = {-1.00,   -1.00,   -1.00    };
static  SiiLibVideoRGB_t sClr_Black     = { 0.00,    0.00,    0.00    };
static  SiiLibVideoRGB_t sClr_White1    = { 0.75,    0.75,    0.75    };
static  SiiLibVideoRGB_t sClr_White2    = { 1.00,    1.00,    1.00    };
static  SiiLibVideoRGB_t sClr_Gray1     = {(float) 0.21896, (float)0.21896, (float)0.21896 };
//static  SiiLibVideoRGB_t sClr_Gray2     = { 0.00,    0.00,    0.00    };
//static  SiiLibVideoRGB_t sClr_Yellow1   = { 0.75,    0.75,    0.00    };
static  SiiLibVideoRGB_t sClr_Yellow2   = { 1.00,    1.00,    0.00    };
//static  SiiLibVideoRGB_t sClr_Cyan1     = { 0.00,    0.75,    0.75    };
static  SiiLibVideoRGB_t sClr_Cyan2     = { 0.00,    1.00,    1.00    };
//static  SiiLibVideoRGB_t sClr_Green1    = { 0.00,    0.75,    0.00    };
static  SiiLibVideoRGB_t sClr_Green2    = { 0.00,    1.00,    0.00    };
//static  SiiLibVideoRGB_t sClr_Magenta1  = { 0.75,    0.00,    0.75    };
static  SiiLibVideoRGB_t sClr_Magenta2  = { 1.00,    0.00,    1.00    };
//static  SiiLibVideoRGB_t sClr_Red1      = { 0.75,    0.00,    0.00    };
static  SiiLibVideoRGB_t sClr_Red2      = { 1.00,    0.00,    0.00    };
static  SiiLibVideoRGB_t sClr_Blue1     = { 0.00,    0.00,    0.75    };
static  SiiLibVideoRGB_t sClr_Blue2     = { 0.00,    0.00,    1.00    };
static  SiiLibVideoRGB_t sClr_Blue3     = { 0.00,    0.00,    1.00    };

//static TpgObj_t* pObj = NULL;

/***** public functions ******************************************************/

SiiInst_t SiiDrvTpgCreate(SiiDrvTpgConfig_t* pConfig)
{
    /* Enforce module to be used as singleton only */
    //SII_PLATFORM_DEBUG_ASSERT(!pObj);
	TpgObj_t* pObj;

    /* Allocate memory for object */
	pObj = (TpgObj_t*)SII_LIB_OBJ_CREATE("tpg", sizeof(TpgObj_t));
	SII_PLATFORM_DEBUG_ASSERT(pObj);
	
    /* Initialize Instance */
    pObj->pConfig       = pConfig;
    SiiLibVideoTimingDetailGet(SII_LIB_VIDEO_VM__720P60, SII_LIB_VIDEO_MOD3D__NONE, false, &pObj->timing);
    pObj->mode          = SII_DRV_TPG_MODE__PATTERN;
    pObj->pattern       = SII_DRV_TPG_PATTERN__SIMP92; 
    pObj->fBarPos       = 0.5;
    pObj->absPos        = 0;
    pObj->barWidth      = 2;
    pObj->clrSpc        = SII_LIB_VIDEO_CLRSPC__601;
    pObj->fPictAspRat   = SII_LIB_VIDEO_ASPRAT__178;
    pObj->bFullRange    = false;
    pObj->upSamp        = 1;
    pObj->fAlphaBS      = 0.0;
	pObj->seqInst       = SII_LIB_SEQ_TIMER_CREATE("BlueScrnTimeOut", sBlack2BlueScreenHandler, 0, 2, SII_TIMER_BLUESCRN);
	SII_PLATFORM_DEBUG_ASSERT(pObj->seqInst);

	pObj->vtgInst = SiiDrvVtgCreate(pObj->pConfig->instTxCra);

	SiiDrvPllVoCreate(pObj->pConfig->instTxCra);

	SII_PLATFORM_DEBUG_ASSERT(pObj->vtgInst);

	//return pObj->vtgInst;
	return SII_LIB_OBJ_INST(pObj);

	//sUpdateMode(pObj);
}

void SiiDrvTpgDelete( SiiInst_t inst )
{
    //SII_PLATFORM_DEBUG_ASSERT(pObj);
	TpgObj_t* pObj = (TpgObj_t*)SII_LIB_OBJ_PNTR(inst);

	SiiDrvVtgDelete();
	SiiLibSeqTimerDelete(pObj->seqInst);
	SII_LIB_OBJ_DELETE(pObj); /* delete object */
	pObj = NULL;
}

void SiiDrvTpgTimingSet( SiiInst_t inst, SiiDrvTpgVM_t vm )
{
	TpgObj_t* pObj = (TpgObj_t*)SII_LIB_OBJ_PNTR(inst);

    SiiLibVideoTiming_t     timing; 

	SiiLibVideoTimingDetailGet((SiiLibVideoVM_t) vm, SII_LIB_VIDEO_MOD3D__NONE, false, &timing);

    //SII_PLATFORM_DEBUG_ASSERT(pObj);
	
    pObj->timing = timing;
    switch( pObj->timing.pictAr )
    {
        case SII_LIB_VIDEO_PICTAR__4X3  : pObj->fPictAspRat = SII_LIB_VIDEO_ASPRAT__133; break;
        case SII_LIB_VIDEO_PICTAR__16X9 : pObj->fPictAspRat = SII_LIB_VIDEO_ASPRAT__178; break;
        case SII_LIB_VIDEO_PICTAR__SQP  : pObj->fPictAspRat = (float)pObj->timing.hActive / (float)pObj->timing.vActive;
        default : break;
    }
	//SiiDrvVtgOutputVideoTimingSet(pObj->vtgInst, &timing); //warning_comment : no need to pass pObj->vtgInst!
	SiiDrvVtgOutputVideoTimingSet(&timing);
    sUpdateMode(pObj);
	SiiDrvPllVoOutputClockFrequencySet(pObj->pConfig->instTxCra, timing.pixelFreq);
}


void SiiDrvTpgModeSet( SiiInst_t inst, SiiDrvTpgMode_t mode )
{
    //SII_PLATFORM_DEBUG_ASSERT(pObj);
	TpgObj_t* pObj = (TpgObj_t*)SII_LIB_OBJ_PNTR(inst);

    pObj->mode = mode;
    sUpdateMode(pObj);
}

#if 0 // not used
SiiDrvTpgMode_t SiiDrvTpgModeGet( void )
{
    SII_PLATFORM_DEBUG_ASSERT(pObj);

    return pObj->mode;
}
#endif

void SiiDrvTpgPatternSet( SiiInst_t inst, SiiDrvTpgPattern_t pattern )
{
    //SII_PLATFORM_DEBUG_ASSERT(pObj);
	TpgObj_t* pObj = (TpgObj_t*)SII_LIB_OBJ_PNTR(inst);

    pObj->pattern = pattern;
    sUpdatePattern(pObj);
}

#if 0 // not used
SiiDrvTpgPattern_t SiiDrvTpgPatternGet( void )
{
    SII_PLATFORM_DEBUG_ASSERT(pObj);

    return pObj->pattern;
}

void SiiDrvTpgDemoBarPosSet( float fBarPos )
{
    SII_PLATFORM_DEBUG_ASSERT(pObj);
	SII_PLATFORM_DEBUG_ASSERT(0.0<=fBarPos);
	SII_PLATFORM_DEBUG_ASSERT(1.0>=fBarPos);

    pObj->fBarPos = fBarPos;
    sUpdateDemoBarPosition();
}

void SiiDrvTpgDemoBarWidthSet( uint8_t width )
{
    SII_PLATFORM_DEBUG_ASSERT(pObj);

    pObj->barWidth = width;
    sUpdateDemoBar();
}

void SiiDrvTpgColorSpaceSet( SiiLibVideoClrSpc_t clrSpc )
{
    SII_PLATFORM_DEBUG_ASSERT(pObj);

    pObj->clrSpc = clrSpc;
    sUpdateBlankScreenColor(pObj);
    sUpdatePattern(pObj);
}

void SiiDrvTpgFullRangeSet( bool_t bFullRange )
{
    SII_PLATFORM_DEBUG_ASSERT(pObj);

    pObj->bFullRange = bFullRange;
    sUpdateBlankScreenColor(pObj);
    sUpdatePattern(pObj);
}

void SiiDrvTpgHorUpSampFactorSet( uint8_t upSamp )
{
    SII_PLATFORM_DEBUG_ASSERT(pObj);

    pObj->upSamp = upSamp;
    sUpdatePattern(pObj);
}
#endif

/***** local functions *******************************************************/

static void sUpdateMode( TpgObj_t* pObj )
{
	switch( pObj->mode )
	{
	case SII_DRV_TPG_MODE__NONE    :
		//            SII_LIB_OBJ_LOG(SII_LIB_OBJ_LOG_TYPE__INFO, "mode=NONE");
		REG_ENABLE(0);
		sBlankDisable(pObj);
		break;

	case SII_DRV_TPG_MODE__BLANK   :
		//            SII_LIB_OBJ_LOG(SII_LIB_OBJ_LOG_TYPE__INFO, "mode=BLANK");
		sBlankEnable(pObj);
		break;

	case SII_DRV_TPG_MODE__PATTERN :
		//            SII_LIB_OBJ_LOG(SII_LIB_OBJ_LOG_TYPE__INFO, "mode=PATTERN");
		sUpdatePattern(pObj);
		break;
	}
}

static void sUpdatePattern( TpgObj_t* pObj )
{
	if( SII_DRV_TPG_MODE__PATTERN == pObj->mode )
	{
		sBlankEnable(pObj);
		switch( pObj->pattern )
		{

		case SII_DRV_TPG_PATTERN__FRMGEOM         : sSetFrmGeomPattern(pObj); break;
		case SII_DRV_TPG_PATTERN__CHECKERBOARD	  : sSetCheckerBoard(pObj); break;
		case SII_DRV_TPG_PATTERN__WINDOWIRE0      : sSetFlexWindowPattern(pObj, 0.00, 0.00, 0.00, 0.5); break;
		case SII_DRV_TPG_PATTERN__256GRAYRAMP     : sSet256GrayRamp(pObj); break;
		case SII_DRV_TPG_PATTERN__FULLWHITE75     : sSetFlexWindowPattern(pObj, 0.75, 0.75, 0.75, 0.5); break;
		case SII_DRV_TPG_PATTERN__FULLRED75       : sSetFlexWindowPattern(pObj, 0.75, 0.00, 0.00, 0.5); break;
		case SII_DRV_TPG_PATTERN__FULLGREEN75     : sSetFlexWindowPattern(pObj, 0.00, 0.75, 0.00, 0.5); break;
		case SII_DRV_TPG_PATTERN__FULLBLUE75      : sSetFlexWindowPattern(pObj, 0.00, 0.00, 0.75, 0.5); break;
		case SII_DRV_TPG_PATTERN__FULLCYAN75      : sSetFlexWindowPattern(pObj, 0.00, 0.75, 0.75, 0.5); break;
		case SII_DRV_TPG_PATTERN__FULLMAGENTA75   : sSetFlexWindowPattern(pObj, 0.75, 0.00, 0.75, 0.5); break;
		case SII_DRV_TPG_PATTERN__FULLYELLOW75    : sSetFlexWindowPattern(pObj, 0.75, 0.75, 0.00, 0.5); break;
		case SII_DRV_TPG_PATTERN__SIMP92    	  : sSetSimp92(pObj); break;
		case SII_DRV_TPG_PATTERN__CLR8BARS100     : sSetColor8Bars100(pObj); break;

		default                                   : SII_PLATFORM_DEBUG_ASSERT(0); break;
		}
		REG_ENABLE(1);
		sBlankDisable(pObj);
	}
}

static void sSetLineColor8Bars100( TpgObj_t* pObj )
{
	uint16_t X = (uint16_t)(pObj->timing.hActive);
	uint16_t A = (uint16_t)(0.125 * X);		/* Width of individual bars */
	uint16_t B = (uint16_t)(X - (7*A));		/* Width of last black bar */

	/* Program lookup tables */
	sSetColorLut(pObj, 0, &sClr_White2);
	sSetColorLut(pObj, 1, &sClr_Yellow2);
	sSetColorLut(pObj, 2, &sClr_Cyan2);
	sSetColorLut(pObj, 3, &sClr_Green2);
	sSetColorLut(pObj, 4, &sClr_Magenta2);
	sSetColorLut(pObj, 5, &sClr_Red2);
	sSetColorLut(pObj, 6, &sClr_Blue2);
	sSetColorLut(pObj, 7, &sClr_Black);

	REG_ADDRESS(0);                     /* Set Block RAM Address to 0 */
	sSetLineGen(pObj, 7, A, B);
}

static void sSetColor8Bars100( TpgObj_t* pObj )
{
	uint16_t H = pObj->timing.vActive;
	uint16_t i;

	/* Program Color bar line */
	sSetLineColor8Bars100(pObj);

	/* Program line index */
	REG_ADDRESS(0);                     /* Set Block RAM Address to 0 */
	for( i=0; i<H; i++ )
	{
		REG_LINEMAP(0x0000);
	}
}


static void sSetLineGenCheckerBoard(TpgObj_t* pObj, uint8_t line, uint16_t a, uint16_t b )
{
	uint16_t	i;
	uint8_t	 j;
	uint32_t	Data=0;
	/* Generate pixels (line 0 & 1)*/
	for( j=0; j < line; j++ )
	{
		for( i=1; i<=a; i++ )
		{
			//REG_LINEGEN((0<<3)|((j%2)<<0)); 
			Data=0;
			Data |= (j%2) << (0 * TPG_INDEX_NUM_OF_BITS);
			Data |= ((j+1)%2) << (1 * TPG_INDEX_NUM_OF_BITS); 
			REG_LINEGEN(Data);
		}
	}
	for( i=1; i<=b; i++ )
	{
		Data=0;
		Data |= (line%2) << (0 * TPG_INDEX_NUM_OF_BITS);
		Data |= ((line+1)%2) << (1 * TPG_INDEX_NUM_OF_BITS); 
		REG_LINEGEN(Data);
	}
}

static void sSetLineCheckerBoard( TpgObj_t* pObj )
{
	uint16_t X = (uint16_t)(pObj->timing.hActive);
	uint16_t A = (uint16_t)(0.125 * X);		/* Width of individual bars */
	uint16_t B = (uint16_t)(X - (7*A));		/* Width of last black bar */

	/* Program lookup tables */
	sSetColorLut(pObj, 0, &sClr_White2);
	sSetColorLut(pObj, 1, &sClr_Black);

	REG_ADDRESS(0);					 /* Set Block RAM Address to 0 */
	sSetLineGenCheckerBoard(pObj, 7, A, B);
}


static void sSetCheckerBoard( TpgObj_t* pObj )
{
	uint16_t H = pObj->timing.vActive;
	uint16_t A = (uint16_t)(0.167 * H);		/* Width of individual bars */
//	uint16_t B = (uint16_t)(H - (7*A));		/* Width of last black bar */

	uint16_t i;
	uint16_t j;

	/* Program Color bar line */
	sSetLineCheckerBoard(pObj);

	/* Program line index */
	REG_ADDRESS(0);					 /* Set Block RAM Address to 0 */
	for( i=0; i<6; i++ )
	{
		for (j=0; j < A; j++)
		{
			if ((i%2) == 0)
				REG_LINEMAP(0x0000);
			else
				REG_LINEMAP(0x0001);
		}
	}
}

static void sUpdateBlankScreenColor( TpgObj_t* pObj )
{
	SiiLibVideoRGB_t rgb;
	SiiLibVideoYUV_t yuv;

	/* Check alpha value: 0.0 <= alpha <= 1.0 */
	SII_PLATFORM_DEBUG_ASSERT(0.0 <= pObj->fAlphaBS);
	SII_PLATFORM_DEBUG_ASSERT(1.0 >= pObj->fAlphaBS);

	/* Apply RGB based alpha mixture between Black and BlueScrn color definition */
	rgb.fR = (float)(BLANK_SCRN_COLOR.fR*(1.0-pObj->fAlphaBS) + BLUE_SCRN_COLOR.fR*pObj->fAlphaBS);
	rgb.fG = (float)(BLANK_SCRN_COLOR.fG*(1.0-pObj->fAlphaBS) + BLUE_SCRN_COLOR.fG*pObj->fAlphaBS);
	rgb.fB = (float)(BLANK_SCRN_COLOR.fB*(1.0-pObj->fAlphaBS) + BLUE_SCRN_COLOR.fB*pObj->fAlphaBS);

	/* Convert RGB color to YCbCr color */
	sRgb2Yc(pObj, &yuv, &rgb);

	/* Apply YC color to Solid screen registers */
	REG_SOLID_COLOR_Y((uint16_t)yuv.fY);
	REG_SOLID_COLOR_CB((uint16_t)yuv.fCb);
	REG_SOLID_COLOR_CR((uint16_t)yuv.fCr);
}

static void sBlack2BlueScreenHandler( SiiInst_t inst )
{
	TpgObj_t* pObj = (TpgObj_t*)SII_LIB_OBJ_PNTR(inst);

	pObj->fAlphaBS += (float)0.005;
	if( 1.0 <= pObj->fAlphaBS )
	{
		pObj->fAlphaBS = 1.0;
		SiiLibSeqTimerStop(pObj->seqInst);
	}
	sUpdateBlankScreenColor(pObj);
}

static void sBlankEnable( TpgObj_t* pObj )
{
	pObj->fAlphaBS = 0.0;
	sUpdateBlankScreenColor(pObj);

	/* Enable video output blank */
	REG_CFG_SOLID_COLOR_ENABLE(1);

	/* Trigger a 10 second time out for changing black screen into blue screen */
	SiiLibSeqTimerStart(pObj->seqInst, 6000, 20);
}

static void sBlankDisable( TpgObj_t* pObj )
{
	/* Disable video output blank */
	REG_CFG_SOLID_COLOR_ENABLE(0);

	/* Stop blue screen time-out request */
	SiiLibSeqTimerStop(pObj->seqInst);
}

static void sRgb2Yc(TpgObj_t* pObj, SiiLibVideoYUV_t* pYuv, const SiiLibVideoRGB_t* pRgb )
{

	SiiLibVideoRGB_t Rgb;

	if( pObj->bFullRange )
	{
		Rgb = *pRgb;
	}
	else
	{
		SiiLibVideoFullRgbToLimitedRgb(&Rgb, pRgb);
	}

	switch (pObj->clrSpc)
	{
	case SII_LIB_VIDEO_CLRSPC__709:
		SiiLibVideoRgbToYc709(pYuv, &Rgb);
		SiiLibVideoQuantizeYc(pYuv, 12); /* Quantize to 12bit for Y and 12bit for C */
		break;
	case SII_LIB_VIDEO_CLRSPC__601:
		SiiLibVideoRgbToYc601(pYuv, &Rgb);
		SiiLibVideoQuantizeYc(pYuv, 12); /* Quantize to 12bit for Y and 12bit for C */
		break;
	case SII_LIB_VIDEO_CLRSPC__RGB:
		SiiLibVideoRgbToRGB(pYuv, &Rgb);
		SiiLibVideoQuantizeRgb((SiiLibVideoRGB_t*)pRgb, 12); /* Quantize to 12bit for Y and 12bit for C */
		break;
    default:
        break;
	}
}

static void sSetLumaLut(TpgObj_t* pObj, uint8_t indx, uint16_t luma )
{
	/* Program Luma */
	REG_ADDRESS(indx);
	REG_COLOR_Y(luma);
}

static void sSetChromaLut(TpgObj_t* pObj, uint8_t indx, uint16_t cb, uint16_t cr )
{
	/* Program Chroma */
	REG_ADDRESS(indx);
	REG_COLOR_CB(cb);
	REG_ADDRESS(indx);
	REG_COLOR_CR(cr);
}

static void sSetColorLut(TpgObj_t* pObj, uint8_t indx, const SiiLibVideoRGB_t* pRgb )
{
	SiiLibVideoYUV_t yuv;

	sRgb2Yc(pObj, &yuv, pRgb);

	sSetLumaLut(pObj, indx, (uint16_t)yuv.fY);
	sSetChromaLut(pObj, indx, (uint16_t)yuv.fCb, (uint16_t)yuv.fCr);
}


static void sSetLineGen(TpgObj_t* pObj, uint8_t line, uint16_t a, uint16_t b )
{
	uint16_t i;
	uint8_t  j;

	/* Generate pixels (line 0)*/
	for( j=0; j < line; j++ )
		for( i=1; i<=a; i++ )
			REG_LINEGEN((0<<3)|(j<<0));

	for( i=1; i<=b; i++ )
		REG_LINEGEN((0<<3)|(line<<0));
}

static void sSetFrmGeomPattern( TpgObj_t* pObj )
{
#define    L1    ((uint8_t)( 1   ))
#define    L2    ((uint8_t)( 2*L1))
#define    L3    ((uint8_t)( 3*L1))
#define    L4    ((uint8_t)( 4*L1))
#define    L5    ((uint8_t)( 5*L1))
#define    L6    ((uint8_t)( 6*L1))
#define    L7    ((uint8_t)( 7*L1))
#define    L9    ((uint8_t)( 9*L1))
#define    L10   ((uint8_t)(10*L1))
#define    L12   ((uint8_t)(12*L1))
#define    L13   ((uint8_t)(13*L1))
#define    L15   ((uint8_t)(15*L1))
#define    L16   ((uint8_t)(16*L1))

	uint16_t X  = (uint16_t)(pObj->timing.hActive);
	uint16_t Y  = (uint16_t)(pObj->timing.vActive);
	uint16_t A  = (uint16_t)(0.5 * X);
	uint16_t B  = (uint16_t)(0.5 * Y);
	uint16_t C  = (uint16_t)(0.5*Y - 21);
	uint16_t D  = (uint16_t)(0.757 * C);
	uint16_t E  = (uint16_t)(0.567 * C);
	uint16_t F  = (uint16_t)(0.787 * C);
	uint16_t G  = (uint16_t)(0.567 * C * ((float)X/Y) * (3.0/4.0));
	uint16_t T  = (uint16_t)((A - G - L2)/2);
	uint16_t U  = (uint16_t)(((C-L2) - F + L2)/2);
	uint16_t V  = (uint16_t)(((C-L2) - D + L4)/2);
	uint16_t W  = (uint16_t)(((C-L2) - E + L6)/2);
	uint8_t  Lh = (X%A)?(3*L1):(2*L1); /* For Odd number hor. resolutions */
	uint8_t  Lv = (Y%B)?(3):(2);       /* For Odd number ver. resolutions */
	uint16_t i;

	SII_PLATFORM_DEBUG_ASSERT((L5+A-L5-L16+L12+L3+Lh+L3+L12+A-L5-L16+L5)==X);
	SII_PLATFORM_DEBUG_ASSERT((2*C+Lv+40)==Y);

	/* Program lookup tables */
	sSetColorLut(pObj, 0, &sClr_Black);
	sSetColorLut(pObj, 1, &sClr_White1);
	sSetColorLut(pObj, 2, &sClr_White1);
	sSetColorLut(pObj, 3, &sClr_Black);
	sSetColorLut(pObj, 4, &sClr_White1);
	sSetColorLut(pObj, 5, &sClr_Gray1);
	sSetColorLut(pObj, 6, &sClr_Gray1);
	sSetColorLut(pObj, 7, &sClr_White1);
	sSetColorLut(pObj, 15, &sClr_White1);

	sSetColorLut(pObj, (1 << TPG_INDEX_NUM_OF_BITS) | 0, &sClr_Black);
	sSetColorLut(pObj, (1 << TPG_INDEX_NUM_OF_BITS) | 1, &sClr_White1);
	sSetColorLut(pObj, (1 << TPG_INDEX_NUM_OF_BITS) | 2, &sClr_Black);
	sSetColorLut(pObj, (1 << TPG_INDEX_NUM_OF_BITS) | 3, &sClr_White1);
	sSetColorLut(pObj, (1 << TPG_INDEX_NUM_OF_BITS) | 4, &sClr_Blue3);
	sSetColorLut(pObj, (1 << TPG_INDEX_NUM_OF_BITS) | 5, &sClr_White1);
	sSetColorLut(pObj, (1 << TPG_INDEX_NUM_OF_BITS) | 6, &sClr_Black);
	sSetColorLut(pObj, (1 << TPG_INDEX_NUM_OF_BITS) | 7, &sClr_White1);
	sSetColorLut(pObj, (1 << TPG_INDEX_NUM_OF_BITS) | 15, &sClr_White1);

	/* Program line index */
	REG_ADDRESS(0);   /* Set Block RAM Address to 0 */

	for( i=1; i<=X; i++ )
	{
		uint32_t Data = 0;

		/* Generate pixels (line 0)*/
		if     ( i > (A-L16+L12+L3+Lh+L3+L12+A-L5-L16)     )  Data |= (1UL<<(0 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (A-L16+L12+L3+Lh+L3+L12)              )  Data |= (2UL<<(0 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (A-L16+L12+L3+Lh+L3)                  )  Data |= (0UL<<(0 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (A-L16+L12+L3+Lh)                     )  Data |= (2UL<<(0 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (A-L16+L12+L3)                        )  Data |= (1UL<<(0 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (A-L16+L12)                           )  Data |= (2UL<<(0 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (A-L16)                               )  Data |= (0UL<<(0 * TPG_INDEX_NUM_OF_BITS));
		else if( i > L5                                    )  Data |= (2UL<<(0 * TPG_INDEX_NUM_OF_BITS));
		else                                                  Data |= (1UL<<(0 * TPG_INDEX_NUM_OF_BITS));

		/* Generate pixels (line 1)*/
		if     ( i > (L1+L4+A-L5-L7+L6+Lh+L6+A-L5-L7+L4)   )  Data |= (2UL<<(1 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L1+L4+A-L5-L7+L6+Lh+L6+A-L5-L7)      )  Data |= (1UL<<(1 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L1+L4+A-L5-L7+L6+Lh+L6)              )  Data |= (0UL<<(1 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L1+L4+A-L5-L7+L6+Lh)                 )  Data |= (2UL<<(1 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L1+L4+A-L5-L7+L6)                    )  Data |= (1UL<<(1 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L1+L4+A-L5-L7)                       )  Data |= (2UL<<(1 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L1+L4)                               )  Data |= (0UL<<(1 * TPG_INDEX_NUM_OF_BITS));
		else if( i > L1                                    )  Data |= (1UL<<(1 * TPG_INDEX_NUM_OF_BITS));
		else                                                  Data |= (2UL<<(1 * TPG_INDEX_NUM_OF_BITS));

		/* Generate pixels (line 2)*/
		if     ( i > (L2+L3+A-L5-L10+L9+Lh+L9+A-L5-L10+L3) )  Data |= (2UL<<(2 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L3+A-L5-L10+L9+Lh+L9+A-L5-L10)    )  Data |= (3UL<<(2 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L3+A-L5-L10+L9+Lh+L9)             )  Data |= (0UL<<(2 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L3+A-L5-L10+L9+Lh)                )  Data |= (2UL<<(2 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L3+A-L5-L10+L9)                   )  Data |= (1UL<<(2 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L3+A-L5-L10)                      )  Data |= (2UL<<(2 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L3)                               )  Data |= (0UL<<(2 * TPG_INDEX_NUM_OF_BITS));
		else if( i > L2                                    )  Data |= (3UL<<(2 * TPG_INDEX_NUM_OF_BITS));
		else                                                  Data |= (2UL<<(2 * TPG_INDEX_NUM_OF_BITS));

		/* Generate pixels (line 3)*/
		if     ( i > (L2+L1+L2+A-L5-L13+L12+Lh+L12+A-L5-L13+L2+L1)    )  Data |= (2UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L1+L2+A-L5-L13+L12+Lh+L12+A-L5-L13+L2)       )  Data |= (0UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L1+L2+A-L5-L13+L12+Lh+L12+A-L5-L13)          )  Data |= (3UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L1+L2+A-L5-L13+L12+Lh+L12)                   )  Data |= (0UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L1+L2+A-L5-L13+L12+Lh)                       )  Data |= (2UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L1+L2+A-L5-L13+L12)                          )  Data |= (1UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L1+L2+A-L5-L13)                              )  Data |= (2UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L1+L2)                                       )  Data |= (0UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L1)                                          )  Data |= (3UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else if( i > L2                                               )  Data |= (0UL<<(3 * TPG_INDEX_NUM_OF_BITS));
		else                                                             Data |= (2UL<<(3 * TPG_INDEX_NUM_OF_BITS));

		/* Generate pixels (line 4)*/
		if     ( i > (L2+L2+L1+A-L5-L16+L15+Lh+L15+A-L5-L16+L1+L2)   )  Data |= (2UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L2+L1+A-L5-L16+L15+Lh+L15+A-L5-L16+L1)      )  Data |= (0UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L2+L1+A-L5-L16+L15+Lh+L15+A-L5-L16)         )  Data |= (3UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L2+L1+A-L5-L16+L15+Lh+L15)                  )  Data |= (0UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L2+L1+A-L5-L16+L15+Lh)                      )  Data |= (2UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L2+L1+A-L5-L16+L15)                         )  Data |= (1UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L2+L1+A-L5-L16)                             )  Data |= (2UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L2+L1)                                      )  Data |= (0UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (L2+L2)                                         )  Data |= (3UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else if( i > L2                                              )  Data |= (0UL<<(4 * TPG_INDEX_NUM_OF_BITS));
		else                                                            Data |= (2UL<<(4 * TPG_INDEX_NUM_OF_BITS));

		/* Generate pixels (line 5)*/
		if     ( i > (T+L1+G+L1+A-G-T-L3+Lh+A-G-T-L3+L1+G+T)         )  Data |= (1UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T+L1+G+L1+A-G-T-L3+Lh+A-G-T-L3+L1+G+L1)        )  Data |= (0UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T+L1+G+L1+A-G-T-L3+Lh+A-G-T-L3+L1+G)           )  Data |= (1UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T+L1+G+L1+A-G-T-L3+Lh+A-G-T-L3+L1)             )  Data |= (4UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T+L1+G+L1+A-G-T-L3+Lh+A-G-T-L3)                )  Data |= (1UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T+L1+G+L1+A-G-T-L3+Lh)                         )  Data |= (0UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T+L1+G+L1+A-G-T-L3)                            )  Data |= (1UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T+L1+G+L1)                                     )  Data |= (0UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T+L1+G)                                        )  Data |= (1UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T+L1)                                          )  Data |= (4UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > (T)                                             )  Data |= (1UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else if( i > L1                                              )  Data |= (0UL<<(5 * TPG_INDEX_NUM_OF_BITS));
		else                                                            Data |= (1UL<<(5 * TPG_INDEX_NUM_OF_BITS));

		REG_LINEGEN(Data);
	}

	/* Program line index */
	REG_ADDRESS(0);   /* Set Block RAM Address to 0 */

	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0000);
	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0001);
	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0002);
	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0003);
	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0004);		
	REG_FIX_PATTERN(6);

	for( i=1; i<=C; i++ )
	{
		if ( (i==1) || (i==U) || (i==V) || (i==W) || (i==C-W) || (i==C-V) || (i==C-U) || (i==C) )
		{
			REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 5);             /* Line value 5, Lut 0 */
		}
		else
		{
			REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 5);            /* Line value 5, Lut 1 */
		}
	}

	/* Line value 4, LUT 1 */
	for( i=1; i<=3; i++ )   
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 4);    //4

	for( i=1; i<=3; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 3);    //3

	for( i=1; i<=3; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 2);    //2

	for( i=1; i<=3; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 1);     //1

	for( i=1; i<=3; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 0);     //0

	for( i=1; i<=Lv; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 6);    //6

	for( i=1; i<=3; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 0);     //0

	for( i=1 ;i<=3; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 1);     //1

	for( i=1; i<=3; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 2);    //2

	for( i=1; i<=3; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 3);    //3

	for( i=1; i<=3; i++ )
		REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 4);    //4

	for( i=1; i<=C; i++ )
	{
		if( (i==1) || (i==U) || (i==V) || (i==W) || (i==C-W) || (i==C-V) || (i==C-U) || (i==C) )
		{
			REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 5);
		}
		else
		{
			REG_LINEMAP((1 << TPG_BANK_SELECT_BIT) | 5);
		}
	}

	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0004);
	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0003);
	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0002);
	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0001);
	REG_LINEMAP((0 << TPG_BANK_SELECT_BIT) | 0x0000);		

}

#if 0 //not used
static void sSetEvOtPix( void )
{
	uint16_t i;
	uint16_t W = pObj->timing.hActive;
	uint16_t H = pObj->timing.vActive/2;

	/* Program lookup tables */
	sSetColorLut(pObj, 0, &sClr_Black);
	sSetColorLut(pObj, 1, &sClr_White1);

	/* Generate pixels (line 0 and 1) */
	REG_ADDRESS(0);                     /* Set Block RAM Address to 0 */
	for( i=0; i<W; i++ )
	{
		if( i%2 )
		{
			REG_LINEGEN((1<<3)|(0<<0));
		}
		else
		{
			REG_LINEGEN((0<<3)|(1<<0));
		}
	}

	/* Program line index */
	REG_ADDRESS(0);                     /* Set Block RAM Address to 0 */
	for( i=0; i<H; i++ )
	{
		REG_LINEMAP(0);
		REG_LINEMAP(1);
	}
}
#endif

static void sSet256GrayRamp( TpgObj_t* pObj )
{
	uint16_t H = pObj->timing.hActive;
	uint16_t V = pObj->timing.vActive;
	uint8_t step = (256 << TPG_INDEX_NUM_OF_BITS) / H;
//	uint16_t count=0;
	uint16_t gray=0;
	uint16_t i;

	//===== set up line gen memory =====
	REG_ADDRESS(0);
	for(i=0; i < H; i++)
	{
		REG_LINEGEN(gray); 
		gray += step;
	}

	//===== set up line map memory =====
	REG_ADDRESS(0);
	for(i=0; i < V; i++)
		REG_LINEMAP(0x80);
}

//---------------------------------------------------------------------------
// tpg_simp92() -- program Samsung simp92 pattern
//
// Inputs:  format   -- video format
//                      use one of the defines from video_formats.ats
//          mode     -- color mode
//                      use one of the defines from video_formats.ats
//          pattern  -- tpg pattern
//                      use one of the defines above
// Outputs: none
//---------------------------------------------------------------------------

//void tpg_simp92(int format,int mode,int pattern)
static void sSetSimp92( TpgObj_t* pObj )
{
	SiiLibVideoRGB_t 	Rgb;

	uint16_t X  					= (uint16_t)(pObj->timing.hActive);
	uint16_t Y  					= (uint16_t)(pObj->timing.vActive);
	uint16_t tpg_bits   			= 12;
	uint8_t tpg_bit_shift 			= tpg_bits-8;
	uint16_t tpg_index_bits       	= 4;
//	uint16_t tpg_bank_bits        	= 3;
	uint16_t tpg_bank_select_bit  	= 3;
	uint16_t tpg_transparency_bit 	= 0;
	uint16_t bank_start;
	uint16_t i, j;
	uint32_t val,temp;

	// Bank # - green ramp
	bank_start = 0 * (1 << tpg_index_bits);

	// Bank #0 - green ramp
	REG_ADDRESS(0);	
	Rgb.fR = 0;
	Rgb.fB = 0;
	for(i = 0; i < (uint16_t)(1<<tpg_index_bits); i++)
	{
		/* Program lookup tables */
		Rgb.fG = (float)((i * 17) << tpg_bit_shift);
		sInt2Float(&Rgb);
		sSetColorLut(pObj, bank_start + i, &Rgb);
	}

	// Bank #1 - red ramp
	bank_start = 1 * (1 << tpg_index_bits);

	Rgb.fG = 0;
	Rgb.fB = 0;
	for(i = 0; i < (uint16_t)(1<<tpg_index_bits); i++)
	{
		Rgb.fR = (float)((i * 17) << tpg_bit_shift);
		sInt2Float(&Rgb);
		sSetColorLut(pObj, bank_start + i, &Rgb);
	}

	// Bank #2 - blue ramp
	bank_start = 2 * (1 << tpg_index_bits);
	Rgb.fR = 0;
	Rgb.fG = 0;
	for(i = 0; i < (uint16_t)(1 << tpg_index_bits); i++)
	{
		Rgb.fB = (float)((i * 17) << tpg_bit_shift);
		sInt2Float(&Rgb);
		sSetColorLut(pObj, bank_start + i, &Rgb);
	}

	// Bank #3 - gray ramp
	bank_start = 3 * (1 << tpg_index_bits);

	for(i = 0; i < (uint16_t)(1 << tpg_index_bits); i++)
	{
		Rgb.fR = (float)((i * 17) << tpg_bit_shift);
		Rgb.fG = (float)((i * 17) << tpg_bit_shift);
		Rgb.fB = (float)((i * 17) << tpg_bit_shift);
		sInt2Float(&Rgb);
		sSetColorLut(pObj, bank_start + i, &Rgb);
	}

	// Bank #4 - on off gray / gray ramp 8
	bank_start = 4 * (1 << tpg_index_bits);

	Rgb.fR = (float)(255 << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(255 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 0, &Rgb);

	Rgb.fR = (float)(0 << tpg_bit_shift);
	Rgb.fG = (float)(0 << tpg_bit_shift);
	Rgb.fB = (float)(0 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 1, &Rgb);

	Rgb.fR = (float)(178 << tpg_bit_shift);
	Rgb.fG = (float)(178 << tpg_bit_shift);
	Rgb.fB = (float)(178 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 2, &Rgb);

	Rgb.fR = (float)(0 << tpg_bit_shift);
	Rgb.fG = (float)(0 << tpg_bit_shift);
	Rgb.fB = (float)(0 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 3, &Rgb);

	Rgb.fR = (float)(13 << tpg_bit_shift);
	Rgb.fG = (float)(13 << tpg_bit_shift);
	Rgb.fB = (float)(13 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 4, &Rgb);

	Rgb.fR = (float)(26 << tpg_bit_shift);
	Rgb.fG = (float)(26 << tpg_bit_shift);
	Rgb.fB = (float)(26 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 5, &Rgb);

	Rgb.fR = (float)(38 << tpg_bit_shift);
	Rgb.fG = (float)(38 << tpg_bit_shift);
	Rgb.fB = (float)(38 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 6, &Rgb);

	Rgb.fR = (float)(233 << tpg_bit_shift);
	Rgb.fG = (float)(233 << tpg_bit_shift);
	Rgb.fB = (float)(233 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 7, &Rgb);

	Rgb.fR = (float)(240 << tpg_bit_shift);
	Rgb.fG = (float)(240 << tpg_bit_shift);
	Rgb.fB = (float)(240 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 8, &Rgb);

	Rgb.fR = (float)(248 << tpg_bit_shift);
	Rgb.fG = (float)(248 << tpg_bit_shift);
	Rgb.fB = (float)(248 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 9, &Rgb);

	Rgb.fR = (float)(255 << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(255 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 10, &Rgb);


	Rgb.fR = 0;
	Rgb.fG = 0;
	Rgb.fB = 0;
	for(i = 11; i < (uint16_t)(1 << tpg_index_bits); i++)
	{
		sInt2Float(&Rgb);
		sSetColorLut(pObj, bank_start + i, &Rgb);
	}

	// Bank #5 - dark gray ramp
	bank_start = 5 * (1 << tpg_index_bits);

	for(i = 0; i < (uint16_t)(1 << tpg_index_bits); i++)
	{
		Rgb.fR = (float)(((i * 55) / 15) << tpg_bit_shift);
		Rgb.fG = (float)(((i * 55) / 15) << tpg_bit_shift);
		Rgb.fB = (float)(((i * 55) / 15) << tpg_bit_shift);
		sInt2Float(&Rgb);
		sSetColorLut(pObj, bank_start + i, &Rgb);
	}

	// Bank #6 - CGM
	bank_start = 6 * (1 << tpg_index_bits);

	Rgb.fR = (float)(0   << tpg_bit_shift);
	Rgb.fG = (float)(61  << tpg_bit_shift);
	Rgb.fB = (float)(102 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 0, &Rgb);

	Rgb.fR = (float)(50 << tpg_bit_shift);
	Rgb.fG = (float)(50 << tpg_bit_shift);
	Rgb.fB = (float)(50 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 1, &Rgb);

	Rgb.fR = (float)(61  << tpg_bit_shift);
	Rgb.fG = (float)(0   << tpg_bit_shift);
	Rgb.fB = (float)(102 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 2, &Rgb);

	Rgb.fR = 0;
	Rgb.fG = 0;
	Rgb.fB = 0;
	for(i = 3; i < (uint16_t)(1 << tpg_index_bits); i++)
	{
		sInt2Float(&Rgb);
		sSetColorLut(pObj, bank_start + i, &Rgb);
	}

	// Bank #7 - YCP
	bank_start = 7 * (1 << tpg_index_bits);

	Rgb.fR = (float)(255 << tpg_bit_shift);
	Rgb.fG = (float)(204 << tpg_bit_shift);
	Rgb.fB = (float)(0   << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 0, &Rgb);

	Rgb.fR = (float)(255 << tpg_bit_shift);
	Rgb.fG = (float)(229 << tpg_bit_shift);
	Rgb.fB = (float)(0   << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 1, &Rgb);

	Rgb.fR = (float)(255 << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(0   << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 2, &Rgb);

	Rgb.fR = (float)(229 << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(0   << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 3, &Rgb);

	Rgb.fR = (float)(204 << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(0   << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 4, &Rgb);


	Rgb.fR = (float)(0   << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(204 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 5, &Rgb);

	Rgb.fR = (float)(0   << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(209 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 6, &Rgb);

	Rgb.fR = (float)(0   << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(255 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 7, &Rgb);

	Rgb.fR = (float)(0   << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(255 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 8, &Rgb);

	Rgb.fR = (float)(0   << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(255 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 9, &Rgb);

	Rgb.fR = (float)(0   << tpg_bit_shift);
	Rgb.fG = (float)(255 << tpg_bit_shift);
	Rgb.fB = (float)(255 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 10, &Rgb);

	Rgb.fR = (float)(204 << tpg_bit_shift);
	Rgb.fG = (float)(0   << tpg_bit_shift);
	Rgb.fB = (float)(255 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 11, &Rgb);

	Rgb.fR = (float)(209 << tpg_bit_shift);
	Rgb.fG = (float)(0   << tpg_bit_shift);
	Rgb.fB = (float)(255 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 12, &Rgb);

	Rgb.fR = (float)(255 << tpg_bit_shift);
	Rgb.fG = (float)(0   << tpg_bit_shift);
	Rgb.fB = (float)(255 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 13, &Rgb);

	Rgb.fR = (float)(255 << tpg_bit_shift);
	Rgb.fG = (float)(0   << tpg_bit_shift);
	Rgb.fB = (float)(229 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 14, &Rgb);

	Rgb.fR = (float)(255 << tpg_bit_shift);
	Rgb.fG = (float)(0 << tpg_bit_shift);
	Rgb.fB = (float)(204 << tpg_bit_shift);
	sInt2Float(&Rgb);
	sSetColorLut(pObj, bank_start + 15, &Rgb);

	//------------------------------------------------------------
	// Linegen data - generate pixels
	//------------------------------------------------------------

	REG_ADDRESS(0);                     /* Set Block RAM Address to 0 */
	for( i=1; i<=X; i++ )
	{
		uint32_t Data = 0;

		/* Generate pixels (pattern line 0)*/
		// 16 boxes per row
		Data |= (16UL * (i-1) / X) << (0 * tpg_index_bits);

		/* Generate pixels (pattern line 1)*/
		// on/off gray #0
		if     ( i > ((104UL * X) / 128UL)) Data |= (0 + (i-1)%2) << (1 * tpg_index_bits); // white/black alternating
		else if( i > (( 24UL * X) / 128UL)) Data |=  2            << (1 * tpg_index_bits); // gray
		else                            Data |=  0            << (1 * tpg_index_bits); // white

		/* Generate pixels (pattern line 2)*/
		// on/off gray #1
		if   ( i >   ((104UL * X) / 128UL)) Data |= (0 + (i-1)%2)   << (2 * tpg_index_bits); // white/black alternating
		else if( i > (( 24UL * X) / 128UL)) Data |= 2             << (2 * tpg_index_bits); // gray
		else                            Data |= 1             << (2 * tpg_index_bits); // black

		/* Generate pixels (pattern line 3)*/
		// CGM
		if     ( i > (104UL * X / 128)) Data |= 2 << (3 * tpg_index_bits); // M
		else if( i > ( 24UL * X / 128)) Data |= 1 << (3 * tpg_index_bits); // G
		else                            Data |= 0 << (3 * tpg_index_bits); // C

		/* Generate pixels (pattern line 4)*/
		// 8 boxes per row
		Data |= (3 + (8UL * (i-1) / X)) << (4 * tpg_index_bits);

		REG_LINEGEN(Data);
	}

	//------------------------------------------------------------
	// LineMap Memory
	//------------------------------------------------------------
	// Allocate memory
	//if(tpg_map.line = (int *)calloc(tpg_lines,sizeof(int)))

	/* Program line index */
	REG_ADDRESS(0);   /* Set Block RAM Address to 0 */

	// Pattern #0 / LUT #0 - green ramp
	for(j=0;j<2*Y/24;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (0 << tpg_bank_select_bit) | 0;
		val = (0 << tpg_transparency_bit) | (0 << tpg_bank_select_bit) | 0;
		REG_LINEMAP(val);
	}

	// Pattern #0 / LUT #1 - red ramp
	for(;j<3*Y/24;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (1 << tpg_bank_select_bit) | 0;
		val = (0 << tpg_transparency_bit) | (1 << tpg_bank_select_bit) | 0;
		REG_LINEMAP(val);			
	}
	// Pattern #0 / LUT #2 - blue ramp
	for(;j<4*Y/24;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (2 << tpg_bank_select_bit) | 0;
		val = (0 << tpg_transparency_bit) | (2 << tpg_bank_select_bit) | 0;
		REG_LINEMAP(val);			
	}
	// Pattern #0 / LUT #3 - gray ramp
	for(;j<5*Y/24;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (3 << tpg_bank_select_bit) | 0;
		val = (0 << tpg_transparency_bit) | (3 << tpg_bank_select_bit) | 0;
		REG_LINEMAP(val);			
	}

	temp = 345UL*Y/720;
	// Pattern #1/#2 / LUT #4 - on-off gray
	for(;j < temp;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (4 << tpg_bank_select_bit) | (1 + ((j-5*Y/24)/2)%2);
		val = (0 << tpg_transparency_bit) | (4 << tpg_bank_select_bit) | (1 + ((j-5UL*Y/24)/2)%2);
		REG_LINEMAP(val);			
	}

	temp = 405UL*Y/720UL;
	// Pattern #0 / LUT #5 - dark gray ramp
	for(;j<temp;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (5 << tpg_bank_select_bit) | 0;
		val = (0 << tpg_transparency_bit) | (5 << tpg_bank_select_bit) | 0;
		REG_LINEMAP(val);			
	}	
	// Pattern #3 / LUT #6 - CGM
	for(;j<20UL*Y/24;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (6 << tpg_bank_select_bit) | 3;
		val = (0 << tpg_transparency_bit) | (6 << tpg_bank_select_bit) | 3;
		REG_LINEMAP(val);			
	}

	// Pattern #0 / LUT #3 - gray ramp
	for(;j<21UL*Y/24;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (3 << tpg_bank_select_bit) | 0;
		val = (0 << tpg_transparency_bit) | (3 << tpg_bank_select_bit) | 0;
		REG_LINEMAP(val);			
	}

	// Pattern #0 / LUT #7 - YCP
	for(;j<22UL*Y/24;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (7 << tpg_bank_select_bit) | 0;
		val = (0 << tpg_transparency_bit) | (7 << tpg_bank_select_bit) | 0;
		REG_LINEMAP(val);			
	}

	// Pattern #4 / LUT #4 - gray ramp 8
	for(;j<Y;j++)
	{
		//tpg_map.line[j] = (0 << tpg_transparency_bit) | (4 << tpg_bank_select_bit) | 4;
		val = (0 << tpg_transparency_bit) | (4 << tpg_bank_select_bit) | 4;
		REG_LINEMAP(val);			
	}	
}

static void sSetFlexWindowPattern(TpgObj_t* pObj, float fRed, float fGreen, float fBlue, float fSize )
{
	uint16_t A = (uint16_t)(fSize*pObj->timing.hActive+0.5);
	uint16_t B = A;
	uint16_t C = (pObj->timing.hActive - B)/2;
	uint16_t D = pObj->timing.hActive - B - C;
	uint16_t E = (uint16_t)(fSize*pObj->timing.vActive+0.5);
	uint16_t F = E;
	uint16_t G = (pObj->timing.vActive - F)/2;
	uint16_t H = pObj->timing.vActive - F - G;
	uint16_t i;
	SiiLibVideoRGB_t RGB;

	if( 0.0 > fSize )
		fSize = 0.0;
	else if( 1.0 < fSize )
		fSize = 1.0;

	/* Program lookup tables */
	RGB.fR = fRed;
	RGB.fG = fGreen;
	RGB.fB = fBlue;
	sSetColorLut(pObj, 7, &sClr_UltraBlk);
	sSetColorLut(pObj, 0, &sClr_UltraBlk);
	sSetColorLut(pObj, 1, &RGB);

	/* Generate pixels (line 0)*/
	REG_ADDRESS(0);                     /* Set Block RAM Address to 0 */

	for( i=0; i<C; i++ )
		REG_LINEGEN(0);

	for( i=0; i<B; i++ )
		REG_LINEGEN(1);

	for( i=0; i<D; i++ )
		REG_LINEGEN(0);

	/* Program line index */
	REG_ADDRESS(0);                     /* Set Block RAM Address to 0 */
	for( i=0; i<G; i++ )
		REG_LINEMAP(0x0006);

	for( i=0; i<F; i++ )
		REG_LINEMAP(0x0000);

	for( i=0; i<H; i++ )
		REG_LINEMAP(0x0006);
}

#define INT2FLOAT(a) (a/(255 << 4))
static void sInt2Float(SiiLibVideoRGB_t* pRgb )
{
	pRgb->fG  = INT2FLOAT(pRgb->fG);
	pRgb->fB = INT2FLOAT(pRgb->fB);
	pRgb->fR = INT2FLOAT(pRgb->fR);

}


/** END of File *********************************************************/
