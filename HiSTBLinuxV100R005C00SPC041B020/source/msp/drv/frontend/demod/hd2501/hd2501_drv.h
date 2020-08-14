/*************************************************************
** file name:		hd2501_drv.h
** brief: 
** COPYRIGHT(C) HDIC
** -----------------------------------------------------------
** module information: 
** 
** -----------------------------------------------------------
** revision history 
** -----------------------------------------------------------
**	date 		version		action				author   
** -----------------------------------------------------------
**	2016-2-26 	1.0			created				   
** 
*************************************************************/
#include "hi_drv_tuner.h"

#ifndef __HD2501_DEMOD_H__
#define __HD2501_DEMOD_H__

#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/* include -------------------------------------------------*/

#include "drv_demod.h"

/* macro & structure ---------------------------------------*/


#define RX_Undefined 0xFF	/**< Not defined Value */

#define HDIC2501_I2C_ADDRESS				(0xEE)
#define HDIC2501_RESET_GPIO_GNUM            (0)
#define HDIC2501_RESET_GPIO_PNUM            (7)
#define HDIC2501_RESET_GPIO                 (HDIC2501_RESET_GPIO_GNUM * 8 + HDIC2501_RESET_GPIO_PNUM)

#define HDIC_GPIO_INPUT                     (1)
#define HDIC_GPIO_OUTPUT                    (0)
#define HDIC_GPIO_HIGH                      (1)
#define HDIC_GPIO_LOW                       (0)

#define HDIC_Demod_Print					HI_INFO_TUNER

//for TS Edge Select
#define TSEDGE_POSEDGE						0x01
#define TSEDGE_NEGEDGE						0x00

//for TS Serial Select
#define TS_PARALLEL							0x02
#define TS_SERIAL							0x00

//for TS NullPacket Select
#define NULLPACKET_ENABLED					0x08   
#define NULLPACKET_DELETED					0x00

//for TS TSorGS Select
#define TSORGS_MASK							0x80

#define TSORGS_TS							0x00 //Transport Stream
#define TSORGS_GS							0x80 //Gernal Stream

// Tuner slave address
#define HDIC_TUNER_DEV_ADDR_RDA581x			(0x18)	//RDA5812, RDA5815, RDA5815M, RDA5815S
#define HDIC_TUNER_DEV_ADDR_RDA5812			HDIC_TUNER_DEV_ADDR_RDA581x
#define HDIC_TUNER_DEV_ADDR_AV2026          (0xc6)	// 2018, 2028

//for SymbolRateAutoDetect Select
#define SYMBOLRATEAUTODETECT_AUTO			0x30
#define SYMBOLRATEAUTODETECT_MANUAL			0x00


/* Function return error code */

typedef enum 
{													
	DEMOD_TYPE_HD2818,
	DEMOD_TYPE_HD2501,
	DEMOD_TYPE_INVALID
} HDIC_DEMOD_Type_t;

typedef enum
{
    HDIC_TUNER_UNKNOWN,
    HDIC_TUNER_RDA5812,
    HDIC_TUNER_RDA5815,
    HDIC_TUNER_RDA5815M,
    HDIC_TUNER_AV2026,
    /*
    HDIC_TUNER_SCM5008L,
    HDIC_TUNER_SHARP6903,
    HDIC_TUNER_SCM5108L,
    HDIC_TUNER_SHARP6306,
    */
} HDIC_Abss_TunerType_t;



/* global & static variables -------------------------------*/
/* extern functions declaration ----------------------------*/

U8  tuner_register_write(U8 I2CSlaveAddr, U8 *data, U32 length);
U8  tuner_register_read(U8 I2CSlaveAddr, U8 sub_addr, U8 *data, U32 length);

void HDIC2501_Init(void);
void HDIC2501_Wait(U32 millisecond);
U8 HDIC2501_SetTuner(U32 Frequency,U32 sysm);
U8 HDIC2501_SetParameters(U8 SymbolRateAutoDetect,U32 SymbolRate_Hz);
U8 HDIC2501_SoftReset(void);
U8 HDIC2501_IsDemodLocked(U8 *locked, U8 *Status);
//U8 HDIC2501_GetSignalBER(double *pSignalBER);
U8 HDIC2501_GetSignalBER(U32 *pSignalBER);
//U8 HDIC2501_GetSignalSNR(double *SignalSNR);
U8 HDIC2501_GetSignalSNR(U32 *SignalSNR);
U8 HDIC2501_GetFieldStrength(U16 *FieldStrength);
U8 HDIC2501_Demod_AutoCheck( void ) ;
U8 HDIC2501_TunerAutoCheck(HI_U32 u32TunerPort);
U8 HDIC2501_SetPolar( HI_UNF_TUNER_FE_POLARIZATION_E Polar );
U8 HDIC2501_Standby(void);
U8 HDIC2501_WakeUp(void);



/* static functions declaration ----------------------------*/
/* functions define ----------------------------------------*/


/* static functions definition -----------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif  //#define __HD2501_DEMOD_H__

