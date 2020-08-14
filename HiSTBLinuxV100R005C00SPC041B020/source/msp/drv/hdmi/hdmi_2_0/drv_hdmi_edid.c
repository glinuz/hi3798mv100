/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_hdmi_edid.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/12/10
  Description   :
  History       :
  Date          : 2014/12/10
  Author        : t00273561
  Modification  :
*******************************************************************************/
#include "drv_hdmi_platform.h"
#include "drv_hdmi_intf.h"
#include "drv_hdmi_edid.h"
#include "drv_hdmi_edid_test.h"

/********************** private define ********************************************/
#define  BIT0_MASK                				0x01
#define  BIT1_MASK                				0x02
#define  BIT2_MASK                				0x04
#define  BIT3_MASK                				0x08
#define  BIT4_MASK                				0x10
#define  BIT5_MASK                				0x20
#define  BIT6_MASK                				0x40
#define  BIT7_MASK                				0x80

#define  EDID_EXTENSION_BLK_ADDR 				0x7e

/* VENDOR  INFO */
#define  EDID_VEND_NAME_CHAR_MASK				0x1F
#define  EDID_VEND_CHAR_LOW_INVALID			    0
#define  EDID_VEND_CHAR_HIGH_INVALID			27
#define  EDID_VEND_YEAR_BASE 					1990

/* STD TIMING */
#define  EDID_STDTMING_UNUSED_FLAG				0x01
#define  EDID_STDTMING_RATE_BASE				60
#define  EDID_HORACTIVE_FACTOR					8
#define  EDID_HORACTIVE_BASE					31
#define  EDID_REFRESH_RATE_MASK 				0x3F
#define  EDID_ASPECT_RATIO_MASK				    0xC0
#define  EDID_ASPECT_RATIO_16_10   			    0
#define  EDID_ASPECT_RATIO_4_3     			    1
#define  EDID_ASPECT_RATIO_5_4   				2
#define  EDID_ASPECT_RATIO_16_9    			    3

/* Detailed Timing Descriptor */
#define  EDID_FST_BLK_DTD_OFFSET				0x36
#define  EDID_DTD_SIZE							18
#define  EDID_PIXCLK_KHZ_FACTOR 				10

#define  EDID_UPPER_NIBBLE_MASK				    0xF0
#define  EDID_LOWER_NIBBLE_MASK				    0x0F

#define  EDID_HSO_MASK 						    0xC0
#define  EDID_HSPW_MASK 						0x30
#define  EDID_VSO_MASK 						    0x0C
#define  EDID_VSPW_MASK 						0x03

#define  EDID_STEREO_MASK 						0x60
#define  EDID_STEREO_SEQUENTIAL_R 				0x02
#define  EDID_STEREO_SEQUENTIAL_L 				0x04
#define  EDID_STEREO_INTERLEAVED_2R 			0x03
#define  EDID_STEREO_INTERLEAVED_2L 			0x05
#define  EDID_STEREO_INTERLEAVED_4 			    0x06
#define  EDID_STEREO_INTERLEAVED_SBS			0x07

#define  EDID_SYNC_SIGNAL_TYPE_MASK 			0x0E
#define  EDID_SYNC_DCS_WS_0 					0x00
#define  EDID_SYNC_DCS_WS_1 					0x01
#define  EDID_SYNC_DCS_DS_2 					0x02
#define  EDID_SYNC_DCS_DS_3 					0x03
#define  EDID_SYNC_DSS_VN_HN_4 				    0x04
#define  EDID_SYNC_DSS_VN_HP_5 				    0x05
#define  EDID_SYNC_DSS_VP_HN_6 				    0x06
#define  EDID_SYNC_DSS_VP_HP_7 				    0x07

/* Monitor Descriptor */
#define  EDID_TYPE_MONITOR_SERNUM               0xFF
#define  EDID_TYPE_MONITOR_ASIIC                0xFE
#define  EDID_TYPE_MONITOR_RANGE                0xFD
#define  EDID_TYPE_MONITOR_NAME                 0xFC 


/* EXTENSION BLOCK */
#define  EDID_CEA_EXTVERSION3_TAG         		0x02
#define  EDID_CEA_EXTVERSION3_REVISION          0x03
#define  EDID_DB_LEN_MASK  					    0x1F
#define  EDID_DB_TAG_CODE_MASK 				    0xE0

/* Tag Code */
#define  EDID_REVERSED_DATA_BLOCK    			0x00
#define  EDID_AUDIO_DATA_BLOCK    				0x01
#define  EDID_VIDEO_DATA_BLOCK    				0x02
#define  EDID_VENDOR_DATA_BLOCK   				0x03
#define  EDID_SPEAKER_DATA_BLOCK  				0x04
#define  EDID_VESA_DTC_DATA_BLOCK 				0x05
#define  EDID_USE_EXT_DATA_BLOCK  				0x07

/* Ext Tag Code */
#define  EDID_VIDEO_CAPABILITY_DB      		    0x00
//#define   EDID_VENDOR_SPECIFIC_VIDEO_DB 		0x01
//#define   EDID_VESA_DISPLAY_DEVICE_DB     	0x02
//#define   EDID_RESERVED_VESA_VIDEO_DB   		0x03
//#define   EDID_VESA_VIDEO_TIMING_DB   		0x04
#define  EDID_COLORIMETRY_DB           		    0x05
#ifdef HDMI_HDR_SUPPORT
#define  EDID_DOLBY_VENDOR_SPECIFIC_VIDEO_DB 	0x01    // Dolby mode
#define  EDID_HDR_STATIC_METADATA_DB            0x06    // HDR
#endif
#define  EDID_VIDEO_FMT_PREFER_DB				0x0d
#define  EDID_Y420_VIDEO_DB           			0x0e
#define  EDID_Y420_CAPABILITY_MAP_DB  			0x0f
//#define   EDID_MISCELLANENOUS_AUDIO_FIELDS    0x10
//#define   EDID_VENDOR_SPECIFIC_AUDIO_DB 		0x11
#define  EDID_RESERVED_HDMI_AUDIO_DB  			0x12
#define  EDID_INFOFRAME_DB  					0x20

/* AUDIO DATA BLOCK */
#define  EDID_AUDIO_FORMAT_MASK   				0x78
#define  EDID_AUDIO_CHANNEL_MASK   			    0x07
#define  EDID_AUDIO_EXT_TYPE_CODE 				0xf8
#define  EDID_AUDIO_BITRATE_FACTOR				8

/* VIDEO DATA BLOCK */
#define  EDID_VIC_NATIVE_MAX            		64
#define  EDID_VIC_LOWER7_MASK            		0x7F
#define  EDID_VIC_INVALID_LOW            		128
#define  EDID_VIC_INVALID_HIGH					192
#define  EDID_VIC_INVALID_ZERO					0

/* HFVSDB */
#define  EDID_HFVSDB_VERSION					1
#define  EDID_MAX_HDMI14_TMDS_RATE				340
#define  EDID_TMDS_FACTOR 						5
#define  EDID_IEEE_VSDB_1ST 					0x03
#define  EDID_IEEE_VSDB_2ND 					0x0c
#define  EDID_IEEE_VSDB_3RD 					0x00

#define  EDID_IEEE_HFVSDB_1ST 					0xd8
#define  EDID_IEEE_HFVSDB_2ND 					0x5d
#define  EDID_IEEE_HFVSDB_3RD 					0xc4


/* VSDB */
#define  EDID_CEC_INVALID_ADDR 				    0xF
#define  EDID_IMG_SIZE_MASK 					0x18
#define  EDID_HDMI_3D_LEN_MASK 				    0x1F
#define  EDID_HDMI_VIC_LEN_MASK 				0xE0
#define  EDID_3DMULTI_PRESENT_LOWER8			0x01
#define  EDID_3DMULTI_PRESENT_UPPER8			0x02
#ifdef HDMI_HDR_SUPPORT
/*Dolby VSVDB*/
#define  EDID_DOLBY_VSVDB_VERSION_0             0x00
#define  EDID_DOLBY_VSVDB_VERSION_1             0x01
#define  EDID_DOLBY_VSVDB_VERSION_0_LEN         0x19
#define  EDID_DOLBY_VSVDB_VERSION_1_LEN         0x0E

#define  EDID_DOLBY_LOWER_2BIT_MASK             0x03
#define  EDID_DOLBY_LOWER_3BIT_MASK             0x07
#define  EDID_DOLBY_LOWER_NIBBLE_MASK           0x0F
#define  EDID_DOLBY_LOWER_7BIT_MASK             0x7F

#endif

#define BIT_CHECK(p, mask)  ((p) & (mask))

#define EDID_NULL_CHK_NO_RET(p)	do{\
	if(HI_NULL==p)\
	{EDID_ERR("NULL pionter!\n");return ;}\
}while(0);

#define EDID_NULL_CHK(p)	do{\
	if(HI_NULL==p)\
	{EDID_ERR("NULL pionter!\n");return HI_FAILURE;}\
}while(0);

#define EDID_SUCCESS_CHK(res)	do{\
	if(HI_FAILURE==res)\
	{EDID_ERR("return failure!\n");return HI_FAILURE;}\
}while(0);

typedef struct
{
    HI_U8 au8PixelClk[2];
    HI_U8 u8H_Active;
    HI_U8 u8H_Blank;
    HI_U8 u8H_ActiveBlank;
    HI_U8 u8V_Active;
    HI_U8 u8V_Blank;
    HI_U8 u8V_ActiveBlank;
    HI_U8 u8H_SyncOffset;
    HI_U8 u8H_SyncPulseWidth;
    HI_U8 u8VS_OffsetPulseWidth;
    HI_U8 u8HS_Offset_VS_Offset;
    HI_U8 u8H_ImageSize;
    HI_U8 u8V_ImageSize;
    HI_U8 u8H_V_ImageSize;
    HI_U8 u8H_Border;
    HI_U8 u8V_Border;
    HI_U8 u8Flags;
} __attribute__((packed))EDID_DTD_TIMING_BLOCK;

typedef struct
{
    HI_U8 u8Flags[3];
    HI_U8 u8DataTag;
    HI_U8 u8Flag;
    HI_U8 u8DesData[13];
} __attribute__((packed))EDID_MONITOR_DES_BLOCK;


typedef struct
{
	HI_U8 au8Header[8];
	/* Vendor & product info */
	HI_U8 au8MfgId[2];
	HI_U8 au8ProdCode[2];
	/* FIXME: byte order */
	HI_U8 au8Serial[4];
	HI_U8 u8MfgWeek;
	HI_U8 u8MfgYear;
	/* EDID version */
	HI_U8 u8Version;
	HI_U8 u8Revision;
	/* Display info: */
	HI_U8 u8Input;
	HI_U8 u8WidthCm;
	HI_U8 u8HeightCm;
	HI_U8 u8Gamma;
	HI_U8 u8Features;
	/* Color characteristics */
	HI_U8 u8RedGreenLow;
	HI_U8 u8BlackWhiteLow;
	HI_U8 u8RedX;
	HI_U8 u8RedY;
	HI_U8 u8GreenX;
	HI_U8 u8GreenY;
	HI_U8 u8BlueX;
	HI_U8 u8BlueY;
	HI_U8 u8WhiteX;
	HI_U8 u8WhiteY;
    HI_U8 au8EstTiming[3];
    HI_U8 au8StdTiming[16];
    HI_U8 au8DetailedTiming1[18];
    HI_U8 au8DetailedTiming2[18];
    HI_U8 au8DetailedTiming3[18];
    HI_U8 au8DetailedTiming4[18];
	/* Number of 128 byte ext. blocks */
	HI_U8 u8ExtBlocks;
	/* Checksum */
	HI_U8 u8ChkSum;
} __attribute__((packed))EDID_FIRST_BLOCK_INFO;


/********************** private interface ********************************************/

static HI_S32 EdidBlockChkSum(HI_U8 *pau8Data,
							HDMI_EDID_STATUS_S *pstStatus)
{
    HI_U8 	i = 0;
    HI_U32 	u32CheckSum = 0;

    EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstStatus);

    for(i = 0; i < HDMI_EDID_BLOCK_SIZE; i++)
    {
        u32CheckSum += pau8Data[i];
    }

    /* EDID header */
    EDID_INFO("    %-20s : %02X\n", "Check Sum",pau8Data[127]);

    if((u32CheckSum & 0xff) != 0x00)
    {
        if(pau8Data[32] != 0x00)
        {
            pstStatus->enParseErr = EDID_PARSE_ERR_CHECKSUM;
    		EDID_ERR("Check Sum Fail\n");
    		return HI_FAILURE;
        }
        else
        {
            /* If the data is 0x00 at 0x20 in block0, which is modified by clear scramble in boot.
               But the real EDID is crrect. */
            EDID_INFO("EDID data maybe changed by clr scramble.\n");
        }
    }

    return HI_SUCCESS;
}

static HI_S32 EdidHeaderChk(HI_U8 *pau8Data,
								HDMI_EDID_STATUS_S *pstStatus)
{
    HI_U32 i = 0;
    const HI_U8 au8Blkheader[] ={0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00};

	EDID_NULL_CHK(pstStatus);

    EDID_INFO("EDID Header...\n");
    for (i = 0; i < 8; i++)
    {
        if(pau8Data[i] != au8Blkheader[i])
        {
        	pstStatus->enParseErr = EDID_PARSE_ERR_HEADER;
			EDID_ERR("Header Chk: DATA[%d]=0x%x\n",i,pau8Data[i]);
			//return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 EdidVendorInfo(HDMI_EDID_INFO_S *pstEdidInfo)
{
    HI_U16 							i = 0;
	HI_U16 							u16Data = 0;
    HDMI_SINK_CAPABILITY_S 			*pstCap = HI_NULL;
    HDMI_EDID_MANUFACTURE_INFO_S 	*pstVendor = HI_NULL;
	EDID_FIRST_BLOCK_INFO			*pstFstBlk = HI_NULL;
	HDMI_EDID_STATUS_S				*pstStatus = HI_NULL;

	EDID_NULL_CHK(pstEdidInfo);

    EDID_INFO("    Vendor / Product Identification...\n");
	pstFstBlk	= (EDID_FIRST_BLOCK_INFO *)pstEdidInfo->au8EdidRaw;
	EDID_NULL_CHK(pstFstBlk);
	pstCap 		= &pstEdidInfo->stCapability;
	pstStatus	= &pstEdidInfo->stStatus;
	pstVendor 	= &pstCap->stMfrsInfo;

	u16Data = (pstFstBlk->au8MfgId[0] << 8) | (pstFstBlk->au8MfgId[1]);

	/* 2Bytes(16bits) width,5-bits a character. */
	for(i = 0; i < 3; i++)
	{
	    pstVendor->u8MfrsName[2 - i] = ((u16Data & (EDID_VEND_NAME_CHAR_MASK << (5*i))) >> (5*i));

		/* 'A'~'Z' is refered to 1~26 */
		if((EDID_VEND_CHAR_LOW_INVALID < pstVendor->u8MfrsName[2 - i])
			&& (EDID_VEND_CHAR_HIGH_INVALID > pstVendor->u8MfrsName[2 - i]))
		{
			pstVendor->u8MfrsName[2 - i] += 'A' - 1;
		}
		else
		{
			pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_VENDOR_INVALID;
		    //EDID_WARN("Vend INFO MFG NAME!\n");
		}
	}

	pstVendor->u32ProductCode       = (pstFstBlk->au8ProdCode[1] << 8) | pstFstBlk->au8ProdCode[0];
	pstVendor->u32SerialNumber      = (pstFstBlk->au8Serial[3] << 24) | (pstFstBlk->au8Serial[2] << 16) \
									  | (pstFstBlk->au8Serial[1] << 8) | (pstFstBlk->au8Serial[0]);
	pstVendor->u32Week 				= pstFstBlk->u8MfgWeek;
	pstVendor->u32Year 				= pstFstBlk->u8MfgYear + EDID_VEND_YEAR_BASE;

    EDID_INFO("    %-20s : %s\n", "ID Manufacturer Name",pstVendor->u8MfrsName);
    EDID_INFO("    %-20s : 0x%0.4X\n", "ID Product Code", pstVendor->u32ProductCode);
	EDID_INFO("    %-20s : 0x%0.8lX\n", "ID Serial Number",pstVendor->u32SerialNumber);
	EDID_INFO("    %-20s : %02d\n", "Week of Manufacture",pstVendor->u32Week);
	EDID_INFO("    %-20s : %d\n", "Year of Manufacture",pstVendor->u32Year);


    return HI_SUCCESS;
}

static HI_S32 EdidVersion(HDMI_EDID_INFO_S *pstEdidInfo)
{
    HDMI_SINK_CAPABILITY_S 	*pstCap = HI_NULL;
	EDID_FIRST_BLOCK_INFO 	*pEdidFstInfo = HI_NULL;
	HDMI_EDID_STATUS_S		*pstStatus = HI_NULL;

	EDID_NULL_CHK(pstEdidInfo);
	pstCap = &pstEdidInfo->stCapability;
	EDID_NULL_CHK(pstCap);
    pEdidFstInfo = (EDID_FIRST_BLOCK_INFO*)pstEdidInfo->au8EdidRaw;
	EDID_NULL_CHK(pEdidFstInfo);
	pstStatus	= &pstEdidInfo->stStatus;

    pstCap->u8Version  = pEdidFstInfo->u8Version;
    pstCap->u8Revision = pEdidFstInfo->u8Revision;
	EDID_INFO("    EDID Version...\n");
	if ( (pstCap->u8Version != 1) || (pstCap->u8Revision != 3 ) )
	{
		EDID_ERR("EDID first blk is not version1.3:%d.%d\n!",
					pstCap->u8Version,pstCap->u8Revision);
		pstStatus->enParseErr = EDID_PARSE_ERR_FST_BLK_VER;
		//return HI_FAILURE;

	}

    EDID_INFO("    %-20s : %02d\n", "Version no.",pstCap->u8Version);
	EDID_INFO("    %-20s : %02d\n", "Revision no.",pstCap->u8Revision);

    return HI_SUCCESS;
}

static HI_S32 EdidBasicDispPara(HDMI_EDID_INFO_S *pstEdidInfo)
{
    HDMI_SINK_CAPABILITY_S 	*pstCap     = HI_NULL;
    EDID_FIRST_BLOCK_INFO	*pstFstBlk  = HI_NULL;

    EDID_NULL_CHK(pstEdidInfo);

    pstCap = &pstEdidInfo->stCapability;
    EDID_NULL_CHK(pstCap);

    pstFstBlk = (EDID_FIRST_BLOCK_INFO *)pstEdidInfo->au8EdidRaw;

    pstCap->stBasicDispPara.u8MaxImageWidth     = pstFstBlk->u8WidthCm;
    pstCap->stBasicDispPara.u8MaxImageHeight    = pstFstBlk->u8HeightCm;

    return HI_SUCCESS;
}

static HI_S32 EdidPhosphor(HDMI_EDID_INFO_S *pstEdidInfo)
{
    HDMI_SINK_CAPABILITY_S 			*pstCap = HI_NULL;
	EDID_FIRST_BLOCK_INFO			*pstFstBlk = HI_NULL;

    EDID_NULL_CHK(pstEdidInfo);

    EDID_INFO("    Phosphor or Filter Chromaticity...\n");
    pstCap = &pstEdidInfo->stCapability;
    EDID_NULL_CHK(pstCap);

    pstFstBlk = (EDID_FIRST_BLOCK_INFO *)pstEdidInfo->au8EdidRaw;

    pstCap->stPhosOrChromCap.u16Red_x = (pstFstBlk->u8RedX << 2) | ((pstFstBlk->u8RedGreenLow >> 6) & 0x03);
    pstCap->stPhosOrChromCap.u16Red_y = (pstFstBlk->u8RedY << 2) | ((pstFstBlk->u8RedGreenLow >> 4) & 0x03);
    pstCap->stPhosOrChromCap.u16Green_x = (pstFstBlk->u8GreenX << 2) | ((pstFstBlk->u8RedGreenLow >> 2) & 0x03);
    pstCap->stPhosOrChromCap.u16Green_y = (pstFstBlk->u8GreenY << 2) | (pstFstBlk->u8RedGreenLow & 0x03);

    pstCap->stPhosOrChromCap.u16Blue_x = (pstFstBlk->u8BlueX << 2) | ((pstFstBlk->u8BlackWhiteLow >> 6) & 0x03);
    pstCap->stPhosOrChromCap.u16Blue_y = (pstFstBlk->u8BlueY << 2) | ((pstFstBlk->u8BlackWhiteLow >> 4) & 0x03);
    pstCap->stPhosOrChromCap.u16White_x = (pstFstBlk->u8WhiteX << 2) | ((pstFstBlk->u8BlackWhiteLow >> 2) & 0x03);
    pstCap->stPhosOrChromCap.u16White_y = (pstFstBlk->u8WhiteY << 2) | (pstFstBlk->u8BlackWhiteLow & 0x03);

    return HI_SUCCESS;
}


static HI_S32 EdidStdTiming(HDMI_EDID_INFO_S *pstEdidInfo)
{
	HI_U8					i = 0;
	HI_U8 					u8AspRatio = 0;
    HI_U8  					*pau8Data = HI_NULL;
    HDMI_SINK_CAPABILITY_S 	*pstCap = HI_NULL;
	EDID_FIRST_BLOCK_INFO 	*pstFstBlk = HI_NULL;

	EDID_NULL_CHK(pstEdidInfo);
	pstCap = &pstEdidInfo->stCapability;
    pstFstBlk = (EDID_FIRST_BLOCK_INFO*)pstEdidInfo->au8EdidRaw;
	EDID_NULL_CHK(pstFstBlk);
	pau8Data = pstFstBlk->au8StdTiming;
	EDID_NULL_CHK(pau8Data);

	HDMI_MEMSET(pstCap->stStdTiming,0,sizeof(pstCap->stStdTiming));
    EDID_INFO("    Standard VESA Timing identifications...\n");
    for(i = 0; i < HDMI_EDID_MAX_STDTIMNG_COUNT ; i++,pau8Data+=2)
    {
        if((pau8Data[0] == EDID_STDTMING_UNUSED_FLAG)
			&&(pau8Data[1]==EDID_STDTMING_UNUSED_FLAG))
        {
           // EDID_INFO("STDTIMNG field[%d] un-used!\n",i);
        }
        else
        {
            pstCap->stStdTiming[i].u32HorActive = (pau8Data[0] + EDID_HORACTIVE_BASE) * EDID_HORACTIVE_FACTOR;
			pstCap->stStdTiming[i].u32RefreshRate	= (pau8Data[1] & EDID_REFRESH_RATE_MASK) + EDID_STDTMING_RATE_BASE;
            u8AspRatio = (pau8Data[1] & EDID_ASPECT_RATIO_MASK) >> 6;

			switch(u8AspRatio)
			{
				case EDID_ASPECT_RATIO_16_10:
					pstCap->stStdTiming[i].u32VerActive = pstCap->stStdTiming[i].u32HorActive*10/16 ;
					break;
				case EDID_ASPECT_RATIO_5_4:
					pstCap->stStdTiming[i].u32VerActive = pstCap->stStdTiming[i].u32HorActive*4/5 ;
					break;
				case EDID_ASPECT_RATIO_4_3:
					pstCap->stStdTiming[i].u32VerActive = pstCap->stStdTiming[i].u32HorActive*3/4 ;
					break;
				case EDID_ASPECT_RATIO_16_9:
					pstCap->stStdTiming[i].u32VerActive = pstCap->stStdTiming[i].u32HorActive*9/16 ;
					break;

				default:
					break;
			}
			EDID_INFO("STDTIMNG field[%d] u32HorActive:%u\n",i,pstCap->stStdTiming[i].u32HorActive);
			EDID_INFO("STDTIMNG field[%d] u32VerActive:%u\n",i,pstCap->stStdTiming[i].u32VerActive);
			EDID_INFO("STDTIMNG field[%d] u32RefreshRate:%u\n",i,pstCap->stStdTiming[i].u32RefreshRate);

        }

    }

    return HI_SUCCESS;
}


static HI_S32 EdidEstablishTiming(HDMI_EDID_INFO_S *pstEdidInfo)
{
	HI_U32	i = 0;
    HDMI_SINK_CAPABILITY_S 	*pstCap = HI_NULL;
	EDID_FIRST_BLOCK_INFO	*pstFstBlk = HI_NULL;

	EDID_NULL_CHK(pstEdidInfo);

	pstCap = &pstEdidInfo->stCapability;
	pstFstBlk = (EDID_FIRST_BLOCK_INFO	*)pstEdidInfo->au8EdidRaw;
	EDID_NULL_CHK(pstFstBlk);

	for (i = 0; i <= HDMI_EDID_ESTABTIMG_BUTT; i++)
	{
		if(pstCap->u32EstabNum >= HDMI_EDID_ESTABTIMG_BUTT)
		{
			EDID_WARN("EstablishTiming u32EstabNum over:%d!\n",pstCap->u32EstabNum);
			break;
		}
		/*if BYTE(i/8),BIT[i%8] is 1*/
		if ( pstFstBlk->au8EstTiming[i/8] & (0x01<<(i%8)) )
		{
			pstCap->au32EstabTiming[pstCap->u32EstabNum++] = i;
			//EDID_INFO("EstablishTiming u32EstabTiming[%d]:%d\n",pstCap->u32EstabNum,i);
		}
	}

    {
		HI_U8  i     = 0;
		HI_U32 dat32 = 0;

		EDID_INFO("    Established VESA Timings...\n");

		/* Read Data */
		for( i=0; i<3; i++ )
		{
			dat32 |= ((HI_U32)pstFstBlk->au8EstTiming[i])<<(8*(3-i));
		}

		i=0;
		while( dat32 )
        {
			if( dat32&0x80000000 )
			{
				//EDID_INFO(("    Established Timing  "));
				switch( i )
				{
				case  0 : EDID_INFO("    Established Timing  %-10s : %s\n", "(IBM)","720x400p,   70Hz, 9:5"); break;
				case  1 : EDID_INFO("    Established Timing  %-10s : %s\n", "(IBM, XGA2)","720x400p,   88Hz, 9:5"); break;
				case  2 : EDID_INFO("    Established Timing  %-10s : %s\n", "(IBM, VGA)","640x480p,   60Hz, 4:3"); break;
				case  4 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","640x480p,   72Hz, 4:3"); break;
				case  5 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","640x480p,   75Hz, 4:3"); break;
				case  6 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","800x600p,   56Hz, 4:3"); break;
				case  7 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","800x600p,   60Hz, 4:3"); break;
				case  8 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","800x600p,   72Hz, 4:3"); break;
				case  9 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","800x600p,   75Hz, 4:3"); break;
				case 11 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","1024x768i,  87Hz, 4:3"); break;
				case 12 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","1024x768p,  60Hz, 4:3"); break;
				case 13 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","1024x768p,  70Hz, 4:3"); break;
				case 14 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","1024x768p,  75Hz, 4:3"); break;
				case 15 : EDID_INFO("    Established Timing  %-10s : %s\n", "(VESA)","1280x1024p, 75Hz, 5:4"); break;
				case  3 : EDID_INFO("    Established Timing  %-10s : %s\n", "(Apple, Mac II)","640x480p,   67Hz, 4:3"); break;
				case 10 : EDID_INFO("    Established Timing  %-10s : %s\n", "(Apple, Mac II)","832x624p,   75Hz, 4:3"); break;
				case 16 : EDID_INFO("    Established Timing  %-10s : %s\n", "(Apple, Mac II)","1152x870p,  75Hz, 4:3"); break;
				}
			}
			dat32 <<= 1;
			i++;
		}
	}
	return HI_SUCCESS;
}

static HI_VOID EdidStereoViewSupport(EDID_DTD_TIMING_BLOCK *pstDTD)
{
    EDID_NULL_CHK_NO_RET(pstDTD);

    switch(((pstDTD->u8Flags & EDID_STEREO_MASK ) >> 4)|(pstDTD->u8Flags & BIT0_MASK))
	{
		case EDID_STEREO_SEQUENTIAL_L :
			//EDID_INFO("DTDs stereo sequential L\n");
			break;

		case EDID_STEREO_INTERLEAVED_2R :
			//EDID_INFO("DTDs stereo interleaved 2R\n");
			break;

		case EDID_STEREO_INTERLEAVED_2L :
			//EDID_INFO("DTDs stereo interleaved 2L\n");
			break;

		case EDID_STEREO_INTERLEAVED_4 :
			// EDID_INFO("DTDs stereo interleaved 4\n");
			break;

		case EDID_STEREO_INTERLEAVED_SBS :
			//EDID_INFO("DTDs stereo interleaved SBS\n");
			break;

		default:
			//EDID_INFO("DTDs no stereo \n");
			break;
	}

    return;
}

static HI_VOID EdidAnalogSyncSignalDef(EDID_DTD_TIMING_BLOCK *pstDTD)
{
    EDID_NULL_CHK_NO_RET(pstDTD);

    switch((pstDTD->u8Flags & EDID_SYNC_SIGNAL_TYPE_MASK ) >> 1)
	{
		/*Analog Composite Sync - Without Serrations - Sync On Green Signal only*/
		case 0x00:
           // EDID_INFO("DTDs sync acs ws green\n");
			break;

		/*Analog Composite Sync - Without Serrations - Sync On all three (RGB) video signals*/
		case 0x01:
           // EDID_INFO("DTDs sync acs ws all\n");
			break;

		/*Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On Green Signal only*/
		case 0x02:
           // EDID_INFO("DTDs sync acs ds green\n");
			break;

		/*Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On all three (RGB) video signals*/
		case 0x03:
            //EDID_INFO("DTDs sync acs ds all\n");
            break;

		/*Bipolar Analog Composite Sync - Without Serrations; - Sync On Green Signal only*/
		case 0x04:
           // EDID_INFO("DTDs sync bacs ws green\n");
            break;

		/*Bipolar Analog Composite Sync - Without Serrations; - Sync On all three (RGB) video signals*/
		case 0x05:
           // EDID_INFO("DTDs sync bacs ws all\n");
            break;

		/*Bipolar Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On Green Signal only*/
		case 0x06:
           // EDID_INFO("DTDs sync bacs ds green\n");
            break;

		/*Bipolar Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On all three (RGB) video signals*/
		case 0x07:
           // EDID_INFO("DTDs sync bacs ds all\n");
            break;

		default:
			break;

      }
    return;
}

static HI_VOID EdidDigitalSyncSignalDef(EDID_DTD_TIMING_BLOCK *pstDTD, HDMI_EDID_PRE_TIMING_S *pstCapPreTiming)
{
    EDID_NULL_CHK_NO_RET(pstDTD);
    EDID_NULL_CHK_NO_RET(pstCapPreTiming);

    switch((pstDTD->u8Flags & EDID_SYNC_SIGNAL_TYPE_MASK) >> 1)
	{
		/* DCS:Digital Composite Sync */
		case EDID_SYNC_DCS_WS_0:
        case EDID_SYNC_DCS_WS_1:
            pstCapPreTiming->bIHS = HI_FALSE;
            pstCapPreTiming->bIVS = HI_FALSE;
            break;

		case EDID_SYNC_DCS_DS_2:
		case EDID_SYNC_DCS_DS_3:
            break;

		/* DSS:Digital Separate Sync */
		case EDID_SYNC_DSS_VN_HN_4:
            pstCapPreTiming->bIHS = HI_FALSE;
            pstCapPreTiming->bIVS = HI_FALSE;
            break;

		case EDID_SYNC_DSS_VN_HP_5:
            pstCapPreTiming->bIHS = HI_TRUE;
            pstCapPreTiming->bIVS = HI_FALSE;
            break;

		case EDID_SYNC_DSS_VP_HN_6:
            pstCapPreTiming->bIHS = HI_FALSE;
            pstCapPreTiming->bIVS = HI_TRUE;
            break;

		case EDID_SYNC_DSS_VP_HP_7:
            pstCapPreTiming->bIHS = HI_TRUE;
            pstCapPreTiming->bIVS = HI_TRUE;
            break;

		default:
			break;

	}

    return;
}

static HI_S32 EdidDetailTiming(HDMI_SINK_CAPABILITY_S *pstCap,
										HDMI_EDID_STATUS_S *pstStatus,
										HI_U8 *pau8Data,
										HI_U8 u8Len)
{
    HI_U32	 				u32Temp = 0;
	HDMI_EDID_PRE_TIMING_S	*pstCapPreTiming = HI_NULL;
	EDID_DTD_TIMING_BLOCK 	*pstDTD = HI_NULL;
    EDID_MONITOR_DES_BLOCK  *pstMonitorDesc  = HI_NULL;
    HI_U8                   u8MonitorFlag[3] = {0x00, 0x00, 0x00};

	EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstStatus);

	if (pstCap->u32PerferTimingNum >= HDMI_EDID_MAX_DETAIL_TIMING_COUNT
		|| u8Len < EDID_DTD_SIZE)
	{
		pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_DTD_OVER;
		EDID_WARN("DTDs over max DTDs cnt or less u8Len:%d!\n", u8Len);
		return HI_SUCCESS;
	}

    /* case1: Monitor Descriptor */
    if(0 == HDMI_MEMCMP(pau8Data, u8MonitorFlag, sizeof(u8MonitorFlag)))
    {
        /* case 1 */
        if (EDID_TYPE_MONITOR_NAME == pau8Data[3])   /*Display Product Name*/
        {
            pstMonitorDesc = (EDID_MONITOR_DES_BLOCK*)pau8Data;
            HDMI_MEMCPY(pstCap->stMfrsInfo.u8pSinkName, pstMonitorDesc->u8DesData, HDMI_EDID_MAX_SINKNAME_COUNT- 1);
            pstCap->stMfrsInfo.u8pSinkName[HDMI_EDID_MAX_SINKNAME_COUNT - 1] = '\0';
        }
        else
        {
            /* case 1\2 no use */
        }
        return HI_SUCCESS;
    }

    /* case2: Detailed Timing Description */
    
	pstDTD = (EDID_DTD_TIMING_BLOCK*)pau8Data;
	pstCapPreTiming = &pstCap->stPerferTiming[pstCap->u32PerferTimingNum];

    /* pixel clock ,KHz */
    u32Temp = pstDTD->au8PixelClk[1];
	u32Temp <<= 8;
	u32Temp |= pstDTD->au8PixelClk[0];
    u32Temp *= EDID_PIXCLK_KHZ_FACTOR;
	pstCapPreTiming->u32PixelClk = u32Temp;
	//EDID_INFO("DTDs pixel CLK(KHz):%d\n",u32Temp);

	if (u32Temp > 0)
	{
		pstCap->u32PerferTimingNum++;
	}
	else
	{
		EDID_INFO("DTDs empty DTDs,CurNum:%d\n",pstCap->u32PerferTimingNum);
		return HI_SUCCESS;
	}


     /* VFB ,2+4 bits */
    u32Temp = pstDTD->u8HS_Offset_VS_Offset & EDID_VSO_MASK  ;
	u32Temp <<= 2;
    u32Temp |= (pstDTD->u8VS_OffsetPulseWidth & EDID_UPPER_NIBBLE_MASK) >> 4 ;
	//u32Temp += pstDTD->u8V_Border ;
	pstCapPreTiming->u32VFB = u32Temp;
	//EDID_INFO("DTDs VFB :%d\n",u32Temp);

	/*v_active_blank ( vblack) = vfront + vback + vsync = VFB(vfront+ vsync) + VBB(vback )*/

    /* VBB ,4+8 bits*/
    u32Temp = pstDTD->u8V_ActiveBlank & EDID_LOWER_NIBBLE_MASK;
    u32Temp <<= 8;
	u32Temp |= pstDTD->u8V_Blank ;
    pstCapPreTiming->u32VBB = u32Temp - pstCapPreTiming->u32VFB;
    //EDID_INFO("DTDs VBB :%d\n",u32Temp);

    /* VACT ,4+8 bits */
    u32Temp = pstDTD->u8V_ActiveBlank & EDID_UPPER_NIBBLE_MASK;
	u32Temp <<= 4;
	u32Temp|= pstDTD->u8V_Active ;
    pstCapPreTiming->u32VACT = u32Temp;
    //EDID_INFO("DTDs VACT :%d\n",u32Temp);

    /* HFB ,2+8 bits */
    u32Temp = pstDTD->u8HS_Offset_VS_Offset &  EDID_HSO_MASK;
	u32Temp <<= 2;
    u32Temp |= pstDTD->u8H_SyncOffset ;
	//u32Temp += pstDTD->u8H_Border ;
	pstCapPreTiming->u32HFB = u32Temp;
    //EDID_INFO("DTDs HFB :%d\n",u32Temp);

	/*h_active_blank ( hblack ) == hfront + hback + hsync = HFB(hfront + hsync) + HBB(hback)*/
    /* HBB , 4+8 bits */
    u32Temp = (pstDTD->u8H_ActiveBlank & EDID_LOWER_NIBBLE_MASK);
	u32Temp <<= 8;
	u32Temp |= pstDTD->u8H_Blank ;
	pstCapPreTiming->u32HBB = u32Temp - pstCapPreTiming->u32HFB;
    //EDID_INFO("DTDs HBB :%d\n",u32Temp);

    /* HACT,4+8 bits */
    u32Temp = pstDTD->u8H_ActiveBlank & EDID_UPPER_NIBBLE_MASK;
    u32Temp <<= 4;
	u32Temp	|= pstDTD->u8H_Active ;
    pstCapPreTiming->u32HACT = u32Temp;
    //EDID_INFO("DTDs HACT :%d\n",u32Temp);

	/* VPW,2+4 bits */
    u32Temp = pstDTD->u8HS_Offset_VS_Offset & EDID_VSPW_MASK ;
	u32Temp <<= 4;
    u32Temp |= (pstDTD->u8VS_OffsetPulseWidth & EDID_LOWER_NIBBLE_MASK);
    pstCapPreTiming->u32VPW = u32Temp;
	//EDID_INFO("DTDs VPW :%d\n",u32Temp);

    /* HPW,2+8 bits */
    u32Temp = pstDTD->u8HS_Offset_VS_Offset & EDID_HSPW_MASK ;
	u32Temp <<= 4;
    u32Temp |= pstDTD->u8H_SyncPulseWidth ;
    pstCapPreTiming->u32HPW = u32Temp;
	//EDID_INFO("DTDs HPW :%d\n",u32Temp);

    /* H image size(mm),4+8 bits */
    u32Temp = pstDTD->u8H_V_ImageSize & EDID_UPPER_NIBBLE_MASK ;
	u32Temp <<= 4;
	u32Temp |= pstDTD->u8H_ImageSize ;
    pstCapPreTiming->u32ImageWidth = u32Temp;
    //EDID_INFO("DTDs H image size(mm) :%d\n",u32Temp);


    /* V image size(mm),4+8 bits */
    u32Temp = pstDTD->u8H_V_ImageSize & EDID_LOWER_NIBBLE_MASK;
	u32Temp <<= 8;
    u32Temp |= pstDTD->u8V_ImageSize ;
    pstCapPreTiming->u32ImageHeight = u32Temp;
	//EDID_INFO("DTDs V image size(mm) :%d\n",u32Temp);

	/*Interlaced flag*/
    if(pstDTD->u8Flags & BIT7_MASK)
	{
        pstCapPreTiming->bInterlace = HI_TRUE;
		//EDID_INFO("DTDs Output mode: interlaced\n");
	}
	else
	{
        pstCapPreTiming->bInterlace = HI_FALSE;
		//EDID_INFO("DTDs Output mode: progressive\n");
	}

	/*Stereo Viewing Support*/
	EdidStereoViewSupport(pstDTD);

	/*Analog Sync Signal Definitions*/
    if(0 == (pstDTD->u8Flags & BIT4_MASK))
	{
		EdidAnalogSyncSignalDef(pstDTD);
    }
	/* Digital Sync Signal Definitions */
    else
    {
        EdidDigitalSyncSignalDef(pstDTD, pstCapPreTiming);
    }
    pstCapPreTiming->bIDV = HI_FALSE;
    return HI_SUCCESS;
}

static HI_S32 EdidExtNum(HDMI_EDID_INFO_S *pstEdidInfo)
{
	HDMI_EDID_STATUS_S		*pstStatus = HI_NULL;
    HI_U8  					*pau8Data = HI_NULL;
    HDMI_SINK_CAPABILITY_S 	*pstCap = HI_NULL;

	EDID_NULL_CHK(pstEdidInfo);

	pstStatus 	= &pstEdidInfo->stStatus;
	pau8Data 	= pstEdidInfo->au8EdidRaw;
	pstCap 		= &pstEdidInfo->stCapability;

	pstCap->u8ExtBlockNum = (HI_U8)pau8Data[EDID_EXTENSION_BLK_ADDR];
	pstCap->bSupportHdmi = HI_FALSE;
    EDID_INFO("    Extension Flag...\n");
	EDID_INFO("    %-20s : %02d\n", "Extensions",pstCap->u8ExtBlockNum);

	if(pstCap->u8ExtBlockNum == 0)
	{
		pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_EXT_BLK_ZERO;
		EDID_WARN("Ext-BLK zero: 0x%02x \n",pstCap->u8ExtBlockNum);
	}
	else if(pstCap->u8ExtBlockNum > (HDMI_EDID_MAX_BLOCK_NUM - 1))
	{
		pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_EXT_BLK_OVER;
        pstCap->u8ExtBlockNum = HDMI_EDID_MAX_BLOCK_NUM - 1;
		EDID_WARN("Ext-BLK cnt over: 0x%02x \n",pstCap->u8ExtBlockNum);

	}

	return HI_SUCCESS;
}


static HI_S32 EdidFirstBlkParse(HDMI_EDID_INFO_S *pstEdidInfo)
{
	HI_S32					s32Ret = HI_SUCCESS;
	HDMI_EDID_STATUS_S		*pstStatus = HI_NULL;
    HI_U8  					*pau8Data = HI_NULL;
	HDMI_SINK_CAPABILITY_S 	*pstCap = HI_NULL;
	EDID_FIRST_BLOCK_INFO	*pFstInfo = HI_NULL;


	EDID_NULL_CHK(pstEdidInfo);

    EDID_INFO("   First Block Parse...\n");
	pstStatus = &pstEdidInfo->stStatus;
	pau8Data = pstEdidInfo->au8EdidRaw;
	pstCap = &pstEdidInfo->stCapability;
	pFstInfo = (EDID_FIRST_BLOCK_INFO *)pau8Data;
	EDID_NULL_CHK(pau8Data);


	s32Ret |= EdidBlockChkSum(pau8Data,pstStatus);
	EDID_SUCCESS_CHK(s32Ret);
    /* 00h-07h Header */
    s32Ret |= EdidHeaderChk(pau8Data,pstStatus);
	EDID_SUCCESS_CHK(s32Ret);
    /* 08h-11h Vendor Info */
    s32Ret |= EdidVendorInfo(pstEdidInfo);
	EDID_SUCCESS_CHK(s32Ret);
	/* 12h-13h Version */
    s32Ret |= EdidVersion(pstEdidInfo);
	EDID_SUCCESS_CHK(s32Ret);
    /* 14h-18h Basic Display Parameters/Features */
    s32Ret |= EdidBasicDispPara(pstEdidInfo);
	EDID_SUCCESS_CHK(s32Ret);
    /* 19h-22h Color Characteristics */
    s32Ret |= EdidPhosphor(pstEdidInfo);
	EDID_SUCCESS_CHK(s32Ret);
	/* 23h-25h Established Timings */
    s32Ret |= EdidEstablishTiming(pstEdidInfo);
	EDID_SUCCESS_CHK(s32Ret);
	/* 26h-35h Standard Timing Identification */
    s32Ret |= EdidStdTiming(pstEdidInfo);
	EDID_SUCCESS_CHK(s32Ret);
	/* 36h-47h First Detailed Timing Descriptions */
    s32Ret |= EdidDetailTiming(pstCap, pstStatus, pFstInfo->au8DetailedTiming1, EDID_DTD_SIZE);
    EDID_SUCCESS_CHK(s32Ret);
    s32Ret |= EdidDetailTiming(pstCap, pstStatus, pFstInfo->au8DetailedTiming2, EDID_DTD_SIZE);
	EDID_SUCCESS_CHK(s32Ret);
    s32Ret |= EdidDetailTiming(pstCap, pstStatus, pFstInfo->au8DetailedTiming3, EDID_DTD_SIZE);
    EDID_SUCCESS_CHK(s32Ret);
    s32Ret |= EdidDetailTiming(pstCap, pstStatus, pFstInfo->au8DetailedTiming4, EDID_DTD_SIZE);
	EDID_SUCCESS_CHK(s32Ret);

	s32Ret |= EdidExtNum(pstEdidInfo);
	EDID_SUCCESS_CHK(s32Ret);

    return s32Ret;
}

#ifndef HI_ADVCA_FUNCTION_RELEASE
static HI_CHAR* u8AudioCodeStr[] =
{
    "Reserved",
    "Linear PCM",
    "AC3",
    "MPEG1 (Layers 1 & 2)",
    "MP3 (MPEG1 Layer 2)",
    "MPEG2 (multichannel)",
    "AAC",
    "DTS",
    "ATREC",
    "One Bit Audio",
    "Dolby Digital+",
    "DTS-HD",
    "MAT (MLP)",
    "DST",
    "WMA Pro",
    "Reserved",
};
static HI_CHAR* u8VideoCodeStr[HDMI_3840X2160P24_16_9] =
{
    "No VID available",
    "640x480p,    60Hz,  4:3",
    "720x480p,    60Hz,  4:3",
    "720x480p,    60Hz, 16:9",
    "1280x720p,   60Hz, 16:9",
    "1920x1080i,  60Hz, 16:9",
    "720x480i,    60Hz,  4:3",
    "720x480i,    60Hz, 16:9",
    "720x240p,    60Hz,  4:3",
    "720x240p,    60Hz, 16:9",
    "2880x480i,   60Hz,  4:3",
    "2880x480i,   60Hz, 16:9",
    "2880x240p,   60Hz,  4:3",
    "2880x240p,   60Hz, 16:9",
    "1440x480p,   60Hz,  4:3",
    "1440x480p,   60Hz, 16:9",
    "1920x1080p,  60Hz, 16:9",
    "720x576p,    50Hz,  4:3",
    "720x576p,    50Hz, 16:9",
    "1280x720p,   50Hz, 16:9",
    "1920x1080i,  50Hz, 16:9",
    "720x576i,    50Hz,  4:3",
    "720x576i,    50Hz, 16:9",
    "720x288p,    50Hz,  4:3",
    "720x288p,    50Hz, 16:9",
    "2880x576i,   50Hz,  4:3",
    "2880x576i,   50Hz, 16:9",
    "2880x288p,   50Hz,  4:3",
    "2880x288p,   50Hz, 16:9",
    "1440x576p,   50Hz,  4:3",
    "1440x576p,   50Hz, 16:9",
    "1920x1080p,  50Hz, 16:9",
    "1920x1080p,  24Hz, 16:9",
    "1920x1080p,  25Hz, 16:9",
    "1920x1080p,  30Hz, 16:9",
    "2880x480p,   60Hz,  4:3",
    "2880x480p,   60Hz, 16:9",
    "2880x576p,   50Hz,  4:3",
    "2880x576p,   50Hz, 16:9",
    "1920x1080i,  50Hz, 16:9",
    "1920x1080i, 100Hz, 16:9",
    "1280x720p,  100Hz, 16:9",
    "720x576p,   100Hz,  4:3",
    "720x576p,   100Hz, 16:9",
    "720x576i,   100Hz,  4:3",
    "720x576i,   100Hz, 16:9",
    "1920x1080i, 120Hz, 16:9",
    "1280x720p,  120Hz, 16:9",
    "720x480p,   120Hz,  4:3",
    "720x480p,   120Hz, 16:9",
    "720x480i,   120Hz,  4:3",
    "720x480i,   120Hz, 16:9",
    "720X576p,   200Hz,  4:3",
    "720X576p,   200Hz, 16:9",
    "720x576i,   200Hz,  4:3",
    "720x576i,   200Hz, 16:9",
    "720x480p,   240Hz,  4:3",
    "720x480p,   240Hz, 16:9",
    "720x480i,   240Hz,  4:3",
    "720x480i,   240Hz, 16:9",
    "1280x720p,  24Hz,  16:9",
    "1280x720p,  25Hz,  16:9",
    "1280x720p,  30Hz,  16:9",
};

static HI_CHAR* u8VideoCode4KStr[] =
{
    "3840x2160p, 24Hz,  16:9",
    "3840x2160p, 25Hz,  16:9",
    "3840x2160p, 30Hz,  16:9",
    "3840x2160p, 50Hz,  16:9",
    "3840x2160p, 60Hz,  16:9",
    "4096x2160p, 24Hz,  16:9",
    "4096x2160p, 25Hz,  16:9",
    "4096x2160p, 30Hz,  16:9",
    "4096x2160p, 50Hz,  16:9",
    "4096x2160p, 60Hz,  16:9",
    "3840x2160p, 24Hz,  4:3",
    "3840x2160p, 25Hz,  4:3",
    "3840x2160p, 30Hz,  4:3",
    "3840x2160p, 50Hz,  4:3",
    "3840x2160p, 60Hz,  4:3",
};
/*
static HI_CHAR* u83dModeStr[]=
{
    "Frame Packing",
    "Field alternative",
    "Line alternative",
    "Side-by-Side (full)",
    "L + depth",
    "L + depth + graph. + graph.-depth",
    "Top-and-Bottom",
    "Reserved",
    "Side-by-Side (half)",
    "Reserved",
};
*/
#else
static HI_CHAR* u8AudioCodeStr[] =
{

};

static HI_CHAR* u8VideoCodeStr[] =
{

};

static HI_CHAR* u8VideoCode4KStr[] =
{

};
/*
static HI_CHAR* u83dModeStr[]=
{

};*/
#endif
static HI_S32 EdidAudioDB(HDMI_SINK_CAPABILITY_S *pstCap,
									HDMI_EDID_STATUS_S *pstStatus,
									HI_U8 *pau8Data,
									HI_U8 u8Len)
{
	HI_U8 						i = 0;
	HI_U8 						u8FrmCode = 0;
	HI_U8 						u8Byte = 0;
	HI_U8 						u8Count = 0;
	HI_U8 						u8CurNum = 0;
    HDMI_EDID_AUDIO_INFO_S	 	*pstAudioInfo = HI_NULL;
    HI_CHAR                     *pAudioCodeStr = HI_NULL;

	EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstStatus);
	pstAudioInfo = pstCap->stAudioInfo;

	EDID_INFO("    Audio Data Block (Length=%02d)...\n", u8Len);
    /*Each Short Audio Descriptor is 3-bytes long*/
    for(i = 0; i < (u8Len / 3); i++)
    {
    	u8Count = 0;
        u8CurNum = pstCap->u32AudioInfoNum;


        if(u8CurNum >= HDMI_EDID_MAX_AUDIO_CAP_COUNT)
        {
        	pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_AUDIO_CNT_OVER;
            EDID_WARN("ADB Audio Capability Count over max-num:%d \n",HDMI_EDID_MAX_AUDIO_CAP_COUNT);
            break;
        }
		/* Byte-0 */
		u8Byte = pau8Data[i*3];
		//EDID_INFO("ADB [u8Byte0]:%02x\n",u8Byte);
		EDID_INFO("    %-20s : 0x%02X, 0x%02X, 0x%02X\n", "Short Audio Descriptor",pau8Data[i*3],pau8Data[i*3+1],pau8Data[i*3+2]);
        u8FrmCode = (u8Byte & EDID_AUDIO_FORMAT_MASK) >> 3;
        pstAudioInfo[u8CurNum].u8AudChannel = (u8Byte & EDID_AUDIO_CHANNEL_MASK) + 1;
		pstAudioInfo[u8CurNum].enAudFmtCode = (HDMI_AUDIO_FORMAT_CODE_E)u8FrmCode;
        pAudioCodeStr = u8AudioCodeStr[u8FrmCode];
        EDID_INFO("    %-20s : %02d\n", "Max. Number of Channels",pstAudioInfo[u8CurNum].u8AudChannel);
        EDID_INFO("    %-20s : %s\n", "Audio Format Code",pAudioCodeStr);
        //EDID_INFO("ADB ====u8CurNum====:%d\n",u8CurNum);
		//EDID_INFO("ADB u8FrmCode:%d\n",u8FrmCode);
		//EDID_INFO("ADB u8AudChannel:%d\n",pstAudioInfo[u8CurNum].u8AudChannel);
		//EDID_INFO("ADB enAudFmtCode:%d\n",pstAudioInfo[u8CurNum].enAudFmtCode);


        //if(u8FrmCode > HDMI_AUDIO_FORMAT_CODE_RESERVED
		//&& u8FrmCode < HDMI_AUDIO_FORMAT_CODE_BUTT)
		if(u8FrmCode > HDMI_AUDIO_CODING_TYPE_STREAM
		&& u8FrmCode < HDMI_AUDIO_CODING_TYPE_BUTT)
		{
            pstCap->u32AudioInfoNum++;
        }
		else
		{
			pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_AUDIO_FMT_INVALID;
			EDID_WARN("ADB CEA-861-F not support Audio Frm:%d \n",u8FrmCode);
			break;
		}

		/* Byte-1 */
        u8Byte = pau8Data[i*3 + 1];
		//EDID_INFO("ADB [u8Byte1]:%02x\n",u8Byte);
        if( u8Byte & BIT0_MASK )
        {
            pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count] = HDMI_SAMPLE_RATE_32K;
    		EDID_INFO("    %-20s : %d Hz\n", "Sample Frequencies",pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count]);
            u8Count++;
        }
        if( u8Byte & BIT1_MASK )
        {
            pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count] = HDMI_SAMPLE_RATE_44K;
		    EDID_INFO("    %-20s : %d Hz\n", "Sample Frequencies",pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count]);
            u8Count++;
        }
        if( u8Byte & BIT2_MASK )
        {
            pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count] = HDMI_SAMPLE_RATE_48K;
    		EDID_INFO("    %-20s : %d Hz\n", "Sample Frequencies",pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count]);
            u8Count++;
        }
        if( u8Byte & BIT3_MASK )
        {
            pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count] = HDMI_SAMPLE_RATE_88K;
    		EDID_INFO("    %-20s : %d Hz\n", "Sample Frequencies",pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count]);
            u8Count++;
        }
        if( u8Byte & BIT4_MASK )
        {
            pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count] = HDMI_SAMPLE_RATE_96K;
    		EDID_INFO("    %-20s : %d Hz\n", "Sample Frequencies",pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count]);
            u8Count++;
        }
		/*  CEA-861-F add for HDMI2.0 */
		if (u8FrmCode >= 1 && u8FrmCode <= 14)
		{
	        if( u8Byte & BIT5_MASK )
	        {
	            pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count] = HDMI_SAMPLE_RATE_176K;
        		EDID_INFO("    %-20s : %d Hz\n", "Sample Frequencies",pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count]);
	            u8Count++;
	        }
	        if( u8Byte & BIT6_MASK )
	        {
	            pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count] = HDMI_SAMPLE_RATE_192K;
        		EDID_INFO("    %-20s : %d Hz\n", "Sample Frequencies",pstAudioInfo[u8CurNum].enSupportSampleRate[u8Count]);
	            u8Count++;
	        }
		}
        pstAudioInfo[u8CurNum].u32SupportSampleRateNum = u8Count;

		/* Byte-2 */
        if(1 == u8FrmCode)
        {
            u8Count = 0;
            u8Byte = pau8Data[i*3 + 2];
			//EDID_INFO("ADB [u8Byte2]:%02x\n",u8Byte);

            if(u8Byte & BIT0_MASK)
            {
                pstAudioInfo[u8CurNum].enSupportBitDepth[u8Count] = HDMI_AUDIO_BIT_DEPTH_16;
                EDID_INFO("    %-20s : %d bit\n", "Bit Depth", pstAudioInfo[u8CurNum].enSupportBitDepth[u8Count]);
                u8Count++;
            }
            if(u8Byte & BIT1_MASK)
            {
                pstAudioInfo[u8CurNum].enSupportBitDepth[u8Count] = HDMI_AUDIO_BIT_DEPTH_20;
			    EDID_INFO("    %-20s : %d bit\n", "Bit Depth", pstAudioInfo[u8CurNum].enSupportBitDepth[u8Count]);
                u8Count++;

            }
            if(u8Byte & BIT2_MASK)
            {
                pstAudioInfo[u8CurNum].enSupportBitDepth[u8Count] = HDMI_AUDIO_BIT_DEPTH_24;
			    EDID_INFO("    %-20s : %d bit\n", "Bit Depth", pstAudioInfo[u8CurNum].enSupportBitDepth[u8Count]);
                u8Count++;
            }

            pstAudioInfo[u8CurNum].u32SupportBitDepthNum = u8Count;
        }
        else if ((u8FrmCode > 1) && (u8FrmCode < 9))
        {
            pstAudioInfo[u8CurNum].u32MaxBitRate = pau8Data[i*3 + 2] * EDID_AUDIO_BITRATE_FACTOR;
            EDID_INFO("    %-20s : %d KHz\n", "Maximum Bit Rate", pstAudioInfo[u8CurNum].u32MaxBitRate);

        }
		/* the following part is CEA-861-F for HDMI2.0,we don't use. */
        else if((u8FrmCode >= 9) && (u8FrmCode < 15))
        {
			//EDID_INFO("ADB Profile:0x%x\n",pau8Data[i*3 + 2]);
			EDID_INFO("    %-20s : %d KHz\n", "Maximum Bit Rate", pau8Data[i*3 + 2]);
        }
		else if (u8FrmCode == 15)
		{
			u8Byte	= pau8Data[i*3 + 2];
            EDID_INFO("    %-20s : %d KHz\n", "Maximum Bit Rate", u8Byte);
			//EDID_INFO("ADB ExtTypeCode:0x%x\n",(u8Byte & EDID_AUDIO_EXT_TYPE_CODE) >> 3);
			//EDID_INFO("ADB MPEG Surround (MPS)(BIT[0]):%d\n",u8Byte & BIT0_MASK);
			//EDID_INFO("ADB 960_TL(BIT[1]):0x%x\n",(u8Byte & BIT1_MASK)>>1);
			//EDID_INFO("ADB 1024_TL(BIT[2]):0x%x\n",(u8Byte & BIT2_MASK)>>2);
		}
		else
		{
			/* reserved */
		}

    }

    return HI_SUCCESS;
}
#if 0
static HI_S32 LookUpCommon4kFmt(HDMI_SINK_CAPABILITY_S *pstCap, HI_U32 u32VideoCode)
{
    HI_U32 i = 0;

    while (i < pstCap->u32SupportVICNum)
    {
        if (pstCap->au32SupportFormat[i] == u32VideoCode)
        {
            return HI_SUCCESS;
        }
        i++;
    }

    return HI_FAILURE;
}
#endif
static HI_S32 EdidVideoDB(HDMI_SINK_CAPABILITY_S *pstCap,
								HDMI_EDID_STATUS_S *pstStatus,
								HI_U8 *pau8Data,
								HI_U8 u8Len)
{
    HI_U8 	i = 0;
	HI_U32 	u32InplicitNative = EDID_VIC_INVALID_ZERO;
	HI_U32	u32VicAll = EDID_VIC_INVALID_ZERO;
    HI_U32 	u32VicLower = EDID_VIC_INVALID_ZERO;
    HI_CHAR *pVideoCodeStr = HI_NULL;

    EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstStatus);

    EDID_INFO("    Video Data Block (Length=%02d)...\n", u8Len);
    for(i = 0; i < u8Len; i++)
    {
    	u32VicAll = pau8Data[i];
        u32VicLower = pau8Data[i] & EDID_VIC_LOWER7_MASK;

		/*avoid full over*/
		if ( pstCap->u32SupportVICNum >= HDMI_EDID_MAX_VIC_COUNT )
		{
			pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_VIC_CNT_OVER;
			EDID_WARN("VDB vic count over:%d !\n",HDMI_EDID_MAX_VIC_COUNT);
			break;
		}

		/*avoid invalid vic*/
		if ( u32VicAll == EDID_VIC_INVALID_ZERO )
		{
			pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_VIC_INVALID;
			EDID_WARN("VDB the %d u32Vic: %d ! \n",i,u32VicAll);
			continue;
		}

		/*explicit native*/
		if ( (EDID_VIC_INVALID_ZERO == pstCap->u32NativeFormat) &&
			 (u32VicAll & BIT7_MASK) && (u32VicLower <= EDID_VIC_NATIVE_MAX) )
		{
			pstCap->u32NativeFormat = u32VicLower;
	    }

		/* set the first valid VIC as implicit native  */
		if ( EDID_VIC_INVALID_ZERO == u32InplicitNative)
		{
			u32InplicitNative = u32VicAll;
		}

		if ((u32VicAll & BIT7_MASK) && (u32VicLower <= EDID_VIC_NATIVE_MAX))
		{
			pstCap->au32SupportFormat[pstCap->u32SupportVICNum] = u32VicLower ;
		}
		else
		{
			pstCap->au32SupportFormat[pstCap->u32SupportVICNum] = u32VicAll;
		}

		//EDID_INFO("VDB Num[%d] u32VicAll :%d(0x%02x)\n",i,u32VicAll,u32VicAll);
		//EDID_INFO("VDB Num[%d] u32RealVic:%d\n",i,pstCap->au32SupportFormat[pstCap->u32SupportVICNum]);
        if (u32VicLower < HDMI_VIDEO_CODE_BUTT)
        {
            pVideoCodeStr = u32VicLower >= HDMI_3840X2160P24_16_9? \
                            u8VideoCode4KStr[u32VicLower-HDMI_3840X2160P24_16_9]:\
                            u8VideoCodeStr[u32VicLower];
            EDID_INFO("    %-20s : %d-%s\n", "Short Video Descriptor", u32VicLower, pVideoCodeStr);
        }
		pstCap->u32SupportVICNum++;

	}

	if (EDID_VIC_INVALID_ZERO == pstCap->u32NativeFormat)
	{
		pstCap->u32NativeFormat = u32InplicitNative;
	}

    pVideoCodeStr = pstCap->u32NativeFormat >= HDMI_3840X2160P24_16_9? \
                    u8VideoCode4KStr[pstCap->u32NativeFormat-HDMI_3840X2160P24_16_9]: \
                    u8VideoCodeStr[pstCap->u32NativeFormat];
    EDID_INFO("    %-20s : %d-%s Native\n", "Short Video Descriptor",pstCap->u32NativeFormat, pVideoCodeStr);

    return HI_SUCCESS;
}

static HI_S32 EdidHFVSDB(HDMI_SINK_CAPABILITY_S *pstCap,
									HDMI_EDID_STATUS_S *pstStatus,
									HI_U8 *pau8Data,
									HI_U8 u8Len )
{

	EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstStatus);

	if (u8Len < 7 || u8Len > 31)
	{
		pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_BLOCKLEN_INVALID;
		EDID_WARN("HFVSDB u8Len:%d\n",u8Len);
		return HI_SUCCESS;
	}

	/* Byte-3 ,Version*/
	if (pau8Data[3] != EDID_HFVSDB_VERSION)
	{
		pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_HFVSDB_INVALID;
		EDID_WARN("    HFVSDB Version:%d\n",pau8Data[3]);
	}
	//EDID_INFO("HFVSDB [byte-3]:0x%02x\n",pau8Data[3]);
	EDID_INFO("    HDMI Forum Vendor Specific Data Block        %-36s : %02d\n","Version", pau8Data[3]);
	/* Byte-4 */
	pstCap->u32MaxTMDSClock = pau8Data[4];
	//EDID_INFO("VSDB [byte-4]:0x%02x\n",pstCap->u32MaxTMDSClock);
	pstCap->u32MaxTMDSClock *= EDID_TMDS_FACTOR;
	pstCap->bSupportHdmi_2_0 = ( pstCap->u32MaxTMDSClock >= EDID_MAX_HDMI14_TMDS_RATE ) ? HI_TRUE : HI_FALSE ;
	//EDID_INFO("HFVSDB Max TMDS Colock:%d\n", pstCap->u32MaxTMDSClock);
    EDID_INFO("    %-20s : %d MHz\n", "Max_TMDS_Character_Rate", pstCap->u32MaxTMDSClock);
	/* Byte-5 */
	//EDID_INFO("HFVSDB [byte-5]:0x%02x\n",pau8Data[5]);
	pstCap->bSupport3dOsdDisparity 			= (pau8Data[5] & BIT0_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->bSupport3dDualView	 			= (pau8Data[5] & BIT1_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->bSupport3dIndependentView 		= (pau8Data[5] & BIT2_MASK) ? HI_TRUE : HI_FALSE ;
#if 1
	pstCap->bSupportLte340McscScrameble 	= (pau8Data[5] & BIT3_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->bSupportRRCapable 				= (pau8Data[5] & BIT6_MASK) ? HI_TRUE : HI_FALSE ;
#endif
	pstCap->bSupportSCDC					= (pau8Data[5] & BIT7_MASK) ? HI_TRUE : HI_FALSE ;

	EDID_INFO("    %-20s : %s\n", "SCDC_Present", pstCap->bSupportSCDC ? "Yes" : "No");
	EDID_INFO("    %-20s : %s\n", "RR_Capable", pstCap->bSupportRRCapable ? "Yes" : "No");
	EDID_INFO("    %-20s : %s\n", "LTE_340Mcsc_scramble", pstCap->bSupportLte340McscScrameble ? "Yes" : "No");
	EDID_INFO("    %-20s : %s\n", "Independent_view", pstCap->bSupport3dIndependentView ? "Yes" : "No");
	EDID_INFO("    %-20s : %s\n", "Dual_View", pstCap->bSupport3dDualView ? "Yes" : "No");
	EDID_INFO("    %-20s : %s\n", "3D_OSD_Disparity", pstCap->bSupport3dOsdDisparity ? "Yes" : "No");
	/* Byte-6 */
	//EDID_INFO("HFVSDB [byte-6]:0x%02x\n",pau8Data[6]);
	pstCap->stDeepColorY420.bDeepColor30Bit = (pau8Data[6] & BIT0_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->stDeepColorY420.bDeepColor36Bit = (pau8Data[6] & BIT1_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->stDeepColorY420.bDeepColor48Bit = (pau8Data[6] & BIT2_MASK) ? HI_TRUE : HI_FALSE ;

	EDID_INFO("    %-20s : %s\n", "DC_48bit_420", pstCap->stDeepColorY420.bDeepColor48Bit ? "Yes" : "No");
	EDID_INFO("    %-20s : %s\n", "DC_36bit_420", pstCap->stDeepColorY420.bDeepColor36Bit ? "Yes" : "No");
	EDID_INFO("    %-20s : %s\n", "DC_30bit_420", pstCap->stDeepColorY420.bDeepColor30Bit ? "Yes" : "No");
	return HI_SUCCESS;
}

static HI_U32 u32Common4kVIC[] =
{
    0,
    HDMI_3840X2160P30_16_9,
    HDMI_3840X2160P25_16_9,
    HDMI_3840X2160P24_16_9,
    HDMI_4096X2160P24_256_135,
};


static HI_VOID EdidVicPrase(HDMI_SINK_CAPABILITY_S *pstCap, HDMI_EDID_STATUS_S *pstStatus,
                            HI_U8 u8HdmiVicLen, HI_U32 u8Len, HI_U8 *pu8Offset, HI_U8 *pau8Data)
{
    HI_U32 i = 0;
    HI_U8 u8Byte = 0;

    if ((u8HdmiVicLen > 0) && (u8Len >= (u8HdmiVicLen + *pu8Offset)))
    {
        for(i = 0; i < u8HdmiVicLen; i++)
        {
            u8Byte = pau8Data[(*pu8Offset)++];
			//EDID_INFO("VSDB [u8Byte-%d]:0x%02x\n",u8Offset-1,u8Byte);

            if (pstCap->u32SupportVICNum < HDMI_EDID_MAX_VIC_COUNT)
            {
                if (u8Byte < sizeof(u32Common4kVIC)/sizeof(u32Common4kVIC[0]))
                {
                    pstCap->au32SupportFormat[pstCap->u32SupportVICNum++] = u32Common4kVIC[u8Byte];
                }
            }
            else
            {
                pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_VIC_CNT_OVER;
                //EDID_WARN("VSDB VICNum over :%d\n",HDMI_EDID_MAX_VIC_COUNT);
                break;
            }

            if (u8Byte < sizeof(u32Common4kVIC)/sizeof(u32Common4kVIC[0]) &&
                (u32Common4kVIC[u8Byte] >= HDMI_3840X2160P24_16_9))
            {
                EDID_INFO("    %-20s : %s\n", "HDMI_VIC",u8VideoCode4KStr[u32Common4kVIC[u8Byte] - HDMI_3840X2160P24_16_9]);
            }
        }
    }

    return ;
}

static HI_VOID Edid3DParse(HDMI_SINK_CAPABILITY_S *pstCap, HDMI_EDID_STATUS_S *pstStatus,
                            HI_U8 *pau8Data, HI_U8 *pu8Offset)
{
    HI_U8 u8Byte = 0;

    /* 3d structure_All_15...8 reseved */
    u8Byte = pau8Data[(*pu8Offset)++] & 0xFF;
    //EDID_INFO("    VSDB 3D_Structure_ALL_15бн8[u8Byte-%d]:0x%02x\n",u8Offset-1,u8Byte);

    pstCap->st3DInfo.bSupport3DType[HDMI_EDID_3D_SIDE_BY_SIDE_HALF] = (u8Byte & BIT0_MASK) ? HI_TRUE : HI_FALSE;

    u8Byte = pau8Data[(*pu8Offset)++] & 0xFF;
    //EDID_INFO("    VSDB 3D_Structure_ALL_7бн0[u8Byte-%d]:0x%02x\n",u8Offset-1,u8Byte);

    pstCap->st3DInfo.bSupport3DType[HDMI_EDID_3D_FRAME_PACKETING]	= (u8Byte & BIT0_MASK) ? HI_TRUE : HI_FALSE;
    pstCap->st3DInfo.bSupport3DType[HDMI_EDID_3D_FIELD_ALTERNATIVE]	= (u8Byte & BIT1_MASK) ? HI_TRUE : HI_FALSE;
    pstCap->st3DInfo.bSupport3DType[HDMI_EDID_3D_LINE_ALTERNATIVE] 	= (u8Byte & BIT2_MASK) ? HI_TRUE : HI_FALSE;
    pstCap->st3DInfo.bSupport3DType[HDMI_EDID_3D_SIDE_BY_SIDE_FULL]	= (u8Byte & BIT3_MASK) ? HI_TRUE : HI_FALSE;
    pstCap->st3DInfo.bSupport3DType[HDMI_EDID_3D_L_DEPTH] 			= (u8Byte & BIT4_MASK) ? HI_TRUE : HI_FALSE;
    pstCap->st3DInfo.bSupport3DType[HDMI_EDID_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH] \
    																= (u8Byte & BIT5_MASK) ? HI_TRUE : HI_FALSE;
    pstCap->st3DInfo.bSupport3DType[HDMI_EDID_3D_TOP_AND_BOTTOM] 	= (u8Byte & BIT6_MASK) ? HI_TRUE : HI_FALSE;

    return;
}


static HI_S32 EdidVSDB(HDMI_SINK_CAPABILITY_S *pstCap,
								HDMI_EDID_STATUS_S *pstStatus,
								HI_U8 *pau8Data,
								HI_U8 u8Len )
{
    HI_U8 u8Byte = 0;
    HI_U8 u8Offset = 0;
    HI_U8 u8HdmiVicLen = 0;
    HI_U8 u8Hdmi3DLen = 0;
    HI_BOOL b3DPresent = 0;
    HI_U8 u83DMultiPresent = 0;
    //HI_U8 u83DStructure = 0;
    HI_BOOL b3DPrase = HI_FALSE;

    EDID_NULL_CHK(pstCap);
    EDID_NULL_CHK(pau8Data);
    EDID_NULL_CHK(pstStatus);

    /*Byte-0~2, IEEE have been parsed outside the func */
    EDID_INFO("    HDMI-LLC Vendor Specific Data Block...\n");
    /*Byte-3~4 ,CEC Addr */
    if( u8Len < 5 )
    {
        EDID_WARN("VSDB len:%d\n",u8Len);
    	pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_BLOCKLEN_INVALID;
        return HI_SUCCESS;
    }

    pstCap->stCECAddr.u8PhyAddrA = (pau8Data[3] & EDID_UPPER_NIBBLE_MASK) >> 4 ;
    pstCap->stCECAddr.u8PhyAddrB = (pau8Data[3] & EDID_LOWER_NIBBLE_MASK) ;
    pstCap->stCECAddr.u8PhyAddrC = (pau8Data[4] & EDID_UPPER_NIBBLE_MASK) >> 4 ;
    pstCap->stCECAddr.u8PhyAddrD = (pau8Data[4] & EDID_LOWER_NIBBLE_MASK) ;

    pstCap->stCECAddr.bPhyAddrValid = (   (pstCap->stCECAddr.u8PhyAddrA != EDID_CEC_INVALID_ADDR )
                                       && (pstCap->stCECAddr.u8PhyAddrB != EDID_CEC_INVALID_ADDR )
                                       && (pstCap->stCECAddr.u8PhyAddrC != EDID_CEC_INVALID_ADDR )
                                       && (pstCap->stCECAddr.u8PhyAddrD != EDID_CEC_INVALID_ADDR ));

	EDID_INFO("    %-20s : %02d.%02d.%02d.%02d\n", "CEC Address",pstCap->stCECAddr.u8PhyAddrA, pstCap->stCECAddr.u8PhyAddrB, pstCap->stCECAddr.u8PhyAddrC, pstCap->stCECAddr.u8PhyAddrD);

	/* Byte-5 */
    if(u8Len < 6)
    {
    	EDID_INFO("VSDB len:%d\n",u8Len);
        return HI_SUCCESS;
    }
	u8Byte = pau8Data[5];
	//EDID_INFO("VSDB [byte-5]:0x%02x\n",u8Byte);
	EDID_INFO("    %-20s : 0x%02X\n", "Color Depth flags", u8Byte);

    pstCap->bSupportDVIDual    			= (u8Byte & BIT0_MASK) ? HI_TRUE : HI_FALSE ;
    pstCap->stDeepColor.bDeepColorY444  = (u8Byte & BIT3_MASK) ? HI_TRUE : HI_FALSE ;
    pstCap->stDeepColor.bDeepColor30Bit = (u8Byte & BIT4_MASK) ? HI_TRUE : HI_FALSE ;
    pstCap->stDeepColor.bDeepColor36Bit = (u8Byte & BIT5_MASK) ? HI_TRUE : HI_FALSE ;
    pstCap->stDeepColor.bDeepColor48Bit = (u8Byte & BIT6_MASK) ? HI_TRUE : HI_FALSE ;
    pstCap->bSupportsAI 				= (u8Byte & BIT7_MASK) ? HI_TRUE : HI_FALSE ;

	/* Byte-6 */
    if(u8Len < 7)
    {
    	EDID_INFO("VSDB len:%d\n",u8Len);
        return HI_SUCCESS;
    }

	pstCap->u32MaxTMDSClock = pau8Data[6];
	//EDID_INFO("VSDB [byte-6]:0x%02x\n",pstCap->u32MaxTMDSClock);
	pstCap->u32MaxTMDSClock *= EDID_TMDS_FACTOR;
	pstCap->bSupportHdmi_2_0 = ( pstCap->u32MaxTMDSClock >= EDID_MAX_HDMI14_TMDS_RATE ) ? HI_TRUE : HI_FALSE ;
	//EDID_INFO("VSDB Max TMDS Colock:%d\n", pstCap->u32MaxTMDSClock);
    EDID_INFO("    %-20s : %d MHz\n", "Max TMDS Clock",pstCap->u32MaxTMDSClock);
	/* Byte-7 */
    if(u8Len < 8)
    {
    	EDID_INFO("VSDB len:%d\n",u8Len);
        return HI_SUCCESS;
    }
/*
    // NOTE:HDMI_Video_present is used as 3d support tag now.
    		we don't use 3D_present to judge 3dsupport
    pstCap->st3DInfo.bSupport3D = (pau8Data[7] & BIT5_MASK) ? HI_TRUE : HI_FALSE ;
*/
	u8Byte = pau8Data[7];
	//EDID_INFO("VSDB [byte-7]:0x%02x\n",u8Byte);

	pstCap->bHDMIVideoPresent 		= (u8Byte & BIT5_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->bILatencyFieldsPresent	= (u8Byte & BIT6_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->bLatencyFieldsPresent	= (u8Byte & BIT7_MASK) ? HI_TRUE : HI_FALSE ;
	//EDID_INFO("VSDB HDMI_Video_present:%d\n",pstCap->bHDMIVideoPresent);
	//EDID_INFO("VSDB bI_Latency_Fields_Present:%d\n",pstCap->bILatencyFieldsPresent);
	//EDID_INFO("VSDB bLatency_Fields_Present:%d\n",pstCap->bLatencyFieldsPresent);
    //EDID_INFO("VSDB CNC:0x%02x\n",(u8Byte & EDID_LOWER_NIBBLE_MASK));

	/* Byte8~11,A&V Latecy */

	if(u8Len < 9)
	{
		EDID_INFO("VSDB len:%d\n",u8Len);
		return HI_SUCCESS;
	}

    u8Offset = 8;
    if (pstCap->bLatencyFieldsPresent)
    {
    	pstCap->u8VideoLatency 			 = pau8Data[u8Offset];
        u8Offset++;
    	pstCap->u8AudioLatency 			 = pau8Data[u8Offset];
        u8Offset++;
        EDID_INFO("    %-20s : 0x%02X\n", "Video Latency", pstCap->u8VideoLatency);
        EDID_INFO("    %-20s : 0x%02X\n", "Audio Latency", pstCap->u8AudioLatency );
    }

    if (pstCap->bILatencyFieldsPresent)
    {
    	pstCap->u8InterlacedVideoLatency = pau8Data[u8Offset];
        u8Offset++;
    	pstCap->u8InterlacedAudioLatency = pau8Data[u8Offset];
        u8Offset++;
        EDID_INFO("    %-20s : 0x%02X\n", "Interlaced Video Latency", pstCap->u8InterlacedVideoLatency);
        EDID_INFO("    %-20s : 0x%02X\n", "Interlaced Audio Latency", pstCap->u8InterlacedAudioLatency);
    }

    //EDID_INFO("VSDB u8Video_Latency[u8Byte-8]:%d(0x%02x)\n",pstCap->u8VideoLatency,pstCap->u8VideoLatency);
    //EDID_INFO("VSDB u8Audio_Latency[u8Byte-9]:%d(0x%02x)\n",pstCap->u8AudioLatency,pstCap->u8AudioLatency);
    //EDID_INFO("VSDB Interlaced_Video_Latency[u8Byte-10]:%d(0x%02x)\n",pstCap->u8InterlacedVideoLatency,pstCap->u8InterlacedVideoLatency);
	//EDID_INFO("VSDB Interlaced_Audio_Latency[u8Byte-11]:%d(0x%02x)\n",pstCap->u8InterlacedAudioLatency,pstCap->u8InterlacedAudioLatency);


	u8Byte = pau8Data[u8Offset];
    u8Offset++;
	//EDID_INFO("VSDB [u8Byte-12]:0x%02x\n",u8Byte);
	if (pstCap->bHDMIVideoPresent)
	{
	    //EDID_INFO("VSDB u8ImagSize:%d \n",((u8Byte & (BIT4_MASK | BIT3_MASK) ) >> 3));
	    u83DMultiPresent = (u8Byte & (BIT6_MASK | BIT5_MASK)) >> 5;
		b3DPresent = (u8Byte & BIT7_MASK) ? HI_TRUE : HI_FALSE ;
		pstCap->st3DInfo.bSupport3D = (u8Byte & BIT7_MASK) ? HI_TRUE : HI_FALSE ;
	    //EDID_INFO("VSDB 3D present:%d \n",b3DPresent);
		//EDID_INFO("VSDB u83DMultiPresent:%d \n",u83DMultiPresent);
		EDID_INFO("    %-20s : %s\n", "3D-Support", b3DPresent ? "yes" : "no");
        EDID_INFO("    %-20s : %02d\n", "3D_Multi_present", u83DMultiPresent);
        EDID_INFO("    %-20s : %02d\n", "Image_Size", (u8Byte & (BIT4_MASK | BIT3_MASK))>>3);
	}

	if(u8Len < 10)
    {
    	EDID_INFO("VSDB len:%d\n",u8Len);
        return HI_SUCCESS;
    }

 	/* Byte-13 */
	u8Byte = pau8Data[u8Offset];
	//EDID_INFO("VSDB [u8Byte-13]:0x%02x\n",u8Byte);
    u8Hdmi3DLen = (u8Byte & EDID_HDMI_3D_LEN_MASK );
    u8HdmiVicLen = (u8Byte & EDID_HDMI_VIC_LEN_MASK ) >> 5;
	//EDID_INFO("VSDB u8Hdmi3DLen:%d \n",u8Hdmi3DLen);
	//EDID_INFO("VSDB u8HdmiVicLen:%d \n",u8HdmiVicLen);
	EDID_INFO("    %-20s : %02d\n", "HDMI_VIC_LEN", u8HdmiVicLen);
	EDID_INFO("    %-20s : %02d\n", "HDMI_3D_LEN", u8Hdmi3DLen);

	/* Byte-14 & following,VIC*/
	u8Offset++;

    EdidVicPrase(pstCap, pstStatus, u8HdmiVicLen, u8Len, &u8Offset, pau8Data);

	/* Byte-following, 3D */
    b3DPrase = (   (HI_TRUE == b3DPresent)
                && (u8Hdmi3DLen > 0)
                && (u8Len >= (u8Hdmi3DLen + u8Offset))
                && ((u83DMultiPresent == EDID_3DMULTI_PRESENT_LOWER8)
                || (u83DMultiPresent == EDID_3DMULTI_PRESENT_UPPER8)));

    if(HI_TRUE == b3DPrase)
    {
        Edid3DParse(pstCap, pstStatus, pau8Data, &u8Offset);
    }

    return HI_SUCCESS;
}

static HI_S32 EdidSpeakerDB(HDMI_SINK_CAPABILITY_S *pstCap,
									HDMI_EDID_STATUS_S *pstStatus,
									HI_U8 *pau8Data,
									HI_U8 u8Len)
{
	HI_U8 i = 0;
    HI_U8 u8Byte = 0;

	EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstStatus);

	if (u8Len < 2)
	{
		pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_BLOCKLEN_INVALID;
		pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_SPKDB_INVALID;
		EDID_WARN("SPKDB u8Len:%d\n",u8Len);
		return HI_SUCCESS;
	}
    EDID_INFO("    Speaker Alloc Data Block (Length=%02d)...\n", u8Len);
	/* Byte-0 */
	u8Byte = pau8Data[0];
    //EDID_INFO("SPKDB Byte-0:0x%02x\n",u8Byte);

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_FL_FR]   = (u8Byte & BIT0_MASK) ? HI_TRUE : HI_FALSE ;

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_LFE] 	  = (u8Byte & BIT1_MASK) ? HI_TRUE : HI_FALSE ;

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_FC] 	  = (u8Byte & BIT2_MASK) ? HI_TRUE : HI_FALSE ;

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_RL_RR]   = (u8Byte & BIT3_MASK) ? HI_TRUE : HI_FALSE ;

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_RC] 	  = (u8Byte & BIT4_MASK) ? HI_TRUE : HI_FALSE ;

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_FLC_FRC] = (u8Byte & BIT5_MASK) ? HI_TRUE : HI_FALSE ;

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_RLC_RRC] = (u8Byte & BIT6_MASK) ? HI_TRUE : HI_FALSE ;

	/* NOTE: the following is CEA-861-F add for HDMI2.0 */
	pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_FLW_FRW] = (u8Byte & BIT7_MASK) ? HI_TRUE : HI_FALSE ;


	/* Byte-1 */
    u8Byte = pau8Data[1];
    //EDID_INFO("SPKDB Byte-1:0x%02x\n",u8Byte);

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_FLH_FRH] = (u8Byte & BIT0_MASK) ? HI_TRUE : HI_FALSE ;

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_TC] 	  = (u8Byte & BIT1_MASK) ? HI_TRUE : HI_FALSE ;

    pstCap->bSupportAudioSpeaker[HDMI_EDID_AUDIO_SPEAKER_FCH] 	  = (u8Byte & BIT2_MASK) ? HI_TRUE : HI_FALSE ;

    for(i = 0; i <= HDMI_EDID_AUDIO_SPEAKER_FCH; i++)
    {
        if(pstCap->bSupportAudioSpeaker[i])
        {
        	//EDID_INFO("    SPKDB bSupportAudioSpeaker[%d]:HI_TRUE\n",i);
        }
    }

    /* Byte-2 , reversed */
    //EDID_INFO("SPKDB Byte-2:0x%02x\n",pau8Data[2]);

    return HI_SUCCESS;
}

static HI_S32 EdidColormetryDB(HDMI_SINK_CAPABILITY_S *pstCap,
										HDMI_EDID_STATUS_S *pstStatus,
										HI_U8 *pau8Data,
										HI_U8 u8Len)
{
	HI_U8 i = 0;
	HI_U8 u8Byte = 0;
	HI_BOOL *pBool = HI_NULL;

	EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstStatus);

	/*1 ext tag code + 2 bytes payload*/
	if (u8Len < 3)
	{
		pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_BLOCKLEN_INVALID;
		EDID_ERR("ColormetryDB Len!\n");
		return HI_SUCCESS;
	}

	/* Byte-0 */
	u8Byte = pau8Data[0];
	//EDID_INFO("    ColormetryDB u8Byte0:0x%02x\n",u8Byte);
	pstCap->stColorMetry.bxvYCC601 		= (u8Byte & BIT0_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->stColorMetry.bxvYCC709 		= (u8Byte & BIT1_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->stColorMetry.bsYCC601 		= (u8Byte & BIT2_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->stColorMetry.bAdobleYCC601 	= (u8Byte & BIT3_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->stColorMetry.bAdobleRGB 	= (u8Byte & BIT4_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->stColorMetry.bBT2020cYCC 	= (u8Byte & BIT5_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->stColorMetry.bBT2020YCC 	= (u8Byte & BIT6_MASK) ? HI_TRUE : HI_FALSE ;
	pstCap->stColorMetry.bBT2020RGB 	= (u8Byte & BIT7_MASK) ? HI_TRUE : HI_FALSE ;

	for(i = 0,pBool = (HI_BOOL *)&pstCap->stColorMetry;
		i<8 && pBool;
		i++,pBool++)
	{
		//EDID_INFO("    ColormetryDB stColorMetry[%d]:0x%02x\n",i,*pBool);
	}

	/* Byte-1 */
	pstCap->u8MDBit = pau8Data[1];
	//EDID_INFO("    ColormetryDB u8Byte1:0x%02x\n",pstCap->u8MDBit);
	//EDID_INFO("    ColormetryDB gamut-related metadata[BIT0~3]:0x%x!\n",(pstCap->u8MDBit & EDID_LOWER_NIBBLE_MASK));

	return HI_SUCCESS;

}

static HI_S32 EdidY420VideoDB(HDMI_SINK_CAPABILITY_S *pstCap,
										HDMI_EDID_STATUS_S *pstStatus,
										HI_U8 *pau8Data,
										HI_U8 u8Len)
{
	HI_U8			i = 0;
	HI_U32			u32Vic = 0;

	EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstStatus);

	for(i = 0; i < u8Len-1; i++)
    {
        u32Vic = pau8Data[i] ;

		if (pstCap->u32OnlySupportY420VICNum >= HDMI_EDID_MAX_VIC_COUNT)
		{
			pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_Y420VDB_VIC_OVER;
			EDID_WARN("Y420VideoDB vic count over:%d !\n",HDMI_EDID_MAX_VIC_COUNT);
			break;
		}

		if (( u32Vic == EDID_VIC_INVALID_ZERO) ||
            ((u32Vic >= EDID_VIC_INVALID_LOW) && (u32Vic <= EDID_VIC_INVALID_HIGH)))
		{
			pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_VIC_INVALID;
			EDID_WARN("Y420VideoDB the %d is invalid Vic: %d ! \n",i,u32Vic);
			continue;
		}

		pstCap->au32OnlySupportY420Format[pstCap->u32OnlySupportY420VICNum++] = u32Vic ;
		pstCap->stColorSpace.bYCbCr420 = HI_TRUE;
		//EDID_INFO("Y420VideoDB u32Vic:%u(0x%02x)\n",u32Vic,u32Vic);
		if (u32Vic < HDMI_VIDEO_CODE_BUTT)
		{
            EDID_INFO("    %-20s : %d-%s\n", "Short Video Descriptor Only Support YUV420", u32Vic, (u32Vic >= HDMI_3840X2160P24_16_9) ? u8VideoCode4KStr[u32Vic-HDMI_3840X2160P24_16_9] : u8VideoCodeStr[u32Vic]);
        }

    }

	return HI_SUCCESS;
}

static HI_S32 EdidY420CapMapDB(HDMI_SINK_CAPABILITY_S *pstCap,
											HI_U8 *pau8Data,
											HI_U8 u8Len)
{
	HI_U32			i = 0;
	HI_U32			u32Loop = 0;

	EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);

	/*pstCap->au32SupportY420Format  is  SVDs parsed from Video Data Block(s) in order.
		When the Length field is  1, indicates that all the SVDs  support YUV420 .  */
	if (1 == u8Len)
	{
		for (i = 0 ; i < pstCap->u32SupportVICNum ; i++)
		{
			if (pstCap->au32SupportFormat[i] <= HDMI_EDID_MAX_REAL_VIC)
			{
				pstCap->au32SupportY420Format[pstCap->u32SupportY420VICNum++] \
											= pstCap->au32SupportFormat[i];
    			pstCap->stColorSpace.bYCbCr420 = HI_TRUE;
                EDID_INFO("    %-20s : %d-%s\n", "Short Video Descriptor Support YUV420", pstCap->au32SupportFormat[i], (pstCap->au32SupportFormat[i] >= HDMI_3840X2160P24_16_9) ? u8VideoCode4KStr[pstCap->au32SupportFormat[i]-HDMI_3840X2160P24_16_9] : u8VideoCodeStr[pstCap->au32SupportFormat[i]]);

			}
		}

	}
	/* Or,for SVDs which support YUV420, the corresponding bit  shall be set to 1 here. */
	else
	{
		u32Loop = u8Len*8;
		for(i = 0; (i < u32Loop) && (i < pstCap->u32SupportVICNum); i++)
	    {
	    	/* BYTE n(i/8),BIT x(i%8) */
	    	if (pau8Data[i/8] & (0x01<<(i%8)) )
	    	{
				pstCap->au32SupportY420Format[pstCap->u32SupportY420VICNum++] \
												= pstCap->au32SupportFormat[i] ;
                pstCap->stColorSpace.bYCbCr420 = HI_TRUE;
                EDID_INFO("    %-20s : %d-%s\n", "Short Video Descriptor Support YUV420", pstCap->au32SupportFormat[i], (pstCap->au32SupportFormat[i] >= HDMI_3840X2160P24_16_9) ? u8VideoCode4KStr[pstCap->au32SupportFormat[i]-HDMI_3840X2160P24_16_9] : u8VideoCodeStr[pstCap->au32SupportFormat[i]]);

            }
		}
	}

	return HI_SUCCESS;
}

#ifdef HDMI_HDR_SUPPORT

static HI_U32 EdidDolbyVSVDB(HDMI_SINK_CAPABILITY_S *pstCap,
                                 HDMI_EDID_STATUS_S *pstStatus,
											  HI_U8 *pau8Data,
											   HI_U8 u8Len)
{
    HDMI_EDID_DOLBY_CAP_S *pstDolbyCap;
    HI_U32 u32DolbyIEEEOUI = 0;

    EDID_NULL_CHK(pstCap);
    EDID_NULL_CHK(pau8Data);

    if ((u8Len != EDID_DOLBY_VSVDB_VERSION_0_LEN) && (u8Len != EDID_DOLBY_VSVDB_VERSION_1_LEN))
    {
        pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_BLOCKLEN_INVALID;
        EDID_WARN("Dolby VSVDB u8Len:%d\n",u8Len);
        return HI_SUCCESS;
    }

    pstDolbyCap = &pstCap->stDolbyCap;
    EDID_NULL_CHK(pstDolbyCap);

    u32DolbyIEEEOUI = pau8Data[0] | (pau8Data[1] << 8) | (pau8Data[2] << 16);
    if(u32DolbyIEEEOUI != DOLBY_IEEE_OUI)
    {
        return HI_FAILURE;
    }

    pstDolbyCap->u32DolbyOui    = u32DolbyIEEEOUI;
    pstDolbyCap->bYUV422_12Bit  = (pau8Data[3] & BIT0_MASK) ? HI_TRUE : HI_FALSE;
    pstDolbyCap->b2160P60       = (pau8Data[3] & BIT1_MASK) ? HI_TRUE : HI_FALSE;
    pstDolbyCap->u8VSVDBVersion = (pau8Data[3] >> 5);
    switch(pstDolbyCap->u8VSVDBVersion)
    {
        case EDID_DOLBY_VSVDB_VERSION_0:
            pstDolbyCap->bGlobalDimming   = (pau8Data[3] & BIT2_MASK) ? HI_TRUE : HI_FALSE;
            pstDolbyCap->u16Red_X         = (pau8Data[5] << 4) | ((pau8Data[4] >> 4) & EDID_DOLBY_LOWER_NIBBLE_MASK);
            pstDolbyCap->u16Red_Y         = (pau8Data[6] << 4) | ((pau8Data[4]) & EDID_DOLBY_LOWER_NIBBLE_MASK);
            pstDolbyCap->u16Green_X       = (pau8Data[8] << 4) | ((pau8Data[7] >> 4) & EDID_DOLBY_LOWER_NIBBLE_MASK);
            pstDolbyCap->u16Green_Y       = (pau8Data[9] << 4) | (pau8Data[7] & EDID_DOLBY_LOWER_NIBBLE_MASK);
            pstDolbyCap->u16Blue_X        = (pau8Data[11] << 4) | ((pau8Data[10] >> 4) & EDID_DOLBY_LOWER_NIBBLE_MASK);
            pstDolbyCap->u16Blue_Y        = (pau8Data[12] << 4) | (pau8Data[10] & EDID_DOLBY_LOWER_NIBBLE_MASK);


            pstDolbyCap->unOtherCaps.stDolbyCapsVer0.u16White_X = (pau8Data[14] << 4) | ((pau8Data[13] >> 4) & EDID_DOLBY_LOWER_NIBBLE_MASK);
            pstDolbyCap->unOtherCaps.stDolbyCapsVer0.u16White_Y = (pau8Data[15] << 4) | (pau8Data[13] & EDID_DOLBY_LOWER_NIBBLE_MASK);
            pstDolbyCap->u16MinLuminance  = (pau8Data[17] << 4) | ((pau8Data[16] >> 4) & EDID_DOLBY_LOWER_NIBBLE_MASK);
            pstDolbyCap->u16MaxLuminance  = (pau8Data[18] << 4) | (pau8Data[16] & EDID_DOLBY_LOWER_NIBBLE_MASK);

            pstDolbyCap->unOtherCaps.stDolbyCapsVer0.u8DMminorVersion = (pau8Data[19] & EDID_DOLBY_LOWER_NIBBLE_MASK);
            pstDolbyCap->unOtherCaps.stDolbyCapsVer0.u8DMmajorVersion = ((pau8Data[19] >> 4) & EDID_DOLBY_LOWER_NIBBLE_MASK);
            break;
        case EDID_DOLBY_VSVDB_VERSION_1:
            pstDolbyCap->unOtherCaps.stDolbyCapsVer1.u8DMVersion = ((pau8Data[3] >> 2) & EDID_DOLBY_LOWER_3BIT_MASK);
            pstDolbyCap->bGlobalDimming  = (pau8Data[4] & BIT0_MASK);
            pstDolbyCap->u16MaxLuminance = ((pau8Data[4] >> 1) & EDID_DOLBY_LOWER_7BIT_MASK);
            pstDolbyCap->unOtherCaps.stDolbyCapsVer1.bColorimetry = (pau8Data[5] & BIT0_MASK);
            pstDolbyCap->u16MinLuminance = ((pau8Data[5] >> 1) & EDID_DOLBY_LOWER_7BIT_MASK);
            pstDolbyCap->u16Red_X        = pau8Data[7];
            pstDolbyCap->u16Red_Y        = pau8Data[8];
            pstDolbyCap->u16Green_X      = pau8Data[9];
            pstDolbyCap->u16Green_Y      = pau8Data[10];
            pstDolbyCap->u16Blue_X       = pau8Data[11];
            pstDolbyCap->u16Blue_Y       = pau8Data[12];
            break;
        default:
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 EdidHdrMetaDB(HDMI_SINK_CAPABILITY_S *pstCap,
                                HDMI_EDID_STATUS_S *pstStatus,
											 HI_U8 *pau8Data,
											  HI_U8 u8Len)
{
    HDMI_EDID_HDR_CAP_S *pstHdrCap;

    EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);

    pstHdrCap = &pstCap->stHdrCap;
    EDID_NULL_CHK(pstHdrCap);

// BYTE 3
    pstHdrCap->stEotf.bEotfSdr            = BIT_CHECK(pau8Data[0], BIT0_MASK) ? HI_TRUE : HI_FALSE; // ET_0
    pstHdrCap->stEotf.bEotfHdr            = BIT_CHECK(pau8Data[0], BIT1_MASK) ? HI_TRUE : HI_FALSE; // ET_1
    pstHdrCap->stEotf.bEotfSmpteSt2084    = BIT_CHECK(pau8Data[0], BIT2_MASK) ? HI_TRUE : HI_FALSE; // ET_2
    pstHdrCap->stEotf.bEotfFuture         = BIT_CHECK(pau8Data[0], BIT3_MASK) ? HI_TRUE : HI_FALSE; // ET_3
// BYTE 4
    pstHdrCap->stMetadata.bDescriptorType1 = BIT_CHECK(pau8Data[1], BIT0_MASK) ? HI_TRUE : HI_FALSE; // Supported Static Metadata Descriptor
// BYTE 5-7
    pstHdrCap->u8MaxLuminance_CV = (u8Len >= 4) ? (pau8Data[2]) : 0;
    pstHdrCap->u8MinLuminance_CV = (u8Len >= 5) ? (pau8Data[3]) : 0;
    pstHdrCap->u8AverageLumin_CV = (u8Len >= 6) ? (pau8Data[4]) : 0;

    return HI_SUCCESS;

}

static HI_U32 EdidDolbyVideoCapDB(HDMI_SINK_CAPABILITY_S *pstCap,
											       HI_U8 *pau8Data,
											        HI_U8 u8Len)
{
    HDMI_EDID_DOLBY_CAP_S *pDolbyCap;

    EDID_NULL_CHK(pstCap);
    EDID_NULL_CHK(pau8Data);

    pDolbyCap = &pstCap->stDolbyCap;
    EDID_NULL_CHK(pDolbyCap);

    pDolbyCap->bYccQrangeSelectable = BIT_CHECK(pau8Data[0], BIT7_MASK) ? HI_TRUE : HI_FALSE;
    pDolbyCap->bRgbQrangeSelectable = BIT_CHECK(pau8Data[0], BIT6_MASK) ? HI_TRUE : HI_FALSE;

    return HI_SUCCESS;
}


#endif

static HI_S32 EdidExtDataBlockParse(HDMI_SINK_CAPABILITY_S *pstCap,
														HDMI_EDID_STATUS_S *pstStatus,
														HI_U8 *pau8Data,
														HI_U8 u8Len)
{
	HI_S32	u32Ret = HI_SUCCESS;
	HI_U8	u8ExtTagCode = 0;

	EDID_NULL_CHK(pstCap);
	EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstStatus);

	u8ExtTagCode = pau8Data[0];
	pau8Data++;

	//EDID_INFO("EXT-BLK u8ExtTagCode=%d(0x%02x)\n",u8ExtTagCode,u8ExtTagCode);

    switch(u8ExtTagCode)
    {
#ifdef HDMI_HDR_SUPPORT
        case EDID_DOLBY_VENDOR_SPECIFIC_VIDEO_DB:
                u32Ret = EdidDolbyVSVDB(pstCap, pstStatus, pau8Data, u8Len);
                EDID_SUCCESS_CHK(u32Ret);
                break;
#endif
        case 	EDID_COLORIMETRY_DB:
                u32Ret = EdidColormetryDB(pstCap,pstStatus,pau8Data,u8Len);
                EDID_SUCCESS_CHK(u32Ret);
                break;

        case	EDID_Y420_VIDEO_DB :
                u32Ret = EdidY420VideoDB(pstCap,pstStatus,pau8Data,u8Len);
                EDID_SUCCESS_CHK(u32Ret);
                break;

        case	EDID_Y420_CAPABILITY_MAP_DB :
                u32Ret = EdidY420CapMapDB(pstCap,pau8Data,u8Len);
                EDID_SUCCESS_CHK(u32Ret);
                break;
#ifdef HDMI_HDR_SUPPORT
        case    EDID_HDR_STATIC_METADATA_DB:
                u32Ret = EdidHdrMetaDB(pstCap, pstStatus, pau8Data, u8Len);
                EDID_SUCCESS_CHK(u32Ret);
                break;
        case	EDID_VIDEO_CAPABILITY_DB :
                u32Ret = EdidDolbyVideoCapDB(pstCap, pau8Data, u8Len);
                EDID_SUCCESS_CHK(u32Ret);
                break;
#endif
        case	EDID_VIDEO_FMT_PREFER_DB:
        case	EDID_RESERVED_HDMI_AUDIO_DB:
        case 	EDID_INFOFRAME_DB:
        default:
                EDID_WARN("EXT-BLK un-parse Ext data block!Ext-Tag Code:%d!\n",u8ExtTagCode);
                break;

    }
    return HI_SUCCESS;
}


static HI_S32 EdidVendorDataBlockParse(HDMI_SINK_CAPABILITY_S *pstCap, HDMI_EDID_STATUS_S *pstStatus,
                                    HI_U8 *pau8Data, HI_U8 u8BlkLen)
{
    HI_S32   s32Ret         = HI_SUCCESS;
    HI_BOOL  bIsVSDB        = HI_FALSE;
    HI_BOOL  bIsHFVSDB      = HI_FALSE;

    EDID_NULL_CHK(pstCap);
    EDID_NULL_CHK(pstStatus);
    EDID_NULL_CHK(pau8Data);

    bIsVSDB = (  (u8BlkLen >= 3)
              && (pau8Data[0] == EDID_IEEE_VSDB_1ST)
              && (pau8Data[1] == EDID_IEEE_VSDB_2ND)
              && (pau8Data[2] == EDID_IEEE_VSDB_3RD));

    bIsHFVSDB = (   (u8BlkLen >= 3)
                 && (pau8Data[0] == EDID_IEEE_HFVSDB_1ST)
                 && (pau8Data[1] == EDID_IEEE_HFVSDB_2ND)
                 && (pau8Data[2] == EDID_IEEE_HFVSDB_3RD));

    if (HI_TRUE == bIsVSDB)
	{
		/* IEEE  code indicate whether it is HDMI1.4b or HDMI2.0*/
		pstCap->bSupportHdmi = HI_TRUE;
    	s32Ret = EdidVSDB(pstCap, pstStatus, pau8Data, u8BlkLen);
		EDID_SUCCESS_CHK(s32Ret);

	}
	else if (HI_TRUE == bIsHFVSDB)
	{
		s32Ret = EdidHFVSDB(pstCap, pstStatus, pau8Data, u8BlkLen);

		EDID_SUCCESS_CHK(s32Ret);
	}
	else
	{
		if(u8BlkLen >= 3 )
		{
			pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_VSDB_INVALID;
		 	EDID_WARN("(HF)VSDB IEEE:0x%02x, 0x%02x, 0x%02x\n",
			pau8Data[0], pau8Data[1], pau8Data[2]);
		}
		else
		{
			pstStatus->u32ParseWarn |= 1<<EDID_PARSE_WARN_BLOCKLEN_INVALID;
			EDID_WARN("(HF)VSDB  u8BlkLen:%d\n",u8BlkLen);
		}
	}

    return s32Ret;
}

static HI_S32 EdidExtentionBlockParse(HDMI_EDID_INFO_S *pstEdidInfo,
												HI_U8 u8BlkNum)
{
    HI_U8 						u8BlkOffset = 0;
	HI_U8 						u8DTDOffset = 0;
	HI_U8						u8BlkLen = 0;
	HI_U8						u8DBTagCode = 0;
    HI_S32 						s32Ret = HI_SUCCESS;
	HDMI_EDID_STATUS_S			*pstStatus = HI_NULL;
    HI_U8  						*pau8Data = HI_NULL;
    HDMI_SINK_CAPABILITY_S 		*pstCap = HI_NULL;
	HDMI_EDID_COLOR_SPACE_S 	*pstClrSpace= HI_NULL;

	if (u8BlkNum > (HDMI_EDID_TOTAL_BLOCKS-1))
	{
	    EDID_ERR("EXT-BLK = %d\n",u8BlkNum);
        return HI_FAILURE;
	}
	EDID_NULL_CHK(pstEdidInfo);
	pstStatus 	= &pstEdidInfo->stStatus;
	pau8Data 	= pstEdidInfo->au8EdidRaw + (u8BlkNum*HDMI_EDID_BLOCK_SIZE);
	pstCap 		= &pstEdidInfo->stCapability;
	EDID_NULL_CHK(pau8Data);

    s32Ret = EdidBlockChkSum(pau8Data,pstStatus);
	EDID_SUCCESS_CHK(s32Ret);

	EDID_INFO("    Basic Audio SupportCheck for 861B (block %i)...\n", u8BlkNum);

	/* CEA EDID timing extension tag */
	EDID_INFO("    %-20s : %02X\n", "CEA EDID timing extension tag",pau8Data[0]);

	/* Byte-0 :TAG */
    if(pau8Data[0] != EDID_CEA_EXTVERSION3_TAG)
    {
    	pstStatus->enParseErr = EDID_PARSE_ERR_TAG_UNKNOWN;
        EDID_ERR("EXT_BLK CEA TAG:0x%02x\n",pau8Data[0]);
       // return HI_FAILURE;
    }

	/* CEA 861B version */
	EDID_INFO("    %-20s : %02X\n", "CEA 861B version",pau8Data[1]);

	/* Byte-1 :REVISION */
    if (pau8Data[1] != EDID_CEA_EXTVERSION3_REVISION)
    {
    	pstStatus->enParseErr = EDID_PARSE_ERR_CEA_REVERION;
        EDID_ERR("EXT_BLK CEA861 REVISION: 0x%02x\n", pau8Data[1]);
        //return HI_FAILURE;
    }

	/* Byte-2 :DTDs Offset */
    u8DTDOffset = pau8Data[2];
    //EDID_INFO("EXT_BLK u8DTDOffset:%d\n",u8DTDOffset);
    if(u8DTDOffset < 4)
    {
        EDID_INFO("EXT_BLK NO DTDs & reserved Data Blks :d=%d\n",u8DTDOffset);
        return HI_SUCCESS;
    }
	EDID_INFO("    %-20s : %02d\n", "Number of native DTD's", pau8Data[3]&0x0F);
	EDID_INFO("    %-20s : %s\n", "Basic Audio Support", (pau8Data[3]&0x40) ? "Yes" : "No");
    EDID_INFO("    %-20s : RGB444 %s %s\n", "Pixel Format", (pau8Data[3]&0x20) ? "YCbCr444" : "", (pau8Data[3]&0x10) ? "YCbCr422" : "");

    /* Byte-3 :Basic Audio,Color Space,DTDs Cnt */
	pstClrSpace = &pstCap->stColorSpace;
    pstClrSpace->bRGB444   = HI_TRUE;
    pstClrSpace->bYCbCr422 = (pau8Data[3] & BIT4_MASK ? HI_TRUE : HI_FALSE) ;
    pstClrSpace->bYCbCr444 = (pau8Data[3] & BIT5_MASK ? HI_TRUE : HI_FALSE) ;

    //EDID_INFO("EXT_BLK IT Underscan:0x%02x\n", ((pau8Data[3] & BIT7_MASK)>>7));
	//EDID_INFO("EXT_BLK basic audio:0x%02x\n",  (pau8Data[3] & BIT6_MASK)>>6);
	//EDID_INFO("EXT_BLK bYCbCr444:%d\n", pstClrSpace->bYCbCr444);
	//EDID_INFO("EXT_BLK bYCbCr422:%d\n", pstClrSpace->bYCbCr422);
	//EDID_INFO("EXT_BLK native DTDS:0x%02x\n",	pau8Data[3] & EDID_LOWER_NIBBLE_MASK);

	/* sevaral Data Block */
    for (u8BlkOffset = 4 ,pau8Data += u8BlkOffset,u8BlkLen=0 ;
	     (u8BlkOffset < u8DTDOffset) && (pau8Data != HI_NULL);
		 u8BlkOffset += u8BlkLen + 1)
    {
    	pau8Data += u8BlkLen;
        u8BlkLen = (*pau8Data) & EDID_DB_LEN_MASK;
		u8DBTagCode = ( (*pau8Data) & EDID_DB_TAG_CODE_MASK ) >> 5;
		//EDID_INFO("EXT_BLK u8BlkOffset:%d\n",u8BlkOffset);
		//EDID_INFO("EXT_BLK byte_contex:0x%02x\n",*pau8Data);
        //EDID_INFO("EXT_BLK u8DBTagCode:%d\n",u8DBTagCode);
		//EDID_INFO("EXT_BLK u8BlkLen:%d\n",u8BlkLen);

		pau8Data++;
		/* Tag Code */
        switch(u8DBTagCode)
        {
            case	EDID_AUDIO_DATA_BLOCK:
                	s32Ret = EdidAudioDB(pstCap,pstStatus,pau8Data,u8BlkLen);
					EDID_SUCCESS_CHK(s32Ret);
                	break;

            case 	EDID_VIDEO_DATA_BLOCK:
                	s32Ret = EdidVideoDB(pstCap,pstStatus,pau8Data,u8BlkLen);
					EDID_SUCCESS_CHK(s32Ret);
                	break;

            case 	EDID_VENDOR_DATA_BLOCK:
                    s32Ret = EdidVendorDataBlockParse(pstCap, pstStatus, pau8Data, u8BlkLen);
                    EDID_SUCCESS_CHK(s32Ret);
					break;

	        case 	EDID_SPEAKER_DATA_BLOCK:
	                s32Ret = EdidSpeakerDB(pstCap,pstStatus,pau8Data,u8BlkLen);
					EDID_SUCCESS_CHK(s32Ret);
	                break;
            case 	EDID_USE_EXT_DATA_BLOCK:
					/* Ext Tag Code */
					s32Ret = EdidExtDataBlockParse(pstCap,pstStatus,pau8Data,u8BlkLen);
					EDID_SUCCESS_CHK(s32Ret);
	                break;

			case 	EDID_VESA_DTC_DATA_BLOCK:
            default:
	                EDID_WARN("EXT_BLK resvered/un-parse block tag code!\n");
	                break;
        }

    }

	/* DTDs */
	while( ((HDMI_EDID_BLOCK_SIZE-1) - u8BlkOffset) > EDID_DTD_SIZE)
	{

		s32Ret = EdidDetailTiming(pstCap,pstStatus,pau8Data,EDID_DTD_SIZE);
		EDID_SUCCESS_CHK(s32Ret);
		u8BlkOffset += EDID_DTD_SIZE;
		pau8Data += EDID_DTD_SIZE;
	}


    return HI_SUCCESS;

}

static HI_S32 EdidRawParse(HDMI_EDID_INFO_S *pstEdidInfo)
{
	HI_U8					u8BlkNum = 0;
    HI_S32 					s32Ret = HI_SUCCESS;
	HDMI_EDID_STATUS_S		*pstStatus = HI_NULL;
    HI_U8  					*pau8Data = HI_NULL;
    HDMI_SINK_CAPABILITY_S 	*pstCap = HI_NULL;

	EDID_NULL_CHK(pstEdidInfo);
	pstStatus 	= &pstEdidInfo->stStatus;
	pau8Data 	= pstEdidInfo->au8EdidRaw;
	pstCap 		= &pstEdidInfo->stCapability;
	EDID_NULL_CHK(pstStatus);
	EDID_NULL_CHK(pau8Data);
	EDID_NULL_CHK(pstCap);

    s32Ret = EdidFirstBlkParse(pstEdidInfo);
	EDID_SUCCESS_CHK(s32Ret);

    for (u8BlkNum = 1; u8BlkNum <= pstCap->u8ExtBlockNum; u8BlkNum++)
    {
        s32Ret = EdidExtentionBlockParse(pstEdidInfo,u8BlkNum);
        EDID_SUCCESS_CHK(s32Ret);
    }

    return HI_SUCCESS;
}

/********************** public interface ********************************************/
static HI_VOID EdidRawDataPrint(HI_U8 u8EdidRaw[], HI_U32 u32Len)
{
    HI_U32 i = 0;
    HI_U16 u16Offset = 0;
    HI_U8 u8TempBuffer[1000];

    memset(u8TempBuffer, 0, 1000);
    //u8Offset += snprintf(&u8TempBuffer[u8Offset], 1000, "%c", '\n');
    for (i = 0; i < 64; i++)
    {
        if ((i % 16) == 0)
        {
            u16Offset += snprintf(&u8TempBuffer[u16Offset], 1000, "%c", '\n');
        }
        u16Offset += snprintf(&u8TempBuffer[u16Offset], 1000, "%02x ", u8EdidRaw[i]);
    }
    EDID_INFO("Edid Raw Data, first block,byte[0~63]:%s\n",u8TempBuffer);

    u16Offset = 0;
    memset(u8TempBuffer, 0, 1000);
    //u8Offset += snprintf(&u8TempBuffer[u8Offset], 1000, "%c", '\n');
    for (i = 64; i < 128; i++)
    {
        if ((i % 16) == 0)
        {
            u16Offset += snprintf(&u8TempBuffer[u16Offset], 1000, "%c", '\n');
        }
        u16Offset += snprintf(&u8TempBuffer[u16Offset], 1000, "%02x ", u8EdidRaw[i]);
    }
    EDID_INFO("Edid Raw Data, first block, byte[64~127]:%s\n",u8TempBuffer);

    u16Offset = 0;
    memset(u8TempBuffer, 0, 1000);
    //u8Offset += snprintf(&u8TempBuffer[u8Offset], 1000, "%c", '\n');
    for (i = 128; i < 192; i++)
    {
        if ((i % 16) == 0)
        {
            u16Offset += snprintf(&u8TempBuffer[u16Offset], 1000, "%c", '\n');
        }
        u16Offset += snprintf(&u8TempBuffer[u16Offset], 1000, "%02x ", u8EdidRaw[i]);
    }
    EDID_INFO("Edid Raw Data, second block, byte[128~191]:%s\n",u8TempBuffer);

    u16Offset = 0;
    memset(u8TempBuffer, 0, 1000);
    for (i = 192; i < 256; i++)
    {
        if ((i % 16) == 0)
        {
            u16Offset += snprintf(&u8TempBuffer[u16Offset], 1000, "%c", '\n');
        }
        u16Offset += snprintf(&u8TempBuffer[u16Offset], 1000, "%02x ", u8EdidRaw[i]);
    }
    EDID_INFO("Edid Raw Data, second block, byte[192~255]:%s\n",u8TempBuffer);

}

HI_S32 DRV_HDMI_EdidReset(HDMI_EDID_INFO_S *pstEdidInfo)
{
    HDMI_EDID_STATUS_S		*pstStatus = HI_NULL;
    HI_U8  					*pau8Data = HI_NULL;
    HDMI_SINK_CAPABILITY_S 	*pstCap = HI_NULL;

    EDID_NULL_CHK(pstEdidInfo);
    pstStatus 	= &pstEdidInfo->stStatus;
    pstCap 		= &pstEdidInfo->stCapability;
    pau8Data 	= pstEdidInfo->au8EdidRaw;
    EDID_NULL_CHK(pau8Data);

    HDMI_MEMSET(pstCap,0,sizeof(HDMI_SINK_CAPABILITY_S));
    HDMI_MEMSET(pau8Data,0,sizeof(pstEdidInfo->au8EdidRaw));

    pstStatus->bCapSink = HI_FALSE;
    pstStatus->bCapValid = HI_FALSE;
    pstStatus->bRawValid = HI_FALSE;
    pstStatus->u32RawLen = 0;
    pstStatus->enParseErr = EDID_PARSE_ERR_NONE;
    pstStatus->u32ParseWarn = EDID_PARSE_WARN_NONE;

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_EdidUpdate(HDMI_EDID_INFO_S *pstEdidInfo,
									HDMI_EDID_UPDATE_MODE_E enMode)
{
    HI_S32 					s32Ret = HI_SUCCESS;
    HDMI_EDID_STATUS_S		*pstStatus = HI_NULL;
    HI_U8  					*pau8Data = HI_NULL;
    HDMI_SINK_CAPABILITY_S 	*pstCap = HI_NULL;
    HDMI_DEVICE_S			*pstHdmiDev = HI_NULL;
    HDMI_HARDWARE_STATUS_S  stHwStatus;
    HI_BOOL                 bHotplug = HI_FALSE;

    EDID_NULL_CHK(pstEdidInfo);
    pstStatus 	= &pstEdidInfo->stStatus;
    pstCap 		= &pstEdidInfo->stCapability;
    pau8Data 	= pstEdidInfo->au8EdidRaw;
    EDID_NULL_CHK(pau8Data);

    HDMI_MEMSET(pstCap,0,sizeof(HDMI_SINK_CAPABILITY_S));
    HDMI_MEMSET(pau8Data,0,sizeof(pstEdidInfo->au8EdidRaw));
    HDMI_MEMSET(&stHwStatus, 0, sizeof(HDMI_HARDWARE_STATUS_S));

    pstStatus->bCapSink = HI_FALSE;
    pstStatus->bCapValid = HI_FALSE;
    pstStatus->bRawValid = HI_FALSE;
    pstStatus->u32RawLen = 0;
    pstStatus->enParseErr = EDID_PARSE_ERR_NONE;
    pstStatus->u32ParseWarn = EDID_PARSE_WARN_NONE;

    if ( HDMI_EDID_UPDATE_SINK == enMode )
	{
		pstHdmiDev = container_of(pstEdidInfo, typeof(*pstHdmiDev), stEdidInfo);
		EDID_NULL_CHK(pstHdmiDev);
		EDID_NULL_CHK(pstHdmiDev->pstHdmiHal);
		EDID_NULL_CHK(pstHdmiDev->pstHdmiHal->HAL_HDMI_EdidRawDataRead);

        pstHdmiDev->pstHdmiHal->HAL_HDMI_HotPlugStatusGet(pstHdmiDev->pstHdmiHal, &bHotplug);
        if (bHotplug)
        {
            s32Ret = pstHdmiDev->pstHdmiHal->HAL_HDMI_EdidRawDataRead(pstHdmiDev->pstHdmiHal, HDMI_EDID_TOTAL_SIZE, pau8Data);
            pstStatus->bCapSink = HI_TRUE;
        }
        else
        {
            pstStatus->bCapSink = HI_FALSE;
            EDID_WARN("Sink do not connect,forbid to read edid\n",s32Ret);
            return HI_FAILURE;
        }
	}
#ifndef HDMI_CHIP_HI3536
	else if (HDMI_EDID_UPDATE_DEBUG == enMode)
	{
        s32Ret = DRV_HDMI_DebugEdidRawRead(enMode,pau8Data,HDMI_EDID_TOTAL_SIZE);
        pstStatus->bCapSink = HI_FALSE;
	}
#endif
#if HDMI_EDID_RAW_TEST
	else
	{
		s32Ret = DRV_HDMI_EdidTestRawRead(enMode,pau8Data,HDMI_EDID_TOTAL_SIZE);
		pstStatus->bCapSink = HI_FALSE;
	}
#endif

    if ( (0==s32Ret) || (s32Ret % HDMI_EDID_BLOCK_SIZE) )
    {
     	pstStatus->u32RawUpdateErrCnt++;
        EDID_ERR("\nRead EDID raw data fail(0x%x)!\n",s32Ret);
        return HI_FAILURE;
    }
	pstStatus->u32RawLen = (HI_U32)s32Ret;
	pstStatus->bRawValid = HI_TRUE;

    EdidRawDataPrint(pstEdidInfo->au8EdidRaw, 512);

    s32Ret = EdidRawParse(pstEdidInfo);

    EDID_SUCCESS_CHK(s32Ret);
	pstStatus->bCapValid = HI_TRUE;
	return HI_SUCCESS;
}


HDMI_EDID_DATA_E DRV_HDMI_EdidCapabilityGet(HDMI_EDID_INFO_S *pstEdidInfo,
									 				HDMI_SINK_CAPABILITY_S **pstCapDest)
{
	HDMI_EDID_STATUS_S		*pstStatus = HI_NULL;
    HDMI_EDID_DATA_E        enEdidStatus = HDMI_EDID_DATA_INVALID;

	EDID_NULL_CHK(pstCapDest);
	EDID_NULL_CHK(pstEdidInfo);

	pstStatus = &pstEdidInfo->stStatus;
	*pstCapDest = &pstEdidInfo->stCapability;

	if (pstStatus->bCapValid)
	{
        enEdidStatus = (pstStatus->bCapSink) ? HDMI_EDID_DATA_VALIDSINK : HDMI_EDID_DATA_VALIDTEST;
	}
	else
	{
		pstStatus->u32CapGetErrCnt++;
        enEdidStatus = HDMI_EDID_DATA_INVALID;
	}

    return enEdidStatus;
}


HI_S32 DRV_HDMI_EdidRawGet(HDMI_EDID_INFO_S *pstEdidInfo,
											HI_U8 *pu8RawDataDest,
											HI_U32 u32Len)
{
	HDMI_EDID_STATUS_S		*pstStatus = HI_NULL;
    HI_U8  					*pau8DataSrc = HI_NULL;
	HI_U32					u32TmpLen = 0;

	EDID_NULL_CHK(pu8RawDataDest);
	EDID_NULL_CHK(pstEdidInfo);

	pstStatus = &pstEdidInfo->stStatus;
	pau8DataSrc = pstEdidInfo->au8EdidRaw;
	EDID_NULL_CHK(pau8DataSrc);

	if ( (u32Len > HDMI_EDID_TOTAL_SIZE) || (HI_FALSE == pstStatus->bRawValid) )
	{
		pstStatus->u32RawGetErrCnt++;
		return HI_FAILURE;
	}

	u32TmpLen = (u32Len <= pstStatus->u32RawLen) ? u32Len : pstStatus->u32RawLen;
	HDMI_MEMCPY(pu8RawDataDest,pau8DataSrc,u32TmpLen);

	return (HI_S32)u32TmpLen;
}


HI_S32 DRV_HDMI_EdidRawForceGet(HDMI_EDID_INFO_S *pstEdidInfo,
											HI_U8 *pu8RawData,
											HI_U32 u32Len)
{
	HI_S32 s32Ret = 0;

	s32Ret = DRV_HDMI_EdidUpdate(pstEdidInfo,HDMI_EDID_UPDATE_SINK)	;
	EDID_SUCCESS_CHK(s32Ret);

	s32Ret = DRV_HDMI_EdidRawGet(pstEdidInfo,pu8RawData ,u32Len) ;
	return s32Ret;
}


HI_S32 DRV_HDMI_EdidStatusGet(HDMI_EDID_INFO_S *pstEdidInfo,
											HDMI_EDID_STATUS_S *pstStatus)
{

	EDID_NULL_CHK(pstEdidInfo);
	EDID_NULL_CHK(pstStatus);

	HDMI_MEMCPY(pstStatus,&pstEdidInfo->stStatus,sizeof(HDMI_EDID_STATUS_S));

	return HI_SUCCESS;
}

