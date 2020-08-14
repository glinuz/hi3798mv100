#include "drv_hdmi_compatibility.h"
#include "hdmi_osal.h"

#define DEF_FILE_NAMELENGTH 32

#define COMPAT_NULL_CHK(p)	do{\
	if(HI_NULL==p)\
	{HDMI_ERR("NULL pionter!\n");return HI_FAILURE;}\
}while(0);


typedef struct{
    HDMI_EDID_MANUFACTURE_INFO_S stEdid;
    HI_U64                       u64MsUnder340M;
    HI_U64                       u64MsOver340M;
}COMPAT_HDCP_MS_S;

static HDMI_EDID_MANUFACTURE_INFO_S s_astSinkCecUnsupport[] = 
{
    /* Name[4],ProdCode,SerialNO,Week,Year */
    {   "000" ,       0,       0,   0,   0,},
};

static COMPAT_HDCP_MS_S s_astSinkHdcpMs[] = 
{
    /*  {Name[4],ProdCode,SerialNO,Week, Year},  Under340  Over340 */
    {   {"SNY" ,    51203,16843009,   1, 2015},  500,     1200},        //sony KD-65X9300D
};


// product delay 


//Panasonic_TH-L32CH3C
static HDMI_DELAY_TIME_S MEI_TH_L32CH3C =
{
    "MEI",49924,600,200,"MEI_TH_L32CH3C",
};

//Skyworth SKY_47E760A
static HDMI_DELAY_TIME_S SKY_47E760A =
{
    "SKY",48,100,0,"SKY_47E760A",
};

//Skyworth 24E60HR
static HDMI_DELAY_TIME_S SKW_24E60HR =
{
    "SKW",48,1200,0,"24E60HR",
};


//Sharp 24E60HR when close oe in preFormat ,blink green bg 
static HDMI_DELAY_TIME_S SHP_LCD_32Z100AS =
{
    "SHP",4149,0,0,"LCD-32Z100AS",
};

//Sumsung LA40B550K1F
static HDMI_DELAY_TIME_S SAM_LA40B550K1F =
{
    "SAM",1289,100,0,"LA40B550K1F",
};



// Brand Delay
#define SKYWORTH_DELAY 600

static HDMI_DELAY_TIME_S *WholeList[] =
{
    &MEI_TH_L32CH3C,&SKY_47E760A,&SKW_24E60HR,&SHP_LCD_32Z100AS,&SAM_LA40B550K1F,NULL
};


HI_S32 FormatDelayGet(HDMI_DEVICE_S* pstHdmiDev, HI_U32 *pu32DelayTime)
{
    HI_U32 i = 0;
    HI_U32 u32FmtDelay = 0;
    HDMI_SINK_CAPABILITY_S *pstSinkCap = HI_NULL;   
    
    if(pu32DelayTime == NULL || pstHdmiDev == HI_NULL)
    {
        HDMI_ERR("Null Point Error \n");
        return HI_FAILURE;
    }

    if(pstHdmiDev->stDelay.bForceFmtDelay)
    {
        u32FmtDelay = pstHdmiDev->stDelay.u32FmtDelay;
        HDMI_INFO("HDMI SetFormat Force Delay %dms \n",u32FmtDelay);
    }
    else
    {   
        if (HDMI_EDID_DATA_INVALID == DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap))
        {
            HDMI_WARN("Get sink capability fail\n");
            u32FmtDelay = pstHdmiDev->stDelay.u32FmtDelay;
        }
        else
        {
            for (i = 0; WholeList[i] != NULL; i++)
            {
                //if sincap is invalid, the data can not euqal to compatibility list
                if((pstSinkCap->stMfrsInfo.u32ProductCode == WholeList[i]->u32IDProductCode) && 
                    IS_STR_EQUAL(pstSinkCap->stMfrsInfo.u8MfrsName,WholeList[i]->u8IDManufactureName))
                {
                    u32FmtDelay = WholeList[i]->u32DelayTimes;
                    HDMI_INFO("Sink %s need Delay %d ms \n",WholeList[i]->u8ProductType,u32FmtDelay);
                }
            }

            if(u32FmtDelay == 0)
            {
                //In SKY_47E760A MfrsName SKY,And 24E60HR SKW
                if(IS_STR_EQUAL(pstSinkCap->stMfrsInfo.u8MfrsName,"SKY") || 
                    IS_STR_EQUAL(pstSinkCap->stMfrsInfo.u8MfrsName,"SKW"))
                {
                    u32FmtDelay = SKYWORTH_DELAY;
                    HDMI_INFO("Sink SKYWorth should Delay %d ms \n",u32FmtDelay);
                }
            }

            if(u32FmtDelay == 0)
            {
                u32FmtDelay = pstHdmiDev->stDelay.u32FmtDelay;
                HDMI_INFO("HDMI SetFormat Global Delay %dms \n",u32FmtDelay);
            }
        }

    }

    *pu32DelayTime = u32FmtDelay;
    return HI_SUCCESS;
};

HI_S32 AvMuteDelayGet(HDMI_DEVICE_S* pstHdmiDev, HI_U32 *pu32DelayTime)
{
    HI_U32                  u32MuteDelay = 0;
    HI_U32                  i = 0;
    HDMI_VO_ATTR_S          *pstVoAttr = HI_NULL;
    HDMI_SINK_CAPABILITY_S  *pstSinkCap = HI_NULL;   
    
    if(pu32DelayTime == NULL || pstHdmiDev == HI_NULL)
    {
        HDMI_ERR("Null Point Error \n");
        return HI_FAILURE;
    }

    pstVoAttr = &pstHdmiDev->stAttr.stVOAttr;

    if(pstHdmiDev->stDelay.bForceMuteDelay)
    {
        u32MuteDelay = pstHdmiDev->stDelay.u32MuteDelay;
        HDMI_INFO("HDMI mute Force Delay %dms \n",u32MuteDelay);
    }
    else
    {
        if (HDMI_EDID_DATA_INVALID == DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap))
        {
            HDMI_WARN("Get sink capability fail\n");
            u32MuteDelay = pstHdmiDev->stDelay.u32MuteDelay;
        }
        else
        {
            for (i = 0; WholeList[i] != NULL; i++)
            {
                if((pstSinkCap->stMfrsInfo.u32ProductCode == WholeList[i]->u32IDProductCode) && 
                   IS_STR_EQUAL(pstSinkCap->stMfrsInfo.u8MfrsName,WholeList[i]->u8IDManufactureName))
                {
                    u32MuteDelay = WholeList[i]->u32MuteDelay;
                    HDMI_INFO("Sink %s need Avmute Delay %d ms \n",WholeList[i]->u8ProductType,u32MuteDelay);
                }
            }

            if(u32MuteDelay == 0)
            {
                if(pstVoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1920X1080P_30000 ||
                   pstVoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1920X1080P_25000 ||
                   pstVoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1920X1080P_24000)
                {
                    //minimum requirement (2 frames + 50%) time needed in 24Hz timing
                    //(1/24hz)*2 + 1/24Hz * 0.5 = 105ms
                    u32MuteDelay = 120;
                }
                else
                {
                    //minimum requirement (2 frames + 50%) time needed in 24Hz timing
                    //(1/50hz)*2 + 1/50Hz * 0.5 = 50ms 
                    u32MuteDelay = 50;
                }
            }

        }
    }

    *pu32DelayTime = u32MuteDelay;
    return HI_SUCCESS;
}


HI_BOOL DRV_HDMI_CompatCecValidCheck(HDMI_DEVICE_S *pstHdmiDev)
{
    HI_U32                          i = 0;
    HI_BOOL                         bValid = HI_TRUE;
    HDMI_SINK_CAPABILITY_S          *pstSinkCap = HI_NULL; 
    HDMI_EDID_MANUFACTURE_INFO_S    *pstTmpManuInfo = HI_NULL;
    HDMI_EDID_MANUFACTURE_INFO_S    *pstSinkManuInfo= HI_NULL ;
    
    if (HDMI_EDID_DATA_INVALID == DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap))
    {
        HDMI_WARN("Get sink capability fail\n");
        bValid  = HI_FALSE;
    }
    else
    {
        pstSinkManuInfo = &pstSinkCap->stMfrsInfo;

        for(i=0,pstTmpManuInfo = &s_astSinkCecUnsupport[0];
            pstTmpManuInfo && pstSinkManuInfo &&(i < HDMI_ARRAY_SIZE(s_astSinkCecUnsupport));
            pstTmpManuInfo++,i++)
        {
            if( (pstTmpManuInfo->u8MfrsName[0] == pstSinkManuInfo->u8MfrsName[0])
                && (pstTmpManuInfo->u8MfrsName[1] == pstSinkManuInfo->u8MfrsName[1])
                && (pstTmpManuInfo->u8MfrsName[2] == pstSinkManuInfo->u8MfrsName[2])
                && (pstTmpManuInfo->u8MfrsName[3] == pstSinkManuInfo->u8MfrsName[3])
                && (pstTmpManuInfo->u32ProductCode == pstSinkManuInfo->u32ProductCode)
                && (pstTmpManuInfo->u32SerialNumber == pstSinkManuInfo->u32SerialNumber)
                && (pstTmpManuInfo->u32Week == pstSinkManuInfo->u32Week)
                && (pstTmpManuInfo->u32Year == pstSinkManuInfo->u32Year) )
            {
                bValid  = HI_FALSE;
                HDMI_WARN("error CEC PHY ADDR,not support.\n");
                break;
            }
        }
    }
    pstSinkCap->stCECAddr.bPhyAddrValid &= bValid ;

    return pstSinkCap->stCECAddr.bPhyAddrValid;
}


HI_S32  DRV_HDMI_CompatHdcpMsGet(HDMI_DEVICE_ID_E enHdmiId,HI_U64 *pu64WaitMs)
{
    HI_U32                          i = 0;
    HDMI_SINK_CAPABILITY_S          *pstSinkCap = HI_NULL; 
    COMPAT_HDCP_MS_S                *pstHdcpMs = HI_NULL;
    HDMI_EDID_MANUFACTURE_INFO_S    *pstTmpManuInfo = HI_NULL ;
    HDMI_EDID_MANUFACTURE_INFO_S    *pstSinkManuInfo= HI_NULL ;
    HDMI_DEVICE_S                   *pstHdmiDev = HI_NULL;

    pstHdmiDev = GetHdmiDevice(enHdmiId);
    COMPAT_NULL_CHK(pstHdmiDev);
    COMPAT_NULL_CHK(pu64WaitMs);

    /* default value */
    *pu64WaitMs = 500;

    if (HDMI_EDID_DATA_INVALID == DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap))
    {
        HDMI_WARN("Get sink capability fail\n");
        return HI_FAILURE;
    }
    else
    {
        pstSinkManuInfo = &pstSinkCap->stMfrsInfo;

        for(i=0,pstHdcpMs = &s_astSinkHdcpMs[0];
            pstHdcpMs &&(i < HDMI_ARRAY_SIZE(s_astSinkHdcpMs));
            pstHdcpMs++,i++)
        {
            pstTmpManuInfo = &pstHdcpMs->stEdid;
            
            if( (pstTmpManuInfo->u8MfrsName[0] == pstSinkManuInfo->u8MfrsName[0])
                && (pstTmpManuInfo->u8MfrsName[1] == pstSinkManuInfo->u8MfrsName[1])
                && (pstTmpManuInfo->u8MfrsName[2] == pstSinkManuInfo->u8MfrsName[2])
                && (pstTmpManuInfo->u8MfrsName[3] == pstSinkManuInfo->u8MfrsName[3])
                && (pstTmpManuInfo->u32ProductCode == pstSinkManuInfo->u32ProductCode)
                && (pstTmpManuInfo->u32SerialNumber == pstSinkManuInfo->u32SerialNumber)
                && (pstTmpManuInfo->u32Week == pstSinkManuInfo->u32Week)
                && (pstTmpManuInfo->u32Year == pstSinkManuInfo->u32Year) )
            {
                *pu64WaitMs = pstHdmiDev->stAttr.stVOAttr.u32HdmiAdaptPixClk >= FMT_PIX_CLK_340000 ? 
                                pstHdcpMs->u64MsOver340M : pstHdcpMs->u64MsUnder340M;
                break;
            }
        }
    }

    return HI_SUCCESS;
}    


