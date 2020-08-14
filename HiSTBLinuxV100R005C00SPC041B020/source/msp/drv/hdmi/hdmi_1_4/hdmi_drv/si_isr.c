/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include <linux/timer.h>
#include <linux/delay.h>
#include "si_typedefs.h"
#include "si_isr.h"
#include "si_eeprom.h"
#include "si_infofrm.h"
#include "si_timer.h"
#include "si_video.h"
#include "si_hdmitx.h"
#include "si_hdcp.h"
#include "si_delay.h"
#include "si_audio.h"
#include "si_phy.h"
#include "drv_hdmi.h"
#include <linux/interrupt.h>

#define GetVSyncInt()        ( ReadByteHDMITXP0( HDMI_INT2_ADDR) & BIT_INT_VSYNC )
#define ClearVSyncInt()      WriteByteHDMITXP0( HDMI_INT2_ADDR, BIT_INT_VSYNC )
#define GetChangePixClockInt()    ( ReadByteHDMITXP0( HDMI_INT2_ADDR) & BIT_INT_FPIXCHANGE )
#define ClearStablePixClockInt()  WriteByteHDMITXP0( HDMI_INT2_ADDR, BIT_INT_FPIXCHANGE )

extern HI_U8 AuthState;
extern HI_U8 OutChangeFlag;
extern HI_U8 HDMIPlugInFlag;

extern HI_U32 HPDIsrCount;
extern HI_U32 HPDKernelCallbackCount;
extern HI_U32 HPDUserCallbackCount;
extern HI_U32 HPDAuthCount;

extern ROM const  HI_U32 N_Val[];

extern HI_U8 SI_FindFs(void);
extern void SI_Set_N_Fs(void);

#define HDCP_MUTEXT_LOCK
#define HDCP_MUTEXT_UNLOCK

// Oscar Added 20091001 for Philips TV blanking issue.
#define RI_128_COUNTER                  0x40
#define RI_CHECK_TIMERS             3
#define KSVReady_TIMEOUT            2450   /* 2500 */

static HDMI_INT_Err_Conut HDMI_INT_ERRCount = {0};

//------------------------------------------------------------
Bool SI_WaitNewVSync( void )
{
    HI_U8 TimeOutCount = 255;
    Bool Result = FALSE;
    ClearVSyncInt();
    while((TimeOutCount--)&&GetVSyncInt())
        ;
    if(TimeOutCount)
        Result = TRUE;
    return Result;

}
//------------------------------------------------------------
Bool SI_WaitPixClockStable(void)
{
    Bool Result = FALSE;
    ClearStablePixClockInt();          // write "1" to 0x72:0x72[1]
    if(SI_WaitNewVSync())
    {                // VSync int asserted (0x72:0x72[0])
        if(!GetChangePixClockInt())    // No Pixel clock change detected for 1 VSync
            Result = TRUE;             // Pixel clock stable
    }
    return Result;                     // Pixel clock unstable
}
//------------------------------------------------------------
void SI_InitDVIInterruptMasks(void)
{
    // make enable VSync, Ri check and HotPlug Interrupts
    WriteByteHDMITXP0( HDMI_INT_ADDR, DVI_CLR_MASK);
    WriteByteHDMITXP0( HDMI_INT_MASK_ADDR,DVI_CLR_MASK);
}
//--------------------------------------------------------------
void SI_RiCheckInterruptMask(HI_U8 Enabled)
{
    HI_U8 RegVal;
    RegVal = ReadByteHDMITXP0( HDMI_INT_MASK_ADDR);
    if(Enabled)
        WriteByteHDMITXP0( HDMI_INT_MASK_ADDR, RegVal | BIT_INT_Ri_CHECK);
    else
        WriteByteHDMITXP0(HDMI_INT_MASK_ADDR, RegVal &(~ BIT_INT_Ri_CHECK));
}

//--------------------------------------------------------------
void SI_HotPlugInOrOut(void)
{
    //HI_U8 ucNewStat = GetSysStat() & BIT_HPD_PIN;

    HDCP_MUTEXT_LOCK;
    OutChangeFlag = 1;//We use this flag to reset OutputState and AuthState!
    HPDIsrCount ++;
    HDCP_MUTEXT_UNLOCK;

#if defined (HDCP_SUPPORT)
    /* Force to stop HDCP Authentication! */
    SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
    //HI_INFO_HDMI("GetSysStat():0x%x, ucNewStat=0x%x\n", GetSysStat(), ucNewStat);
    HDCP_INFO("SI_TX_GetHpdFlag = %d \n",SI_HPD_Status());
#endif
#if 0
    if(ucNewStat != 0)
    {
        //HDMI Cable Really Plug In
        HDMIPlugInFlag = HI_TRUE;
    }
    else
    {
        //HDMI Cable Really Plug Out
        HDMIPlugInFlag = HI_FALSE;
    }
#endif
    HDMIPlugInFlag = SI_HPD_Status();
#if defined (HDCP_SUPPORT)
    HDCP_INFO("wwGetSysStat():0x%x, jiffies:0x%x\n", GetSysStat(), (HI_U32)jiffies);

    if(AuthState == AUTHENTICATED)
    {
        AuthState = REAUTHENTATION_REQ;
        SI_SendCP_Packet(ON);
        HDCP_INFO("0GetSysStat():0x%x, jiffies:0x%x\n", GetSysStat(), (HI_U32)jiffies);
        SI_SetEncryption(OFF);      // Must turn encryption off when AVMUTE
        HDCP_INFO("1GetSysStat():0x%x, jiffies:0x%x\n", GetSysStat(), (HI_U32)jiffies);

        SI_AutoRiCheck( OFF );


    }
#endif
    COM_INFO("2GetSysStat():0x%x, jiffies:0x%x\n", GetSysStat(), (HI_U32)jiffies);
}


HI_U32 SI_HPD_Status( void )
{
    if(DRV_Get_IsForceOutput())
    {
        return HI_TRUE;
    }
    else
    {
        return (GetSysStat() & (BIT_HPD_PIN))?HI_TRUE:HI_FALSE;
    }
}

HI_U32 SI_RSEN_Status( void )
{
    if(DRV_Get_ForcePowerState())
    {
        return HI_TRUE;
    }
    else
    {
        return (GetSysStat() & (BIT_RSEN))?HI_TRUE:HI_FALSE;
    }
}

HI_U32 SI_HPD_SetHPDKernelCallbackCount( HI_U32 Count )
{
    HPDKernelCallbackCount = Count;
    return HI_TRUE;
}

HI_U32 SI_HPD_SetHPDUserCallbackCount( void )
{
    HPDUserCallbackCount = HPDIsrCount;
    return HI_TRUE;
}
#if defined (HDCP_SUPPORT)
HI_U32 SI_HDCP_SetHPDAuthCount( HI_U32 Count )
{
    HPDAuthCount = Count;
    return HI_TRUE;
}
#endif
HI_U32 SI_Is_HPDKernelCallback_DetectHPD( void )
{
    //HI_U8 ucNewStat = 0;

    if (HPDKernelCallbackCount != HPDIsrCount)
    {
        return HI_TRUE;
    }
    else
    {
       if(HI_TRUE == SI_HPD_Status())
       {
          return HI_FALSE;
       }
       return HI_TRUE;
#if 0
        /*  System Status Register:SYS_STAT(0x72,0x09) */
        /* Only BIT_RSEN&BIT_HPD_PIN is asserted, HPD is ok */
        ucNewStat = GetSysStat() & BIT_HPD_PIN;
        //COM_INFO("GetSysStat:0x%x\n", GetSysStat());

        if (ucNewStat && BIT_HPD_PIN)
        {
            return HI_FALSE;
        }
        else
        {
            return HI_TRUE;
        }
#endif
    }
}

HI_U32 SI_Is_HPDUserCallback_DetectHPD( void )
{
   // HI_U8 ucNewStat = 0;

    if (HPDUserCallbackCount != HPDIsrCount)
    {
        return HI_TRUE;
    }
    else
    {
       if(HI_TRUE == SI_HPD_Status())
       {
          return HI_FALSE;
       }
       return HI_TRUE;
#if 0
        /*  System Status Register:SYS_STAT(0x72,0x09) */
        /* Only BIT_RSEN&BIT_HPD_PIN is asserted, HPD is ok */
        ucNewStat = GetSysStat() & BIT_HPD_PIN;
        //COM_INFO("GetSysStat:0x%x\n", GetSysStat());

        if (ucNewStat && BIT_HPD_PIN)
        {
            return HI_FALSE;
        }
        else
        {
            return HI_TRUE;
        }
#endif
    }
}
#if defined (HDCP_SUPPORT)
HI_U32 SI_Is_HDCPAuth_DetectHPD( void )
{
  //  HI_U8 ucNewStat = 0;

    if (HPDAuthCount != HPDIsrCount)
    {
        return HI_TRUE;
    }
    else
    {
       if(HI_TRUE == SI_HPD_Status())
       {
          return HI_FALSE;
       }
       return HI_TRUE;
#if 0
        /*  System Status Register:SYS_STAT(0x72,0x09) */
        /* Only BIT_RSEN&BIT_HPD_PIN is asserted, HPD is ok */
        ucNewStat = GetSysStat() & BIT_HPD_PIN;
        //COM_INFO("GetSysStat:0x%x\n", GetSysStat());

        if (ucNewStat && BIT_HPD_PIN)
        {
            return HI_FALSE;
        }
        else
        {
            return HI_TRUE;
        }
#endif
    }
}
#endif

static void SI_ErrCount(void)
{
    HI_U32 u32Int1, u32Int2;

    u32Int1 = ReadByteHDMITXP0(HDMI_INT1_ADDR);
    u32Int2 = ReadByteHDMITXP0(HDMI_INT2_ADDR);
    HDMI_INT_ERRCount.overrun      += u32Int1 & OVER_RUN;
    HDMI_INT_ERRCount.under_run    += u32Int1 & UNDER_RUN;
    HDMI_INT_ERRCount.spdif_par    += u32Int2 & SPDIF_PAR;

    return ;
}

void SI_GetErrCount(HDMI_INT_Err_Conut *pHDMI_INT_ERRCount)
{
    if(!!pHDMI_INT_ERRCount)
    {
        memset(pHDMI_INT_ERRCount, 0, sizeof(HDMI_INT_Err_Conut));
        memcpy(pHDMI_INT_ERRCount, &HDMI_INT_ERRCount, sizeof(HDMI_INT_Err_Conut));
    }
    else
    {
        HPD_INFO("SI_GetErrCount param pointer is emtpy\n");
    }

    return ;
}

void SI_InterruptHandler( void )
{
    HI_U8 IntReg[4] = {0};
    //COM_INFO("%d\n",SI_RSEN_Status());
    //HI_U8 IntRegTemp[4] ;

    SI_ErrCount();
    /* Read HDMI Interrupt Status Register */
    //if(0 != ReadByteHDMITXP0(HDMI_INT_STATE))
    //IntStatus = ReadByteHDMITXP0(HDMI_INT_STATE);
    SI_ReadBlockHDMITXP0(HDMI_INT_ADDR, 4, IntReg);
    //HI_ERR_HDMI("IntStatus 0x%x IntReg[0] 0x%x\n",IntStatus,IntReg[0]);

    if( (0 != ReadByteHDMITXP0(HDMI_INT_STATE)) || (IntReg[0] & BIT_INT_HOT_PLUG) || (IntReg[0] & BIT_INT_RSEN))
    {
        //SI_ReadBlockHDMITXP0(HDMI_INT_ADDR, 4, IntReg);
        SI_WriteBlockHDMITXP0(HDMI_INT_ADDR, 4, IntReg);   // Now clear all other interrupts
        //SI_ReadBlockHDMITXP0(HDMI_INT_ADDR, 4, IntRegTemp);

        //HI_INFO_HDMI("\n !! After clear !! 0x%x 0x%x 0x%x 0x%x \n",IntRegTemp[0],IntRegTemp[1],IntRegTemp[2],IntRegTemp[3]);

        /* Judge HPD Bit:
           Monitor Detect Interrupt - asserted if Hot Plug Detect has changed state.
           The HDMI Transmitter signals a change in the connectivity to a Sink, either unplug or plug.
        */
        if(IntReg[0] & BIT_INT_HOT_PLUG)
        {
            HPD_INFO("BIT_INT_HOT_PLUG!!!!!\n");
            HPD_INFO("interupthandler: %x %x %x %x, jiffies:0x%x\n", IntReg[0], IntReg[1], IntReg[2], IntReg[3], (HI_U32)jiffies);
            //We need to use this change flag to re-organize our process!!
            SI_timer_start();
            SI_HotPlugInOrOut();              // in order not to fail bouncing detection
#if defined (HDCP_SUPPORT)
            SI_timer_count();

            SI_AutoRiCheck( OFF );

            SI_AutoKSVReadyCheck( OFF );
	     if (DRV_HDMI_GetSoftResume()  == HI_FALSE)
	     {
                SI_SendCP_Packet(ON);
	     }
            SI_SetEncryption(OFF);
#endif
        }

        if(IntReg[0] & BIT_INT_RSEN)
        {
            HPD_INFO("\nBIT_INT_RSEN!!!\n");
            if(HI_TRUE == SI_RSEN_Status())
            {
                HPD_INFO("BIT_INT_RSEN is connected");
               
                DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_RSEN_CONNECT);
                if(!(IntReg[0] & BIT_INT_HOT_PLUG))
                {                    
                    HI_INFO_HDMI("interupthandler: %x %x %x %x, jiffies:0x%x\n", IntReg[0], IntReg[1], IntReg[2], IntReg[3], (HI_U32)jiffies);
                    SI_timer_start();
                    SI_HotPlugInOrOut();              // in order not to fail bouncing detection
#if defined (HDCP_SUPPORT)                    
                    SI_timer_count();

                    SI_AutoRiCheck( OFF );
                    
        	        SI_AutoKSVReadyCheck( OFF );
        	        SI_SendCP_Packet(ON);
        	        SI_SetEncryption(OFF);
#endif                    
                }
            }
            else
            {
                HPD_INFO("BIT_INT_RSEN is disconnected\n");
                
                DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_RSEN_DISCONNECT);
            }
        }

        if(IntReg[1] & 0x20)
        {
            HPD_INFO("Encrypt Disable!!!\n");
        }

        SI_WriteBlockHDMITXP0(HDMI_INT_ADDR, 4, IntReg);   // Now clear all other interrupts
#if defined (HDCP_SUPPORT)
        /* Change AuthState for HDCP Repeater Authenticaiton */
        if((IntReg[1] & BIT_INT_KSV_READY ) && (AuthState == REPEATER_AUTH_REQ))
        {
            // BIT_INT_KSV_READY:Means HDCP(0x74:0x40[5]:READY,KSV FIFO ready bit) is set to 1.
            // So, we can really enter Auth Step 2.
            SI_AutoKSVReadyCheck( OFF );

            SI_AutoRiCheck(OFF);

            /* Change AuthState from REPEATER_AUTH_REQ to REQ_SHA_CALC */
            AuthState = REQ_SHA_CALC;
#ifdef _9032_SHA_
            AuthState = REQ_SHA_HW_CALC;
#endif
        }

        if (IntReg[2] & MASK_AUTO_RI_9134_SPECIFIC)
        {
            SI_AutoRiFailureHandler( IntReg[2] );
        }


#endif
        if((IntReg[0] & BIT_BIPHASE_ERROR)||(IntReg[0] & BIT_DROP_SAMPLE))
        {
            SI_Set_N_Fs();
        }

        WriteByteHDMITXP0(HDMI_INT_STATE, 1);
    }//HDMI_INT_STATE
}

void SI_EnableInterrupts(void)
{
    HI_U8 intAddr = 0;
    //HI_U8 intMask = 0;

    //intAddr = ReadByteHDMITXP0(HDMI_INT_ADDR);
    //intMask = ReadByteHDMITXP0(HDMI_INT_MASK_ADDR);
    WriteByteHDMITXP0( HDMI_INT_ADDR, CLR_MASK);
    WriteByteHDMITXP0( HDMI_INT_MASK_ADDR, CLR_MASK);

    intAddr = ReadByteHDMITXP0(HDMI_INT_ADDR);
    COM_INFO("Clear Interrupts 0x%02x \n",intAddr);
}

void SI_EnableHDMICEC(Bool bCECEn)
{
    HI_U8 u8Reg = 0;
    HI_U8 u8RegNew = 0;
    
    u8Reg = ReadByteHDMITXP0(REG_BANK0);
    if(bCECEn)
    { 
       u8RegNew = u8Reg | BIT_REG_BANK0_HDMI_W_EN;
    }
    else
    { 
       u8RegNew = u8Reg & (~BIT_REG_BANK0_HDMI_W_EN);
    }
    
    if(u8RegNew != u8Reg)
    { 
        WriteByteHDMITXP0( REG_BANK0, u8RegNew);
        COM_INFO("%s<--0x%02x \n", __FUNCTION__, u8RegNew);
    }  
}
