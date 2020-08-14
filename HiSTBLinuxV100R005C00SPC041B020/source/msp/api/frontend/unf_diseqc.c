/******************************************************************************

  Copyright (C), 2012, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : unf_diseqc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/02/25
  Description   :
  History       :
  1.Date        : 2012/02/25
    Author      : l00185424
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief For DiSEqC V4.2.
 */

/*----------------------------- INCLUDE FILES ------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_frontend.h"
//#include "drv_tuner_ext.h"
#include "drv_tuner_ioctl.h"

/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/

#define UNF_TUNER_NUM 5

#define LO_FREQUENCY_L (0x0)
#define LO_FREQUENCY_H (0x1)
#define POLARISATION_V (0x0)
#define POLARISATION_H (0x2)
/*#define SAT_POSITION_A (0x0)
#define SAT_POSITION_B (0x4)
#define SWITCH_OPTION_A (0x0)
#define SWITCH_OPTION_B (0x8)*/
#define PORT_GROUP_CLR_BITS (0xF0)

//#define DISEQC_CMD_LEN (6)

#define FRAMING_BYTE (0)
#define ADDRESS_BYTE (1)
#define COMMAND_BYTE (2)
#define DATA_BYTE_0 (3)
#define DATA_BYTE_1 (4)
#define DATA_BYTE_2 (5)

#define MAX_LONGITUDE (1800)
#define MAX_LATITUDE (900)
#define ANGLE_MULTIPLE (10.0)
 
#define PI (3.14159265)
#define USALS_CONST_1 ((double)3561680000.0)
#define USALS_CONST_2 ((double)180.0)
#define USALS_CONST_3 ((double)6370.0)
#define USALS_CONST_4 ((double)40576900.0)
#define USALS_CONST_5 ((double)1821416900.0)
#define USALS_CONST_6 ((double)537628000.0)

#define DISEQC_DELAY_BETWEEN_CMD_MS (25)
#define MOTOR_RUNNING_STEPS_MIN		(0)
#define MOTOR_RUNNING_STEPS_MAX		(128)

#ifndef ABS
 #define ABS(arg) ((arg) < 0 ? -(arg) : (arg))
#endif

#define FORMAT_DISEQC_CMD(a, F, A, C) \
    { \
        a[FRAMING_BYTE] = F; \
        a[ADDRESS_BYTE] = A; \
        a[COMMAND_BYTE] = C; \
    }

#define FORMAT_DISEQC_CMD_VALUE(a, F, A, C, aD, L) \
    { \
        int i; \
        a[FRAMING_BYTE] = F; \
        a[ADDRESS_BYTE] = A; \
        a[COMMAND_BYTE] = C; \
        for (i = 0; i < L; i++){  a[DATA_BYTE_0 + i] = ((HI_U8*)aD)[i]; } \
    }

/*-------------------- STATIC STRUCTURE DECLARATIONS -----------------------------------*/

/* DiSEqC Framing code */
//typedef enum
//{
#define MASTER_NOREPLY_FIRST       0xE0
//#define MASTER_NOREPLY_REPEAT      0xE1
#define MASTER_REPLY_FIRST         0xE2
/*#define MASTER_REPLY_REPEAT        0xE3
#define SLAVE_OK                   0xE4
#define SLAVE_NOT_SUPPORT          0xE5
#define SLAVE_PARITY_ERROR         0xE6
#define SLAVE_BAD_COMMAND          0xE7*/
//} DISEQC_FRAMING_E;

/* DiSEqC Device address */
//typedef enum
//{
#define DEVICE_ANY                      0x00
#define DEVICE_ANY_LNB_SW_SMATV         0x10
/*#define DEVICE_LNB                      0x11
#define DEVICE_LNB_LOOPTHROUGH          0x12
#define DEVICE_SW                       0x14
#define DEVICE_SW_LOOPTHROUGH           0x15
#define DEVICE_SMATV                    0x18
#define DEVICE_ANY_POLARISER            0x20
#define DEVICE_LINEAR_POLARISER         0x21
#define DEVICE_ANY_POSITIONER           0x30*/
#define DEVICE_AZIMUTH_POSITIONER       0x31
/*#define DEVICE_ELEVATION_POSITIONER     0x32
#define DEVICE_ANY_INSTALLER_AID        0x40
#define DEVICE_SIGNAL_STRENGTH          0x41
#define DEVICE_ANY_INTELLIGENT          0x70
#define DEVICE_SUBSCRIBER_CONTROL       0x71
#define DEVICE_BUTT                     0x72*/       
//} DISEQC_DEVICE_E;

/* DiSEqC Command */
//typedef enum
//{
        /* M*R/1.0 */
#define CMD_RESET             0x00       /**<Reset DiSEqC microcontroller*/
        /* R/2.0 */
//#define CMD_CLR_RESET         0x01   /**<Clear the "Reset" flag*/
        /* R/1.0 */
#define CMD_STANDBY           0x02    /**<Switch peripheral power supply off*/
#define CMD_POWER_ON          0x03    /**<Switch peripheral power supply on*/
        /* S/2.0 */
//#define CMD_SET_CONTEND       0x04 /**<Set Contention flag*/
//#define CMD_CONTEND           0x05     /**<Return address only if Contention flag is set*/
        /* R/2.0 */
//#define CMD_CLR_CONTEND       0x06 /**<Clear Contention flag*/
        /* S/2.0 */
//#define CMD_ADDRESS           0x07     /**<Return address unless Contention flag is set*/
//#define CMD_MOVE_C            0x08      /**<Change address only if Contention flag is set*/
        /* R/2.0 */
#if 0
#define CMD_MOVE              0x90    /**<Change address unless Contention flag is set*/
#define CMD_STATUS            0x10    /**<Read Status register flags*/
#define CMD_CONFIG            0x11    /**<Read Configuration flags (peripheral hardware)*/
#define CMD_SWITCH_0          0x14    /**<Read Switching state flags (Committed port)*/
#define CMD_SWITCH_1          0x15    /**<Read Switching state flags (Uncommitted port)*/
#define CMD_SWITCH_2          0x16
#define CMD_SWITCH_3          0x17
 
        /* R/1.0 */
#define CMD_SET_LO            0x20       /**<Select the Low Local Oscillator frequency*/
#define CMD_SET_VR            0x21       /**<Select Vertical Polarisation (or Right circular)*/
#define CMD_SET_POS_A         0x22    /**<Select Satellite position A (or position C)*/
#define CMD_SET_S0_A          0x23     /**<Select Switch Option A (e.g. positions A/B)*/
#define CMD_SET_HI            0x24       /**<Select the High Local Oscillator frequency*/
#define CMD_SET_HL            0x25       /**<Select Horizontal Polarisation (or Left circular)*/
#define CMD_SET_POS_B         0x26    /**<Select Satellite position B (or position D)*/
#define CMD_SET_S0_B          0x27     /**<Select Switch Option B (e.g. positions C/D)*/
        /* R/1.1 */                
#define CMD_SET_S1_A          0x28     /**<Select switch S1 input A (deselect input B)*/
#define CMD_SET_S2_A          0x29     /**<Select switch S2 input A (deselect input B)*/
#define CMD_SET_S3_A          0x2A     /**<Select switch S3 input A (deselect input B)*/
#define CMD_SET_S4_A          0x2B     /**<Select switch S4 input A (deselect input B)*/
#define CMD_SET_S1_B          0x2C     /**<Select switch S1 input B (deselect input A)*/
#define CMD_SET_S2_B          0x2D     /**<Select switch S2 input B (deselect input A)*/
#define CMD_SET_S3_B          0x2E     /**<Select switch S3 input B (deselect input A)*/
#define CMD_SET_S4_B          0x2F     /**<Select switch S4 input B (deselect input A)*/
#define CMD_SLEEP             0x30        /**<Ignore all bus commands except "Awake"*/
#define CMD_AWAKE             0x31        /**<Respond to future bus commands normally*/
#endif
        /* M/1.0 */
#define CMD_WRITE_N0          0x38    /**<Write to Port group 0 (Committed switches)*/
        /* M/1.1 */           
#define CMD_WRITE_N1          0x39    /**<Write to Port group 1 (Uncommitted switches)*/
//#define CMD_WRITE_N2          0x3A
//#define CMD_WRITE_N3          0x3B
 
        /* S/2.0 */
#if 0
#define CMD_READ_A0           0x40      /**<Read Analogue value A0*/
#define CMD_READ_A1           0x41      /**<Read Analogue value A1*/
        /* R/1.2 */
#define CMD_WRITE_A0          0x48    /**<Write Analogue value A0 (e.g. Skew)*/
        /* S/1.2 */          
#define CMD_WRITE_A1          0x49    /**<Write Analogue value A1*/
        /* S/2.0 */
#define CMD_LO_STRING         0x50   /**<Read current frequency [Reply = BCD string]*/
        /* R/2.0 */
#define CMD_LO_NOW            0x51      /**<Read current frequency table entry number*/
        /* S/2.0 */
#define CMD_LO_LO             0x52       /**<Read Lo frequency table entry number*/
#define CMD_LO_HI             0x53       /**<Read Hi frequency table entry number*/
#endif
        /* M/1.1 */
//#define CMD_WRITE_RREQ        0x58  /**<Write channel frequency (BCD string)*/
//#define CMD_CH_NO             0x59       /**<Write (receiver's) selected channel number*/
        /* M/1.2 */
#define CMD_HALT              0x60        /**<Stop Positioner movement*/
#define CMD_LIMITS_OFF        0x63  /**<Disable Limits*/
        /* R/2.2 */
//#define CMD_POS_STAT          0x64    /**<Read Positioner Status Register*/
        /* M/1.2 */
#define CMD_LIMIT_E           0x66     /**<Set East Limit (& Enable recommended)*/
#define CMD_LIMIT_W           0x67     /**<Set West Limit (& Enable recommended)*/
#define CMD_DRIVE_EAST        0x68  /**<Drive Motor East (with optional timeout/steps)*/
#define CMD_DRIVE_WEST        0x69  /**<Drive Motor West (with optional timeout/steps)*/
#define CMD_STORE_NN          0x6A    /**<Store Satellite Position & Enable Limits*/
#define CMD_GOTO_NN           0x6B     /**<Drive Motor to Satellite Position nn*/
#define CMD_GOTO_XX           0x6E     /**<Drive Motor to Angular Position*/
        /* R/1.2 */
#define CMD_SET_POSNS         0x6F   /**<(Re-)Calculate Satellite Positions*/
//#define CMD_BUTT
//} DISEQC_CMD_E;

typedef struct
{
    HI_UNF_TUNER_DISEQC_SWITCH4PORT_S stPort1_0;
    HI_UNF_TUNER_DISEQC_SWITCH16PORT_S stPort1_1;
} DISEQC_STATUS_S;

/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

extern HI_UNF_TUNER_SWITCH_TONEBURST_E TUNER_DISEQC_GetToneBurstStatus(HI_U32 u32TunerId);

/*------------------------- STATIC DECLARATIONS ----------------------------------------*/

static DISEQC_STATUS_S s_stDiSEqCStatus[UNF_TUNER_NUM] =
{
    {
        .stPort1_0 = 
        {
            HI_UNF_TUNER_DISEQC_LEVEL_1_X, 
            HI_UNF_TUNER_DISEQC_SWITCH_NONE,
            HI_UNF_TUNER_FE_POLARIZATION_H, 
            HI_UNF_TUNER_FE_LNB_22K_OFF
        },
        .stPort1_1 = 
        {
            HI_UNF_TUNER_DISEQC_LEVEL_1_X, 
            HI_UNF_TUNER_DISEQC_SWITCH_NONE
        }
    },
    {
        .stPort1_0 = 
        {
            HI_UNF_TUNER_DISEQC_LEVEL_1_X, 
            HI_UNF_TUNER_DISEQC_SWITCH_NONE,
            HI_UNF_TUNER_FE_POLARIZATION_H, 
            HI_UNF_TUNER_FE_LNB_22K_OFF
        },
        .stPort1_1 = 
        {
            HI_UNF_TUNER_DISEQC_LEVEL_1_X, 
            HI_UNF_TUNER_DISEQC_SWITCH_NONE
        }
    },
    {
        .stPort1_0 = 
        {
            HI_UNF_TUNER_DISEQC_LEVEL_1_X, 
            HI_UNF_TUNER_DISEQC_SWITCH_NONE,
            HI_UNF_TUNER_FE_POLARIZATION_H, 
            HI_UNF_TUNER_FE_LNB_22K_OFF
        },
        .stPort1_1 = 
        {
            HI_UNF_TUNER_DISEQC_LEVEL_1_X, 
            HI_UNF_TUNER_DISEQC_SWITCH_NONE
        }
    }
};

/*------------------------------------ CODE --------------------------------------------*/
extern HI_S32 TUNER_DISEQC_SendRecvMessage(HI_U32 u32TunerId,
                                           const HI_UNF_TUNER_DISEQC_SENDMSG_S * pstSendMsg,
                                           HI_UNF_TUNER_DISEQC_RECVMSG_S * pstRecvMsg);

static HI_S32 DISEQC_SendCmd1_0(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_SWITCH4PORT_S* pstPara)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    //HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8PortGroupBits;
    HI_U8 u8CMD;
    HI_U8 u8Framing;
    HI_U8 u8Polar;
    HI_U8 u8LOFreq;

    /* Polarization */
    if ((HI_UNF_TUNER_FE_POLARIZATION_V == pstPara->enPolar)
       || (HI_UNF_TUNER_FE_POLARIZATION_R == pstPara->enPolar))
    {
        u8Polar = POLARISATION_V;
    }
    else
    {
        u8Polar = POLARISATION_H;
    }

    /* LO, 22K */
    u8LOFreq = (HI_UNF_TUNER_FE_LNB_22K_ON == pstPara->enLNB22K) ? LO_FREQUENCY_H : LO_FREQUENCY_L;

    /* Init parameter */
    memset(&stSendMsg, 0, sizeof(stSendMsg));
    //memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    stSendMsg.enLevel = pstPara->enLevel;
    stSendMsg.enToneBurst = TUNER_DISEQC_GetToneBurstStatus(u32TunerId);
    stSendMsg.u8Length = 4;
    stSendMsg.u8RepeatTimes = 0;

    u8Framing = (pstPara->enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;

    /* Send WRITE N0(0x38) command */
    u8PortGroupBits = PORT_GROUP_CLR_BITS | (((HI_U8)(pstPara->enPort - 1)) << 2) | u8Polar | u8LOFreq;
    u8CMD = CMD_WRITE_N0;
    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, u8Framing, DEVICE_ANY_LNB_SW_SMATV, u8CMD, &u8PortGroupBits, 1);

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, NULL/*&stRecvMsg*/);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send WRITE N0 fail.\n");
        return s32Ret;
    }

    /* If support level 2.x, handle received message here. */

    return HI_SUCCESS;
}

static HI_S32 DISEQC_SendCmd1_1(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_SWITCH16PORT_S* pstPara)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    //HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8PortGroupBits;
    HI_U8 u8CMD;
    HI_U8 u8Framing;

    /* Init parameter */
    memset(&stSendMsg, 0, sizeof(stSendMsg));
    //memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    stSendMsg.enLevel = pstPara->enLevel;
    stSendMsg.enToneBurst = TUNER_DISEQC_GetToneBurstStatus(u32TunerId);
    stSendMsg.u8Length = 4;
    stSendMsg.u8RepeatTimes = 0;

    u8Framing = (pstPara->enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;

    /* Send WRITE N1(0x39) command */
    u8PortGroupBits = PORT_GROUP_CLR_BITS | (pstPara->enPort - 1);
    u8CMD = CMD_WRITE_N1;
    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, u8Framing, DEVICE_ANY_LNB_SW_SMATV, u8CMD, &u8PortGroupBits, 1);

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, NULL/*&stRecvMsg*/);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send WRITE N1 fail.\n");
        return s32Ret;
    }

    /* If support level 2.x, handle received message here. */    

    return HI_SUCCESS;
}


HI_S32 HI_UNF_TUNER_DISEQC_Switch4Port(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_SWITCH4PORT_S* pstPara)
{
    HI_S32 s32Ret;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstPara->enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", pstPara->enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }
    
    if (HI_UNF_TUNER_DISEQC_SWITCH_PORT_5 <= pstPara->enPort)
    {
        HI_ERR_TUNER("Switch port invalid: %d\n", pstPara->enPort);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    /* Save port parameter */
    s_stDiSEqCStatus[u32TunerId].stPort1_0 = *pstPara;

    /* If NONE, only save. */
    if (HI_UNF_TUNER_DISEQC_SWITCH_NONE == pstPara->enPort)
    {
        return HI_SUCCESS;
    }

    /* If use 4port device, other parameter must be valid. */
    if (HI_UNF_TUNER_FE_POLARIZATION_BUTT <= pstPara->enPolar)
    {
        HI_ERR_TUNER("Polarization invalid: %d\n", pstPara->enPolar);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_FE_LNB_22K_BUTT <= pstPara->enLNB22K)
    {
        HI_ERR_TUNER("LNB 22K invalid: %d\n", pstPara->enLNB22K);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    /* Switch 1.0 */
    s32Ret = DISEQC_SendCmd1_0(u32TunerId, pstPara);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send cmd 1.0 fail.\n");
        return s32Ret;
    }

    /*
     * If has switch 1.1, set it here.
     * Support Tuner - DiSEqC1.0 - DiSEqC1.1 - Other switch - LNB cascaded.
     */
    if (HI_UNF_TUNER_DISEQC_SWITCH_NONE != s_stDiSEqCStatus[u32TunerId].stPort1_1.enPort)
    {
        usleep(DISEQC_DELAY_BETWEEN_CMD_MS * 1000);
        s32Ret = DISEQC_SendCmd1_1(u32TunerId, &(s_stDiSEqCStatus[u32TunerId].stPort1_1));
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("Send cmd 1.1 fail.\n");
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_DISEQC_Switch16Port(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_SWITCH16PORT_S* pstPara)
{
    HI_S32 s32Ret;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstPara->enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", pstPara->enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }
    
    if (HI_UNF_TUNER_DISEQC_SWITCH_PORT_BUTT <= pstPara->enPort)
    {
        HI_ERR_TUNER("Switch port invalid: %d\n", pstPara->enPort);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    /* Save port parameter */
    s_stDiSEqCStatus[u32TunerId].stPort1_1 = *pstPara;

    /* If NONE, return. */
    if (HI_UNF_TUNER_DISEQC_SWITCH_NONE == pstPara->enPort)
    {
        return HI_SUCCESS;
    }

    /* If have 1.0 switch, set if first */
    if (HI_UNF_TUNER_DISEQC_SWITCH_NONE != s_stDiSEqCStatus[u32TunerId].stPort1_0.enPort)
    {
        s32Ret = DISEQC_SendCmd1_0(u32TunerId, &(s_stDiSEqCStatus[u32TunerId].stPort1_0));
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("Send cmd 1.0 fail.\n");
            return s32Ret;
        }

        usleep(DISEQC_DELAY_BETWEEN_CMD_MS * 1000);
    }

    /* Switch 1.1 */
    s32Ret = DISEQC_SendCmd1_1(u32TunerId, pstPara);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send cmd 1.1 fail.\n");
        return s32Ret;
    }

    /*
     * If have 1.0 switch, repeat.
     * Support Tuner - DiSEqC1.1 - DiSEqC1.0 - Other switch - LNB cascaded.
     */
    if (HI_UNF_TUNER_DISEQC_SWITCH_NONE != s_stDiSEqCStatus[u32TunerId].stPort1_0.enPort)
    {
        usleep(DISEQC_DELAY_BETWEEN_CMD_MS * 1000);
        s32Ret = DISEQC_SendCmd1_0(u32TunerId, &(s_stDiSEqCStatus[u32TunerId].stPort1_0));
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("Send cmd 1.0 fail.\n");
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_DISEQC_StorePos(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_POSITION_S *pstPara)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;
    HI_U8 u8Pos;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstPara->enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", pstPara->enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (DISEQC_MAX_MOTOR_PISITION < pstPara->u32Pos)
    {
        HI_ERR_TUNER("Input parameter(pstPara->u32Pos) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (pstPara->enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;
    u8Pos = (HI_U8)pstPara->u32Pos;

    stSendMsg.enLevel = pstPara->enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, u8Framing, DEVICE_AZIMUTH_POSITIONER, CMD_STORE_NN, &u8Pos, 1);
    stSendMsg.u8Length = 4;
    stSendMsg.u8RepeatTimes = 0;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send Store N fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_GotoPos(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_POSITION_S *pstPara)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;
    HI_U8 u8Pos;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstPara->enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", pstPara->enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (pstPara->u32Pos > DISEQC_MAX_MOTOR_PISITION)
    {
        HI_ERR_TUNER("Input parameter(pstPara->u32Pos) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (pstPara->enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;
    u8Pos = (HI_U8)pstPara->u32Pos;

    stSendMsg.enLevel = pstPara->enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, u8Framing, DEVICE_AZIMUTH_POSITIONER, CMD_GOTO_NN, &u8Pos, 1);
    stSendMsg.u8Length = 4;
    stSendMsg.u8RepeatTimes = 0;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send Goto N fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_SetLimit(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_LIMIT_S* pstPara)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;
    HI_U8 u8CMD;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstPara->enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", pstPara->enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_DISEQC_LIMIT_BUTT <= pstPara->enLimit)
    {
        HI_ERR_TUNER("Input parameter(pstPara->enLimit) invalid: %d\n", pstPara->enLimit);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (pstPara->enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;
    switch (pstPara->enLimit)
    {
    case HI_UNF_TUNER_DISEQC_LIMIT_OFF:
    default:
        u8CMD = CMD_LIMITS_OFF;
        break;

    case HI_UNF_TUNER_DISEQC_LIMIT_EAST:
        u8CMD = CMD_LIMIT_E;
        break;

    case HI_UNF_TUNER_DISEQC_LIMIT_WEST:
        u8CMD = CMD_LIMIT_W;
        break;
    }

    stSendMsg.enLevel = pstPara->enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    FORMAT_DISEQC_CMD(stSendMsg.au8Msg, u8Framing, DEVICE_AZIMUTH_POSITIONER, u8CMD);
    stSendMsg.u8Length = 3;
    stSendMsg.u8RepeatTimes = 0;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send limit cmd fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_Move(HI_U32 u32TunerId,  HI_UNF_TUNER_DISEQC_MOVE_S* pstPara)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;
    HI_U8 u8CMD;
    HI_U8 u8Value;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstPara->enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", pstPara->enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_DISEQC_MOVE_DIR_BUTT <= pstPara->enDir)
    {
        HI_ERR_TUNER("Input parameter(pstPara->enDir) invalid, %d\n", pstPara->enDir);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_DISEQC_MOVE_TYPE_BUTT <= pstPara->enType)
    {
        HI_ERR_TUNER("Input parameter(pstPara->enType) invalid, %d\n", pstPara->enType);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (pstPara->enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;

    /* Direction */
    switch (pstPara->enDir)
    {
    case HI_UNF_TUNER_DISEQC_MOVE_DIR_EAST:
    case HI_UNF_TUNER_DISEQC_MOVE_DIR_BUTT:
    default:
        u8CMD = CMD_DRIVE_EAST;
        break;

    case HI_UNF_TUNER_DISEQC_MOVE_DIR_WEST:
        u8CMD = CMD_DRIVE_WEST;
        break;
    }

    /* Value */
    switch (pstPara->enType)
    {
    case HI_UNF_TUNER_DISEQC_MOVE_STEP_SLOW:
    case HI_UNF_TUNER_DISEQC_MOVE_TYPE_BUTT:
    default:
        u8Value = 0xff;
        break;
	case HI_UNF_TUNER_DISEQC_MOVE_STEP_SLOW1:
		u8Value = 0xfe;
		break;
	case HI_UNF_TUNER_DISEQC_MOVE_STEP_SLOW2:
		u8Value = 0xfd;
		break;
	case HI_UNF_TUNER_DISEQC_MOVE_STEP_SLOW3:
		u8Value = 0xfc;
		break;
	case HI_UNF_TUNER_DISEQC_MOVE_STEP_SLOW4:
		u8Value = 0xfb;
		break;

    case HI_UNF_TUNER_DISEQC_MOVE_STEP_FAST:
        u8Value = 0xfa;
        break;
	case HI_UNF_TUNER_DISEQC_MOVE_STEP_FAST1:
        u8Value = 0xf9;
        break;
	case HI_UNF_TUNER_DISEQC_MOVE_STEP_FAST2:
        u8Value = 0xf8;
        break;
	case HI_UNF_TUNER_DISEQC_MOVE_STEP_FAST3:
        u8Value = 0xf7;
        break;
	case HI_UNF_TUNER_DISEQC_MOVE_STEP_FAST4:
        u8Value = 0xf6;
        break;

    case HI_UNF_TUNER_DISEQC_MOVE_CONTINUE:
        u8Value = 0x00;
        break;
    }

    stSendMsg.enLevel = pstPara->enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, u8Framing, DEVICE_AZIMUTH_POSITIONER, u8CMD, &u8Value, 1);
    stSendMsg.u8Length = 4;
    stSendMsg.u8RepeatTimes = 0;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send drive cmd fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_RunStep(HI_U32 u32TunerId,  HI_UNF_TUNER_DISEQC_RUN_S* pstPara)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;
    HI_U8 u8CMD;
    HI_U8 u8Value;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstPara->enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", pstPara->enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_DISEQC_MOVE_DIR_BUTT <= pstPara->enDir)
    {
        HI_ERR_TUNER("Input parameter(pstPara->enDir) invalid, %d\n", pstPara->enDir);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (MOTOR_RUNNING_STEPS_MAX < pstPara->u32RunningSteps)
    {
        HI_ERR_TUNER("Input parameter(pstPara->u32RunningSteps) invalid, %d\n", pstPara->u32RunningSteps);
        return HI_ERR_TUNER_INVALID_PARA;
    }

	switch(pstPara->enLevel)
	{
		case HI_UNF_TUNER_DISEQC_LEVEL_1_X:
			HI_INFO_TUNER("Level 1_x.\n");
			break;
		case HI_UNF_TUNER_DISEQC_LEVEL_2_X:
			HI_INFO_TUNER("Level 2_x.\n");
			break;
		case HI_UNF_TUNER_DISEQC_LEVEL_BUTT:
			HI_INFO_TUNER("Level init err.\n");
			break;
	}

	switch(pstPara->enDir)
	{
		case HI_UNF_TUNER_DISEQC_MOVE_DIR_EAST:
			HI_INFO_TUNER("move east.\n");
			break;
		case HI_UNF_TUNER_DISEQC_MOVE_DIR_WEST:
			HI_INFO_TUNER("move west.\n");
			break;
        case HI_UNF_TUNER_DISEQC_MOVE_DIR_BUTT:
			HI_INFO_TUNER("move err.\n");
			break;
	}


	HI_INFO_TUNER("Running steps:%d\n", pstPara->u32RunningSteps);
	

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (pstPara->enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;

    /* Direction */
    switch (pstPara->enDir)
    {
    case HI_UNF_TUNER_DISEQC_MOVE_DIR_EAST:
    case HI_UNF_TUNER_DISEQC_MOVE_DIR_BUTT:
    default:
        u8CMD = CMD_DRIVE_EAST;
        break;

    case HI_UNF_TUNER_DISEQC_MOVE_DIR_WEST:
        u8CMD = CMD_DRIVE_WEST;
        break;
    }

	/* Value */
	if(MOTOR_RUNNING_STEPS_MIN == pstPara->u32RunningSteps)
	{
		u8Value = 0;
	}
	else
	{
		/* run 1 step every time,u8Value = 0xff
		 * run 2 step every time,u8Value = 0xfe
		 * run 3 step every time,u8Value = 0xfd
		 * run 4 step every time,u8Value = 0xfc
		 * run 5 step every time,u8Value = 0xfb
		 * run 6 step every time,u8Value = 0xfa
		 * run 7 step every time,u8Value = 0xf9
		 * run 8 step every time,u8Value = 0xf8
		*/
		u8Value = 0xff - pstPara->u32RunningSteps + 1;
	}

    stSendMsg.enLevel = pstPara->enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, u8Framing, DEVICE_AZIMUTH_POSITIONER, u8CMD, &u8Value, 1);
    stSendMsg.u8Length = 4;
    stSendMsg.u8RepeatTimes = 0;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send drive cmd fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_Stop(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;

    stSendMsg.enLevel = enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    FORMAT_DISEQC_CMD(stSendMsg.au8Msg, u8Framing, DEVICE_AZIMUTH_POSITIONER, CMD_HALT);
    stSendMsg.u8Length = 3;
    stSendMsg.u8RepeatTimes = 0;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send halt fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_Recalculate(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_RECALCULATE_S* pstPara)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstPara->enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", pstPara->enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (pstPara->enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;
    stSendMsg.enLevel = pstPara->enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    stSendMsg.au8Msg[FRAMING_BYTE] = u8Framing;
    stSendMsg.au8Msg[ADDRESS_BYTE] = DEVICE_AZIMUTH_POSITIONER;
    stSendMsg.au8Msg[COMMAND_BYTE] = CMD_SET_POSNS;
    stSendMsg.au8Msg[DATA_BYTE_0] = pstPara->u8Para1;
    stSendMsg.au8Msg[DATA_BYTE_1] = pstPara->u8Para2;
    stSendMsg.au8Msg[DATA_BYTE_2] = pstPara->u8Para3;

	if ((0 == pstPara->u8Para2) && (0 == pstPara->u8Para3))
	{
		stSendMsg.u8Length = 4;
	}
	else
	{
		stSendMsg.u8Length = 6;
	}
    stSendMsg.u8RepeatTimes = 0;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send Set Posns fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_CalcAngular(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_USALS_PARA_S* pstPara)
{
    HI_U8 u8Byte1 = 0;
    HI_U8 u8Byte2 = 0;
    HI_U8 u8RotateAngle;
    HI_U8 u8Fractional;
    HI_U16 u16Long;
    HI_U16 u16Lat;
    HI_U16 u16SatLong;
    HI_DOUBLE dSatAngle;
    HI_DOUBLE dLongitude;
    HI_DOUBLE dLatitude;
    HI_DOUBLE dT1;
    HI_DOUBLE dT2;
    HI_DOUBLE dT3;
    HI_DOUBLE dT4;
    HI_DOUBLE dTemp;
    HI_U8 u8Pos[] = {0x0, 0x2, 0x3, 0x5, 0x6, 0x8, 0xA, 0xB, 0xD, 0xE};

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (pstPara->u16LocalLongitude > 2*MAX_LONGITUDE)
    {
        HI_ERR_TUNER("Input parameter(pstPara->u16LocalLongitude) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (pstPara->u16LocalLatitude > 2*MAX_LATITUDE)
    {
        HI_ERR_TUNER("Input parameter(pstPara->u16LocalLatitude) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (pstPara->u16SatLongitude > 2*MAX_LONGITUDE)
    {
        HI_ERR_TUNER("Input parameter(pstPara->u16SatLongitude) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    /* Convert angle */
    u16Long = pstPara->u16LocalLongitude;
    u16Lat = pstPara->u16LocalLatitude;
    u16SatLong = pstPara->u16SatLongitude;
    dLongitude = ((u16Long < MAX_LONGITUDE) ? u16Long : -(2*MAX_LONGITUDE - u16Long)) / ANGLE_MULTIPLE;    
    dLatitude = ((u16Lat < MAX_LATITUDE) ? u16Lat : -(2*MAX_LATITUDE - u16Lat)) / ANGLE_MULTIPLE;    
    dSatAngle = ((u16SatLong < MAX_LONGITUDE) ? u16SatLong : -(2*MAX_LONGITUDE - u16SatLong)) / ANGLE_MULTIPLE;

    HI_ERR_TUNER("dLongitude=%f, dLatitude=%f, dSatAngle=%f\n", dLongitude, dLatitude, dSatAngle);

    dTemp = dSatAngle - dLongitude;
    dT1 = (HI_DOUBLE)(USALS_CONST_1 * cosf((HI_FLOAT)(PI * ABS(dTemp) / USALS_CONST_2)));
    dT1 = (HI_DOUBLE) sqrtf((HI_FLOAT)(USALS_CONST_1 - dT1));
    dT1 = dT1 * dT1;
    dT2 = USALS_CONST_3 * sinf((HI_FLOAT)(PI * dLatitude / USALS_CONST_2));
    dT2 = dT2 * dT2;
    dT3 = (HI_DOUBLE) sqrtf((HI_FLOAT)(USALS_CONST_4 - dT2));
    dT3 = (HI_DOUBLE) dT3 * sinf((HI_FLOAT)(PI * ABS (dTemp) / USALS_CONST_2));
    dT3 = dT3 * dT3;
    dT3 = (HI_DOUBLE) sqrtf((HI_FLOAT)(dT2 + dT3));
    dT3 = (asinf((HI_FLOAT)(dT3/USALS_CONST_3)) * USALS_CONST_2) / PI;
    dT3 = (HI_DOUBLE) sqrtf((HI_FLOAT)(USALS_CONST_5 - (USALS_CONST_6 * cosf((HI_FLOAT)(PI * dT3 / USALS_CONST_2)))));
    dT4 = (HI_DOUBLE) sqrtf((HI_FLOAT)(USALS_CONST_5 - (USALS_CONST_6 * cosf((HI_FLOAT)(PI * dLatitude / USALS_CONST_2)))));
    dT4 = (acosf((HI_FLOAT)(((dT3 * dT3) + (dT4 * dT4) - dT1) / (2.0 * dT3 * dT4))) * USALS_CONST_2) / PI;

    /* Handle negative case */
    if (((dLatitude > 0) && 
           (((dSatAngle<dLongitude) && (ABS(dLongitude-dSatAngle)<180.0)) || 
             ((dSatAngle>dLongitude) && (ABS(dLongitude-dSatAngle)>180.0)))) || 
         ((dLatitude <= 0) && 
           (((dSatAngle>dLongitude) && (ABS(dLongitude-dSatAngle)<180.0)) || 
             ((dSatAngle<dLongitude) && (ABS(dLongitude-dSatAngle)>180.0)))))
    {
        dT4 = -dT4;
    }

    u8RotateAngle = (HI_U8) ABS(dT4);
    u8Fractional = (HI_U8)((ABS(dT4) - (double) u8RotateAngle) * ANGLE_MULTIPLE);

    /* 
     * Generally, mator can't support rotation angle > 85;
     * and, if the difference between local longitude and satellite longitude is too large, 
     * the calculate result may be 0.
     */
    if ((u8RotateAngle > 85) || 
        ((pstPara->u16SatLongitude != pstPara->u16LocalLongitude) && (u8RotateAngle==0) && (u8Fractional==0)))
    {
        HI_ERR_TUNER("Rotation angle too large!\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    /* Handle the first nibble */
    u8Byte1 = ((dT4 < 0) ? 0xD0 : 0xE0) | (u8RotateAngle >> 4);
    u8Byte2 = (u8RotateAngle & 0xF) << 4;

    /* According to section 3.10 of <positioner_appli_notice.pdf> */
    if (u8Fractional <= 9)
    {
        u8Byte2 |= u8Pos[u8Fractional];
    }
    else
    {
        u8Byte2 |= 0x0;
    }

    pstPara->u16Angular  = 0;
    pstPara->u16Angular |= (HI_U16)u8Byte1 << 8;
    pstPara->u16Angular |= (HI_U16)u8Byte2;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_DISEQC_GotoAngular(HI_U32 u32TunerId, const HI_UNF_TUNER_DISEQC_USALS_ANGULAR_S* pstPara)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;
    HI_U8 u8Value[2];

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstPara)
    {
        HI_ERR_TUNER("Input parameter(pstPara) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstPara->enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", pstPara->enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (pstPara->enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;

    stSendMsg.enLevel = pstPara->enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    u8Value[0] = (HI_U8)(pstPara->u16Angular>>8);
    u8Value[1] = (HI_U8)(pstPara->u16Angular);
    FORMAT_DISEQC_CMD_VALUE(stSendMsg.au8Msg, u8Framing, DEVICE_AZIMUTH_POSITIONER, CMD_GOTO_XX, u8Value, 2);
    stSendMsg.u8Length = 5;
    stSendMsg.u8RepeatTimes = 0;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send Goto XX fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_Reset(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;
    stSendMsg.enLevel = enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    FORMAT_DISEQC_CMD(stSendMsg.au8Msg, u8Framing, DEVICE_ANY, CMD_RESET);
    stSendMsg.u8Length = 3;
    stSendMsg.u8RepeatTimes = 2;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("DiSEqC reset fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_Standby(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;
    stSendMsg.enLevel = enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    FORMAT_DISEQC_CMD(stSendMsg.au8Msg, u8Framing, DEVICE_ANY, CMD_STANDBY);
    stSendMsg.u8Length = 3;
    stSendMsg.u8RepeatTimes = 2;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send standby fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

HI_S32 HI_UNF_TUNER_DISEQC_WakeUp(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel)
{
    HI_UNF_TUNER_DISEQC_SENDMSG_S stSendMsg;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    HI_S32 s32Ret;
    HI_U8 u8Framing;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= enLevel)
    {
        HI_ERR_TUNER("DiSEqC level invalid: %d\n", enLevel);
        return HI_ERR_TUNER_INVALID_PARA;
    }

    memset(&stSendMsg, 0, sizeof(stSendMsg));
    memset(&stRecvMsg, 0, sizeof(stRecvMsg));
    u8Framing = (enLevel == HI_UNF_TUNER_DISEQC_LEVEL_2_X) ? MASTER_REPLY_FIRST : MASTER_NOREPLY_FIRST;
    stSendMsg.enLevel = enLevel;
    stSendMsg.enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    FORMAT_DISEQC_CMD(stSendMsg.au8Msg, u8Framing, DEVICE_ANY, CMD_POWER_ON);
    stSendMsg.u8Length = 3;
    stSendMsg.u8RepeatTimes = 2;

    s32Ret = TUNER_DISEQC_SendRecvMessage(u32TunerId, &stSendMsg, &stRecvMsg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Send power on fail.\n");
        return s32Ret;
    }

    /* TODO: If level 2.x, handle received message here. */

    return s32Ret;
}

