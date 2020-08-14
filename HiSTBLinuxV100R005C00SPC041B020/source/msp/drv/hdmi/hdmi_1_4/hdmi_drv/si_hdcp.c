/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include "si_globdefs.h"
#include "si_global.h"
#include "si_hdcpdefs.h"
#include "si_hdmitx.h"
#include "si_delay.h"
#include "si_hdcp.h"
#include "si_isr.h"
#include "si_infofrm.h"
#include "si_mddc.h"
#include "si_timer.h"
//#include "memmap.h"
#include "hi_mpi_mem.h"
#include "si_audio.h"
#include "drv_hdmi.h"
#include "hi_drv_sys.h"
#include "si_mpi_hdmi.h"
#include "si_phy.h"
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/time.h>
#include <linux/interrupt.h>

#if defined (HDCP_SUPPORT)

HI_U8 MX_TX[8];
HI_U16 RiFailureCount = 0;

//-------------------------------------------------------------------------
HI_U8 MDDCBlockWriteHDCPRX(HI_U8 NBytes, HI_U8 Addr, HI_U8 * pData) 
{
    TmpDType TmpD;
    memset((void*)&TmpD, 0, sizeof(TmpD));
    
    TmpD.MDDC.SlaveAddr = HDCPRX_SLV;
    TmpD.MDDC.Offset = 0;
    TmpD.MDDC.RegAddr = Addr;
    TmpD.MDDC.NBytesLSB = NBytes;
    TmpD.MDDC.NBytesMSB = 0;
    TmpD.MDDC.Dummy = 0;
    TmpD.MDDC.Cmd = MASTER_CMD_SEQ_WR;
    TmpD.MDDC.PData = pData;
    return SI_BlockWrite_MDDC(&TmpD.MDDC);
}

//-------------------------------------------------------------------------
HI_U8 MDDCBlockReadHDCPRX(HI_U8 NBytes, HI_U8 Addr, HI_U8 *pData) 
{
    TmpDType TmpD;
    memset(&TmpD, 0, sizeof(TmpD));

  TmpD.MDDC.SlaveAddr = HDCPRX_SLV;
  TmpD.MDDC.Offset = 0;
  TmpD.MDDC.RegAddr = Addr;
  TmpD.MDDC.NBytesLSB = NBytes;
  TmpD.MDDC.NBytesMSB = 0;
  TmpD.MDDC.Dummy = 0;
  TmpD.MDDC.Cmd = MASTER_CMD_SEQ_RD;
  TmpD.MDDC.PData = pData;
  return SI_BlockRead_MDDC(&TmpD.MDDC);
}


static void BlockWriteHDCPTX(HI_U8 NBytes, HI_U8 Addr, HI_U8 * Data)
{
    I2CShortCommandType I2CComm;
    memset(&I2CComm, 0, sizeof(I2CComm));

    I2CComm.Bus = BUS_1;
    I2CComm.SlaveAddr = TX_SLV0;
    I2CComm.Flags = 0;
    I2CComm.NBytes = NBytes;
    I2CComm.RegAddrL = Addr;

    BlockWrite_8BAS((I2CShortCommandType *)&I2CComm, Data);
}


//-----------------------------------------------------------
static void BlockReadHDCPTX(HI_U8 NBytes, HI_U8 Addr, HI_U8 * Data)
{
    I2CShortCommandType I2CComm;
    memset(&I2CComm, 0, sizeof(I2CComm));
    I2CComm.Bus = BUS_1;
    I2CComm.SlaveAddr = TX_SLV0;
    I2CComm.Flags = 0;
    I2CComm.NBytes = NBytes;
    I2CComm.RegAddrL = Addr;

    BlockRead_8BAS((I2CShortCommandType *)&I2CComm, Data);
}
//------------------------------------------------------------
static void GenerateAN(void)
{

    HI_U8 RegVal;

     RegVal = ReadByteHDMITXP0(HDCP_CTRL_ADDR);
     WriteByteHDMITXP0(HDCP_CTRL_ADDR, RegVal & (~ BIT_AN_STOP)); // Start AN Gen
     DelayMS(10);
     WriteByteHDMITXP0(HDCP_CTRL_ADDR, RegVal | BIT_AN_STOP); // Stop AN Gen

}
//------------------------------------------------------------
static void RepeaterBitInTX( HI_U8 RXMode )
{

    HI_U8 RegVal;

     RegVal = ReadByteHDMITXP0(HDCP_CTRL_ADDR);
     if (RXMode)
        WriteByteHDMITXP0(HDCP_CTRL_ADDR, RegVal | BIT_RX_REPEATER );
     else
        WriteByteHDMITXP0(HDCP_CTRL_ADDR, RegVal & (~BIT_RX_REPEATER) );
}

static HI_U8 IsRepeater(void)
{
    HI_U8 RegVal;
    HI_U8 ret = 0;

    ret = MDDCBlockReadHDCPRX(1, DDC_BCAPS_ADDR , &RegVal);
    return RegVal & DDC_BIT_REPEATER;
}

//---------------------------------------------------------------
static void ReleaseCPReset( void )
{
    HI_U8 RegVal;
    RegVal = ReadByteHDMITXP0(HDCP_CTRL_ADDR);
    WriteByteHDMITXP0(HDCP_CTRL_ADDR, RegVal | BIT_CP_RESET_N);
}

//------------------------------------------------------------
static HI_U8 IsBKSVError(void)
{
     return ReadByteHDMITXP0(HDCP_CTRL_ADDR) & BIT_BKSV_ERROR;
}

//------------------------------------------------------------
HI_U8 AreR0sMatch(HI_U8 * Match)
{
    HI_U16 R0RX, R0TX = 0;
    HI_U8 Error;
	
    Error = MDDCBlockReadHDCPRX(2, DDC_Ri_ADDR, (HI_U8 *)&R0RX);
    if(!Error) 
    {
        BlockReadHDCPTX(2, Ri_ADDR, (HI_U8 *)&R0TX);
        //HDCP_INFO("AreR0sMatch:R0RX:0x%x should == R0TX:0x%x\n", R0RX, R0TX);
        if(R0RX == R0TX)
              *Match = TRUE;
        else
              *Match = FALSE;
    }
    
    return Error;
}

//------------------------------------------------------------

//------------------------------------------------------------
#undef SII_DVI_REPEATER
static void MakeCopyM0(void)
{
    WriteByteHDMITXP0 ( SHA_CONTROL_ADDR, ( ReadByteHDMITXP0 ( SHA_CONTROL_ADDR ) | BIT_M0_READ_EN ) );


     BlockReadHDCPTX(8, AN_ADDR, MX_TX);
#ifdef SII_DVI_REPEATER
            AuxMX = (MX_TX[7]<<8)|MX_TX[6];
            AuxMX = (AuxMX >> 6) & 0xFF;
            MX_TX[7] = (HI_U8)AuxMX;
#endif


    WriteByteHDMITXP0 ( SHA_CONTROL_ADDR, ( ReadByteHDMITXP0 ( SHA_CONTROL_ADDR ) | ~BIT_M0_READ_EN ) );


    HDCP_INFO("M0 MX_TX :0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", \
        MX_TX[0], MX_TX[1], MX_TX[2], MX_TX[3], MX_TX[4], MX_TX[5], MX_TX[6], MX_TX[7]);
       
}

void SI_SetEncryption(HI_U8 OnOff)
{
    HI_U8 RegVal;

    RegVal = ReadByteHDMITXP0(HDCP_CTRL_ADDR);
    if (OnOff)
    {
        WriteByteHDMITXP0(HDCP_CTRL_ADDR, RegVal | BIT_ENC_EN);
    }
    else
    {
        WriteByteHDMITXP0(HDCP_CTRL_ADDR, RegVal & ~BIT_ENC_EN);
    }
}
//---------------------------------------------------------------


HI_U8 SI_CheckHDCPDevice(HI_U8 * pHDCP)
{
    HI_U8 BKSV_Data[5], Error;
    HI_U8 i, j, Ones = 0, Mask = 0x01;

    /// //   SI_AutoRiCheck(OFF);  // done by SI_SuspendAutoRiCheck()

   Error = MDDCBlockReadHDCPRX(5, DDC_BKSV_ADDR , BKSV_Data); // read BKSV from RX

   if(!Error) 
   {
      for(i = 0; i < 5; i++) 
      {
         Mask = 0x01;
         for(j = 0; j < 8; j++)
         {
            if(BKSV_Data[i] & Mask)
               Ones++;
            Mask <<= 1;
         }

      }
      
      if(Ones == 20)
         *pHDCP = TRUE;
      else
         *pHDCP = FALSE;
   }
   return Error;
}

//------------------------------------------------------------------------------
// Function Name:  SI_AutoRiCheck
// Function Description:  This function turn on or off automatic Ri check,
//                        the check should be on only if HDCP enabled,
//                        in order to access to Master DDC it should be off
// Accepts: Bool On or Off
// Returns: none
// Globals: none
//------------------------------------------------------------------------------
/*
void SI_AutoRiCheck ( Bool qOn ){

    if ( qOn ) {
        // Turn on the following Auto Ri interrupts
        //              OFF bit 7 Ri reading was not done within one frame
        //              OFF bit 6 Ri did not change between frame 127 and 0
        //              ON  bit 5 Ri did not match during 2nd frame test (0)
        //              ON  bit 4 Ri did not match during 1st frame test (127)
        WriteByteHDMITXP0 ( HDMI_INT3_MASK, ((ReadByteHDMITXP0( HDMI_INT3_MASK )) | MASK_AUTO_RI ) );
        WriteByteHDMITXP0 ( RI_CMD_ADDR, ((ReadByteHDMITXP0( RI_CMD_ADDR )) | BIT_RI_CMD_RI_EN | BIT_RI_CMD_BCAP_EN) );

    }
    else {
        WriteByteHDMITXP0 ( HDMI_INT3_MASK, ((ReadByteHDMITXP0( HDMI_INT3_MASK )) & (~MASK_AUTO_RI)) );
        WriteByteHDMITXP0 ( RI_CMD_ADDR, ((ReadByteHDMITXP0( RI_CMD_ADDR )) & (~BIT_RI_CMD_RI_EN) & (~BIT_RI_CMD_BCAP_EN)) );
    }
}
*/

void SI_AutoRiCheck ( Bool qOn )
{
    HDCP_INFO("SI_AutoRiCheck ON=%d\n", qOn);
    /* TE updates Ri' for every 128th frame from the first encrypted frame */
    if ( qOn) 
    {
        // Turn on the following Auto Ri interrupts
        //              OFF bit 7 Ri reading was not done within one frame
        //              OFF bit 6 Ri did not change between frame 127 and 0
        //              ON  bit 5 Ri did not match during 2nd frame test (0)
        //              ON  bit 4 Ri did not match during 1st frame test (127)
        WriteByteHDMITXP0 ( HDMI_INT3_MASK, ((ReadByteHDMITXP0( HDMI_INT3_MASK )) | MASK_AUTO_RI_9134_SPECIFIC ) );
        WriteByteHDMITXP0 ( RI_CMD_ADDR, ((ReadByteHDMITXP0( RI_CMD_ADDR )) | BIT_RI_CMD_RI_EN) );

    }
    else 
    {
        WriteByteHDMITXP0 ( HDMI_INT3_MASK, ((ReadByteHDMITXP0( HDMI_INT3_MASK )) & (~MASK_AUTO_RI_9134_SPECIFIC)) );
        WriteByteHDMITXP0 ( RI_CMD_ADDR, ((ReadByteHDMITXP0( RI_CMD_ADDR )) & (~BIT_RI_CMD_RI_EN)) );
    }
}


//------------------------------------------------------------------------------
// Function Name:
// Function Description:
//------------------------------------------------------------------------------
static void PrintRiFalureInfo(  HI_U8 bRiFIntr )
{

    if ( bRiFIntr & MASK_RI_READING_MORE_ONE_FRAME )
    {
        HI_PRINT( "RI_READING_MORE_ONE_FRAME\n" );
        // do nothing because the check of Ri cannot be true
    }
    else if ( bRiFIntr & MASK_RI_MISS_MATCH_LAST_FRAME )
    {
        HI_PRINT( "RI_MISS_MATCH_LAST_FRAME\n" );
    }
    else if ( bRiFIntr & MASK_RI_MISS_MATCH_FIRST_FRAME )
    {
        HI_PRINT( "RI_MISS_MATCH_FIRST_FRAME\n" );

    }
    else if ( bRiFIntr & MASK_RI_NOT_CHANGED )
    {
        HI_PRINT( "MASK_RI_NOT_CHANGED\n" );
    }
}
//------------------------------------------------------------------------------
// Function Name: SI_AutoRiFailureHandler
// Function Description:  Checks if Link integraty is failed
//
// Accepts: Ri Errors Interrupts
// Returns: none
// Globals: none
//------------------------------------------------------------------------------
void SI_AutoRiFailureHandler( HI_U8 bRiFIntr )
{
    if (bRiFIntr & MASK_AUTO_RI_9134_SPECIFIC)
    {
        PrintRiFalureInfo( bRiFIntr);
        HDCP_INFO("Ri interrupt asserted %d times\n", ++RiFailureCount);

    	SI_AutoRiCheck ( OFF );
       
        SI_AutoKSVReadyCheck( OFF );
        SI_SendCP_Packet(ON);
        SI_SetEncryption(OFF);          
        AuthState = REAUTHENTATION_REQ;

        DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
    }
}



//------------------------------------------------------------------------------
// Function Name:
// Function Description:
//
// Accepts: none
// Returns: none
// Globals: none
//------------------------------------------------------------------------------

void SI_AutoKSVReadyCheck ( Bool qOn )
{
    if ( qOn ) 
    {
        WriteByteHDMITXP0 ( HDMI_INT2_MASK, ((ReadByteHDMITXP0( HDMI_INT2_MASK )) | MASK_AUTO_KSV_READY ) );
        WriteByteHDMITXP0 ( RI_CMD_ADDR, ((ReadByteHDMITXP0( RI_CMD_ADDR )) | BIT_RI_CMD_RI_EN | BIT_RI_CMD_BCAP_EN) );
    }
    else 
    {
        WriteByteHDMITXP0 ( HDMI_INT2_MASK, ((ReadByteHDMITXP0( HDMI_INT2_MASK )) & (~MASK_AUTO_KSV_READY)) );
		// Oscar Ding added 20091008;
       	WriteByteHDMITXP0(RI_CMD_ADDR, ((ReadByteHDMITXP0(RI_CMD_ADDR)) & (~(BIT_RI_CMD_RI_EN |BIT_RI_CMD_BCAP_EN))) );   
    }
}



/*
/usr/include/bits/time.h
struct timeval
{
    __time_t tv_sec;		// Seconds.  
    __suseconds_t tv_usec;	// Microseconds.  
};
*/
#ifdef __USER_LEVEL__
static struct timeval TimeR0, TimeSKVList;
HI_U8 SI_ReadKSVList_Timeout(void)
{
    gettimeofday(&TimeSKVList, NULL);    

    if( (6 <= (TimeSKVList.tv_sec - TimeR0.tv_sec))
     || ((5 == (TimeSKVList.tv_sec - TimeR0.tv_sec)) && (TimeSKVList.tv_usec > TimeR0.tv_usec) && (TimeSKVList.tv_usec >= TimeR0.tv_usec))
      )
    {
        /*>=5, we will think timeout is occur!!*/ /*CNcomment:>= 5, we will think timeout is occur!!¡¡*/
        HDCP_ERR("\n****Timeout(%d, %d, diff:%d s.%d us) in get KSVlist, ReAuthentication***\n", (HI_U32)TimeSKVList.tv_sec, (HI_U32)TimeR0.tv_sec,
                (HI_U32)(TimeSKVList.tv_sec - TimeR0.tv_sec), (HI_U32)(TimeSKVList.tv_usec - TimeR0.tv_usec));
        return HI_TRUE;
    }

    //HDCP_INFO("hdcp interval is :%x s.%x us\n", (HI_U32)(TimeSKVList.tv_sec - TimeR0.tv_sec), (HI_U32)(TimeSKVList.tv_usec - TimeR0.tv_usec));
    return HI_FALSE;
}
#else
static HI_U32 jiffies_now = 0, jiffies_org = 0;
HI_U8 SI_ReadKSVList_Timeout(void)
{
    HI_U32 tick_to_jiffies;
    jiffies_now = jiffies; 
    
    tick_to_jiffies = msecs_to_jiffies(5000);

    if (jiffies_now > jiffies_org)
    {
        if (tick_to_jiffies < (jiffies_now - jiffies_org))
        {
            /*>=5 , we will think timeout is occur!!*//* CNcomment:>= 5, we will think timeout is occur!!¡¡*/
            HDCP_ERR("\n****Timeout(%d, %d, diff:0x%x ms) in get KSVlist, ReAuthentication***\n", (HI_U32)jiffies_now,
                    (HI_U32)jiffies_org, (HI_U32)(jiffies_now - jiffies_org));
            return HI_TRUE;
        }
    }
    else
    {
        if (tick_to_jiffies < jiffies_now)
        {
            /*>5, we will think timeout is occur!!*//*CNcomment:>= 5, we will think timeout is occur!!¡¡*/
            HDCP_ERR("\n****Timeout(%d, %d, diff:0x%x ms) in get KSVlist, ReAuthentication***\n", (HI_U32)jiffies_now,
                    (HI_U32)jiffies_org, (HI_U32)(jiffies_now - jiffies_org));
            return HI_TRUE;
        }
    }

    //HDCP_INFO("hdcp interval is :%d ms\n", (HI_U32)(jiffies_now - jiffies_org));
    return HI_FALSE;
}
#endif
HI_BOOL SI_CheckHDCPAKSV(HI_U8 * pAksv)
{
      HI_U32 u32Aksv = 0;
      for(u32Aksv = 0; u32Aksv < 5; u32Aksv++)
      {
      	if(pAksv[u32Aksv] != 0x00)
      	{
      	   return HI_TRUE;
 	    }
      } 
      return HI_FALSE;
}
//---------------------------------------------------------------
HI_U8 SI_Authentication( void )
{
    HI_U8 AN_KSV_Data[8], Err;

    typedef union Tmp {
        HI_U8 HDCPDev;
        HI_U8 R0sMatch;
    }TmpType;

    TmpType tmp;
    
    SI_timer_count();
    if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
    {
        //High priority to deal with HPD!, before communciantion!!
        HDCP_WARN("Need to reAuthentication 1\n");
        DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
        return REAUTHENTATION_REQ;
    }
#if 1 //lc check, Read BKSV should be later!
    Err = SI_CheckHDCPDevice(&tmp.HDCPDev);
    if(Err)
    {        
        HDCP_WARN("%s SI_CheckHDCPDevice ERROR\n", __FUNCTION__);
        DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
        return NO_ACK_FROM_HDCP_DEV; 

    }
        
    if(!tmp.HDCPDev)
    {
        HDCP_WARN("%s NO_DEVICE_WITH_HDCP_SLAVE_ADDR\n", __FUNCTION__);
        DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
        return NO_DEVICE_WITH_HDCP_SLAVE_ADDR;  // no 20 ones and zeros
    }
#endif    



    ReleaseCPReset();
    if(IsRepeater())
        RepeaterBitInTX( ON );
    else
        RepeaterBitInTX( OFF );
       
    /*
    Authenication is initiated by the HDCP Transmitter by sending an 
    initiation message containing its KSV(Aksv) and a 64-bit pesudo-random value(An)
    */
    GenerateAN();

    BlockReadHDCPTX(8, AN_ADDR, AN_KSV_Data);      // Read AN
/*    HDCP_INFO("\nHDCP AN Data(8):0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", \
        AN_KSV_Data[0],AN_KSV_Data[1],AN_KSV_Data[2],AN_KSV_Data[3],AN_KSV_Data[4],AN_KSV_Data[5],AN_KSV_Data[6],AN_KSV_Data[7]);
*/
    HDCP_INFO("new setting to create HDCP Auth1\n");
    if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
    {
        //High priority to deal with HPD!, before communciantion!!
        HDCP_WARN("Need to reAuthentication 2\n");
        DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
        return REAUTHENTATION_REQ;
    }
    Err = MDDCBlockWriteHDCPRX(8, DDC_AN_ADDR, AN_KSV_Data); // Write AN to RX
    if(!Err) 
    {
        if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
        {
            //High priority to deal with HPD!, before communciantion!!
            HDCP_WARN("Need to reAuthentication 3\n");
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
            return REAUTHENTATION_REQ;
        }
        // Exchange KSVs values
        BlockReadHDCPTX(5, AKSV_ADDR, AN_KSV_Data); // read AKSV from TX

        if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
        {
            //High priority to deal with HPD!, before communciantion!!
            HDCP_WARN("Need to reAuthentication 4\n");
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
            return REAUTHENTATION_REQ;
        }
        Err = MDDCBlockWriteHDCPRX(5, DDC_AKSV_ADDR, AN_KSV_Data);// write AKSV into RX

        HDCP_INFO("\n\nHDCP AKSV(5):0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n\n", \
            AN_KSV_Data[0],AN_KSV_Data[1],AN_KSV_Data[2],AN_KSV_Data[3],AN_KSV_Data[4]);

        if(!Err)
        {
            if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
            {
                //High priority to deal with HPD!, before communciantion!!
                HDCP_WARN("Need to reAuthentication 5\n");
                DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
                return REAUTHENTATION_REQ;
            }
            //check AKSV ; if 20 zeros ,must return error
            if (SI_CheckHDCPAKSV(AN_KSV_Data) != HI_TRUE)
            {
                HDCP_WARN("AKSV 20 zeros\n");
                DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
                return NO_ACK_FROM_HDCP_DEV;
    	    }
            Err = MDDCBlockReadHDCPRX(5, DDC_BKSV_ADDR , AN_KSV_Data); // read BKSV from RX            

            HDCP_INFO("HDCP BKSV(5):0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n", \
                AN_KSV_Data[0],AN_KSV_Data[1],AN_KSV_Data[2],AN_KSV_Data[3],AN_KSV_Data[4]);

            if (0 != Err)
            {
                HDCP_ERR("MDDCBlockReadHDCPRX DDC_BKSV_ADDR ERROR\n");
            }
        }
        else
        {
            HDCP_ERR("MDDCBlockWriteHDCPRX DDC_AKSV_ADDR ERROR\n");
        }
   }
   else
   {
        HDCP_ERR("MDDCBlockWriteHDCPRX DDC_AN_ADDR ERROR\n");
   }
   
    if(0 != Err)
    {
        HDCP_WARN("NO_ACK_FROM_HDCP_DEV\n");
        DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
        return NO_ACK_FROM_HDCP_DEV;
    }

    HDCP_INFO("Exchange KSVs ok.\n");

    BlockWriteHDCPTX(5, BKSV_ADDR, AN_KSV_Data); // Write BKSV into TX
    if(IsBKSVError())
    {
        HDCP_WARN("BKSV_ERROR\n");
        DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
        return BKSV_ERROR;
    }

    if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
    {
        //High priority to deal with HPD!, before communciantion!!
        HDCP_WARN("Need to reAuthentication 6\n");
        DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
        return REAUTHENTATION_REQ;
    }  
    /* If DUT does not wait at least 100ms to read R0' after writing Aksv, then FAIL */
    /* Inorder to deal with HDP during these 100ms, we need to devide it by 10 */    
#if 0    
    DelayMS(100); // Delay for R0 calculation
    if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
    {
        //High priority to deal with HPD!, before communciantion!!
        HDCP_INFO("Need to reAuthentication 7\n");
        return REAUTHENTATION_REQ;
    }
#else
{
	int index;
	
	for(index = 0; index < 10; index ++)
	{
	    DelayMS(10); // Delay for R0 calculation
	    if (HI_TRUE == SI_Is_HDCPAuth_DetectHPD())
	    {
	        //High priority to deal with HPD!, before communciantion!!
	        HDCP_WARN("Need to reAuthentication 8\n");
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
	        return REAUTHENTATION_REQ;
	    }
	}
}
#endif

    //HDCP_INFO("Protocol step 1:come to compare R0 and R0`\n");
    Err = AreR0sMatch(&tmp.R0sMatch);
    if(!Err)
    {
         if(!tmp.R0sMatch)
         {
             HDCP_WARN("R0s_ARE_MISSMATCH.\n");
             DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
             return R0s_ARE_MISSMATCH;
         }
    }
    else
    {
        HDCP_WARN("AreR0sMatch Error:NO_ACK_FROM_HDCP_DEV\n");
        DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
        return NO_ACK_FROM_HDCP_DEV;
    }

    //HDCP_INFO("Protocol step 1 sucess!\n");
    if(IsRepeater())
    {
        MakeCopyM0();
        SI_SetEncryption(ON);
        
    	SI_AutoRiCheck ( ON );

        SI_AutoKSVReadyCheck ( ON );   /// //    ONLY Call to this function
        
        /* Get current R0' time */
#ifdef __USER_LEVEL__
        gettimeofday(&TimeR0, NULL);
#else
        jiffies_org = jiffies;
        HDCP_INFO("5s is %d jiffers\n", (HI_U32)msecs_to_jiffies(5000));
#endif
        HI_PRINT("Enter Protocol step 2:repeater authetication\n");
        SI_timer_count();
        return REPEATER_AUTH_REQ;
   }

    SI_SetEncryption(ON);

    SI_AutoRiCheck ( ON );

    SI_timer_count();
    return  AUTHENTICATED;
}
//----------------------------------------------------------------------------------

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
unsigned int hdcp_counter = 10;
void reset_hdcp_counter(void)
{
	hdcp_counter = 0;
	return ;	
}

void increase_hdcp_counter(void)
{
	hdcp_counter ++;
	return ;	
}

unsigned int  get_hdcp_counter(void)
{
	return hdcp_counter;	
}
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

void SI_ReAthentication( void )
{
    //HI_U32 Reg, Reg_tmp, hdcpKeyState, hdcpKeyCheckTimeout;
    HI_U8 Mode = _DVI_;

    HI_U32 time_out = 0;
    HI_U32 epst;     
    
#if defined (DVI_SUPPORT)
    HI_U8 BlankValue[3];
#endif
    //HI_U8 uRSEN  = GetSysStat() & BIT_RSEN;

    HI_PRINT("HDCP AUTO Check RI\n");

    //if(uRSEN != BIT_RSEN)
    if(!SI_RSEN_Status())
    {
    	HI_PRINT("Sink is in standy mode\n");
#if !defined (SUPPORT_FPGA)
    	return;
#endif

    }
    else
    {
    	HDCP_INFO("RSEN is power\n");
    }

    HDCP_INFO("SI_ReAthentication AuthState:%d\n", AuthState);
    SI_timer_count();
    if(SI_IsTXInHDMIMode())
    {
        Mode = _HDMI_;
    }
#if defined (DVI_SUPPORT)
    else 
    {
        Mode = _DVI_;
        HDCP_INFO ("Output is DVI mode, Call SI_Init_DVITX()\n");
        // Reinitialization in DVI mode, to make clear CP reset bit
        SI_Init_DVITX();
        BlankValue[0] = 0x00; //R
        BlankValue[1] = 0x00; //G
        BlankValue[2] = 0x00; //R
        //LC added
        SI_WriteBlockHDMITXP0(TX_VID_BLANK1_BLUE, 3, BlankValue);
        WriteByteHDMITXP0(DATA_CTRL_ADDR, MUTE_ALL);
    }
#endif

    //SI_SetHdmiVideo(TRUE);        //Video Must be ON
    SI_EnableHdmiDevice();        //Double check to enable HDMI PHY
    //SI_SendCP_Packet(ON);         //Video can no output
    SI_SetEncryption(OFF);        // Must turn encryption off when AVMUTE

    HDCP_INFO("OutputEnable : %d\n", SI_TX_PHY_GetOutPutEnable());

    DRV_HDMI_WriteRegister((HI_U32)CIPHER_HDCP_MODE_CRL, 0x3);//time clock

    WriteByteHDMITXP0(RI_CMD_ADDR,0x0);//RI CMD
    WriteByteHDMITXP0(HDCP_CTRL_ADDR,0x0);//RI CMD
    WriteByteHDMITXP0(HDCP_CTRL_ADDR,0x04);//RI CMD

    // load Aksv    
    WriteByteHDMITXP0(0xf9,0x0);
    WriteByteHDMITXP0(0xfa,0x20);
    
    msleep(10);
    time_out = 4;
    epst = ReadByteHDMITXP0(0xf9);

    HDCP_INFO("L[%d] epst = %#x.\n", __LINE__, epst);

    while((epst & 0x1) != 1 && time_out != 0)    
    {        
       epst = ReadByteHDMITXP0(0xf9);

       msleep(10);
       HDCP_INFO("L[%d] epst = %#x.\n", __LINE__, epst);
       time_out --;
    }
    if((epst & 0x7f) != 1)
       HDCP_ERR("error(1)\n");

    // Aksv check CRC 
    WriteByteHDMITXP0(0xf9,0x0);
    WriteByteHDMITXP0(0xfa,0x04);

    time_out = 10;    

    epst = ReadByteHDMITXP0(0xf9);
    HDCP_INFO("L[%d] epst = %#x.\n", __LINE__, epst);
    while((epst & 0x1) != 1 && time_out != 0)
    {

       epst = ReadByteHDMITXP0(0xf9);
       msleep(10);
       HDCP_INFO("L[%d] epst = %#x.\n", __LINE__, epst);
       time_out --;
    }
    if((epst & 0x02) == 0x2)
    {
       HDCP_ERR("hdcp crc check is error!\n");
    }
    else
    {
       HDCP_INFO("hdcp crc check is right!\n");
    }

    
 
     AuthState = SI_Authentication();
     SI_PrintAuthState();


     if((AuthState == AUTHENTICATED)||(AuthState == REPEATER_AUTH_REQ)) 
     {
        DisplEncryptionOn();
        SI_SendCP_Packet(OFF);
        SI_RiCheckInterruptMask(ON);
        
        if (AuthState == AUTHENTICATED)
        {
            SI_timer_count();
            HDCP_INFO("HDMI receive is sink device, enter hdcp protocol 3\n");
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_SUCCESS);
        }
        
        if(Mode == _DVI_)
        {
            SI_SetHdmiVideo(TRUE);
            SI_SetHdmiAudio(FALSE);
        }
        else
        {
            SI_SetHdmiVideo(TRUE);
            SI_SetHdmiAudio(TRUE);
        }
    }
    else if((AuthState == NO_ACK_FROM_HDCP_DEV)||  (AuthState == R0s_ARE_MISSMATCH) 
        || (AuthState == BKSV_ERROR) || (AuthState == NO_DEVICE_WITH_HDCP_SLAVE_ADDR))
    {
    	HDCP_INFO("line:%d:AuthState:0x%x\n",__LINE__,AuthState);
    	SI_SendCP_Packet(ON);
    }

    else 
    {
        DisplEncryptionOff();
        SI_SendCP_Packet(ON);
    }
}

#else
void SI_SetEncryption(HI_U8 OnOff)
{
    return ;
}


void SI_AutoRiCheck ( Bool qOn )
{
    return ;
}

void SI_AutoKSVReadyCheck ( Bool qOn )
{
    return ;

} 

void SI_AutoRiFailureHandler( HI_U8 bRiFIntr )
{
    return ;
} 
#endif
/*-----------------------------------------END---------------------------------*/
