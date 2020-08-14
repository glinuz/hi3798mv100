/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
// used to read FIFO form DDC bus
//-------------------------------------------------------------------
#include <linux/string.h>
#include <linux/delay.h>
#include "si_typedefs.h"
#include "si_hdcpdefs.h"
#include "si_global.h"
#include "si_mddc.h"
#include "si_hdmitx.h"
#include "si_hlviic.h"
#include "si_delay.h"
#if defined (HDCP_SUPPORT)
#define GetAvailableBytesInFIFO()   ReadByteHDMITXP0(MDDC_FIFO_CNT_ADDR)

//-------------------------------------------------------------------
HI_U8 SI_MDDCInitReadBlockFromFIFO(HI_U8 Addr, HI_U16 NBytes)
{
    TmpDType TmpD;
    
    memset((void*)&TmpD, 0, sizeof(TmpD));
    
    MDDCCommand(MASTER_CMD_ABORT);
    MDDCCommand(MASTER_CMD_CLEAR_FIFO);
    
    TmpD.MDDC.SlaveAddr = HDCPRX_SLV;
    TmpD.MDDC.Offset = 0;
    TmpD.MDDC.RegAddr = Addr;
    TmpD.MDDC.NBytesLSB = (HI_U8) (NBytes & 0xFF);
    TmpD.MDDC.NBytesMSB = (HI_U8) (NBytes >> 8);
    TmpD.MDDC.Dummy = 0;
    TmpD.MDDC.Cmd = MASTER_CMD_SEQ_RD;
    /*In order to change the adress of DDC written and read ,only seven bytes are transmited,the data is transmited by MDDC FIFO*//*CNcomment:仅传输前7个字节的数据,目的是为了改变DDC的读写地址，数据通过后续的函数从 MDDC FIFO 传输 */
    return SI_BlockWriteHDMITXP0(MASTER_BASE + 1, 7, (HI_U8 *)&TmpD.MDDC);
}

//-------------------------------------------------------------------------

HI_U8 SI_MDDCTakeBlockFromFIFO(HI_U8 NBytes, HI_U8 * Data)
{
    HI_U8 TimeOutCount, ErrCode;

    TimeOutCount = NBytes * 2;
    while((GetAvailableBytesInFIFO() < NBytes)&& TimeOutCount--)
    {
        DelayMS(1);
    }
    
    if(TimeOutCount)
    {
        //* modified by z00146142
        //        ErrCode = SI_BlockReadHDMITXP0(MDDC_FIFO_ADDR, NBytes, Data);
        ErrCode = SI_BlockReadMDDCFifo(NBytes, Data);
    }
    else
    {
        ErrCode =  _IIC_NOACK;
    }

    if (ErrCode != IIC_OK)
    {
        COM_ERR("%s timeout _IIC_NOACK\n", __FUNCTION__);
    }
     
    return ErrCode;
}
#endif
//-------------------------------------------------------------------------

