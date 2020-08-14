/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_globdefs.h"
#include "si_timer.h"
#include "si_delay.h"
#include "si_hdcp.h"
#include "si_video.h"
#include "si_hdmitx.h"
#include "si_hdcpdefs.h"
#include "si_edid.h"
#include "si_eeprom.h"
#include "si_audio.h"
#include "si_isr.h"
#include "si_sha_hw.h"
#include "si_mpi_hdmi.h"
#include "drv_hdmi.h"
#include "drv_global.h"
#include "si_phy.h"
#define TIMEROVERFLOW TF2

Bool TimerSrvF;

SyncInfoType SyncInfo;

static HI_U8 OldOutputState = 0xFF;            /*HDMI last output state note*//*CNcomment:HDMI上一个输出状态备份 */
HI_U8 OutputState    = CABLE_UNPLUG_;   /*HDMI output state at the present time*//*CNcomment: HDMI当前输出状态 */
HI_U8 OutChangeFlag  = 0;
HI_U8 HDMIPlugInFlag = HI_FALSE;

/*
         Timer(K)            Event(U)           Timer(K)
  |----------------------|-------------------|--------------|----->Time
 HPD(0)                 (1)                 (2)            (3)
 
 (0)~(1):HPD respond process in kernel layer
 (1)~(2):HPD respond process in user layer
 (2)~(3):HDCP impower process in HPD kernel layer
 you must think over the different instance of restore operation when the HPD operation
 is interrupted by hotplug,so the HPD need the timer to control the process
*/
/*CNcomment:
         Timer(K)            Event(U)           Timer(K)
  |----------------------|-------------------|--------------|----->Time
 HPD(0)                 (1)                 (2)            (3)
 
 (0)~(1):为HPD内核层响应过程
 (1)~(2):为HPD用户层响应过程
 (2)~(3):为HPD内核层HDCP授权过程
 其间都会被热插拔所打断，需要考虑不同的过程的复位操作。   
 所以需要下面的计数器来控制流程。
*/
HI_U32 HPDIsrCount            = 0;      /*the timer of HPD interrupt*//*CNcomment:HPD中断产生的计数器 */
HI_U32 HPDKernelCallbackCount = 0;      /*the timer of the HPD kernel level Callback*//*CNcomment: HPD Kernel level Callback处理相对应的HDP的计数器 */
HI_U32 HPDUserCallbackCount   = 0;      /*the timer of the HPD user level Callback*//*CNcomment: HPD User level Callback处理相对应的HDP的计数器 */
HI_U32 HPDAuthCount           = 0;      /*the timer of the HPD authentication*//*CNcomment:HPD HDCP Auth处理中使用的HDP的计数器 */

static HI_U8 OldAuthState   = 0xFF;     /*the HDCP last state of authorization*//*CNcomment: HDCP授权上一个状态备份 */
HI_U8 AuthState      = NO_HDCP;         /*the HDCP current state of authorization*//*CNcomment:HDCP授权当前状态 */

#define HDCP_MUTEXT_LOCK
#define HDCP_MUTEXT_UNLOCK

//extern HI_BOOL g_bHdmiEdidPrepareOk;

HI_U32 SI_GetHDMIOutputStatus(void)
{
    HPD_INFO("set OutputState:%d\n", OutputState);
    return OutputState;
}

void SI_SetHDMIOutputStatus(HI_U32 u32OutputState)
{
    OutputState = u32OutputState;
    return;
}

HI_U32 SI_GetHDMIOldOutputStatus(void)
{
    return OldAuthState;
}

void SI_SetHDMIOldOutputStatus(HI_U32 u32OutputOldState)
{
    OldAuthState= u32OutputOldState;
    OldOutputState =u32OutputOldState;
    return;
}

#if defined (DEBUG_SUPPORT) 
static HI_U8 timer_flag = 0;
static struct timeval pretick;

struct timeval si_time_now(void)
{
    struct timeval tv;
#ifdef __USER_LEVEL__    
    gettimeofday(&tv, NULL);
#else
    do_gettimeofday(&tv);
#endif    
    return tv;
}

HI_U32 SI_timer_diff(struct timeval time_before, struct timeval time_after)
{
    HI_U32 time_used;
    
    time_used = 1000000 * ( time_after.tv_sec - time_before.tv_sec ) + time_after.tv_usec - time_before.tv_usec;
    time_used = time_used/1000;
    HPD_INFO("duration:%u ms\n", time_used);

    return time_used;
}

//Check during, unit:ms
HI_U32 SI_timer_count(void)
{
    struct timeval time_now;
    HI_U32 time_used;
    
    if(! timer_flag)
        return 0;
        
    time_now = si_time_now();
    time_used = 1000000 * ( time_now.tv_sec - pretick.tv_sec ) + time_now.tv_usec - pretick.tv_usec;
    time_used = time_used/1000;

    HPD_INFO("duration:%u ms\n", time_used);
    //pretick = time_now;
    return time_used;
}

HI_VOID SI_timer_start(void)
{
    timer_flag = 1;
    pretick = si_time_now();
    SI_timer_count();
}
HI_VOID SI_timer_stop(void)
{
    SI_timer_count();
    HPD_WARN("HDMI Event finish, timer duration:%u ms\n", SI_timer_count());
    timer_flag = 0;
}
#else

HI_U32 SI_timer_count(void)
{
	return 0;
}
HI_VOID SI_timer_start(void)
{
}
HI_VOID SI_timer_stop(void)
{
}
#endif

//---------------------------------------------------------------------------
void SI_GetSyncInfo( void )
{

    SyncInfo.VFreq = GetMCUClocksPerVSyncPeriod();
    SyncInfo.VFreq = (HI_U16) ((HI_U32) REF_FREQ/SyncInfo.VFreq); // scale  to Hz*100
    SyncInfo.HTot =  SI_GetHSyncsPerFrame();
    SyncInfo.RefrTypeVHPol = SI_GetRefrTypeHVPol();

}
#if defined (HDCP_SUPPORT)
//---------------------------------------------------------------------------
void SI_PrintAuthState(void)
{
    if((AuthState == OldAuthState)||(AuthState == 0))
        return;
    HDCP_INFO("{AuthState}: \n");
    switch(AuthState)
    {
    case NO_HDCP:
        HDCP_INFO("No HDCP\n");
        break;
    case NO_DEVICE_WITH_HDCP_SLAVE_ADDR:
        HDCP_INFO("No HDCP Dev with HDCP Slave Addr\n");
        break;
    case NO_ACK_FROM_HDCP_DEV:
        HDCP_INFO("No ACK from HDCP dev\n");
        break;
    case BKSV_ERROR:
        HDCP_INFO("BKSV Err\n");
        break;
    case R0s_ARE_MISSMATCH:
        HDCP_INFO("R0 Err\n");
        break;
    case RIs_ARE_MISSMATCH:
        HDCP_INFO("Ri Err\n");
        break;
    case AUTHENTICATED:
        HDCP_INFO("Authenticated\n");
        break;
    case REPEATER_AUTH_REQ:
        HDCP_INFO("Rptr Auth. Req\n");
        break;
    case REQ_SHA_CALC:
        HDCP_INFO("Req SHA calc\n");
        break;
    case REQ_SHA_HW_CALC:
        HDCP_INFO("Req SHA HW calc\n");
        break;
    case FAILED_ViERROR:
        HDCP_INFO("V Err\n");
        break;
    case REAUTHENTATION_REQ:
        HDCP_INFO("Reauthentication Req\n");
        break;
    case REQ_AUTHENTICATION:
        HDCP_INFO("Req authentication\n");
        break;
    default:
        HDCP_INFO("Unknown %02i\n",AuthState);
        break;
    }
    
    HDMI_PRINT("\n");
    OldAuthState = AuthState;
}
#endif
static void PrintOutputStatus(void)
{
    if (OutputState == CABLE_UNPLUG_)
    {
        HPD_INFO(": Cable unplug\n");
        HPD_INFO("**** Plug Out ****\n");
    }
    else
    {
        switch(OutputState)
        {
        case CABLE_PLUGIN_CHECK_EDID:
        	HPD_INFO("**** Plug In ****\n");
            HPD_INFO(": Cable plugin - OutputState=check EDID\n");
            break;
        case CABLE_PLUGIN_CHECK_EDID_OK:
            HPD_INFO(": Cable plugin - OutputState=EDID OK\n");
            break;
        case CABLE_PLUGIN_CHECK_EDID_CORRUPTED:
            HPD_INFO(": Cable plugin - OutputState=EDID Corrupt\n");
            break;
        case CABLE_PLUGIN_HDMI_OUT:
            HPD_INFO(": Cable plugin - OutputState=HDMI Out\n");
            break;
        case CABLE_PLUGIN_DVI_OUT:
            HPD_INFO(": Cable plugin - OutputState=DVI Out\n");
            break;
        default:
            HPD_INFO("Unknown %d\n",OutputState);
            break;
        }
    }
    
    return;
}

//---------------------------------------------------------------------------
static void SI_Check_OutputStatus(void)
{
    if(HI_TRUE == SI_HPD_Status())
    {

        if ( (OldOutputState == 0xFF) || (OldOutputState == CABLE_UNPLUG_) )
        {
           /*HDMI init ,there isn't HPD event*/ /*CNcomment:HDMI初始化，当前没有办法产生HPD!*/
            HPD_INFO("force to set HDMI Status\n");
            //Plug In Now!
            if(OutputState < CABLE_PLUGIN_CHECK_EDID)
            {
                OutputState = CABLE_PLUGIN_CHECK_EDID;
            }
            SI_timer_start();
        }
        SI_EnableHDMICEC(HI_TRUE);
    }    
    else
    {
        SI_EnableHDMICEC(HI_FALSE);
    }

    if( OldOutputState == OutputState )
        return;

    HPD_INFO("OutputState:%d, OldOutputState:%d SI_HPD_Status():%d\n", OutputState, OldOutputState, SI_HPD_Status());
    OldOutputState = OutputState;
    
    if (OutputState == CABLE_UNPLUG_)
    {
    	//HPD_INFO("need to reset g_bHdmiEdidPrepareOk to False in Unplug\n");
        //g_bHdmiEdidPrepareOk = HI_FALSE;
        SI_timer_count();

#if defined (HDCP_SUPPORT)
        SI_AutoRiCheck( OFF );

        SI_AutoKSVReadyCheck( OFF );
        SI_SendCP_Packet(ON);

        SI_SetEncryption(OFF);
        /* Force to stop HDCP Authentication! */
        SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
        SI_timer_count();
#endif
        if(HI_FALSE == SI_HPD_Status())
        {
            SI_TX_PHY_PowerDown(HI_TRUE);
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_NO_PLUG);
        }
        SI_timer_count();
    }
    PrintOutputStatus();
    
    SI_timer_count();
    
    return;
}

//---------------------------------------------------------------------------
void SI_SetOutputMode(void)
{
    HPD_INFO("---> OutputState=%d\n", OutputState);

    if(OutputState == CABLE_PLUGIN_HDMI_OUT)
    {
        if (!SI_IsTXInHDMIMode() )
        {
            HPD_INFO ("Call SI_Init_HDMITX()\n");
            SI_Start_HDMITX();
        }
        else
        {
            HPD_INFO ("OutputState == CABLE_PLUGIN_HDMI_OUT, and TX is IN HDMI.\n");
        }
    }
#if defined (DVI_SUPPORT)
    else  if(OutputState == CABLE_PLUGIN_DVI_OUT)
    {
        if (SI_IsTXInHDMIMode() )
        {
            HPD_INFO ("Call SI_Init_DVITX()\n");
            SI_Init_DVITX();
        }
    }
#endif
    else
    {
        HPD_INFO("OutputState:%d, NOT processed.\n", OutputState);
    }
    return;
}

//---------------------------------------------------------------------------
void SI_TimeOutHandler(void)
{

    if(OutputState == CABLE_PLUGIN_CHECK_EDID)
    {
        OutputState = CABLE_PLUGIN_DVI_OUT;
        SI_SetOutputMode();
    }
}

//---------------------------------------------------------------------------
void SI_OutputProcessing(void)
{
    HI_U8 NewHPDState, RegVal;

    HDCP_MUTEXT_LOCK;
    if (OutChangeFlag == 1)
    {
        //we should force to do something here!
        OutChangeFlag = 0;
    }
    else
    {
        return; //Do come to next step!!
    }
    HDCP_MUTEXT_UNLOCK;
    
    /* Get Real Plug In or Plug Out Status!*/
    RegVal = GetSysStat(); // 0x72:0x09
    NewHPDState = RegVal & BIT_HPD_PIN;
    HPD_INFO("SI_OutputProcessing GetSysStat():0x%x, jiffies:0x%x\n", GetSysStat(), (HI_U32)jiffies);

    if(NewHPDState) //PlugIn
    {
#if 0        
        //Former status is PlugIn, So, we do not need to deal with it!
        if(OutputState != CABLE_UNPLUG_)
        {
            DEBUG_PRINTK("Unused Output State NewHPDState:0x%x, GetSysStat():0x%x\n", NewHPDState, GetSysStat());
            return;
        }
#endif
        HPD_INFO("HDMI Isr set CABLE_PLUGIN_CHECK_EDID\n");
        OutputState = CABLE_PLUGIN_CHECK_EDID;
    }
    else //PlugOut
    {
        HPD_INFO("HDMI Isr set CABLE_UNPLUG_\n");
        OutputState = CABLE_UNPLUG_;
		SI_TX_PHY_PowerDown(HI_TRUE);
    }
    SI_timer_count();
    
    return;
}

//---------------------------------------------------------------------------
void SI_AssignOutputState( HI_U8 DisplType )
{
    HI_U8 RegVal;

    RegVal = GetSysStat();
    if( DisplType == HDMI_DISPLAY )
        OutputState = CABLE_PLUGIN_HDMI_OUT;
    else
        OutputState = CABLE_PLUGIN_DVI_OUT;
}

void SI_AssignDefaltOutputState(void)
{
    if(HI_UNF_HDMI_DEFAULT_ACTION_HDMI == DRV_Get_DefaultOutputMode(HI_UNF_HDMI_ID_0))
    {
        OutputState = CABLE_PLUGIN_HDMI_OUT;
    }
    else
    {
        OutputState = CABLE_PLUGIN_DVI_OUT;
    }
}

//---------------------------------------------------------------------------
HI_S32 SI_EDIDProcessing( void )
{
    HI_U8 DisplayType;
    //IRAM HI_U16 OldParseCRC;
    
    if (HI_TRUE != SI_HPD_Status())
    {
        return HI_FAILURE;
    }

    if (EDID_READ_FIRST_BLOCK_ERR == SI_ReadSinkEDID())
    {
        /* We should set this status to user setting default mode*/
        EDID_WARN("come to here force setting EDID,Try Read EDID Again\n");
        //SI_EDID_Force_Setting();
        SI_AssignDefaltOutputState();
        return HI_FAILURE;
    }

    
    if (HI_TRUE != SI_HPD_Status())
    {
        return HI_FAILURE;
    }
    SI_timer_count();
    if(! SI_ParseEDID(&DisplayType))
    {
        //OldParseCRC = EDIDParsedData.CRC16;
        SI_AssignOutputState(DisplayType);
        EDID_WARN("SI_ParseEDID ok, DisplayType:%d, OutputState:%d\n", DisplayType, OutputState);
        EDID_WARN("finish SI_EDIDProcessing\n");
        SI_timer_count();
        
        return HI_SUCCESS;
    }
    else
    {
        EDID_WARN("SI_ParseEDID err!\n");
        /* SI_ParseEDID Err, Force to change DVI Output */
        //SI_EDID_Force_Setting();
        SI_AssignDefaltOutputState();
        return HI_FAILURE;
    }
    
}

#if defined (HDCP_SUPPORT)
static HI_BOOL g_bHDCPState = OFF;
#endif

HI_U8 SI_TimerHandler(void)
{
     HI_U32  u32Count = 0;
    /* Read Interupt status to get PlugIn/Out status */
    SI_InterruptHandler();
    /* Use HPD to set Output Status */
    SI_OutputProcessing();
    /* Print Current Output Status, and send out un_plug event to user */
    SI_Check_OutputStatus();

    //HPD_INFO("--> OutputState:%d/Auth:%d.\n", OutputState, AuthState);
    /* Check HDMI Cable Output Status for HotPlug Detect */
    if(OutputState == CABLE_PLUGIN_CHECK_EDID)
    {
        /* we first read EDID before send out the event */
        SI_timer_count();
        while(HI_SUCCESS != SI_EDIDProcessing())
        {
            if(++u32Count > 5)
            {
            	EDID_ERR("Read EDID Fail.Read %d times\n",u32Count);
            	break;
            }
            EDID_WARN("Read EDID times:%d\n",u32Count);
        }
        EDID_INFO("Read EDID successed times :%d\n",u32Count);
        SI_timer_count();
        PrintOutputStatus();
        
        if( (OutputState == CABLE_PLUGIN_HDMI_OUT) || (OutputState == CABLE_PLUGIN_DVI_OUT) )
        {
            //In HPD callback, let customer set HDMI Output param and 
            EDID_INFO("After SI_EDIDProcessing, OutputState:%d.\n", OutputState);
            /* Only deal with callback, we can set this value */
            SI_HPD_SetHPDKernelCallbackCount(HPDIsrCount);
            SI_timer_count();
        		DelayMS(200);
    		    SI_TX_PHY_PowerDown(HI_FALSE);
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HOTPLUG);
            SI_timer_count();
        }
        else if (OutputState == CABLE_PLUGIN_CHECK_EDID_CORRUPTED)
        {
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_EDID_FAIL);
        }
    }
    /* Deal with PlugOut effect */
    if(HI_TRUE != SI_HPD_Status())
    {
        return HI_FAILURE;
    }
#if defined (HDCP_SUPPORT)
    /* Below is deal with HDCP protocol */
    if((OutputState == CABLE_PLUGIN_HDMI_OUT) || (OutputState == CABLE_PLUGIN_DVI_OUT))
    {
        /* If NO_HDCP, we can return immediately */
        if( AuthState == NO_HDCP )
        {
            if (TRUE == SI_IsTXHDCP_EEsetting())
            {

             //   SI_SendCP_Packet(ON);      // Enable AVMute
                SI_SetEncryption(OFF);     // Must turn encryption off when AVMUTE
    
                AuthState = REQ_AUTHENTICATION;
                HDCP_INFO("Try to Begin to HDCP Authenication!\n");
                DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_USERSETTING);
                
                return HI_SUCCESS;
            }
            else
            {
            	if(g_bHDCPState == ON)
            	{
            	 SI_SendCP_Packet(OFF);
            	 g_bHDCPState = OFF;
        	     }
                return HI_SUCCESS;
            }
        }

        if (( AuthState != NO_HDCP ) && (HI_TRUE != SI_IsTXHDCP_EEsetting()) )
        {
        	g_bHDCPState = ON;
            HDCP_INFO("force HDCP stop\n");

        	SI_AutoRiCheck( OFF );          // disable Auto Ri Check

            SI_AutoKSVReadyCheck( OFF );    // disable KSVList Ready Check
            //SI_SendCP_Packet(OFF);          // disable AVMute, enable AV output
            SI_SetEncryption(OFF);          // disable HDCP Encryption
            AuthState = NO_HDCP;
            
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_USERSETTING);
            
            return HI_SUCCESS;
        }

        //HI_INFO_HDMI("AuthState:%d\n", AuthState);

        /* Add 5s timeout to check READY Bit */
        /* If DUT does not read Bcaps() within five seconds after reading R0', then fail */
        if( (AuthState == REPEATER_AUTH_REQ) || ( AuthState == REQ_SHA_CALC ) || ( AuthState == REQ_SHA_HW_CALC ))
        {
            /* 1 to check timeout */
            if (HI_TRUE == SI_ReadKSVList_Timeout())
            {
                HDCP_ERR("timeout to read KSVList\n");

        		SI_AutoRiCheck( OFF );

                SI_AutoKSVReadyCheck( OFF );
                SI_SendCP_Packet(ON);
                SI_SetEncryption(OFF);
                DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
                AuthState = REAUTHENTATION_REQ;
            }
            /* 2 to check HPD */
            if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
            {
                //High priority to deal with HPD!, before communciantion!!
                HDCP_ERR("Need to reAuthentication 9\n");
        		SI_AutoRiCheck( OFF );		                
                SI_AutoKSVReadyCheck( OFF );
                SI_SendCP_Packet(ON);
                SI_SetEncryption(OFF);
                AuthState = REAUTHENTATION_REQ;
            }
            /* 3 to deal with REPEATER_AUTH_REQ */
            if(AuthState == REPEATER_AUTH_REQ)
            {
                //HI_INFO_HDMI("AuthState is REPEATER_AUTH_REQ\n");
            }
            /* 4 to check HPD */
            if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
            {
                //High priority to deal with HPD!, before communciantion!!
                HDCP_INFO("Need to reAuthentication 10\n");
        		SI_AutoRiCheck( OFF );  
                SI_AutoKSVReadyCheck( OFF );
                SI_SendCP_Packet(ON);
                SI_SetEncryption(OFF);
                AuthState = REAUTHENTATION_REQ;
            }
            /* 4 to deal with REQ_SHA_CALC */
            if( AuthState == REQ_SHA_CALC )
            {
            	HDCP_INFO("hdcp protocol 2:come to SI_SHAHandler\n");
                SI_SHAHandler();
            }

#ifdef _9032_SHA_
            /* 4 to deal with REQ_SHA_HW_CALC */
            if( AuthState == REQ_SHA_HW_CALC )
            {
            	HDCP_INFO("hdcp protocol 2:come to SHA_HW_Handler\n");
                if(HI_TRUE == SHA_HW_Handler())
                {                
                    HDCP_INFO("SHA_HW_Handler ok, enter hdcp protocol 3\n");
                    //DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_SUCCESS);
                }
                else
                {
                    HDCP_ERR("SHA_HW_Handler(hdcp protocol 2) fail\n");
                    //DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
                }

            }
#endif
        }
        else if(AuthState != AUTHENTICATED)
        {
            if (HI_TRUE != SI_IsTXHDCP_EEsetting())
            {
                HDCP_ERR("HDCP Flag is change, force to NO_HDCP!\n");
                AuthState = NO_HDCP;
                return HI_SUCCESS;
            }
            SI_HDCP_SetHPDAuthCount(HPDIsrCount);

            SI_ReAthentication();
        }
    }//HDCP process, after PLUGIN_XXX_OUT
#endif
    
    return HI_SUCCESS;
}

/*----------------:-------------END-----------------------------*/
