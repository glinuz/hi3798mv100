
#include <linux/string.h>

#include "si_edid.h"
//#include "mddc.h"
#include "si_vmode.h"

#include "hdmitx.h"
#include "hi_common.h"
#include "hi_unf_hdmi.h"

static HI_U8 g_EdidMen[EDID_SIZE];
static HI_UNF_EDID_BASE_INFO_S g_stEdidInfo;
static HI_BOOL g_bErrList[EDID_MAX_ERR_NUM] = {0};
static HI_BOOL g_bGetReadEdidFlag = HI_FALSE;

static HI_S32 BlockCheckSum(HI_U8 *pEDID)
{
    HI_U8 u8Index;
    HI_U32 u32CheckSum = 0;
    //HI_U8 u8CheckSum = 0;

    if(pEDID == HI_NULL)
    {
        HI_ERR_HDMI("null point checksum\n");
        return HI_FAILURE;
    }

    for(u8Index = 0; u8Index < EDID_BLOCK_SIZE; u8Index++)
    {
        u32CheckSum += pEDID[u8Index];
    }

    //u8CheckSum = (256 - (u32Sum%256))%256;
    if((u32CheckSum & 0xff) != 0x00)
    {
         HDMIPrint("addr(0x7F):0x%02x,checksum:0x%02x,sum:0x%x\n",
                    pEDID[EDID_BLOCK_SIZE - 1],u32CheckSum);
         
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_VOID SI_DDCStatus (HI_U32 u32Status)
{
     if(u32Status & FWT_USE)
     {
        HDMIPrint("DDC FIFO Write In Use\n");
     }
     if (u32Status & FRD_USE)
     {
        HDMIPrint("DDC FIFO Read In Use\n");
     }
     if (u32Status & FIFO_EMP)
     {
        HDMIPrint("DDC FIFO Empty\n");
     }
     if (u32Status & FIFO_FULL)
     {
        HDMIPrint("DDC FIFO Full\n");
     }
     if (u32Status & IN_PROG)
     {
        HDMIPrint(" DDC operation in progress\n");
     }
     if (u32Status & NO_ACK)
     {
        HDMIPrint(" DDC NO_ACK\n");
     }
     if (u32Status & BUS_LOW)
     {
        HDMIPrint(" DDC BUS_LOW\n");
     }
     HDMIPrint("DDC u32Status:0x%x\n",u32Status);
}

HI_S32 SI_ReadEDID (HI_U8 NBytes, HI_U16 OffsetAddr, HI_U8 * pData)
{
    HI_U32 u32RegValue;
    HI_U32 u32TryCnt = 0;
    HI_U32 u32Index = 0;
    HI_U32 u32Reg;
    WriteByteHDMITXP0(DDC_DELAY_CNT, 0x1a);
    WriteByteHDMITXP0(DDC_DEV_ADDR, EDID_DEV_ADDR);
    WriteByteHDMITXP0(DDC_OFFSET, OffsetAddr);
    WriteByteHDMITXP0(DDC_COUNT1, NBytes);//default NBytes == 16
    WriteByteHDMITXP0(DDC_COUNT2, 0);
    WriteByteHDMITXP0(DDC_CMD, CLEAR_FIFO);
    WriteByteHDMITXP0(DDC_CMD, SEQ_NO_ACK);//start read EDID
    WriteByteHDMITXP0(DDC_DEV_ADDR, EDID_DEV_ADDR);
    u32RegValue = ReadByteHDMITXP0(DDC_STATUS);
    while(u32TryCnt++ < 20)
    {
         u32RegValue = ReadByteHDMITXP0(DDC_STATUS);
         u32Reg = ReadByteHDMITXP0(DDC_FIFOCNT);
         u32Reg =u32Reg&0x1F;
          if(((u32RegValue&IN_PROG) != IN_PROG) && (NBytes == u32Reg))
         //if(((u32RegValue&IN_PROG) != IN_PROG) && (NBytes == ReadWordHDMITXP0(DDC_FIFOCNT)))
         {
            for(u32Index = 0; u32Index < NBytes; u32Index++)
            {
                pData[u32Index] = ReadByteHDMITXP0(DDC_DATA);
            }
            break; //read over!
         }
         DelayMS(1);//delay 1ms
    }
    if(u32TryCnt >= 20)
    {
       return HI_FAILURE;
    }
    //HDMIPrint("u32TryCnt:%d\n",u32TryCnt);
    return HI_SUCCESS;
}


static HI_S32 GetExtBlockNum(HI_U8 ExtBlockNum)
{
    if(ExtBlockNum > 3)
    {
        ExtBlockNum = 3;
        HI_ERR_HDMI("Ext Block num is too big\n");
    }
    return ExtBlockNum;
}

HI_S32 SI_ReadSinkEDID(HI_VOID)
{
    HI_U32 u32Index, u32ExtBlockNum, s32Ret, i;
    HI_U8  u8Data[16];
    HI_U16 u16Byte = 16;
    memset(g_EdidMen, 0, EDID_SIZE);
   // DDC_Adjust();
    HDMIPrint("read block one edid\n");
    //read first block edid
    for(u32Index = 0; u32Index < EDID_BLOCK_SIZE; u32Index += u16Byte)
    {
        s32Ret = SI_ReadEDID(u16Byte, u32Index, u8Data);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_HDMI("can't read edid block 1\n");
            return EDID_READ_FIRST_BLOCK_ERR;
        }
       
        for(i = 0; i < u16Byte; i++)
        {
            g_EdidMen[u32Index + i] = u8Data[i];//save mem
        }
        
        if((u32Index)%EDID_BLOCK_SIZE == 0)
        {
            HDMIPrint("EDID Debug Begin, Block_Num:%d(*128bytes):\n", (u32Index/EDID_BLOCK_SIZE + 1));
        }
        HDMIPrint("%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X\n",
        u8Data[0], u8Data[1], u8Data[2], u8Data[3], u8Data[4], u8Data[5], u8Data[6], u8Data[7],
        u8Data[8], u8Data[9], u8Data[10], u8Data[11], u8Data[12], u8Data[13], u8Data[14], u8Data[15]);
        //HDMIPrint("\n");
    }

    u32ExtBlockNum = GetExtBlockNum(g_EdidMen[EXT_BLOCK_ADDR]);
    g_stEdidInfo.u8ExtBlockNum = u32ExtBlockNum;

    if(u32ExtBlockNum > 3)
    {
        HDMIPrint("Extern block too big, correct to 3 \n");
        u32ExtBlockNum = 3;
    }
    
    //check first block sum
    s32Ret = BlockCheckSum(g_EdidMen);
    if(s32Ret != HI_SUCCESS)
    {
        HDMIPrint("First Block Crc Error! \n");
        g_bErrList[_1ST_BLOCK_CRC_ERROR] = HI_TRUE;
    }

    //read extern block
    HDMIPrint("read extern block edid\n");
    for(u32Index = EDID_BLOCK_SIZE; u32Index < EDID_BLOCK_SIZE *(u32ExtBlockNum + 1); u32Index += u16Byte)
    {
        s32Ret = SI_ReadEDID(u16Byte, u32Index, u8Data);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_HDMI("can't read edid block %d \n",(u32Index/EDID_BLOCK_SIZE + 1));
            return (u32Index/EDID_BLOCK_SIZE + 1); // return err block no
        }
        
        for(i = 0; i < u16Byte; i++)
        {
            g_EdidMen[u32Index + i] = u8Data[i];//save mem
            //HDMIPrint("0x%02x,",u8Data[i]); 
        }

        if((u32Index)%EDID_BLOCK_SIZE == 0)
        {
            HDMIPrint("EDID Debug Begin, Block_Num:%d(*128bytes):\n", (u32Index/EDID_BLOCK_SIZE + 1));
        }
        HDMIPrint("%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X\n",
        u8Data[0], u8Data[1], u8Data[2], u8Data[3], u8Data[4], u8Data[5], u8Data[6], u8Data[7],
        u8Data[8], u8Data[9], u8Data[10], u8Data[11], u8Data[12], u8Data[13], u8Data[14], u8Data[15]);
        //HDMIPrint("\n");
    }
    
    return HI_SUCCESS;
}


static HI_S32 CheckHeader(HI_U8 *pData)
{
    HI_U32 u32Index;
    const HI_U8 Block_Zero_header[] ={0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00};

    for (u32Index = 0; u32Index < 8; u32Index ++)
    {
        if(pData[u32Index] != Block_Zero_header[u32Index])
        {
            HDMIPrint("Index:%d, 0x%02x\n",u32Index, pData[u32Index]);
            g_bErrList[BAD_HEADER] = HI_TRUE;
        }        
    }
    return HI_SUCCESS;
}

static HI_S32 ParseVendorInfo(EDID_FIRST_BLOCK_INFO *pData)
{
    HI_U16 u16Index,u16Data; 
    HI_UNF_EDID_BASE_INFO_S *pSinkCap = &g_stEdidInfo;
    HI_UNF_EDID_MANUFACTURE_INFO_S *pstVendor = &pSinkCap->stMfrsInfo;
	    
   	if(pData == NULL)
	{
		return HI_FAILURE;
    }
  
	u16Data = (pData->mfg_id[0]<<8) | (pData->mfg_id[1]);
 
	for(u16Index = 0; u16Index < 3; u16Index++)
	{
	    pstVendor->u8MfrsName[2 - u16Index]= ((u16Data & (0x1F << (5*u16Index))) >> (5*u16Index));
		if((0 < pstVendor->u8MfrsName[2 - u16Index])&&\
				(27 > pstVendor->u8MfrsName[2 - u16Index]))
		{
			pstVendor->u8MfrsName[2 - u16Index] += 'A' - 1;
		}
		else
		{
		    HDMIPrint("can't parse manufacture name\n");
			//return HI_FAILURE; //no must return 
		}
	}
	pstVendor->u32ProductCode       = (pData->prod_code[1] << 8) | pData->prod_code[0];
	pstVendor->u32SerialNumber      = (pData->serial[3] << 24) | (pData->serial[2] << 16) | (pData->serial[1] << 8) | (pData->serial[0]);
	pstVendor->u32Week = pData->mfg_week;
	pstVendor->u32Year = pData->mfg_year + 1990;

	HDMIPrint("mfg name[%s]\n",pstVendor->u8MfrsName);
	HDMIPrint("code:%d\n",pstVendor->u32ProductCode);
	HDMIPrint("serial:%d\n",pstVendor->u32SerialNumber);
	HDMIPrint("year:%d,week:%d\n",pstVendor->u32Year, pstVendor->u32Week);

    return HI_SUCCESS;
}

HI_S32 ParseVersion(HI_U8 *pData1,HI_U8 *pData2)
{
    HI_UNF_EDID_BASE_INFO_S *pSinkCap = &g_stEdidInfo;
    
    pSinkCap->u8Version  = *pData1;
    pSinkCap->u8Revision = *pData2;
    
    HDMIPrint("ver:%02d,rever:%02d\n",pSinkCap->u8Version,pSinkCap->u8Revision);
    return HI_SUCCESS;
}

HI_S32 ParseStandardTiming(HI_U8 *pData)
{
    HI_U8 TmpVal, i;
    HI_U32 Hor, Ver,aspect_ratio,freq;

    for(i = 0; i < STANDARDTIMING_SIZE; i += 2)
    {
        if((pData[i] == 0x01)&&(pData[i + 1]==0x01))
        {
            HDMIPrint("Mode %d wasn't defined! \n", (int)pData[i]);
        }
        else
        {
            Hor = (pData[i]+31)*8;
            HDMIPrint(" Hor Act pixels %d \n", Hor);
            TmpVal = pData[i + 1] & 0xC0;
            if(TmpVal==0x00)
            {
                HDMIPrint("Aspect ratio:16:10\n");
                aspect_ratio = _16_10;
                Ver = Hor *10/16;
            }
            else  if(TmpVal==0x40)
            {
                HDMIPrint("Aspect ratio:4:3\n");
                aspect_ratio = _4;
                Ver = Hor *3/4;
            }
            else  if(TmpVal==0x80)
            {
                HDMIPrint("Aspect ratio:5:4\n");
                aspect_ratio = _5_4;
                Ver = Hor *4/5;
            }
            else //0xc0
            {
                HDMIPrint("Aspect ratio:16:9\n");
                aspect_ratio = _16;
                Ver = Hor *9/16;
            }
            freq = ((pData[i + 1])& 0x3F) + 60;
            HDMIPrint(" Refresh rate %d Hz \n", freq);
        }
    }
    return HI_SUCCESS;
}

static HI_S32 ParseEstablishTiming(HI_U8 * pData)
{

    if(pData[0]& 0x80)
        HDMIPrint("720 x 400 @ 70Hz\n");
    if(pData[0]& 0x40)
        HDMIPrint("720 x 400 @ 88Hz\n");
    if(pData[0]& 0x20)
    {
        HDMIPrint("640 x 480 @ 60Hz\n");
        g_stEdidInfo.bSupportFormat[HI_UNF_ENC_FMT_861D_640X480_60] = HI_TRUE;
    }
    if(pData[0]& 0x10)
        HDMIPrint("640 x 480 @ 67Hz\n");
    if(pData[0]& 0x08)
        HDMIPrint("640 x 480 @ 72Hz\n");
    if(pData[0]& 0x04)
        HDMIPrint("640 x 480 @ 75Hz\n");
    if(pData[0]& 0x02)
        HDMIPrint("800 x 600 @ 56Hz\n");

    if(pData[0]& 0x01)
    {
        g_stEdidInfo.bSupportFormat[HI_UNF_ENC_FMT_VESA_800X600_60] = HI_TRUE;
        HDMIPrint("800 x 400 @ 60Hz\n");
    }

    if(pData[1]& 0x80)
        HDMIPrint("800 x 600 @ 72Hz\n");
    if(pData[1]& 0x40)
        HDMIPrint("800 x 600 @ 75Hz\n");
    if(pData[1]& 0x20)
        HDMIPrint("832 x 624 @ 75Hz\n");
    if(pData[1]& 0x10)
        HDMIPrint("1024 x 768 @ 87Hz\n");
    
    if(pData[1]& 0x08)
    {
        g_stEdidInfo.bSupportFormat[HI_UNF_ENC_FMT_VESA_1024X768_60] = HI_TRUE;
        HDMIPrint("1024 x 768 @ 60Hz\n");
    }
    if(pData[1]& 0x04)
        HDMIPrint("1024 x 768 @ 70Hz\n");
    if(pData[1]& 0x02)
        HDMIPrint("1024 x 768 @ 75Hz\n");
    if(pData[1]& 0x01)
        HDMIPrint("1280 x 1024 @ 75Hz\n");

    if(pData[2]& 0x80)
        HDMIPrint("1152 x 870 @ 75Hz\n");

    if((!pData[0])&&(!pData[1])&&(!pData[2]))
        HDMIPrint("No established video modes\n");
    
    return HI_SUCCESS;
}

HI_S32 ParsePreferredTiming(HI_U8 * pData)
{
    HI_U32 u32Temp;
    DETAILED_TIMING_BLOCK *pDetailed = (DETAILED_TIMING_BLOCK*)pData;
    HI_UNF_EDID_TIMING_S *pEdidTiming = &g_stEdidInfo.stPerferTiming;

     //pixel clock
    u32Temp = (pDetailed->pixel_clk[0]|(pDetailed->pixel_clk[1]<<8))*10; //unit HZ
    HDMIPrint("pixel clock :%d\n",u32Temp);
    pEdidTiming->u32PixelClk = u32Temp;

     //vfb
    u32Temp = pDetailed->v_border ;
    u32Temp += (pDetailed->vs_offset_pulse_width >> 4)| ((pDetailed->hs_offset_vs_offset&0x0C) << 4);
    HDMIPrint("VFB :%d\n",u32Temp);
    pEdidTiming->u32VFB = u32Temp;

    //vbb
    //v_active_blank == vblack == vfront + vback + vsync = VFB(vfront) + VBB(vback + vsync)
    u32Temp = ((pDetailed->v_active_blank & 0x0F)<<8)|pDetailed->v_blank;
    HDMIPrint("VBB :%d\n",u32Temp);
    pEdidTiming->u32VBB = u32Temp - pEdidTiming->u32VFB;

    //vact
    u32Temp = ((pDetailed->v_active_blank & 0xF0) << 4)|pDetailed->v_active;
    HDMIPrint("VACT :%d\n",u32Temp);
    pEdidTiming->u32VACT = u32Temp;

    //HFB
    u32Temp = pDetailed->h_border;
    u32Temp += pDetailed->h_sync_offset|((pDetailed->hs_offset_vs_offset & 0xC0)<<2);
    HDMIPrint("HFB :%d\n",u32Temp);
    pEdidTiming->u32HFB = u32Temp;
    //HBB
    u32Temp = pDetailed->h_blank;
    u32Temp += (pDetailed->h_active_blank&0x0F) << 8;
    HDMIPrint("HBB :%d\n",u32Temp);
    //h_active_blank == hblack == hfront + hback + hsync = HFB(hfront) + HBB(hback + hsync)
    pEdidTiming->u32HBB = u32Temp - pEdidTiming->u32HFB;
		
    //HACT
    u32Temp = ((pDetailed->h_active_blank & 0xF0) << 4)| pDetailed->h_active;
    HDMIPrint("HACT :%d\n",u32Temp);
    pEdidTiming->u32HACT = u32Temp;

	//VPW
    u32Temp = (pDetailed->hs_offset_vs_offset & 0x03) << 4;
    u32Temp |= (pDetailed->vs_offset_pulse_width &0x0F);
    HDMIPrint("VPW :%d\n",u32Temp); 
    pEdidTiming->u32VPW = u32Temp;
    //HPW
    u32Temp = (pDetailed->hs_offset_vs_offset & 0x30) << 4;
    u32Temp |= pDetailed->h_sync_pulse_width;
    HDMIPrint("HPW :%d\n",u32Temp);
    pEdidTiming->u32HPW = u32Temp;

    // H image size
    u32Temp = pDetailed->h_image_size;
    u32Temp |= (pDetailed->h_v_image_size & 0xF0) << 4;
    HDMIPrint("H image size :%d\n",u32Temp);
    pEdidTiming->u32ImageWidth = u32Temp;

    //V image size
    u32Temp = (pDetailed->h_v_image_size & 0x0F) << 8;
    u32Temp  |= pDetailed->v_image_size ;
    HDMIPrint("V image size :%d\n",u32Temp);
    pEdidTiming->u32ImageHeight = u32Temp;
    if(pDetailed->flags & 0x80)         /*Interlaced flag*/
	{
        HDMIPrint("Output mode: interlaced\n");
        pEdidTiming->bInterlace = HI_TRUE;
	}
	else
	{
        HDMIPrint("Output mode: progressive\n");
        pEdidTiming->bInterlace = HI_FALSE;
	}

    /*Sync Signal Definitions Type*/
    if(0 == (pDetailed->flags & 0x10))     /*Analog Sync Signal Definitions*/ 
	{
		switch((pDetailed->flags & 0x0E) >> 1)
		{
			case 0x00:          /*Analog Composite Sync - Without Serrations - Sync On Green Signal only*/ 
                HDMIPrint("sync acs ws green\n");
				break;

			case 0x01:                  /*Analog Composite Sync - Without Serrations - Sync On all three (RGB) video signals*/  
                HDMIPrint("sync acs ws all\n");
				break;
                
			case 0x02:                  /*Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On Green Signal only*/   
                HDMIPrint("sync acs ds green\n");
				break;

			case 0x03:                  /*Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On all three (RGB) video signals*/
                HDMIPrint("sync acs ds all\n");
                break;
                
			case 0x04:                  /*Bipolar Analog Composite Sync - Without Serrations; - Sync On Green Signal only*/   
                HDMIPrint("sync bacs ws green\n");
                break;
                
			case 0x05:                  /*Bipolar Analog Composite Sync - Without Serrations; - Sync On all three (RGB) video signals*/   
                HDMIPrint("sync bacs ws all\n");
                break;
                
			case 0x06:                  /*Bipolar Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On Green Signal only*/   
                HDMIPrint("sync bacs ds green\n");
                break;
                
			case 0x07:                  /*Bipolar Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On all three (RGB) video signals*/    
                HDMIPrint("sync bacs ds all\n");
                break;
			default: 
				break;

          }
    }
    else                /*Digital Sync Signal Definitions*/ 
    {
 		switch((pDetailed->flags & 0x0E) >> 1)
		{
			case 0x01:
            case 0x00:                  /*Digital Composite Sync - Without Serrations*/ 
				//new_customer->Sync_type = HI_UNF_EDID_SYNC_DCS_WS;
                pEdidTiming->bIHS = 0;
                pEdidTiming->bIVS = 0;
                break;
                
			case 0x02:                   /*Digital Composite Sync - With Serrations (H-sync during V-sync)*/
			case 0x03:                    
				//new_customer->Sync_type = HI_UNF_EDID_SYNC_DCS_DS;
                break;
              
			case 0x04:                  /*Digital Separate Sync Vsync(-) Hsync(-)*/   
				//new_customer->Sync_type = HI_UNF_EDID_SYNC_DSS_VN_HN;
                pEdidTiming->bIHS = 0;
                pEdidTiming->bIVS = 0;
                break;
                
			case 0x05:                  /*Digital Separate Sync Vsync(-) Hsync(+)*/   
				//new_customer->Sync_type = HI_UNF_EDID_SYNC_DSS_VN_HP;
                pEdidTiming->bIHS = 1;
                pEdidTiming->bIVS = 0;
                break;
                
			case 0x06:                  /*Digital Separate Sync Vsync(+) Hsync(-)*/
				//new_customer->Sync_type = HI_UNF_EDID_SYNC_DSS_VP_HN;
                pEdidTiming->bIHS = 0;
                pEdidTiming->bIVS = 1;
                break;
                
			case 0x07:                  /*Digital Separate Sync Vsync(+) Hsync(+)*/   
			//	new_customer->Sync_type = HI_UNF_EDID_SYNC_DSS_VP_HP;
                pEdidTiming->bIHS = 1;
                pEdidTiming->bIVS = 1;
                break;
               
			default: 
				break;

		}
    }
    pEdidTiming->bIDV = 0;
    /*Stereo Viewing Support*/
    switch(((pDetailed->flags & 0x60) >> 4)|(pDetailed->flags & 0x01))
	{
		case 0x02:
            HDMIPrint("stereo sequential R\n");
			break;

		case 0x04:                 
            HDMIPrint("stereo sequential L\n");
			break;

		case 0x03:                 
            HDMIPrint("stereo interleaved 2R\n");
			break;

		case 0x05: 
            HDMIPrint("stereo interleaved 2L\n");
			break;
            
		case 0x06: 
             HDMIPrint("stereo interleaved 4\n");
			break;

		case 0x07: 
            HDMIPrint("stereo interleaved SBS\n");
			break;
            
		default: 
            HDMIPrint("stereo no\n");
			break;

	}
     return HI_SUCCESS;
}

HI_S32 ParseFirstBlock(HI_U8 *pData)
{
    EDID_FIRST_BLOCK_INFO *pEdidInfo = (EDID_FIRST_BLOCK_INFO*)pData;
  
    CheckHeader(pData);
    ParseVendorInfo(pEdidInfo);
    ParseVersion(&pEdidInfo->version,&pEdidInfo->revision);
    ParseEstablishTiming(pEdidInfo->est_timing);
    ParseStandardTiming(pEdidInfo->std_timing);
    ParsePreferredTiming(&pData[FIRST_DETAILED_TIMING_ADDR]);
    return HI_SUCCESS;
}

static HI_VOID ParseDTVMonitorAttr(HI_U8 *pEDID)
{  
    HI_UNF_EDID_COLOR_SPACE_S *pColorSpace = &g_stEdidInfo.stColorSpace;

    pColorSpace->bRGB444   = HI_TRUE;
    pColorSpace->bYCbCr422 = (pEDID[0] & BIT4 ? HI_TRUE : HI_FALSE) ;
    pColorSpace->bYCbCr444 = (pEDID[0] & BIT5 ? HI_TRUE : HI_FALSE);
    HDMIPrint("IT Underscan:0x%02x\n", ((pEDID[0] & BIT7)>>7));
    //pColorSpace->unScanInfo.u32ScanInfo = (pEDID[0] & BIT7) >> 4;
    HDMIPrint("basic audio:0x%02x\n",  (pEDID[0] & BIT6));
    HDMIPrint("bYCbCr444:%d\n", pColorSpace->bYCbCr444);
    HDMIPrint("bYCbCr422:%d\n", pColorSpace->bYCbCr422);
    HDMIPrint("DTDS num:0x%02x\n",  (pEDID[0] & 0x0F));
}

static HI_S32 ParseAudioBlock(HI_U8 *pData, HI_U8 u8Len)
{
    HI_U8 Index, format_code, u8bit, u8Count;
    HI_UNF_EDID_AUDIO_INFO_S *pstAudioInfo = g_stEdidInfo.stAudioInfo;
    /*one short audio descriptors length is 3 Bytes*/
    for(Index = 0; Index < (u8Len / 3); Index ++)
    {
        u8Count = 0;
        format_code = (pData[Index*3] & AUDIO_FORMAT_CODE)>>3;
        HDMIPrint("audio format:0x%02x, data:0x%02x\n", format_code,pData[Index*3]);
        pstAudioInfo[Index].u8AudChannel = (pData[Index*3] & AUDIO_MAX_CHANNEL) + 1;
        u8bit = pData[Index*3 + 1];
        HDMIPrint("sample rate:0x%02x\n",u8bit);
        if(u8bit&0x01)
        {
            pstAudioInfo[Index].enSupportSampleRate[u8Count] = HI_UNF_SAMPLE_RATE_32K;
            u8Count++;
        }
        if(u8bit&0x02)
        {
            pstAudioInfo[Index].enSupportSampleRate[u8Count] = HI_UNF_SAMPLE_RATE_44K;
            u8Count++;
        }
        if(u8bit&0x04)
        {
            pstAudioInfo[Index].enSupportSampleRate[u8Count] = HI_UNF_SAMPLE_RATE_48K;
            u8Count++;
        }
        if(u8bit&0x08)
        {
            pstAudioInfo[Index].enSupportSampleRate[u8Count] = HI_UNF_SAMPLE_RATE_88K;
            u8Count++;
        }
        if(u8bit&0x10)
        {
            pstAudioInfo[Index].enSupportSampleRate[u8Count] = HI_UNF_SAMPLE_RATE_96K;
            u8Count++;
        }
        if(u8bit&0x20)
        {
            pstAudioInfo[Index].enSupportSampleRate[u8Count] = HI_UNF_SAMPLE_RATE_176K;
            u8Count++;
        }
        if(u8bit&0x40)
        {
            pstAudioInfo[Index].enSupportSampleRate[u8Count] = HI_UNF_SAMPLE_RATE_192K;
            u8Count++;
        }
        HDMIPrint("sample rate num:%d\n",u8Count);
        pstAudioInfo[Index].u32SupportSampleRateNum = u8Count;
        
        pstAudioInfo[Index].enAudFmtCode = (HI_UNF_EDID_AUDIO_FORMAT_CODE_E)format_code;
        if(format_code > HI_UNF_EDID_AUDIO_FORMAT_CODE_RESERVED && format_code < HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT)
        {
            g_stEdidInfo.u32AudioInfoNum++;
        }
        if(1 == format_code)
        {
            u8Count = 0;
            u8bit = pData[Index*3 + 2];
            HDMIPrint("Bit Depth:0x%02x\n",u8bit);
            if(u8bit&0x01)
            {
                pstAudioInfo[Index].bSupportBitDepth[u8Count] = HI_UNF_BIT_DEPTH_16;
                u8Count++;
            }
            if(u8bit&0x02)
            {
                pstAudioInfo[Index].bSupportBitDepth[u8Count] = HI_UNF_BIT_DEPTH_20;
                u8Count++;
            }
            if(u8bit&0x04)
            {
                pstAudioInfo[Index].bSupportBitDepth[u8Count] = HI_UNF_BIT_DEPTH_24;
                u8Count++;
            }
            HDMIPrint("Bit Depth num:%d\n",u8Count);
            pstAudioInfo[Index].u32SupportBitDepthNum = u8Count;
        }
        else if ((format_code > 1) && (format_code < 9))
        {
            pstAudioInfo[Index].u32MaxBitRate = pData[Index*3 + 2]/8;
            HDMIPrint("Max Bit Rate:%d\n",pstAudioInfo[Index].u32MaxBitRate);
        }
        else //9 -15 reserve
        {
            //g_stEdidInfo.stAudInfo[format_code].u32Reserve = pData[Index*3 + 2];
        }
    }
    return HI_SUCCESS;
}

static HI_S32 ParseVideoDataBlock(HI_U8 *pData, HI_U8 u8Len)
{
    HI_U8 Index;
    HI_UNF_ENC_FMT_E enFmt, enNativeFmt = HI_UNF_ENC_FMT_BUTT;
    HI_U32 vic,hdmiModeIdx;

    if(pData == HI_NULL)
    {
        HI_ERR_HDMI("hdmi video point null!\n");
        return HI_FAILURE;
    }
    
    //init native fmt
    g_stEdidInfo.enNativeFormat = HI_UNF_ENC_FMT_BUTT;
    for(Index = 0; Index < u8Len; Index++)
    {
        vic = pData[Index] & EDID_VIC;
        HDMIPrint("vic:0x%02x\n",vic);
        for(hdmiModeIdx = 0; hdmiModeIdx < NMODES; hdmiModeIdx++)
        {
            if (VModeTables[hdmiModeIdx].ModeId.Mode_C1 == vic) 
            {
                if(VModeTables[hdmiModeIdx].ModeId.enUNFFmt_C1 >= HI_UNF_ENC_FMT_BUTT)
                {
                    continue;
                }
                enFmt = VModeTables[hdmiModeIdx].ModeId.enUNFFmt_C1;
                g_stEdidInfo.bSupportFormat[enFmt] = HI_TRUE;  /* support timing foramt */
                if(enNativeFmt == HI_UNF_ENC_FMT_BUTT)
                {
                    enNativeFmt = enFmt; //if no native flag set first define format
                }

                if(pData[Index]&0x80)
                {
                    if(HI_UNF_ENC_FMT_BUTT == g_stEdidInfo.enNativeFormat)
                    {
                        g_stEdidInfo.enNativeFormat = enFmt;
                    }
                    HDMIPrint("Native fmt :0x%02x\n",pData[Index]);
                }
            }
        }
    }
    
    if(HI_UNF_ENC_FMT_BUTT == g_stEdidInfo.enNativeFormat)
    {
        g_stEdidInfo.enNativeFormat = enNativeFmt; //set The first order fmt 
    }
    HDMIPrint("*****Native fmt :0x%02x\n",g_stEdidInfo.enNativeFormat);
    return HI_SUCCESS;
}

static HI_S32 ParseVendorSpecificDataBlock(HI_U8 *pData, HI_U8 u8Len )
{
    HI_U8 index ,HDMI_VIC_LEN, HDMI_3D_LEN,u83D_present, u83D_Multi_present,u83D_Structure;
    HI_U8 u8Temp;

    HDMIPrint("IEERegId:0x%02x, 0x%02x, 0x%02x\n", pData[0], pData[1], pData[2]);
    if ((pData[0] == 0x03) && (pData[1] == 0x0c)
      && (pData[2] == 0x00))
    {
        HDMIPrint("This is HDMI Device\n");
        g_stEdidInfo.bSupportHdmi = HI_TRUE;
    }
    else
    {
        g_stEdidInfo.bSupportHdmi = HI_FALSE;
        HDMIPrint("This is DVI Device, we don't parse it\n");
        return HI_FAILURE;
    }
    
    if(u8Len < 4)
    {
        HDMIPrint("len:%d\n",u8Len);
        return HI_SUCCESS;
    }
        /* Vendor Specific Data Block */
    g_stEdidInfo.stCECAddr.u8PhyAddrA = (pData[3] & 0xF0)>>4;
    g_stEdidInfo.stCECAddr.u8PhyAddrB = (pData[3] & 0x0F);
    g_stEdidInfo.stCECAddr.u8PhyAddrC = (pData[4] & 0xF0)>>4;
    g_stEdidInfo.stCECAddr.u8PhyAddrD = (pData[4] & 0x0F);

    if((g_stEdidInfo.stCECAddr.u8PhyAddrA != 0xF )&&(g_stEdidInfo.stCECAddr.u8PhyAddrB != 0xF )&&
       (g_stEdidInfo.stCECAddr.u8PhyAddrC != 0xF )&&(g_stEdidInfo.stCECAddr.u8PhyAddrD != 0xF ))
    {
        g_stEdidInfo.stCECAddr.bPhyAddrValid = TRUE;
    }
    else
    {
        g_stEdidInfo.stCECAddr.bPhyAddrValid = FALSE;
    }
    //HDMIPrint("PhyAddr %02x.%02x.%02x.%02x\n", g_stEdidInfo.u8PhyAddrA, g_stEdidInfo.u8PhyAddrB, g_stEdidInfo.u8PhyAddrC, g_stEdidInfo.u8PhyAddrD);
    if(u8Len < 6)
    {
        HDMIPrint("len:%d\n",u8Len);
        return HI_SUCCESS;
    }
        
    g_stEdidInfo.bSupportDVIDual    = (pData[5] & 0x01);
    g_stEdidInfo.stDeepColor.bDeepColorY444  = (pData[5] & 0x08) >> 3;
    g_stEdidInfo.stDeepColor.bDeepColor30Bit = (pData[5] & 0x10) >> 4;
    g_stEdidInfo.stDeepColor.bDeepColor36Bit = (pData[5] & 0x20) >> 5;
    g_stEdidInfo.stDeepColor.bDeepColor48Bit = (pData[5] & 0x40) >> 6;
    g_stEdidInfo.bSupportsAI = (pData[5] & 0x80) >> 7;
    HDMIPrint("DVI_Dual:%d,Y444:%d,30bit:%d,48bit:%d,AI:%d\n",g_stEdidInfo.bSupportDVIDual,
                g_stEdidInfo.stDeepColor.bDeepColorY444, g_stEdidInfo.stDeepColor.bDeepColor30Bit,
                g_stEdidInfo.stDeepColor.bDeepColor48Bit,g_stEdidInfo.bSupportsAI);
    if(u8Len < 7)
    {
        HDMIPrint("len:%d\n",u8Len);
        return HI_SUCCESS;
    }
   // g_stEdidInfo.u32MaxTMDSClk   = (pData[6] & 0xff) * 5;
    HDMIPrint("Max TMDS Colock:%d\n", (pData[6] & 0xff) * 5);
    
    if(u8Len < 8)
    {
        HDMIPrint("len:%d\n", u8Len);
        return HI_SUCCESS;
    }
    
    g_stEdidInfo.st3DInfo.bSupport3D = (pData[7] & 0x20) >> 5;
    HDMIPrint("support 3d:%d\n",g_stEdidInfo.st3DInfo.bSupport3D);
    HDMIPrint("bLatency_Fields_Present:%d\n",(pData[7] & 0x80) >> 7);
    HDMIPrint("bI_Latency_Fields_Present:%d\n",(pData[7] & 0x40) >> 6);
    HDMIPrint("CNC:%d\n",(pData[7] & 0x0F));
    if(u8Len < 9)
    {
        HDMIPrint("len:%d\n", u8Len);
        return HI_SUCCESS;
    }
    
    HDMIPrint("u8Video_Latency:%d\n",(pData[8] & 0xff));
    HDMIPrint("u8Audio_Latency:%d\n",(pData[9] & 0xff));
    HDMIPrint("u8Interlaced_Video_Latency:%d\n",(pData[10] & 0xff));
    HDMIPrint("u8Interlaced_Audio_Latency:%d\n",(pData[11] & 0xff));

     if(u8Len < 13)
    {
        HDMIPrint("len:%d\n", u8Len);
        return HI_SUCCESS;
    }
    HDMIPrint("u8ImagSize:%d\n",((pData[12]& 0x18)>>2));
    u83D_present = (pData[12]& 0x80) >> 7;
    u83D_Multi_present = (pData[12]& 0x60) >> 5;

    if(u8Len < 14)
    {
        HDMIPrint("len:%d\n", u8Len);
        return HI_SUCCESS;
    }
    
    HDMI_3D_LEN = (pData[13]& 0x1F);
    HDMI_VIC_LEN = (pData[13]& 0xE0) >> 5;
    
    index = 0;

    HDMIPrint("3D_LEN:%d,VIC:%d\n",HDMI_3D_LEN, HDMI_VIC_LEN);
    if ((HDMI_VIC_LEN > 0) &&(u8Len >= (HDMI_VIC_LEN + 14)))
    {
        for(index = 0; index < HDMI_VIC_LEN; index ++)
        {
            //g_stEdidInfo.bSupportFmt4K_2K[index] = HI_TRUE;
            HDMIPrint("4k*2k fmt:%d\n",pData[14 + index]);
        }
    }

    if((HDMI_3D_LEN > 0) && (u8Len >= (HDMI_VIC_LEN + HDMI_3D_LEN + 14))) 
    {
        //index = 0;
        if(u83D_present)
        {
            if ((u83D_Multi_present == 0x1)||(u83D_Multi_present == 0x2))
            {
               
                //3d structure_All_15...8 resever
                HDMIPrint("3D structure_All_15...8 resever\n");
                u8Temp = pData[14 + HDMI_VIC_LEN + 1] & 0xFF;
                HDMIPrint("u8Temp:0x%x\n",u8Temp);
                if(0x01 == u8Temp)
                {
                   g_stEdidInfo.st3DInfo.bSupport3DType[HI_UNF_EDID_3D_SIDE_BY_SIDE_HALF] = HI_TRUE; 
                }
                u8Temp = pData[14 + HDMI_VIC_LEN + 2] & 0xFF;
                HDMIPrint("u8Temp:0x%x\n",u8Temp);
                
                if(0x01&u8Temp)
                {
                   g_stEdidInfo.st3DInfo.bSupport3DType[HI_UNF_EDID_3D_FRAME_PACKETING] = HI_TRUE; 
                }
                if(0x02&u8Temp)
                {
                   g_stEdidInfo.st3DInfo.bSupport3DType[HI_UNF_EDID_3D_FIELD_ALTERNATIVE] = HI_TRUE; 
                }
                if(0x04&u8Temp)
                {
                   g_stEdidInfo.st3DInfo.bSupport3DType[HI_UNF_EDID_3D_LINE_ALTERNATIVE] = HI_TRUE; 
                }
                if(0x08&u8Temp)
                {
                   g_stEdidInfo.st3DInfo.bSupport3DType[HI_UNF_EDID_3D_SIDE_BY_SIDE_FULL] = HI_TRUE; 
                }
                if(0x10&u8Temp)
                {
                   g_stEdidInfo.st3DInfo.bSupport3DType[HI_UNF_EDID_3D_L_DEPTH] = HI_TRUE; 
                }
                if(0x20&u8Temp)
                {
                   g_stEdidInfo.st3DInfo.bSupport3DType[HI_UNF_EDID_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH] = HI_TRUE; 
                }
                if(0x40&u8Temp)
                {
                   g_stEdidInfo.st3DInfo.bSupport3DType[HI_UNF_EDID_3D_TOP_AND_BOTTOM] = HI_TRUE; 
                }
                index += 2;
            }

            if(u83D_Multi_present == 0x1)
            {
                //3D_MASK_0...15 resever
                HDMIPrint("3D_MASK_0...15 resever\n");
                index += 2;
            }
        }
    }

    for(index +=15 ; index < u8Len; index ++)
    {
        u83D_Structure = (pData[index]&0xF0)>>4;
        HDMIPrint("2D_VIC_order:0x%02x\n", u83D_Structure);
        u83D_Structure = (pData[index]&0x0F);
        if(u83D_Structure >= 0x08)
        {
            HDMIPrint("3D_Detailed > 0x08 :0x%02x\n", u83D_Structure);
            u83D_Structure = (pData[index]&0xF0)>>4;
        }
        HDMIPrint("3D_Detailed :0x%02x\n", u83D_Structure);
    }
    

    return HI_SUCCESS;
}

HI_S32 ParseSpeakerDataBlock(HI_U8 *pData, HI_U8 u8Len)
{
    HI_U8 u32Bit;
    u32Bit = pData[0];
    HDMIPrint("Speaker1:0x%02x\n",u32Bit);
    if(u32Bit & 0x01)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_FL_FR] = HI_TRUE;
    }
    if(u32Bit & 0x02)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_LFE] = HI_TRUE;
    }
    if(u32Bit & 0x04)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_FC] = HI_TRUE;
    }
    if(u32Bit & 0x08)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_RL_RR] = HI_TRUE;
    }
    if(u32Bit & 0x10)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_RC] = HI_TRUE;
    }
    if(u32Bit & 0x20)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_FLC_FRC] = HI_TRUE;
    }
    if(u32Bit & 0x40)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_FLW_FRW] = HI_TRUE;
    }
    if(u32Bit & 0x80)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_FLH_FRH] = HI_TRUE;
    }
    u32Bit = pData[1];
    HDMIPrint("Speaker1:0x%02x\n",u32Bit);
    if(u32Bit & 0x01)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_TC] = HI_TRUE;
    }
    if(u32Bit & 0x02)
    {
        g_stEdidInfo.bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_FCH] = HI_TRUE;
    }
    
    return HI_SUCCESS;
}

HI_S32 ParseExtDataBlock(HI_U8 *pData, HI_U8 u8Len)
{
     switch(pData[0])
     {
        case VIDEO_CAPABILITY_DATA_BLOCK:
            HDMIPrint("Video Capability Data Block\n");
            //g_stEdidInfo.unScanInfo.u32ScanInfo |= pData[1]&0xFF;
            HDMIPrint("ScanType:%d\n",pData[1]);
            break;
        case VENDOR_SPECIFIC_VIDEO_DATA_BLOCK:
            HDMIPrint("vendor specific data block\n");
            break;
        case RESERVED_VESA_DISPLAY_DEVICE:
            HDMIPrint("reserved vesa display device\n");
            break;
        case RESERVED_VESA_VIDEO_DATA_BLOCK:
            HDMIPrint("reserved vesa video data block\n");
            break;
        case RESERVED_HDMI_VIDEO_DATA_BLOCK:
            HDMIPrint("reserved hdmi video data block\n");
            break;
        case COLORIMETRY_DATA_BLOCK:
           // g_stEdidInfo.unColorimetry.u32Colorimetry = (pData[1]|(pData[2]<<8));
            if(XVYCC601&pData[1])
            {
                g_stEdidInfo.stColorMetry.bxvYCC601 = HI_TRUE;
            }
            if(XVYCC709&pData[1])
            {
                g_stEdidInfo.stColorMetry.bxvYCC709 = HI_TRUE;
            }
            if(SYCC601&pData[1])
            {
                g_stEdidInfo.stColorMetry.bsYCC601 = HI_TRUE;
            }
            if(ADOBE_XYCC601&pData[1])
            {
                g_stEdidInfo.stColorMetry.bAdobleYCC601 = HI_TRUE;
            }
            if(ADOBE_RGB&pData[1])
            {
                g_stEdidInfo.stColorMetry.bAdobleRGB = HI_TRUE;
            }
            HDMIPrint("Colorimetry:0x%02x\n", pData[1]);
            break;
        case CEA_MISCELLANENOUS_AUDIO_FIELDS:
            HDMIPrint("CEA miscellanenous audio data fileds\n");
            break;
        case VENDOR_SPECIFIC_AUDIO_DATA_BLOCK:
            HDMIPrint("vendor specific audio data block\n");
            break;
        case RESERVED_HDMI_AUDIO_DATA_BLOCK:
            HDMIPrint("reserved hdmi audio data block\n");
            break;
         default:
            break;
     }
     return HI_SUCCESS;
}

HI_S32 ParseDTVBlock(HI_U8 *pData)
{
    HI_U8 offset, length, len;
    HI_S32 s32Ret;
    
    if(pData == HI_NULL)
    {
        HI_ERR_HDMI("null ponit\n");
        return HI_FAILURE;
    }

    HDMIPrint("checksum\n");
    s32Ret = BlockCheckSum(pData);
    
    if(s32Ret != HI_SUCCESS)
    {
        HDMIPrint("Extend Block Crc Error! \n");
        g_bErrList[EXTENSION_BLOCK_CRC_ERROR] = HI_TRUE;
    }

    
    if(pData[0] != EXT_VER_TAG)//0x02
    {
        HI_ERR_HDMI("extern block version err:0x%02x\n",pData[0]);
        return HI_FAILURE;
    }
    if (pData[1] < EXT_REVISION)//0x03
    {
        HI_ERR_HDMI("extern block revison err: 0x%02x\n", pData[1]);
        return HI_FAILURE;
    }
    
    length = pData[2];
    HDMIPrint("data block length:0x%x\n",length);
    if(length == 0)
    {
        HI_ERR_HDMI("no detailed timing data and no reserved provided!\n");
        return HI_FAILURE;
    }
    if(length <= 4)
    {
        HI_ERR_HDMI("no reserved provided! len:%d\n",length);
        return HI_FAILURE;
    }
    offset = 4;
  
    ParseDTVMonitorAttr(&pData[3]);
    
    while(offset < length)
    {
        len = pData[offset] & DATA_BLOCK_LENGTH;
        HDMIPrint("data ID:%d,len:%d,data:0x%02x\n",((pData[offset] & DATA_BLOCK_TAG_CODE) >> 5),len,pData[offset]);
        switch((pData[offset] & DATA_BLOCK_TAG_CODE) >> 5)
        {
            case AUDIO_DATA_BLOCK:
                ParseAudioBlock(&pData[offset + 1], len);
                break;
            case VIDEO_DATA_BLOCK:
                ParseVideoDataBlock(&pData[offset + 1], len);
                break;
            case VENDOR_DATA_BLOCK:
                s32Ret = ParseVendorSpecificDataBlock(&pData[offset + 1], len);
                if(HI_SUCCESS != s32Ret)
                {
                    HI_ERR_HDMI("Vendor parase error!\n");
                    //return s32Ret;
                }
                break;
            case SPEAKER_DATA_BLOCK:
                ParseSpeakerDataBlock(&pData[offset + 1], len);
                break;
            case VESA_DTC_DATA_BLOCK:
                HDMIPrint("VESA_DTC parase\n");
                break;
            case USE_EXT_DATA_BLOCK:
                ParseExtDataBlock(&pData[offset + 1], len);
                break;
             default:
                HDMIPrint("resvered block tag code define");
                break;
        }
        offset += len + 1;
    }
    
    return HI_SUCCESS;
    
}

HI_S32 SI_GetCapability(HI_VOID)
{   
    HI_S32 s32Ret,Index;
    HI_U8 *pData;
    //clear edidifo
    memset(&g_stEdidInfo, 0, sizeof(HI_UNF_EDID_BASE_INFO_S));
    
    s32Ret = SI_ReadSinkEDID();
    if(EDID_READ_FIRST_BLOCK_ERR == s32Ret)
    {
        HDMIPrint("can't read edid\n");
        return HI_FAILURE;
    }
    pData = g_EdidMen; // Parse sink cap
   
    ParseFirstBlock(pData);

    if(g_stEdidInfo.u8ExtBlockNum == 0)
    {
        g_stEdidInfo.bSupportHdmi = HI_FALSE;
        return HI_SUCCESS;
    }

    if(g_stEdidInfo.u8ExtBlockNum > 3)
    {
        HDMIPrint("Extern block too big: 0x%02x \n",g_stEdidInfo.u8ExtBlockNum);
        HDMIPrint("Setting to 3 Block \n");
        g_stEdidInfo.u8ExtBlockNum = 3;
    }
	
    //g_stEdidInfo.bRealEDID = HI_TRUE;
    //g_stEdidInfo.bRGB444 = HI_TRUE;//always HI_TRUE
    for(Index = 1; Index <= g_stEdidInfo.u8ExtBlockNum; Index++)
    {
        s32Ret = ParseDTVBlock(&pData[EDID_BLOCK_SIZE*Index]);
        if(HI_SUCCESS == s32Ret)
        {
            HDMIPrint("Successfully resolved ext block NO:0x%x\n",Index);
            break;
        }
    }
	// in boot,when no support hdmi,then we can  not parse audio in dvi mode
    if(g_stEdidInfo.bSupportHdmi == HI_FALSE)
    {
        if(g_bErrList[BAD_HEADER] || g_bErrList[_1ST_BLOCK_CRC_ERROR] || g_bErrList[EXTENSION_BLOCK_CRC_ERROR])
        {
            HDMIPrint("Unknown mode, Go to Force Mode");
            return HI_FAILURE;
        }
        HDMIPrint("DVI DEVICE\n");
    }
    else
    {
        HDMIPrint("HDMI DEVICE\n");
    }

    return HI_SUCCESS;
}

HI_S32 SI_GetSinkCapability(HI_UNF_EDID_BASE_INFO_S *pSinkCap)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    if(!g_bGetReadEdidFlag)
    {
        s32Ret = SI_GetCapability();
        if(s32Ret == HI_SUCCESS)
        {
            g_bGetReadEdidFlag = HI_TRUE;
        }
    }
    
    *pSinkCap = g_stEdidInfo;
      
    return s32Ret;
}


