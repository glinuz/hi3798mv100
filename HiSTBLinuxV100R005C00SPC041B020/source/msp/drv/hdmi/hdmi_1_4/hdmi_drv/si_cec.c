/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include <linux/timer.h>
#include <linux/delay.h>
#include <si_typedefs.h>
#include <si_cec_reg.h>
#include <si_cec.h>  
#include <si_amf.h>
#include <si_config.h> 
#include <si_hdmitx.h>

#if defined (CEC_SUPPORT)
//------------------------------------------------------------------------------
// Function Name: SiI_9185_CEC_CAPTURE_ID_Set
// Function Description:
//
// Accepts: HI_U8 bInitLA
// Returns: error code
// Globals: none

// Affected Register(s): CEC_CAPTURE_ID
// Document Reference  : CEC Promgramming Interface (CPI) Programmer's Reference
// Warning: Only a single CEC device can be select with this interface even though
//          the all 16 devices can be selected.
//------------------------------------------------------------------------------
//
HI_U8 SI_CEC_CAPTURE_ID_Set( HI_U8 logical_address )
{
    HI_U8 capture_address[2];
    HI_U8 capture_addr_sel = 0x01;

    capture_address[0] = 0;
    capture_address[1] = 0;
    if( logical_address < 8 )
    {
        capture_addr_sel = capture_addr_sel << logical_address;
        capture_address[0] = capture_addr_sel;
    }
    else
    {
        capture_addr_sel   = capture_addr_sel << ( logical_address - 8 );
        capture_address[1] = capture_addr_sel;
    }

    // Set Capture Address, 0x00 and 0x0f will be capture
    SI_CEC_RegisterWriteBlock(REG__CEC_CAPTURE_ID0,capture_address,2);
    SI_CEC_RegisterWrite(REG__CEC_TX_INIT, logical_address);

    return 0;
}

//------------------------------------------------------------------------------
// Function Name: SI_SiI_CEC_SendPing
// Function Description: This command intiate sending a ping, and used for checking available
//                       CEC devices
//
// Accepts: bCECLogAddr
// Returns: none
// Globals: none
//------------------------------------------------------------------------------
void SI_SiI_CEC_SendPing ( HI_U8 bCECLogAddr )
{
    SI_CEC_RegisterWrite( REG__CEC_TX_DEST, BIT_SEND_POLL | bCECLogAddr );
}

//------------------------------------------------------------------------------
// Function Name: SiI_CEC_SetCommand
// Function Description: This command sets data for CEC transmission
//
// Accepts: pSiI_CEC
// Returns: 1 if error; 0 if no error.
// Globals: none
//------------------------------------------------------------------------------
HI_U8 SiI_CEC_SetCommand( SiI_CEC_t * pSiI_CEC )
{
    HI_U8 error = FALSE;
    HI_U8 cec_int_status_reg[2];
    HI_U32 u32Index = 0;
    CEC_INFO("***SiI_CEC_SetCommand start ***\n");
    // Clear Tx Buffer
    SI_CEC_RegisterModify(REG__CEC_DEBUG_3, BIT_FLUSH_TX_FIFO, BIT_FLUSH_TX_FIFO);

    CEC_INFO("\n TX: Dest[0x%02X],OpCode[0x%02X], Count:%d, Oprand[",
        (int)pSiI_CEC->bDestOrRXHeader,
        (int)pSiI_CEC->bOpcode,
        (int)pSiI_CEC->bCount);

    for(u32Index = 0;u32Index < pSiI_CEC->bCount;u32Index++)
    {
        CEC_INFO("0x%02x ",(int)pSiI_CEC->bOperand[u32Index]);
    }

    CEC_INFO("]\n");

    
#ifdef  CEC_TX_AUTO_CALC_ENABLED
    CEC_INFO("use CEC_TX_AUTO_CALC_ENABLED\n");
    // Enable TX_AUTO_CALC, Must be done before fill opcode and oprand!!
    error = SI_CEC_RegisterWrite(REG__CEC_TRANSMIT_DATA, BIT__TX_AUTO_CALC);
#endif
    
    // Clear Tx-related buffers; write 1 to bits to be clear directly; writing 0 has no effect on the status bit
    // Clear Tx Transmit Buffer Full Bit, Tx msg Sent Event Bit, and Tx FIFO Empty Event Bit
    cec_int_status_reg[0] = 0x64 ; 

    // Clear Tx Frame Retranmit Count Exceeded Bit.
    cec_int_status_reg[1] = 0x02 ; 
    SI_CEC_RegisterWriteBlock(REG__CEC_INT_STATUS_0, cec_int_status_reg, 2);

    // Write Source and Destination address
    SI_CEC_RegisterWrite(REG__CEC_TX_DEST,pSiI_CEC->bDestOrRXHeader);

    // Send CEC Opcode AND up to 15 Operands
    SI_CEC_RegisterWrite(REG__CEC_TX_COMMAND, pSiI_CEC->bOpcode);
    SI_CEC_RegisterWriteBlock( REG__CEC_TX_OPERANDS_0, pSiI_CEC->bOperand, pSiI_CEC->bCount);

#if 0 /*-- write register can not return fail // check code cc --*/
    if( error )
    {
        CEC_ERR("\n SiI_CEC_SetCommand(): Fail to write CEC opcode and operands\n") ;
    }
#endif 
    
    SI_CEC_RegisterWrite(REG__CEC_TRANSMIT_DATA, 0x00); //Try only once

#ifndef CEC_TX_AUTO_CALC_ENABLED
    // Write Operand count and activate send
    SI_CEC_RegisterWrite(REG__CEC_TRANSMIT_DATA, BIT_TRANSMIT_CMD | pSiI_CEC->bCount );
#endif
    CEC_INFO("***SiI_CEC_SetCommand stop ***\n");
    return error;
}

//------------------------------------------------------------------------------
// Function Name: SI_CEC_GetCommand
// Function Description: This function gets data from CEC Reception
//
// Accepts: pSiI_CEC
// Returns: none
// Globals: none
//------------------------------------------------------------------------------
HI_U8 SI_CEC_GetCommand( SiI_CEC_t * pSiI_CEC )
{
    HI_U8 error = false, NewRXState;
    HI_U8 bCount;

    bCount = pSiI_CEC->bCount  & CEC_RX_BYTE_CNT; // extract HI_U8 counter, ignore frame counter

    if ( !(pSiI_CEC->bCount & BIT_MSG_ERROR) )
    {
        pSiI_CEC->bDestOrRXHeader = SI_CEC_RegisterRead(REG__CEC_RX_CMD_HEADER); //0-3 bit define dest
        pSiI_CEC->bOpcode = SI_CEC_RegisterRead(CEC_RX_COMMAND);// cmd 0-7 bit
        SI_CEC_RegisterReadBlock(REG__CEC_RX_OPERAND_0, pSiI_CEC->bOperand, bCount);

        CEC_INFO("RX HDR:0x%02X,OpCode:0x%02X, Count:%d, \
           Oprand: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\n",
          (int)pSiI_CEC->bDestOrRXHeader,
          (int)pSiI_CEC->bOpcode,
          (int)bCount,
          (int)pSiI_CEC->bOperand[0], 
          (int)pSiI_CEC->bOperand[1], 
          (int)pSiI_CEC->bOperand[2],
          (int)pSiI_CEC->bOperand[3], 
          (int)pSiI_CEC->bOperand[4], 
          (int)pSiI_CEC->bOperand[5],
          (int)pSiI_CEC->bOperand[6], 
          (int)pSiI_CEC->bOperand[7], 
          (int)pSiI_CEC->bOperand[8],
          (int)pSiI_CEC->bOperand[9], 
          (int)pSiI_CEC->bOperand[10], 
          (int)pSiI_CEC->bOperand[11],
          (int)pSiI_CEC->bOperand[12], 
          (int)pSiI_CEC->bOperand[13], 
          (int)pSiI_CEC->bOperand[14]);
    }
    else
    {
        error = 1;
    }
    // Clear CLR_RX_FIFO_CUR;
    // Clear current frame from Rx FIFO
    SI_CEC_RegisterModify(REG__CEC_RX_CONTROL, BIT_CLR_RX_FIFO_CUR, BIT_CLR_RX_FIFO_CUR );

    // Check if more frame in Rx FIFO, if yes get HI_U8 count of next frame.
    pSiI_CEC->bRXNextCount = 0;
    
    NewRXState = SI_CEC_RegisterRead(REG__CEC_RX_COUNT);
    CEC_INFO("Nex Status:0x%x\n", NewRXState);
    //if( pSiI_CEC->bCount & 0xF0 )
    if(NewRXState & CEC_RX_CMD_CNT)
    {
        pSiI_CEC->bRXNextCount = SI_CEC_RegisterRead(REG__CEC_RX_COUNT);
    }

    return error;
}

//------------------------------------------------------------------------------
// Function Name: SiI_CEC_IntProcessing
// Function Description: This function is called on interrupt events
//                       it makes interrut service
// Accepts: SiI_CEC_Int_t * pInt
// Returns: none
// Globals: none
//------------------------------------------------------------------------------
HI_U8 SI_CEC_Interrupt_Processing ( SiI_CEC_Int_t * pInt )
{
    HI_U8 error = FALSE;
    HI_U8 cec_int_status_reg[2] = {0};
    HI_U8 bTmpRXState = 0;
    
    // Get Interrupts
    pInt->bTXState   = 0;
    pInt->bCECErrors = 0;
    pInt->bRXState   = 0;
    //Get Interrupt status
    SI_CEC_RegisterReadBlock(REG__CEC_INT_STATUS_0, cec_int_status_reg, 2);
    bTmpRXState = SI_CEC_RegisterRead(REG__CEC_RX_COUNT); //RX Count Status will be ignore?

    // Poll Interrupt and RxStatus
    if( ((cec_int_status_reg[0] & 0x7F) || cec_int_status_reg[1])
     || (bTmpRXState & 0x70) )
    {
        CEC_INFO("\nA6A7Reg: %02X %02X\n",
            (int) cec_int_status_reg[0],
            (int) cec_int_status_reg[1]);
           
        if ( cec_int_status_reg[1] & BIT_FRAME_RETRANSM_OV )
        {
           // Frame Retransmit Count Exceeded event
           CEC_INFO("\n!CEC_A7_TX_RETRY_EXCEEDED![%02X][%02X]\n",
               (int) cec_int_status_reg[0],
               (int) cec_int_status_reg[1]);
           
            // flash TX otherwise after writing clear interrupt
            // BIT_FRAME_RETRANSM_OV the TX command will be re-send
           SI_CEC_RegisterModify(REG__CEC_DEBUG_3,BIT_FLUSH_TX_FIFO, BIT_FLUSH_TX_FIFO);
        }
        //
        // Clear set bits that are set, remove realtive interrupt bit
        //
        SI_CEC_RegisterWriteBlock(REG__CEC_INT_STATUS_0,cec_int_status_reg,2);

        CEC_INFO("\nA6A7Reg: %02X %02X\n", (int) cec_int_status_reg[0], (int) cec_int_status_reg[1]);

        msleep(10);//Very Important, Must Add delay!
        // CEC RX Processing
        if (( cec_int_status_reg[0] & BIT_RX_MSG_RECEIVED ) || (bTmpRXState & 0x70))
        {
            if(bTmpRXState & 0x70)
            {
                if(! ( cec_int_status_reg[0] & BIT_RX_MSG_RECEIVED ))
                {
                    CEC_INFO("Interrupt CEC Rx Status do not set! [%02X][%02X], bTmpRXState:[%02X]\n",
                        (int) cec_int_status_reg[0], (int) cec_int_status_reg[1], bTmpRXState);
                }
            }
            // Save number of frames in Rx Buffer
            pInt->bRXState = SI_CEC_RegisterRead(REG__CEC_RX_COUNT);
            CEC_INFO("Receive Frame pInt->bRXState:0x%x\n", pInt->bRXState);
        }

        // RX Errors processing
        if ( cec_int_status_reg[1] & BIT_SHORT_PULSE_DET )
        {
            pInt->bCECErrors |= SiI_CEC_ERROR_SHORTPLUSEDETECT;
        }

        if ( cec_int_status_reg[1] & BIT_START_IRREGULAR )
        {
            pInt->bCECErrors |= SiI_CEC_ERROR_STARTBITIRREGULAR;
        }

        if ( cec_int_status_reg[1] & BIT_RX_FIFO_OVERRUN ) // fixed per Uematsu san
        {
            CEC_INFO("CEC RX Overflow, Flush Rx FIFO\n");
            pInt->bCECErrors |= SiI_CEC_ERROR_RXOVERFLOW;
            // Clear All frame from Rx FIFO
            SI_CEC_RegisterModify(REG__CEC_RX_CONTROL, BIT_CLR_RX_FIFO_ALL, BIT_CLR_RX_FIFO_ALL );
        }

        // TX Processing
        if ( cec_int_status_reg[0] & BIT_TX_FIFO_EMPTY )     //0x04
        {
            pInt->bTXState = SI_CEC_TX_WAITCMD;
        }
        if ( cec_int_status_reg[0] & BIT_TX_MESSAGE_SENT )   //0x20
        {
            pInt->bTXState = SI_CEC_TX_SENDACKED;
        }
        if ( cec_int_status_reg[1] & BIT_FRAME_RETRANSM_OV )   //0x02
        {

            pInt->bTXState = SI_CEC_TX_SENDFAILURE;
        }
    }

    return error;
}

HI_VOID SI_CEC_SetUp(void)
{
    CEC_INFO("set CEC_SETUP\n");
    WriteByteHDMICEC(0X8E, 0x04);
}

/*----------------------------------END-----------------------------*/
#endif

