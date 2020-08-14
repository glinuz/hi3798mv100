/*************************************************************************
* si_drv_tpg_api.h
*************************************************************************/
#ifndef __SI_DRV_TPG_API_H__
#define __SI_DRV_TPG_API_H__

#include "si_datatypes.h"
#include "si_lib_obj_api.h"
#include "si_lib_video_api.h"
#include "si_drv_cra_api.h"

/************************************************************************/
/*  Type Definitions                                                    */
/************************************************************************/
typedef enum
{
	SII_DRV_TPG_MODE__NONE,
	SII_DRV_TPG_MODE__BLANK,
	SII_DRV_TPG_MODE__PATTERN,
} SiiDrvTpgMode_t;

typedef enum
{
	SII_DRV_TPG_PATTERN__FULLRED75,         
	SII_DRV_TPG_PATTERN__FULLGREEN75,       
	SII_DRV_TPG_PATTERN__FULLBLUE75,        
	SII_DRV_TPG_PATTERN__FULLCYAN75,        
	SII_DRV_TPG_PATTERN__FULLMAGENTA75,     
	SII_DRV_TPG_PATTERN__FULLYELLOW75,      
	SII_DRV_TPG_PATTERN__WINDOWIRE0,    //solid black 
	SII_DRV_TPG_PATTERN__FULLWHITE75,               
	SII_DRV_TPG_PATTERN__256GRAYRAMP,                  
	SII_DRV_TPG_PATTERN__CLR8BARS100,
	SII_DRV_TPG_PATTERN__CHECKERBOARD,      //checkboard pattern          
	SII_DRV_TPG_PATTERN__SIMP92,    
	SII_DRV_TPG_PATTERN__FRMGEOM,    

	SII_DRV_TPG_PATTERN__EOL /* End of list */
} SiiDrvTpgPattern_t;

/**
* @brief Video format (resolution)
*/
typedef enum
{
	SII_DRV_TPG_VM__VGA60           , //!<  0 = vm1_640x480p,
	SII_DRV_TPG_VM__480P            , //!<  1 = vm2_3_720x480p,
	SII_DRV_TPG_VM__720P60          , //!<  2 = vm4_1280x720p,
	SII_DRV_TPG_VM__1080I60         , //!<  3 = vm5_1920x1080i,
	SII_DRV_TPG_VM__480I2           , //!<  4 = vm6_7_720_1440x480i,
	SII_DRV_TPG_VM__240P2A          , //!<  5 = vm8_9_720_1440x240p_1,
	SII_DRV_TPG_VM__240P2B          , //!<  6 = vm8_9_720_1440x240p_2,
	SII_DRV_TPG_VM__480I4           , //!<  7 = vm10_11_2880x480i,
	SII_DRV_TPG_VM__240P4A          , //!<  8 = vm12_13_2880x240p_1,
	SII_DRV_TPG_VM__240P4B          , //!<  9 = vm12_13_2880x240p_2,
	SII_DRV_TPG_VM__480P2           , //!< 10 = vm14_15_1440x480p,
	SII_DRV_TPG_VM__1080P60         , //!< 11 = vm16_1920x1080p,
	SII_DRV_TPG_VM__576P            , //!< 12 = vm17_18_720x576p,
	SII_DRV_TPG_VM__720P50          , //!< 13 = vm19_1280x720p,
	SII_DRV_TPG_VM__1080I50         , //!< 14 = vm20_1920x1080i,
	SII_DRV_TPG_VM__576I2           , //!< 15 = vm21_22_720_1440x576i,
	SII_DRV_TPG_VM__288P2A          , //!< 16 = vm23_24_720_1440x288p_1,
	SII_DRV_TPG_VM__288P2B          , //!< 17 = vm23_24_720_1440x288p_2,
	SII_DRV_TPG_VM__288P2C          , //!< 18 = vm23_24_720_1440x288p_3,
	SII_DRV_TPG_VM__576I4           , //!< 19 = vm25_26_2880x576i,
	SII_DRV_TPG_VM__288P4A          , //!< 20 = vm27_28_2880x288p_1,
	SII_DRV_TPG_VM__288P4B          , //!< 21 = vm27_28_2880x288p_2,
	SII_DRV_TPG_VM__288P4C          , //!< 22 = vm27_28_2880x288p_3,
	SII_DRV_TPG_VM__576P2           , //!< 23 = vm29_30_1440x576p,
	SII_DRV_TPG_VM__1080P50         , //!< 24 = vm31_1920x1080p,
	SII_DRV_TPG_VM__1080P24         , //!< 25 = vm32_1920x1080p,
	SII_DRV_TPG_VM__1080P25         , //!< 26 = vm33_1920x1080p,
	SII_DRV_TPG_VM__1080P30         , //!< 27 = vm34_1920x1080p,
	SII_DRV_TPG_VM__480P4           , //!< 28 = vm35_36_2880x480p,
	SII_DRV_TPG_VM__576P4           , //!< 29 = vm37_38_2880x576p,
	SII_DRV_TPG_VM__1080I50A        , //!< 30 = vm39_1920x1080i_1250_total,
	SII_DRV_TPG_VM__1080I100        , //!< 31 = vm40_1920x1080i,
	SII_DRV_TPG_VM__720P100         , //!< 32 = vm41_1280x720p,
	SII_DRV_TPG_VM__576P100         , //!< 33 = vm42_43_720x576p,
	SII_DRV_TPG_VM__576I100         , //!< 34 = vm44_45_720_1440x576i,
	SII_DRV_TPG_VM__1080I120        , //!< 35 = vm46_1920x1080i,
	SII_DRV_TPG_VM__720P120         , //!< 36 = vm47_1280x720p,
	SII_DRV_TPG_VM__480P120         , //!< 37 = vm48_49_720x480p,
	SII_DRV_TPG_VM__480I120         , //!< 38 = vm50_51_720_1440x480i,
	SII_DRV_TPG_VM__576P200         , //!< 39 = vm52_53_720x576p,
	SII_DRV_TPG_VM__576I200         , //!< 40 = vm54_55_720_1440x576i,
	SII_DRV_TPG_VM__480P240         , //!< 41 = vm56_57_720x480p,
	SII_DRV_TPG_VM__480I240         , //!< 42 = vm58_59_720_1440x480i,
	SII_DRV_TPG_VM__720P24          , //!< 43 = vm60_1280x720p,
	SII_DRV_TPG_VM__720P25          , //!< 44 = vm61_1280x720p,
	SII_DRV_TPG_VM__720P30          , //!< 45 = vm62_1280x720p,
	SII_DRV_TPG_VM__1080P120        , //!< 46 = vm63_1920x1080p,
	SII_DRV_TPG_VM__1080P100        , //!< 47 = vm64_1920x1080p,
	SII_DRV_TPG_VM__2160P30         , //!< 48 = vm_3840x2160p,
	SII_DRV_TPG_VM__2160P25         , //!< 49 = vm_3840x2160p,
	SII_DRV_TPG_VM__2160P24         , //!< 50 = vm_3840x2160p,
	SII_DRV_TPG_VM__2160P24S        , //!< 51 = vm_4096x2160p,
	SII_DRV_TPG_VM__VGA72           , //!< 52 = VM_PC_VGA72,
	SII_DRV_TPG_VM__VGA75           , //!< 53 = VM_PC_VGA75,
	SII_DRV_TPG_VM__VGA85_1         , //!< 54 = VM_PC_VGA85_1,
	SII_DRV_TPG_VM__VGA85_2         , //!< 55 = VM_PC_VGA85_2,
	SII_DRV_TPG_VM__SVGA56          , //!< 56 = VM_PC_SVGA56,
	SII_DRV_TPG_VM__SVGA60          , //!< 57 = VM_PC_SVGA60,
	SII_DRV_TPG_VM__SVGA72          , //!< 58 = VM_PC_SVGA72,
	SII_DRV_TPG_VM__SVGA75          , //!< 59 = VM_PC_SVGA75,
	SII_DRV_TPG_VM__SVGA85          , //!< 60 = VM_PC_SVGA85,
	SII_DRV_TPG_VM__XGAI87          , //!< 61 = VM_PC_XGAI87,
	SII_DRV_TPG_VM__XGA60           , //!< 62 = VM_PC_XGA60,
	SII_DRV_TPG_VM__XGA70           , //!< 63 = VM_PC_XGA70,
	SII_DRV_TPG_VM__XGA75_1         , //!< 64 = VM_PC_XGA75_1,
	SII_DRV_TPG_VM__XGA85           , //!< 65 = VM_PC_XGA85,
	SII_DRV_TPG_VM__XGA75_2         , //!< 66 = VM_PC_XGA75_2,
	SII_DRV_TPG_VM__WXGA60_1        , //!< 67 = VM_PC_WXGA60_1,
	SII_DRV_TPG_VM__WXGA60_2        , //!< 68 = VM_PC_WXGA60_2,
	SII_DRV_TPG_VM__WXGA75          , //!< 69 = VM_PC_WXGA75,
	SII_DRV_TPG_VM__WXGA60_800      , //!< 70 = VM_PC_WXGA60_800,
	SII_DRV_TPG_VM__WXGA60_3        , //!< 71 = VM_PC_WXGA60_3,
	SII_DRV_TPG_VM__SXGA60          , //!< 72 = VM_PC_SXGA60,
	SII_DRV_TPG_VM__WSXGA60_1       , //!< 73 = VM_PC_WSXGA60_1,
	SII_DRV_TPG_VM__WSXGA60_2       , //!< 74 = VM_PC_WSXGA60_2,
	SII_DRV_TPG_VM__WXGA85_1        , //!< 75 = VM_PC_WXGA85_1,
	SII_DRV_TPG_VM__WXGA85_2        , //!< 76 = VM_PC_WXGA85_2,
	SII_DRV_TPG_VM__SXGA75          , //!< 77 = VM_PC_SXGA75,
	SII_DRV_TPG_VM__SXGA85          , //!< 78 = VM_PC_SXGA85,
	SII_DRV_TPG_VM__HD60            , //!< 79 = VM_PC_HD60,
	SII_DRV_TPG_VM__WSXGA60         , //!< 80 = VM_PC_WSXGA60,
	SII_DRV_TPG_VM__WSXGA75         , //!< 81 = VM_PC_WSXGA75,
	SII_DRV_TPG_VM__WSXGA50         , //!< 82 = VM_PC_WSXGA50,
	SII_DRV_TPG_VM__HD50_PLUS       , //!< 83 = VM_PC_HD50_PLUS,
	SII_DRV_TPG_VM__UXGA60          , //!< 84 = VM_PC_UXGA60,
	SII_DRV_TPG_VM__WSXGA50_PLUS    , //!< 85 = VM_PC_WSXGA50_PLUS,
	SII_DRV_TPG_VM__WUXGA50         , //!< 86 = VM_PC_WUXGA50,
	SII_DRV_TPG_VM__WUXGA60         , //!< 87 = VM_PC_WUXGA60,
	SII_DRV_TPG_VM__480I            , //!< 88 = VM_720X480i,
	SII_DRV_TPG_VM__576I            , //!< 89 = VM_720X576i,
	SII_DRV_TPG_VM__540X60		  , //!< 90 = VM_960X540,

	SII_DRV_TPG_VM__2160P50         , //!< 91 = vm_3840x2160p,
	SII_DRV_TPG_VM__2160P60         , //!< 92 = vm_3840x2160p,

	SII_DRV_TPG_VM__NONE              //!< No Video
} SiiDrvTpgVM_t;

typedef enum
{
	SII_DRV_TPG_INSTID__MAIN
} SiiDrvTpgInstId_t;

typedef struct
{
	SiiDrvCraAddr_t   baseAddr;    //!< Register base address
	SiiInst_t		  instTxCra;
} SiiDrvTpgConfig_t;

/************************************************************************/
/*  Function Prototype                                                  */
/************************************************************************/
SiiInst_t SiiDrvTpgCreate(SiiDrvTpgConfig_t* pConfig);
void SiiDrvTpgDelete(SiiInst_t inst);

void SiiDrvTpgModeSet(SiiInst_t inst, SiiDrvTpgMode_t mode );
//SiiDrvTpgMode_t SiiDrvTpgModeGet(void);

void SiiDrvTpgPatternSet(SiiInst_t inst, SiiDrvTpgPattern_t pattern );
//SiiDrvTpgPattern_t SiiDrvTpgPatternGet(void);

void SiiDrvTpgTimingSet(SiiInst_t inst, SiiDrvTpgVM_t vm);

#endif /* __SI_DRV_TPG_API_H__ */
