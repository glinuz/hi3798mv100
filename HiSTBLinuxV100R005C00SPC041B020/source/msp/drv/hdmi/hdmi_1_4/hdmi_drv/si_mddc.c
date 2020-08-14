/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include <linux/string.h>
#include <linux/delay.h>
#include "si_cpldefs.h"
#include "si_typedefs.h"
#include "si_global.h"
#include "si_hlviic.h"
#include "si_mddc.h"
#include "si_hdmitx.h"
#include "si_hdcpdefs.h"
#include "si_timer.h"
#include "si_delay.h"

#define IsMDDCReadInProgress()  ReadByteHDMITXP0(MDDC_STATUS_ADDR) & BIT_MDDC_ST_IN_PROGR
#define GetMDDCStatus()         ReadByteHDMITXP0(MDDC_STATUS_ADDR)

extern void SI_AutoRiCheck( Bool );
extern void SI_AutoKSVReadyCheck ( Bool );
extern void SI_SendCP_Packet(HI_U8);
extern void SI_SetEncryption(HI_U8);

#if defined (HDCP_SUPPORT)
Bool SI_SuspendAutoRiCheck(Bool);

extern HI_U8 AuthState;


//------------------------------------------------------------------------------
//  Suspend Auto Ri in order to allow FW access MDDC bus. Poll 0x72:0x26[0]
//  for MDDC bus availability or timeout. Return TRUE if bus available. Return
//  FALSE if timed-out.
//------------------------------------------------------------------------------
Bool SI_SuspendAutoRiCheck(Bool State)
{
    static HI_U8 OldRiStat, OldRiCommand;
    HI_U8 TimeOut = 10;

    //DEBUG_PRINTK("SI_SuspendAutoRiCheck state:0x%x\n", State);
    
    if (State == TRUE)
    {
        //OldRiCommand = ReadByteHDMITXP0( RI_CMD_ADDR ) & 0x01;    // Save original Auto Ri state
        OldRiCommand = ReadByte_8BA(BUS_1, TX_SLV0, RI_CMD_ADDR); 
		OldRiStat = ReadByte_8BA(BUS_1, TX_SLV0, RI_STAT ) & 0x01;    // Save MDDC bus status
		
		if((OldRiCommand & 0x1) != (OldRiStat & 0x1))
			COM_ERR("OldRiCommand(%d) != OldRiStat(%d)\n", OldRiCommand, OldRiStat);
        
        SI_AutoRiCheck( OFF );                         // disable Auto Ri
        WriteByte_8BA(BUS_1, TX_SLV0, RI_CMD_ADDR, 0);

        while (--TimeOut){                           // wait for HW to release MDDC bus
            if( !(ReadByte_8BA(BUS_1, TX_SLV0, RI_STAT ) & 0x01) )
                break;
        }
        if( !TimeOut ){                                  // MDDC bus not relinquished.
            SI_AutoRiCheck( ON );                         // enable Auto Ri Check
            COM_ERR("MDDC isn't release\n");    
            return FALSE;
        }

        //OldRiStat = hlReadByte_8BA( TX_SLV0, RI_STAT ) & 0x01;    // Save MDDC bus status
    }
    else
	{   // State == F. Cancel Auto Ri Suspension.
        //if ((OldRiStat == ON) && (OldRiCommand == ON)) // If Auto Ri was enabled before it was suspended
        //if ( OldRiStat == ON ) // If Auto Ri was enabled before it was suspended
        if (OldRiCommand == ON) // If Auto Ri was enabled before it was suspended
        {            
            SI_AutoRiCheck(ON);    // re-enable Auto Ri           
            WriteByte_8BA(BUS_1, TX_SLV0, RI_CMD_ADDR, OldRiCommand);
        }
    }
	    
    return TRUE;
}



HI_U32 SI_DDC_Debug(void)
{
    HI_U32 index;
    HI_U32 hdmiregister = 0;

    COM_INFO("Debug hdmi DDC register:\n");
    for (index=0xEC; index<=0xF5; index++)
    {
        DRV_HDMI_ReadRegister((0x10170000 + index * 4), &hdmiregister);
        COM_INFO("0x%02x(0x7a, 0x%02x):0x%02x\n", (0x10170000 + index * 4), index, hdmiregister);
    }
    COM_INFO("\nFinish\n");
    return 0;
}
#endif
//-----------------------------------------------------------
HI_U8 SI_BlockWriteHDMITXP0(HI_U8 Addr, HI_U8 NBytes, HI_U8 * Data)
{

    I2CShortCommandType I2CComm;

    memset((void*)&I2CComm, 0, sizeof(I2CComm));

    I2CComm.Bus = BUS_1;
    I2CComm.SlaveAddr = TX_SLV0;
    I2CComm.Flags = 0;
    I2CComm.NBytes = NBytes;
    I2CComm.RegAddrL = Addr;

    return BlockWrite_8BAS((I2CShortCommandType *)&I2CComm, Data);

}
//-----------------------------------------------------------
HI_U8 SI_BlockReadHDMITXP0(HI_U8 Addr, HI_U8 NBytes, HI_U8 * Data)
{

    I2CShortCommandType I2CComm;
    memset((void*)&I2CComm, 0, sizeof(I2CComm));

    I2CComm.Bus = BUS_1;
    I2CComm.SlaveAddr = TX_SLV0;
    I2CComm.Flags = 0;
    I2CComm.NBytes = NBytes;
    I2CComm.RegAddrL = Addr;

    return BlockRead_8BAS((I2CShortCommandType *)&I2CComm, Data);
}

//-------------------------------------------------------------
static void ConvertMDDCStateIntoErrCode( HI_U8 * pErrSatus )
{
    // process error conditions behind device (Master DDC)
    if (*pErrSatus)
    {
        if( *pErrSatus & BIT_MDDC_ST_I2C_LOW)
        {
            COM_ERR("MDDCState:I2C_LOW\n");
            *pErrSatus = _MDDC_CAPTURED;
        }
        else if( *pErrSatus & BIT_MDDC_ST_NO_ACK)
        {
            COM_ERR("MDDCState:NO_ACK\n");
            *pErrSatus = _MDDC_NOACK;
        }
        else if( *pErrSatus & MASTER_FIFO_FULL)
        {
            COM_ERR("MDDCState:FIFO_FULL\n");
            *pErrSatus = _MDDC_FIFO_FULL;
        }
        else
            *pErrSatus = IIC_OK;
    }
}

HI_U8 SI_BlockReadMDDCFifo(HI_U8 ucLen, HI_U8 *pucData)
{
    HI_S32 retry = 0;
    HI_U8 ucIndex = 0;
    HI_U8 ucRet = 0;

    while (retry < 20)
    {
        ucRet = ReadByteHDMITXP0(MDDC_FIFO_CNT_ADDR);
        if (ucRet == 0)
        {
            DelayMS(1);//mdelay(1);
            retry++;
            continue;
        }

        pucData[ucIndex] = ReadByteHDMITXP0(MDDC_FIFO_ADDR);
        ucIndex ++;
        if (ucIndex >= ucLen)
            break;        
    }

    if (ucIndex == ucLen) {

        return IIC_OK;
    }   else  {
       // HI_ERR_HDMI("Read MDDC_FIFO_ERR read %u/%u Byte.\n", ucIndex, ucLen);
        return _IIC_NOACK;
    }
}

HI_U8 SI_BlockWriteMDDCFifo(HI_U8 ucLen, HI_U8 *pucData)
{
    HI_U8 ucIndex = 0;

    COM_INFO("Write MDDC_FIFO ucLen=%d \n", ucLen);

    while (1)
    {
        WriteByteHDMITXP0(MDDC_FIFO_ADDR, pucData[ucIndex]);
        if (ucIndex >= ucLen)
            break;
		
        ucIndex ++;
        DelayMS(1);//mdelay(1);
    }

    if (ucIndex == ucLen)
        return IIC_OK;

    return _IIC_NOACK;
}

//-------------------------------------------------------------
HI_U8 SI_BlockRead_MDDC(MDDCType * MDDCCmd)
{
    HI_U8 TimeOutCount;
    HI_U8 Status;
    HI_U8 Err;

#if defined (HDCP_SUPPORT)
    SI_SuspendAutoRiCheck(ON);
#endif

    //SI_DDC_Debug();
    /*Clear DDC I2C FIFO before read DDC */
    MDDCCommand(MASTER_CMD_CLEAR_FIFO);     // Abort Master DCC operation and Clear FIFO pointer
    //SI_DDC_Debug();
    /* Only transfer first 7 bytes, just to change DDC access Address. DDC data is transfered using DDC FIFO */
    Err = SI_BlockWriteHDMITXP0(MASTER_BASE + 1, 7, (HI_U8 *)MDDCCmd); // sending header, it'll clear DDC Status register too
    //SI_DDC_Debug();
    if(0 == Err)
    {
        TimeOutCount = (MDDCCmd->NBytesLSB + 5);
        while (TimeOutCount --)
        {
            DelayMS(1);//mdelay(1);//msleep(1);
            /* We can read data when DDC is not in IN_PROG Status, and DDC FIFO count is same as needed */
            Status = IsMDDCReadInProgress();
            if ((Status == 0) && (MDDCCmd->NBytesLSB == ReadByteHDMITXP0(MDDC_FIFO_CNT_ADDR)))
            {
                break;
            }
        }
        
        if(0 == TimeOutCount)
        {
            COM_ERR("\n DDC read timeout!\n");
            goto FAILE_RETURN;
        }
    }
    else
    {
        COM_ERR("DDC MASTER_BASE Write Error=%d\n", Err);
        goto FAILE_RETURN;
    }
    //SI_DDC_Debug();
    /* read data from MDDC FIFO */
    Err = SI_BlockReadMDDCFifo(MDDCCmd->NBytesLSB, (HI_U8 *)&MDDCCmd->PData[0]);
    //SI_DDC_Debug();  
    if(0 != Err)
    {
        COM_WARN("Read DDC fifo err!\n");
        goto FAILE_RETURN;
    }
    /* After transfer, check DDC status */
    Status = GetMDDCStatus();
    ConvertMDDCStateIntoErrCode(&Status);
    if(Status)
    {
        if(Status == _MDDC_NOACK)
        {         // can happen if Rx is clock stretching the SCL line. DDC bus unusable
            SI_SendCP_Packet(ON);                  // mute audio and video
            SI_SetEncryption(OFF);
            AuthState = REAUTHENTATION_REQ;     // force re-authentication
        }
        else
        {
            MDDCCommand(MASTER_CMD_ABORT);
            MDDCCommand(MASTER_CMD_CLOCK);
            ClrManualCtrlReg();
        }

        COM_INFO("%s ConvertMDDCStateIntoErrCode Error=%d\n",
            __FUNCTION__, Status);
        goto FAILE_RETURN;
    }
    
#if defined (HDCP_SUPPORT)
    SI_SuspendAutoRiCheck(OFF);     // re-enable Auto Ri
#endif

    return HI_SUCCESS;
    
FAILE_RETURN:
#if defined (HDCP_SUPPORT)
    SI_SuspendAutoRiCheck(OFF);     // re-enable Auto Ri
#endif
    return HI_FAILURE;    
}

//-------------------------------------------------------------
HI_U8 SI_BlockWrite_MDDC(MDDCType * MDDCCmd)
{
    HI_U8 TimeOutCount;

    typedef union Tmp {
        HI_U8 Status;
        HI_U8 Err;
    }TmpType;

    TmpType tmp;

#if defined (HDCP_SUPPORT)
    SI_SuspendAutoRiCheck(ON);
#endif
    /*Clear DDC I2C FIFO before read DDC */
    MDDCCommand(MASTER_CMD_CLEAR_FIFO);
    tmp.Status = GetMDDCStatus();
    /* Only transfer first 7 bytes, just to change DDC access Address. DDC data is transfered using DDC FIFO */
    tmp.Err = SI_BlockWriteHDMITXP0(MASTER_BASE + 1, 7, (HI_U8 *)MDDCCmd); // sending header, it'll clear DDC Status register too

    if(IIC_OK == tmp.Err)
    {
        /* write data from MDDC FIFO */
        tmp.Err = SI_BlockWriteMDDCFifo(MDDCCmd->NBytesLSB, (HI_U8 *)&MDDCCmd->PData[0]);
        if(IIC_OK != tmp.Err)
        {
            COM_ERR("%s Write DDC fifo Error=%d\n", __FUNCTION__, tmp.Err);
        }
    }
    else
    {
        COM_ERR("%s Write DDC MASTER_BASE Error=%d\n", __FUNCTION__, tmp.Err);
    }
    /* After transfer, check DDC status */
    if(IIC_OK != tmp.Err)
    {
        TimeOutCount = MDDCCmd->NBytesLSB;
        while (IsMDDCReadInProgress() && TimeOutCount--)
        {
           DelayMS(1);//mdelay(1);//msleep(1); //wait data has been written in ready
        }
        
        tmp.Status = GetMDDCStatus();
        ConvertMDDCStateIntoErrCode(&tmp.Status);
        if(tmp.Status == _MDDC_NOACK)
        {         // can happen if Rx is clock stretching the SCL line. DDC bus unusable
            SI_SendCP_Packet(ON);                  // mute audio and video
            SI_SetEncryption(OFF);
            AuthState = REAUTHENTATION_REQ;     // force re-authentication
        }
        else
        {
            MDDCCommand(MASTER_CMD_ABORT);
            MDDCCommand(MASTER_CMD_CLOCK);
            ClrManualCtrlReg();
        }
    }

#if defined (HDCP_SUPPORT)
    SI_SuspendAutoRiCheck(OFF);     // re-enable Auto Ri
#endif

    return tmp.Err;
}


//----:---------------------------------------------------------------------
