/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

#ifndef __CEC_H__
#define __CEC_H__

#include "si_typedefs.h"

typedef enum
{
    SiI_CEC_API_Version   = 0x00,
    SiI_CEC_API_Revision  = 0x00,
    SiI_CEC_API_Build     = 0x01
} eSiI_CEC_VerInfo_t;

#define SII_MAX_CMD_SIZE 16 // defnes number of operands

typedef enum
{
    SiI_CEC_LogAddr_TV          = 0x00,
    SiI_CEC_LogAddr_RecDev1     = 0x01,
    SiI_CEC_LogAddr_RecDev2     = 0x02,
    SiI_CEC_LogAddr_STB1        = 0x03,
    SiI_CEC_LogAddr_DVD1        = 0x04,
    SiI_CEC_LogAddr_AudSys      = 0x05,
    SiI_CEC_LogAddr_STB2        = 0x06,
    SiI_CEC_LogAddr_STB3        = 0x07,
    SiI_CEC_LogAddr_DVD2        = 0x08,
    SiI_CEC_LogAddr_RecDev3     = 0x09,
    SiI_CEC_LogAddr_Res1        = 0x0A,
    SiI_CEC_LogAddr_Res2        = 0x0B,
    SiI_CEC_LogAddr_Res3        = 0x0C,
    SiI_CEC_LogAddr_Res4        = 0x0D,
    SiI_CEC_LogAddr_FreeUse     = 0x0E,
    SiI_CEC_LogAddr_Unregister  = 0x0F,
    SiI_CEC_LogAddr_BUTT
} SiI_CEC_LogAddr_t;

#define CEC_PA_EDID_CH0      0x1000
#define CEC_PA_EDID_CH1      0x2000
#define CEC_PA_EDID_CH2      0x3000
#define CEC_PA_EDID_CH3      0x4000

typedef struct
{
    HI_U8 bCount;
    HI_U8 bRXNextCount;
    HI_U8 bDestOrRXHeader;
    HI_U8 bOpcode;
    HI_U8 bOperand[ SII_MAX_CMD_SIZE ];
} SiI_CEC_t;

typedef enum
{
    SiI_CEC_ERROR_STARTBITIRREGULAR = 0x01,
    SiI_CEC_ERROR_SHORTPLUSEDETECT = 0x04,
    SiI_CEC_ERROR_RXOVERFLOW = 0x08,
} SI_CEC_ERRORS_E;

typedef enum
{
    SI_CEC_TX_WAITCMD  = 0,    //CEC TX Wait message to Send Status
    SI_CEC_TX_SENDING,         //CEC TX Sending message
    SI_CEC_TX_SENDACKED,       //CEC TX Send and get Acked.
    SI_CEC_TX_SENDFAILURE      //CEC TX Send Failure
} SI_CEC_TX_STATUS_S;

typedef struct
{
    HI_U8 bRXState;              //CEC RX Status
    HI_U8 bTXState;              //CEC TX Status
    HI_U8 bCECErrors;            //CEC Error message
} SiI_CEC_Int_t;

HI_U8 SI_CEC_CAPTURE_ID_Set( HI_U8 );
void SI_SiI_CEC_SendPing ( HI_U8 );
HI_U8 SiI_CEC_SetCommand( SiI_CEC_t * );
HI_U8 SI_CEC_GetCommand( SiI_CEC_t * );
HI_U8 SI_CEC_Interrupt_Processing ( SiI_CEC_Int_t * );

HI_U32 SI_CEC_AudioPing(HI_U32 *pu32Status);

HI_U32 SI_CEC_Open( HI_U8 );
HI_U32 SI_CEC_Close(void);

//=============================================================================
// C E C   M e s s a g e   I n f o r m a t i o n   L o o k  u p  T a b l e
//=============================================================================
/* Request Opcode */
#define RP (8)
/* Reply Opcode   */
#define RQ (4)
/* Direct Addressing Mode    */
#define DA (2)
/* Broadcast Addressing Mode */
#define BC (1)

typedef enum
{
  CEC_REQST_DA_E = (RQ | DA),
  CEC_REQST_BC_E = (RQ | BC),
  CEC_REQST_BOTH_DA_BC = (RQ | BC | DA),
  CEC_REPLY_DA_E = (RP|DA),
  CEC_REPLY_BC_E = (RP|DA),
  CEC_REPLY_BOTH_DA_BC = (RP|DA|BC),
  CEC_BOTH_REQ_DA_REP_DA = (RP|RQ|DA),
  CEC_BOTH_REQ_BC_REP_BC = (RP|RQ|BC),
  CEC_BOTH_REQ_REP_BOTH_DA_BC =(RP|RQ|DA|BC)
} CEC_RX_MSG_TYPE_ET;

typedef void (*CEC_MSG_HANDLER)( SiI_CEC_t * SiI_CEC );

typedef struct
{
  HI_U8               opcode ;
  HI_U8               num_operand ;
  CEC_RX_MSG_TYPE_ET  msg_type ;
  CEC_MSG_HANDLER     opcode_handler ;
} LUT_CEC_MSG_INFO;

//
//  Used to collect events that will be executed by the Event Handler.
//  Used in an array for deeper buffering.
//
typedef struct
{
  HI_U8  cec_event_id ;
  HI_U8  param[5] ;
  SiI_CEC_t * SiI_CEC ;
} EVENT_DESCRIPTOR;

typedef struct
{
  HI_U8                  cec_event_id ;
  CEC_MSG_HANDLER     cec_event_handler ;
} LUT_CEC_EVENT_HANDLER;

typedef enum
{
  DSCRPTR_EMPTY,  // Descriptor is available for use.
  DSCRPTR_BUSY,   // Descriptor is taken and is being filled/updated.
  DSCRPTR_FULL,   // Descriptor is taken and is full/ready for use.
} DESCRIPTOR_STATUS;

void SI_CEC_Event_Handler( void );

typedef enum 
{
    SiI_CEC_Idle,
    SiI_CEC_ReqPing,
    SiI_CEC_ReqCmd1,
    SiI_CEC_ReqCmd2,
    SiI_CEC_ReqCmd3
} SiI_CECOp_t;

//------------------------------------------------------------------------------
// Function Prototypes for register access routines
//------------------------------------------------------------------------------
HI_U8 SI_CEC_RegisterRead(HI_U16 regAddr);
void SI_CEC_RegisterWrite(HI_U16 regAddr, HI_U8 value);
void SI_CEC_RegisterModify(HI_U16 regAddr, HI_U8 mask, HI_U8 value);
void SI_CEC_RegisterBitToggle(HI_U16 regAddr, HI_U8 mask);

void SI_CEC_RegisterReadBlock(HI_U16 regAddr, HI_U8* buffer, HI_U8 length);
void SI_CEC_RegisterWriteBlock(HI_U16 regAddr, HI_U8* buffer, HI_U8 length);

HI_U32 SI_CEC_SendCommand(HI_UNF_HDMI_CEC_CMD_S  *pCECCmd);



HI_VOID SI_CEC_SetUp(void);

#endif // __CEC_H__



