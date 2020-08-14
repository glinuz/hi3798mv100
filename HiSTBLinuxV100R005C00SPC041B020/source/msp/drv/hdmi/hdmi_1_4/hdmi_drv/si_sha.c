/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
/* All rights reserved contains SHA1 code adapted from                             */
/* http://www.di-mgt.com.au/src/sha1.c.txt                                         */
#include <linux/string.h>
#include "si_sha.h"
#include "si_hdcp.h"
#include "si_hdcpdefs.h"
#include "si_fifo.h"
#include "si_isr.h"
#include "si_infofrm.h"
#include "si_defstx.h"
#include "drv_hdmi.h"
#include "si_timer.h"
#include "drv_global.h"
#include "hi_osal.h"


#define __HISI_ENVIRONMENT__
#if defined (HDCP_SUPPORT)

//------------------------------------------------------------------------------
static HI_U8 GetFIFOSize(void)
{
    HI_U8 BStatusReg = 0;
    MDDCBlockReadHDCPRX(1, DDC_BSTATUS_ADDR, &BStatusReg);
    return BStatusReg & 0x7F;

}
//------------------------------------------------------------------------------
static HI_U8 PlacePadBitCounter(SHA_CTX * Sha)
{
    HI_U8 i, bytePos;
    HI_U8 Status;
    bytePos = (HI_U8)Sha->byteCounter&0x0003;   // get posstion in SHA HI_U16
    i = (HI_U8)((Sha->byteCounter&0x0003F)>>2); // get SHA's HI_U16 address
    if(bytePos == 0)
        Sha->Data[i]|=0x80000000;
    else if(bytePos == 1)
        Sha->Data[i]|=0x00800000;
    else   if(bytePos == 2)
        Sha->Data[i]|=0x00008000;
    else  if(bytePos == 3)
        Sha->Data[i]|=0x00000080;
    Status = PADED;
    if( ((Sha->byteCounter & 0x3f)>>2)< 14)
    {
        Sha->Data[15] = Sha->byteCounter*8;
        Status =  END_PLACED;
    }
    return Status;
}
//------------------------------------------------------------------------------
static void PlaceBitCounter(SHA_CTX * Sha)
{
    HI_U8 i;

    for(i = 0; i < 16; i++) // clear data buffer
        Sha->Data[i]=0;
    Sha->Data[15] = Sha->byteCounter*8;

}
//------------------------------------------------------------------------------

static HI_U8 GetByteBStatusMX(HI_U8 Addr)
{
    HI_U8 Data[2];

    if(Addr <2 )
    {
        // Read B Status
        MDDCBlockReadHDCPRX(2, DDC_BSTATUS_ADDR, Data);
        COM_INFO("BSTATUS:0x%x,0x%x\n", Data[0], Data[1]);
        return Data[Addr];
    }
    else if(Addr < 10)
    {
        // MX
        return MX_TX[Addr-2];
     }

     return 0;
}
//--------------------------------------------------------------------------------------

static HI_U8 GetBStatusMX(SHA_CTX IRAM * Sha)
{
    HI_U8 i, bytePos;



    if((Sha->byteCounter&0x0003F)== 0) // New data frame
        for(i = 0; i < 16; i++)           // clear data buffer
            Sha->Data[i]=0;

    bytePos = (HI_U8)Sha->byteCounter&0x0003;   // get posstion in SHA HI_U16
    i = (HI_U8)((Sha->byteCounter&0x0003F)>>2); // get SHA's HI_U16 address

#if 0
    DEBUG_PRINTK("bytePos:%d, i:%d\n", bytePos, i);
#endif
    if(bytePos == 1)
    {
        Sha->Data[i] |= ((HI_U32)GetByteBStatusMX(Sha->BStatusMXCounter)<<16);
        Sha->BStatusMXCounter++;
        Sha->byteCounter++;
        if(Sha->BStatusMXCounter!=BSTATMX_SIZE)
        {
            Sha->Data[i] |= ((HI_U32)GetByteBStatusMX(Sha->BStatusMXCounter)<<8);
            Sha->BStatusMXCounter++;
            Sha->byteCounter++;
        }
        if(Sha->BStatusMXCounter!=BSTATMX_SIZE)
        {
            if(Sha->BStatusMXCounter >= 10)
                return 0;

            Sha->Data[i] |= (HI_U32)GetByteBStatusMX( Sha->BStatusMXCounter);
            Sha->BStatusMXCounter++;
            Sha->byteCounter++;
        }
    }
    if(bytePos == 2)
    {
        Sha->Data[i] |= ((HI_U32)GetByteBStatusMX( Sha->BStatusMXCounter)<<8);
        Sha->BStatusMXCounter++;
        Sha->byteCounter++;
        if(Sha->BStatusMXCounter)
        {
            if(Sha->BStatusMXCounter >= 10)
                return 0;

            Sha->Data[i] |= (HI_U32)GetByteBStatusMX( Sha->BStatusMXCounter);
            Sha->BStatusMXCounter++;
            Sha->byteCounter++;
        }
    }
    if(bytePos == 3)
    {
        if(Sha->BStatusMXCounter >= 10)
            return 0;
        Sha->Data[i] |= (HI_U32)GetByteBStatusMX( Sha->BStatusMXCounter);
        Sha->BStatusMXCounter++;
        Sha->byteCounter++;
    }
    if(bytePos !=0)
        i++;
#if 0
    {
        HI_U8 index;

        DEBUG_PRINTK("DEBUG_PRINTK sha data xx \n");
        for(index = 0; index < 16; index ++)
        {
            DEBUG_PRINTK("Sha->Data[%d]= 0x%lx \n" , index, Sha->Data[index]);
        }
        DEBUG_PRINTK("\nEnd of Sha data\n");
    }
#endif
    while( i < 16 )
    {
        if(Sha->BStatusMXCounter==BSTATMX_SIZE)
        {// no bytes remain in
            return BSTATMX_FINISHED;

        }
        else
        {
            Sha->Data[i] |= ((HI_U32)GetByteBStatusMX(Sha->BStatusMXCounter) << 24);
            Sha->byteCounter++;
            Sha->BStatusMXCounter++;
            if(Sha->BStatusMXCounter==BSTATMX_SIZE)
            {// no bytes remain in
                return BSTATMX_FINISHED;

            }
            else
            {
                Sha->Data[i]|= (HI_U32)(GetByteBStatusMX((Sha->BStatusMXCounter)&0xff)<<16);
                Sha->byteCounter++;
                Sha->BStatusMXCounter++;
                if(Sha->BStatusMXCounter==BSTATMX_SIZE)
                {// no bytes remain in
                    return BSTATMX_FINISHED;

                }
                else
                {
                    Sha->Data[i] |= ((HI_U32)GetByteBStatusMX((Sha->BStatusMXCounter)&0xff)<<8);
                    Sha->byteCounter++;
                    Sha->BStatusMXCounter++;
                    if(Sha->BStatusMXCounter==BSTATMX_SIZE)
                    {// no bytes remain in
                        return BSTATMX_FINISHED;

                    }
                    else
                    {
                        Sha->Data[i] |= (HI_U32)GetByteBStatusMX((Sha->BStatusMXCounter)&0xff);
                        Sha->byteCounter ++;
                        Sha->BStatusMXCounter++;
                    }
                }
            }
        }

        i++;
#if 0
        {
            HI_U8 index;

            DEBUG_PRINTK("DEBUG_PRINTK sha data yy \n");
            for(index = 0; index < 16; index ++)
            {
                DEBUG_PRINTK("Sha->Data[%d]= 0x%lx \n" , index, Sha->Data[index]);
            }
            DEBUG_PRINTK("\nEnd of Sha data\n");
        }
#endif     
    }
    return    BSTATMX_PULLED;
}
//---------------------------------------------------------------------------------------
static HI_U8 GetKSVList(SHA_CTX IRAM * Sha, HI_U16 i)
{
    HI_U8 Aux = 0;
    HI_U8 BufStatus;
    HI_U8 *tmppoint, *pdata;

    BufStatus = 0;

    for(Aux = 0; Aux < 16; Aux++) // clear data buffer
        Sha->Data[Aux]=0;
    // Check how many bytes remain to read
    // total number = KSV list (up to  5 * 127 = 635) +
    //                BStatus (2 bytes) +
    //                MX (8 bytes)

    i = i - Sha->byteCounter;   // Get how many bytes KSV list remain to receive
    Aux = 0;

    while(Aux < 16)
    {
        if(i==0)
        {// no bytes remain in
            BufStatus = KSV_FINISHED;
            break;
        }
        else
        {
            if(i > 4)
            {
                SI_MDDCTakeBlockFromFIFO(4, (HI_U8 *)&Sha->Data[Aux]);             
                //DEBUG_PRINTK("GetKSVList 1 Sha->Data[%d]:0x%x i:%d\n", Aux, Sha->Data[Aux], i);
                Sha->byteCounter+=4;
                i-=4;
#if 0 /*-- The value i at least == 1 // check code cc--*/
                if(i==0)
                {// no bytes remain in
                    BufStatus = KSV_FINISHED;
                    break;
                }
#endif
            }
            else
            {
                SI_MDDCTakeBlockFromFIFO(i, (HI_U8 *)&Sha->Data[Aux]);
                //DEBUG_PRINTK("GetKSVList 2 Sha->Data[%d]:0x%x  i:%d\n", Aux, Sha->Data[Aux], i);       
                Sha->byteCounter+=i;
                BufStatus = KSV_FINISHED;
                break;
            }
        }

        Aux++;
    }
    
    pdata = (HI_U8 *)&(Sha->Data[0]);
    
#ifdef __HISI_ENVIRONMENT__ /* lc added for HDCP Repeater */
    {
        HI_U8 index_byte, index, tmp, *Data, NBytes;
        HI_U32 pos;
        HI_U8 tmp_buf[512] = {0};

        memset(tmp_buf, 0, ABS_DBG_MEM_IN_BUF_SIZE);
        tmp_buf[0] = '\0';
        pos = 0;
        /*big-endian and little-endian Byte sequence translation*//*CNcomment:大小字节序整体转换 */
        for(index_byte = 0; index_byte < 16; index_byte ++)
        {           
            Data = (HI_U8 *)&Sha->Data[index_byte];
            NBytes = 4;
            //HDCP_INFO("orignal: ");
            HI_OSAL_Snprintf(tmp_buf + pos, ABS_DBG_MEM_IN_BUF_SIZE, "orignal : ");
            pos += strlen("orignal : ");
            for(index = 0; index < NBytes; index ++)
            {
                char  buf[32] = {0};
                
                //HDCP_INFO("0x%02x, ", Data[index]);
                HI_OSAL_Snprintf(buf, 32, "0x%02x, ", Data[index]);
                HI_OSAL_Strncat(tmp_buf + pos, buf, 32);
                pos += strlen(buf);
            }
            //HDCP_INFO("\n");
            HDCP_INFO("%s\n", tmp_buf);
            memset(tmp_buf, 0, ABS_DBG_MEM_IN_BUF_SIZE);
            tmp_buf[0] = '\0';
            pos = 0;

            for(index = 0; index < NBytes / 2; index ++)
            {
                tmp = Data[index];
                Data[index] = Data[NBytes - 1 - index];
                Data[NBytes - 1 - index] = tmp;
            }

            //HDCP_INFO("new: ");
            HI_OSAL_Snprintf(tmp_buf + pos, ABS_DBG_MEM_IN_BUF_SIZE, "new     : ");
            pos += strlen("new     : ");
            for(index = 0; index < NBytes; index ++)
            {
                char  buf[32] = {0};
                //HDCP_INFO("0x%02x, ", Data[index]);
                HI_OSAL_Snprintf(buf, 32, "0x%02x, ", Data[index]);
                HI_OSAL_Strncat(tmp_buf + pos, buf, 32);
                pos += strlen(buf);
            }
            //HDCP_INFO("\n");
            HDCP_INFO("%s\n", tmp_buf);  
            memset(tmp_buf, 0, ABS_DBG_MEM_IN_BUF_SIZE);
            tmp_buf[0] = '\0';
            pos = 0;
        }
    }            
    {
        HI_U8 index;
        HI_U32 pos;
        HI_U8 tmp_buf[ABS_DBG_MEM_IN_BUF_SIZE] = {0};
        
        memset(tmp_buf, 0, ABS_DBG_MEM_IN_BUF_SIZE);
        tmp_buf[0] = '\0';
        pos = 0;
        HDCP_INFO("\n\nBegin GetKSVList debug Aux:%d\n\n", Aux);        
        for(index = 0; index < (Aux+1); index ++)
        {
            char  buf[32] = {0};
            //HDCP_INFO("0x%x, ", Sha->Data[index&0xf]);
            HI_OSAL_Snprintf(buf, 32, "0x%08x, ", Sha->Data[index&0xf]);
            HI_OSAL_Strncat(tmp_buf + pos, buf, 32);
            pos += strlen(buf);

            if(((index + 1) % 5 == 0))
            {
                HDCP_INFO("%s\n", tmp_buf); 
                memset(tmp_buf, 0, ABS_DBG_MEM_IN_BUF_SIZE);
                tmp_buf[0] = '\0';
                pos = 0;
            }
        }
        //HDCP_INFO("\n");
        memset(tmp_buf, 0, ABS_DBG_MEM_IN_BUF_SIZE);
        tmp_buf[0] = '\0';
        pos = 0;
        
        HDCP_INFO("Byte sequence in Sha->Data is:\n");        
        tmppoint = (HI_U8 *)&(Sha->Data[0]);
        for (index = 0; index < ((Aux+1) * 4); index ++)
        {
            char  buf[32] = {0};
            //HDCP_INFO("[%d]:0x%x, ", index, tmppoint[index&0x3f]);
            HI_OSAL_Snprintf(buf, 32, "[%02d]:0x%02x, ", index, tmppoint[index&0x3f]);
            HI_OSAL_Strncat(tmp_buf + pos, buf, 32);
            pos += strlen(buf);

            if(((index + 1) % 5 == 0))
            {
                HDCP_INFO("%s\n", tmp_buf);
                memset(tmp_buf, 0, ABS_DBG_MEM_IN_BUF_SIZE);
                tmp_buf[0] = '\0';
                pos = 0;
            }
        }
        HDCP_INFO("\n");
        memset(tmp_buf, 0, ABS_DBG_MEM_IN_BUF_SIZE);
        tmp_buf[0] = '\0';
        pos = 0;

        HDCP_INFO("\n\n***nEnd of GetKSVList debug***\n\n");
    }
#endif

    return BufStatus;
}
//------------------------------------------------------------------------------

static void SHSTransform(SHA_CTX IRAM *sha)
{
    HI_U32 A, B, C, D, E, TEMP;     /* Local vars */
    HI_U8 i, s;

    A = sha->Digest[ 0 ];
    B = sha->Digest[ 1 ];
    C = sha->Digest[ 2 ];
    D = sha->Digest[ 3 ];
    E = sha->Digest[ 4 ];

    for(i = 0; i<80; i++)
    {
        s = i & 15;
        if(i>=16)
        {
            sha->Data[s]= sha->Data[(s+13)&15]^sha->Data[(s+8)&15]^sha->Data[(s+2)&15]^sha->Data[s];
            sha->Data[s]= (sha->Data[s]<<1)|(sha->Data[s]>>31);
        }
        TEMP = ((A<<5)|(A>>27)) + E + sha->Data[s];
        if(i<=19)
        {
            TEMP+=K1;
            TEMP+=f1(B,C,D);
        }
        else if(i<=39)
        {
            TEMP+=K2;
            TEMP+=f2(B,C,D);
        }
        else if(i<=59)
        {
            TEMP+=K3;
            TEMP+=f3(B,C,D);
        }
        else
        {
            TEMP+=K4;
            TEMP+=f4(B,C,D);
        }
        E = D;
        D = C;
        C = (B<<30)|(B>>2);
        B = A;
        A = TEMP;

    }

    /* Build message digest */
    sha->Digest[ 0 ] += A;
    sha->Digest[ 1 ] += B;
    sha->Digest[ 2 ] += C;
    sha->Digest[ 3 ] += D;
    sha->Digest[ 4 ] += E;
    COM_INFO("Digest value:0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", sha->Digest[ 0 ], sha->Digest[ 1 ], sha->Digest[ 2 ], sha->Digest[ 3 ], sha->Digest[ 4 ]);

}
//------------------------------------------------------------------------------
#ifndef __HISI_ENVIRONMENT__
static void LEndianBEndianConvert(HI_U8 * Number)
{
    HI_U8 TmpVal;

    TmpVal = Number[0];
    Number[0] = Number[3];
    Number[3] = TmpVal;

    TmpVal = Number[1];
    Number[1] = Number[2];
    Number[2] = TmpVal;

}
#endif
//------------------------------------------------------------------------------
static Bool CompareVi(SHA_CTX IRAM * Sha)
{
    HI_U32 VPrime = 0;
    Bool Result = TRUE;
    HI_U8 i;
    
    SI_MDDCInitReadBlockFromFIFO( DDC_V_ADDR, 20 );
    for(i = 0; i < 5; i ++)
    {
        SI_MDDCTakeBlockFromFIFO(4, (HI_U8 *)&VPrime);
#ifndef __HISI_ENVIRONMENT__   /* lc added for HDCP Repeater */
        LEndianBEndianConvert((HI_U8 *)&VPrime);
#endif
        COM_INFO("Sha->Digest[%d]:0x%x, VPrime:0x%x\n", i, Sha->Digest[i], VPrime);
        if( Sha->Digest[i] != VPrime )
        {
            Result = FALSE;
            break;
        }
    }
    COM_INFO("CompareVi is Result 0x%x *****\n", Result);

    return Result;
}

//------------------------------------------------------------------------------
static void SHAInit( SHA_CTX IRAM *shsInfo)
{

    /* Set the h-vars to their initial values */
    shsInfo->Digest[0] = h0init;
    shsInfo->Digest[1] = h1init;
    shsInfo->Digest[2] = h2init;
    shsInfo->Digest[3] = h3init;
    shsInfo->Digest[4] = h4init;

    COM_INFO("SHAInit:0x%x,0x%x,0x%x,0x%x\n", shsInfo->Digest[0], shsInfo->Digest[1], shsInfo->Digest[2], shsInfo->Digest[3]);

    /* Initialise bit count */

    shsInfo->byteCounter = 0;
    shsInfo->BStatusMXCounter = 0;
}

//-------------------------------------------------------------------
void SI_SHAHandler(void)
{
    SHA_CTX IRAM sha;
    HI_U8 BufStatus;
    HI_U16 KSVSize;
    HI_U8 BStatusReg[2];

    if( AuthState == REQ_SHA_CALC )
    {
        HDCP_INFO("Repeater:enter hdcp protocol 2 REQ_SHA_CALC\n");

        SI_AutoRiCheck ( OFF );


        /* Read Bstatus */
        memset((void *)BStatusReg, 0x00, 2);
        MDDCBlockReadHDCPRX(2, DDC_BSTATUS_ADDR, BStatusReg);
        /* Check MAX_DEVS_EXCEEDED and MAX_CASCADE_EXCEEDED */
        HDCP_INFO("BStatusReg[0]:0x%x, BStatusReg[1]:0x%x\n", BStatusReg[0], BStatusReg[1]);
        if ((BStatusReg[0]&DDC_BIT_BSTATUS1_DEV_EXC)
          ||(BStatusReg[1]&DDC_BIT_BSTATUS2_CAS_EXC))
        {
            HDCP_ERR("Reach Max device/deep, Reauthenicatin!!!\n");
            AuthState = REQ_AUTHENTICATION;
            SI_SendCP_Packet(ON);
            SI_SetEncryption(OFF);
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
            return;
        }
        
        KSVSize =  GetFIFOSize()*5;         //Get FIFO size (number of KSV) /// // Must be done BEFORE reading KSV FIFO!!! - 8/24/07
        HDCP_INFO("KSVSize(DEVICE_COUNT*5):%d\n", KSVSize);

        if (KSVSize)
        {
            HDCP_INFO("read KSV List\n");
            SI_MDDCInitReadBlockFromFIFO(DDC_KSV_FIFO_ADDR, KSVSize );
        }
        
        SHAInit(&sha);
        HDCP_INFO("After SHAInit sha->Digest:0x%x, 0x%x, 0x%x, 0x%x\n", sha.Digest[0], sha.Digest[1], sha.Digest[2], sha.Digest[3]);
        while (GetKSVList(&sha, KSVSize)==KSV_PULLED)
        {
            SHSTransform(&sha);
        }
#if 0        
        DEBUG_PRINTK("111 After SHSTransform sha.Digest:0x%x, 0x%x, 0x%x, 0x%x\n", sha.Digest[0], sha.Digest[1], sha.Digest[2], sha.Digest[3]);
        {
            HI_U8 index;

            DEBUG_PRINTK("DEBUG_PRINTK sha data\n");
            for(index = 0; index < 16; index ++)
            {
                DEBUG_PRINTK("sha.Data[%d]= 0x%lx \n" , index, sha.Data[index]);
            }
            DEBUG_PRINTK("\nEnd of sha data\n");
        }
#endif
        if(BSTATMX_PULLED==GetBStatusMX(&sha))		/// Got here if less than 512 bits that were left in FIFO were just read into buffer,
        {
            SHSTransform(&sha);					/// meaning that the buffer has room for more data. Now attempt to copy  into the
            /// same buffer the 2 bytes of BStatus and 8 of MX. if BSTATMX_PULLED, it means that
            /// the last KSV's + BSttus + MX exceeded 512 bit. Transform W/O padding.

            GetBStatusMX(&sha);			/// Now get the leftovers of KSV BStatus and MX. Will never reach 14 DWords. Must pad.
            PlacePadBitCounter(&sha);	/// Pad down to DWord 14. Store length in DWord 15.
            SHSTransform(&sha);			/// Hash this very last block after padding it.
        }
        else
        {
            BufStatus = PlacePadBitCounter(&sha);
            SHSTransform(&sha);
            if(BufStatus != END_PLACED)
            {
                PlaceBitCounter(&sha);
                SHSTransform(&sha);
            }
        }
        SI_timer_count();
        if(CompareVi(&sha))
        {
            AuthState = AUTHENTICATED;

            SI_AutoRiCheck ( ON );

            HDCP_INFO("SI_SHAHandler ok, enter hdcp protocol 3\n");
            SI_timer_count();
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_SUCCESS);
            SI_timer_count();
        }
        else
        {
        	HDCP_ERR("SI_SHAHandler(hdcp protocol 2) fail\n");
            AuthState = FAILED_ViERROR;
            DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HDCP_FAIL);
        }
    }
    return;
}

#endif

